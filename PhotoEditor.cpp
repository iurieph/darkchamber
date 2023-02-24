/**
 * File name: PhotoEditor.cpp
 * Project: DarkChamber (A photo management software)
 *
 * Copyright (C) 2023 Iurie Nistor
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

#include "PhotoEditor.h"
#include "PhotoViewer.h"
#include "PhotoEditorToolBar.h"
#include "PhotoItem.h"

#include <QKeyEvent>
#include <QVBoxLayout>

PhotoEditor::PhotoEditor(QWidget *parent)
        : QWidget(parent)
        , imageViewer{new PhotoViewer(this)}
        , editorToolBar{new PhotoEditorToolBar(this)}
{
        setLayout(new QVBoxLayout(this));
        layout()->addWidget(imageViewer);
        layout()->addWidget(editorToolBar);
}

void PhotoEditor::setImage(PhotoItem *image)
{
        imageViewer->setImage(image);
        editorToolBar->setExposureInfo(image->imageData().getExposureInfo());
}

void PhotoEditor::keyPressEvent(QKeyEvent *event)
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

