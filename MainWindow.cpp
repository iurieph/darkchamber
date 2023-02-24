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

#include "MainWindow.h"
#include "PathLoader.h"
#include "PhotoModel.h"
#include "PhotoBrowser.h"
#include "LocationsView.h"
#include "PhotoItem.h"
#include "PhotoEditor.h"
#include "Application.h"

#include <QFileSystemModel>
#include <QDockWidget>
#include <QKeyEvent>
#include <QStackedWidget>
#include <QSplitter>
#include <QThread>
#include <QTimer>

MainWindow::MainWindow()
        : QMainWindow()
        , photoModel{nullptr}
        , stackedWidget{nullptr}
        , photoBrowser{nullptr}
        , imageViewer{nullptr}
        , widgetSplitter{new QSplitter(Qt::Vertical, this)}
        , pathLoaderThread{new QThread(this)}
        , pathLoader{new PathLoader()}
{
        setWindowTitle("DarkChamber");
	auto locationsModel = new QFileSystemModel(this);
        locationsModel->setOption(QFileSystemModel::Option::DontWatchForChanges, true);
	locationsModel->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	locationsModel->setRootPath("./");
	auto locationsView = new LocationsView;
	locationsView->setModel(locationsModel);
	addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, locationsView);
	addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, new QDockWidget(tr("Collections")));

        pathLoader->moveToThread(pathLoaderThread);
        QObject::connect(locationsView,
                         &LocationsView::pathSelected,
                         pathLoader,
                         &PathLoader::setPath);
        pathLoaderThread->start();
        
        photoModel = new PhotoModel(this);
        QObject::connect(pathLoader,
                         &PathLoader::pathChanged,
                         photoModel,
                         &PhotoModel::clearModel);
        QObject::connect(pathLoader,
                         &PathLoader::photoAvailable,
                         photoModel,
                         &PhotoModel::addPhoto);
        QObject::connect(photoModel,
			 &PhotoModel::viewImage,
			 this,
		         &MainWindow::showImage);
	photoBrowser = new PhotoBrowser(photoModel, this);
        widgetSplitter->addWidget(photoBrowser);
        setCentralWidget(widgetSplitter);
        resize(1300, 800);

        QTimer *timer = new QTimer(this);
        static int n;
        QObject::connect(timer, &QTimer::timeout, [](){
                DARKCHAMBER_LOG_DEBUG() << "time" <<  n++;
        });
        timer->start(50);
}

MainWindow::~MainWindow()
{
        DARKCHAMBER_LOG_DEBUG();
        pathLoader->stop();
        DarkChamberApplication::guiSemaphore().release(10);
        pathLoaderThread->exit();
        pathLoaderThread->wait();
        delete pathLoader;
}

#include <QDebug>

void MainWindow::showImage(PhotoItem *image)
{
        if (!imageViewer) {
                imageViewer = new PhotoEditor(this);
                widgetSplitter->insertWidget(0, imageViewer);
                QObject::connect(imageViewer,
                                 &PhotoEditor::nextImage,
                                 this,
                                 &MainWindow::showNextImage);
                setWindowTitle("DarkChamber - "+ image->photoName());
        }
        imageViewer->setImage(image);
}

void MainWindow::showNextImage(bool direction)
{
        direction ? photoBrowser->nextItem() : photoBrowser->previousItem();
        showImage(photoModel->currentSelectedItem());
}
 
void MainWindow::keyPressEvent(QKeyEvent *event)
{
        switch (event->key()) {
        case Qt::Key_Escape:
                delete imageViewer;                
                break;
        default:
                break;
        }
}
