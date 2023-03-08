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
        , zoomRange{1, 1600}
        , procentageOfZoom{100}
{
        setMinimumSize(150, 150);
        setScene(new QGraphicsScene(rect()));
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
        scene()->setSceneRect(QRect({0, 0}, pm.size()));
        zoomFit();
}

void PhotoViewer::zoomIn()
{
        procentageOfZoom = std::clamp(2 * procentageOfZoom, zoomRange.first, zoomRange.second);
        zoom(procentageOfZoom);
}

void PhotoViewer::zoomOut()
{
        procentageOfZoom = std::clamp(procentageOfZoom / 2, zoomRange.first, zoomRange.second);
        zoom(procentageOfZoom);
}

void PhotoViewer::zoomOneToOne()
{
        procentageOfZoom = 100;
        zoom(procentageOfZoom);
}

void PhotoViewer::zoomFit()
{
        resetTransform();
        fitInView(currentImage, Qt::KeepAspectRatio);
        setDragMode(QGraphicsView::NoDrag);
        procentageOfZoom = 100;
        currentImage->boundingRect().height() / height();
}

void PhotoViewer::zoom(int procentage)
{
        if (!currentImage)
                return;
        
        procentageOfZoom = std::clamp(procentage, zoomRange.first, zoomRange.second);
        auto factor = static_cast<double>(procentageOfZoom) / 100.0;
        auto zoomFactor = factor * (currentImage->boundingRect().height() / height());
        resetTransform();
        fitInView(currentImage, Qt::KeepAspectRatio);
        scale(zoomFactor, zoomFactor);
        updateHandDragMode();
}

void PhotoViewer::resizeEvent(QResizeEvent *event)
{
        if (scene()->items().isEmpty())
                return;
        auto tempTransform = transform();
        fitInView(scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
        setTransform(tempTransform);
        updateHandDragMode();
}

void PhotoViewer::wheelEvent(QWheelEvent *event)
{
        if (event->angleDelta().y() > 0) {
                zoomIn();
        } else if (event->angleDelta().y() < 0) {
                zoomIn();
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
