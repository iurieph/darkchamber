/**
 * File name: PhotoViewer.cpp
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

#include "PhotoViewer.h"
#include "PhotoItem.h"

#include <QScrollBar>
#include <QPainter>
#include <QDebug>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QGraphicsPixmapItem>

PhotoViewer::PhotoViewer(QWidget *parent)
        : QGraphicsView(parent)
        , currentImage{nullptr}
        , zoomRange{1.0, 1600.0}
        , procentageOfZoom{100.0}
{
        setMinimumSize(150, 150);
        auto viewerScene = new QGraphicsScene(rect());
        viewerScene->setBackgroundBrush(Qt::gray);
        setScene(viewerScene);
        setRenderHint(QPainter::Antialiasing);
        setAlignment(Qt::AlignCenter);
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

void PhotoViewer::setImage(PhotoItem *image)
{
        scene()->clear();
        QPixmap pm;
        pm.convertFromImage(image->image());
        currentImage = scene()->addPixmap(pm);
        zoomFit();
        dkch_debug() << scene()->sceneRect().width() << "x" << scene()->sceneRect().height();
        dkch_debug() << width() << "x" << height();
}

void PhotoViewer::zoomIn()
{
        procentageOfZoom = 2 * procentageOfZoom;
        zoom(procentageOfZoom);
}

void PhotoViewer::zoomOut()
{
        procentageOfZoom = procentageOfZoom / 2;
        zoom(procentageOfZoom);
}

void PhotoViewer::zoomOneToOne()
{
        procentageOfZoom = 100;
        zoom(procentageOfZoom);
}

void PhotoViewer::zoomFit()
{
        if (!currentImage)
                return;
        
        resetTransform();
        auto pixmapImage = currentImage->pixmap();
        if (pixmapImage.width() >= pixmapImage.height())
                procentageOfZoom = 100.0 * (static_cast<double>(width()) / pixmapImage.width());
        else
                procentageOfZoom = 100.0 * (static_cast<double>(height()) / pixmapImage.height());
        scene()->setSceneRect(QRect({0, 0}, pixmapImage.size()));
        fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
        setDragMode(QGraphicsView::NoDrag);
        emit zoomUpdated(procentageOfZoom);
}

void PhotoViewer::zoom(double procentage)
{
        if (!currentImage)
                return;
        
        procentageOfZoom = std::clamp(procentage, zoomRange.first, zoomRange.second);
        auto precentageFactor = static_cast<double>(procentageOfZoom) / 100.0;
        auto pixmapImage = currentImage->pixmap();
        double zoomFactor;
        if (pixmapImage.width() >= pixmapImage.height())
                zoomFactor = precentageFactor * (pixmapImage.width() / width());
        else
                zoomFactor = precentageFactor * (pixmapImage.height() / height());
        resetTransform();
        fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
        scale(zoomFactor, zoomFactor);
        updateHandDragMode();
        emit zoomUpdated(procentageOfZoom);
}

void PhotoViewer::resizeEvent(QResizeEvent *event)
{
        if (scene()->items().isEmpty())
                return;
        auto tempTransform = transform();
        fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
        setTransform(tempTransform);
        updateHandDragMode();
}

void PhotoViewer::wheelEvent(QWheelEvent *event)
{
        if (event->angleDelta().y() >= 0) {
                zoomIn();
        } else if (event->angleDelta().y() < 0) {
                zoomOut();
        } else {
                QGraphicsView::wheelEvent(event);
        }
}

void PhotoViewer::updateHandDragMode()
{
        auto mappedRect = transform().mapRect(sceneRect());
        if (viewport()->rect().width() >= mappedRect.width()
            && viewport()->rect().height() >= mappedRect.height()) 
                setDragMode(QGraphicsView::NoDrag);
        else
                setDragMode(QGraphicsView::ScrollHandDrag);
}
