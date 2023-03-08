/**
 * File name: PhotoEditorToolBar.cpp
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

#include "PhotoEditorToolBar.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>

PhotoEditorToolBar::PhotoEditorToolBar(QWidget *parent)
        : QToolBar(parent)
        , exposureInfo{new QLabel(this)}
{
        addWidget(exposureInfo);
        addSeparator();

        auto action = new QAction(QIcon::fromTheme("gnome-util"),
                                  tr("Show effects sidebar"));
        action->setCheckable(true);
        addAction(action);

        addSeparator();

        // Zoom In
        action = new QAction(QIcon::fromTheme("zoom-in"),
                             tr("Zoom in browser thumbnails"));
        QObject::connect(action,
			 &QAction::triggered,
			 this,
		         &PhotoEditorToolBar::zoomIn);
        addAction(action);

        // Zoom Out
        action = new QAction(QIcon::fromTheme("zoom-out"),
                             tr("Zoom out browser thumbnails"));
        QObject::connect(action,
			 &QAction::triggered,
			 this,
		         &PhotoEditorToolBar::zoomOut);
        addAction(action);

        // Zoom to 1:1
        action = new QAction(QIcon::fromTheme("zoom-original"),
                             tr("Zoom to 1:1"));
        QObject::connect(action,
                         &QAction::triggered,
                         this,
                         &PhotoEditorToolBar::zoomOneToOne);
        addAction(action);

        // Zoom fit ti view
        action = new QAction(QIcon::fromTheme("zoom-fit-best"),
                             tr("Fit image to view"));
        QObject::connect(action,
                         &QAction::triggered,
                         this,
                         &PhotoEditorToolBar::zoomFit);
        addAction(action);
}

void PhotoEditorToolBar::setExposureInfo(const QString &info)
{
        exposureInfo->setText(info);
}
