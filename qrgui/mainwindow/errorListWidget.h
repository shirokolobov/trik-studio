#pragma once

#include <QtWidgets/QListWidget>

#include <qrkernel/ids.h>

#include "mainwindow/error.h"

namespace qReal {
	class MainWindow;
}

class ErrorListWidget : public QListWidget
{
	Q_OBJECT

public:
	explicit ErrorListWidget(QWidget *parent = NULL);
	void init(qReal::MainWindow* mainWindow);

signals:
	void clearRequested();

private slots:
	void highlightElement(QListWidgetItem* const item);
	void copyCurrentItem();
	void showContextMenu(QPoint const &pos);

private:

	void initContextMenu();

	qReal::MainWindow* mMainWindow;
	QMenu *mContextMenu;
};

