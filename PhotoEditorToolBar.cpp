/**
 * File name: PhotoEditorToolBar.h
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
{
        // Basic exprosure info label
        addWidget(new QLabel("ISO 32000   f/8   1/250   1000mm"));

        addSeparator();

        // Image info action
        auto list = QIcon::themeSearchPaths();
        DARKCHAMBER_LOG_DEBUG() << "Theme name: " << QIcon::themeName();
        for (const auto &p: list)
                DARKCHAMBER_LOG_DEBUG() << p;
        
        auto photoInfoAct = new QAction(QIcon::fromTheme("photoeditortoolbar-imageinfo"),
                                        tr("Image info"));
        photoInfoAct->setCheckable(true);
        addAction(photoInfoAct);
}
