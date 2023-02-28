/**
 * File name: MainWindow.h
 * Project: DarkChamber (A photo management software)
 *
 * Copyright (C) 2022 Iurie Nistor
 *
 * This file is part of DarkChamber.
 *
 * GeonKick is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QStackedWidget;
class PhotoBrowser;
class PhotoEditor;
class PhotoItem;
class PhotoModel;
class QSplitter;
class QThread;
class PathLoader;

class MainWindow : public QMainWindow
{
        Q_OBJECT

public:
        explicit MainWindow();
        ~MainWindow();

protected:
        void keyPressEvent(QKeyEvent *event) override;
        void createToolBars();

protected slots:
        void showImage(PhotoItem *iamge);
        void showNextImage(bool direction);

private:
        PhotoModel *photoModel;
        PhotoBrowser *photoBrowser;
        PhotoEditor *imageViewer;
        QStackedWidget *stackedWidget;
        QSplitter *widgetSplitter;
        QThread* pathLoaderThread;
        PathLoader* pathLoader;
};

#endif // MAINWINDOW_H
