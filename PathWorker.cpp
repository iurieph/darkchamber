/**
 * File name: PathWorker.cpp
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
 * This program is distributed in the hope that it wil be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "PathWorker.h"
#include "Application.h"
#include "PhotoWorker.h"
#include "PhotoItem.h"

#include <QDirIterator>
#include <QThreadPool>

PathWorker::PathWorker(QObject *parent)
        : QObject(parent)
        , isRunning{true}
{
}

void PathWorker::setPath(const QString path)
{
        isRunning = true;
        rootPath = path;
}

void PathWorker::stop()
{
        isRunning = false;
}

void PathWorker::run()
{
        QDirIterator it(rootPath, QDir::NoDotAndDotDot | QDir::Files);
        while (isRunning && it.hasNext()) {
                auto file = it.fileInfo();
                if (PhotoItem::formats().contains(file.suffix())) {
                        DarkChamberApplication::guiSemaphore().acquire();
                        auto thumbWorker = new PhotoWorker(file.filePath());
                        QObject::connect(thumbWorker,
                                         &PhotoWorker::photoAvailable,
                                         this,
                                         &PathWorker::photoAvailable);
                        QThreadPool::globalInstance()->start(thumbWorker);
                }
                it.next();
        }
}


