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
#include "EffectsWidget.h"
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
#include <QAction>
#include <QToolBar>
#include <QStatusBar>

MainWindow::MainWindow()
        : QMainWindow()
        , photoModel{nullptr}
        , photoBrowser{nullptr}
        , imageViewer{nullptr}
        , stackedWidget{nullptr}
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
	//addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, new EffectsWidget);

        pathLoader->moveToThread(pathLoaderThread);
        QObject::connect(locationsView,
                         &LocationsView::pathSelected,
                         pathLoader,
                         &PathLoader::setPath);
        pathLoaderThread->start();
        
        photoModel = new PhotoModel(this);
        QObject::connect(photoModel,
                         &PhotoModel::addedPhotos,
                         this,
                         &MainWindow::updateStatusBar);
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

        setStatusBar(new QStatusBar(this));
        createToolBars();

#ifdef DKCHAMBER_DEBUG_MAIN_THREAD_TIMER
        auto mainThreadTimer = new QTimer(this);
        constexpr int timeout = 500;
        static auto start = std::chrono::high_resolution_clock::now();
        QObject::connect(mainThreadTimer, &QTimer::timeout, [&](){
                auto stop = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() -  timeout;
                start = stop;
                dkch_debug() << duration;
        });
        mainThreadTimer->start(timeout);
#endif // DKCHAMBER_DEBUG_MAIN_THREAD_TIMER

}

MainWindow::~MainWindow()
{
        DARKCHAMBER_LOG_DEBUG();
        pathLoader->stop();
        pathLoaderThread->exit();
        pathLoaderThread->wait();
        delete pathLoader;
}

void MainWindow::createToolBars()
{       
        // Browser toolbar
        auto toolbar = addToolBar("Browser Toolbar");
        auto action = new QAction(QIcon::fromTheme("info"),
                                  tr("Show photo extif info"));
        toolbar->addAction(action);

        // Zoom In/Out actions
        toolbar->addSeparator();
        action = new QAction(QIcon::fromTheme("zoom-in"),
                             tr("Zoom in browser thumbnails"));
        QObject::connect(action,
			 &QAction::triggered,
			 photoBrowser,
		         &PhotoBrowser::zoomIn);
        toolbar->addAction(action);
        action = new QAction(QIcon::fromTheme("zoom-out"),
                             tr("Zoom out browser thumbnails"));
        QObject::connect(action,
			 &QAction::triggered,
			 photoBrowser,
		         &PhotoBrowser::zoomOut);

        toolbar->addAction(action);
        toolbar->addSeparator();
}

void MainWindow::showImage(PhotoItem *image)
{
        if (!imageViewer) {
                imageViewer = new PhotoEditor(this);
                widgetSplitter->insertWidget(0, imageViewer);
                QObject::connect(imageViewer,
                                 &PhotoEditor::nextImage,
                                 this,
                                 &MainWindow::showNextImage);
        }
        setWindowTitle("DarkChamber - "+ image->photoName());
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

void MainWindow::updateStatusBar(int n)
{
        statusBar()->showMessage(QString("Loaded %1 photos").arg(n));
}
