#!/bin/bash
# Usage: build-installer.sh <path-to-qt> <path-to-qt-ifw> <product-name> [<path-to-build-dir>]
# Path to Qt and Qt Installer Framework must be to bin folder (for example ~/Qt/5.3/gcc_64/bin or ~/Qt/QtIFW-1.5.0/bin/).
# This script will build the installer using the config in './config/$3-config.xml',
# and all components in 'packages/qreal-base' and 'packages/$3'. 'trik-studio' will be renamed to $3.
# If $4 is empty then <path-to-build-dir> = ../

set -o nounset
set -o errexit

#[ -z "${PRODUCT_DISPLAYED_NAME+x}" ] && echo -e "\x1b[93;41mUse corresponding helper script, do not run this one directly\x1b[0m" && exit 3

export QT_DIR=$1/../
export QTIFW_DIR=$2
export PRODUCT=$3
export OS=$OSTYPE

[ -z ${4+x} ] && BUILD_DIR=$(dirname $(readlink -f $0))/.. || BUILD_DIR=$(readlink -f $4)
[ -z $BUILD_DIR ] && exit 1 || export BIN_DIR=$BUILD_DIR/bin/release
echo $BIN_DIR
[ -e $BIN_DIR/trik-studio ] || exit 1
[ -e $(basename $0) ] || cd $(dirname $(readlink -f $0))
export INSTALLER_ROOT=$PWD/

PATH=$QT_DIR/bin:$PATH
# FULL_VERSION is like v3.3.0[-rc9][-20-abc123][-dirty]
FULL_VERSION=$($BIN_DIR/trik-studio --version | grep -Eo '[^ ]+$')
#QT IFW want version like [0-9]+((.|-)[0-9]+)*
VERSION=$(echo $FULL_VERSION | sed 's/[^0-9.-]//g' | sed 's/[^0-9]$//g' )
grep -r -l --include=*.xml '<Version>.*</Version>' | xargs sed -i "s/<Version>.*<\/Version>/<Version>$VERSION<\/Version>/"
cd config
grep -r -l --include=*.xml '<Version>.*</Version>' | xargs sed -i "s/<Version>.*<\/Version>/<Version>$FULL_VERSION<\/Version>/"
cd ..

grep -q "darwin" <<< $OSTYPE && export OS="mac" || :
# All windows platforms can be enumerated below
[ $OSTYPE == "msys" ] && export OS="win32" || :
[ $OSTYPE == "linux-gnu" ] && OS_EXT=$OS`getconf LONG_BIT` || OS_EXT=$OS

if [ $OS == "win32" ] ; then
  if [ -z $(file -b $BIN_DIR/trik-studio | grep -Eo "^PE32 ") ] ; then ADD_BIT=-x64 ; else ADD_BIT=-x86 ; fi
else
  ADD_BIT=
fi
[ $OS == "win32" ] && SSH_DIR=/.ssh || SSH_DIR=~/.ssh

# $2 will be passed to all prebuild.sh scripts
echo "Executing prebuild actions..."
find $PWD/packages/qreal-base -name prebuild-common.sh -print0 | xargs -0 chmod +x
find $PWD/packages/qreal-base -name prebuild-$OS.sh -print0 | xargs -0 chmod +x
find $PWD/packages/qreal-base -name prebuild-common.sh | bash
find $PWD/packages/qreal-base -name prebuild-$OS.sh | bash

find $PWD/packages/$PRODUCT -name prebuild-common.sh -print0 | xargs -0 chmod +x
find $PWD/packages/$PRODUCT -name prebuild-$OS.sh -print0 | xargs -0 chmod +x
find $PWD/packages/$PRODUCT -name prebuild-common.sh | bash
find $PWD/packages/$PRODUCT -name prebuild-$OS.sh | bash

find . -type d -empty -delete

#echo "Building online installer..."
#$QTIFW_DIR/binarycreator --online-only -c config/$PRODUCT-$OS_EXT.xml -p packages/qreal-base -p packages/$PRODUCT ${*:4} $PRODUCT-online-$OS_EXT-installer

echo "Building offline installer..."
$QTIFW_DIR/binarycreator --offline-only -c config/$PRODUCT-$OS_EXT.xml -p packages/qreal-base -p packages/$PRODUCT $PRODUCT-offline-$OS_EXT-installer$ADD_BIT-$FULL_VERSION

grep -r -l --include=*.xml '<Version>.*</Version>' | xargs sed -i "s/<Version>.*<\/Version>/<Version><\/Version>/"

[ -f $SSH_DIR/id_rsa ] && : || { echo "Done"; exit 0; }

#echo "Building updates repository... This step can be safely skipped, the offline installer is already ready, press Ctrl+C if you are not sure what to do next."
#rm -rf $PRODUCT-repository
#$QTIFW_DIR/repogen -p packages/qreal-base -p packages/$PRODUCT ${*:4} $PRODUCT-repository

#echo "Uploading repository to server... This step can be also safely skipped, the offine installer is already ready, press Ctrl+C if you are not sure what to do next."
#scp -oStrictHostKeyChecking=no -oUserKnownHostsFile=/dev/null -r $PRODUCT-repository/* qrealproject@195.19.241.150:/home/qrealproject/public/packages/$PRODUCT-repo-$OS_EXT

echo "Removing temporary files..."
rm -rf $PRODUCT-repository

echo "Done"
