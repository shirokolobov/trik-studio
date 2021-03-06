/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "windowsExternBrowser.h"

#include <windows.h>
#include <stdint.h>

ExternBrowser::ExternBrowser()
{
}

bool ExternBrowser::openPage(const QString &url)
{
	std::wstring wstr = url.toStdWString();// mb scheme()
	const wchar_t * temp = wstr.c_str();
	return intptr_t (ShellExecuteW(nullptr, nullptr, temp, nullptr, nullptr, SW_SHOWNORMAL)) > 32;  // zomg
}
