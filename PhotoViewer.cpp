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
#include "ImageCropper.h"
#include "PhotoItem.h"

#include <QScrollBar>
#include <QPainter>
#include <QDebug>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QGraphicsPixmapItem>

PhotoViewer::PhotoViewer(QWidget *parent)
        : QGraphicsView(parent)
        , imageCropper{new ImageCropper(this)}
        , zoomFactor{1.05}
        , zoomSteps{0}
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
        QPixmap pixmap;
        auto pixmap = QPixmap().convertFromImage(image->image());
        auto item = scene()->addPixmap(pixmap);
        item->setPos(0, 0);
        scene()->setSceneRect(QRect({0, 0}, img.size()));
        resetTransform();
        fitInView(item, Qt::KeepAspectRatio);
        setDragMode(QGraphicsView::NoDrag);
        zoomSteps = 0;
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

void PhotoViewer::keyPressEvent(QKeyEvent *event)
{
        switch (event->key()) {
        case Qt::Key_Right:
                emit nextImage();
                break;
        case Qt::Key_Left:
                emit nextImage(false);
                break;
        default:
                break;
        }
        QWidget::keyPressEvent(event);
}

void PhotoViewer::wheelEvent(QWheelEvent *event)
{
        if (event->angleDelta().y() > 0) {
                scale(zoomFactor, zoomFactor);
                updateHandDragMode();
        } else if (event->angleDelta().y() < 0) {
                scale(1.0 / zoomFactor, 1.0 / zoomFactor);
                updateHandDragMode();
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

