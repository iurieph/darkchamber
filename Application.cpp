/**
 * File name: Application.cpp
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

#include "Application.h"

DarkChamberApplication::DarkChamberApplication(int &argc, char **argv, int falgs)
        : QApplication(argc, argv, falgs)
        , m_thumbnailsSize(150, 150)
{
}

DarkChamberApplication* DarkChamberApplication::getAppInstance()
{
        return static_cast<DarkChamberApplication*>(QApplication::instance());
}

int DarkChamberApplication::guiSemResourceSize()
{
        return 10;
}

void DarkChamberApplication::releaseGuiSemaphore()
{
        guiSemaphore().release(guiSemResourceSize());
}

QSemaphore& DarkChamberApplication::guiSemaphore()
{
        return s_mainThreadSemophore;
}

QSemaphore DarkChamberApplication::s_mainThreadSemophore(DarkChamberApplication::guiSemResourceSize());

const QSize& DarkChamberApplication::thumbnailsSize() const
{
        return m_thumbnailsSize;
}
