/**
 * File name: PathLoader.cpp
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

#include "PathLoader.h"
#include "PhotoItem.h"
#include "PathWorker.h"
#include "PhotoWorker.h"
#include "Application.h"

#include <QThreadPool>

PathLoader::PathLoader(QObject* parent)
        : QObject(parent)
        , pathWorker{new PathWorker(this)}
        , loadRecursively{false}
        , threadPool{QThreadPool::globalInstance()}
        , isSetPath{false}
{

        threadPool->setMaxThreadCount(threadPool->maxThreadCount() + 1);
        DARKCHAMBER_LOG_DEBUG() << "maxThreadCount: " << threadPool->maxThreadCount();

        pathWorker->setAutoDelete(false);
        QObject::connect(pathWorker,
                         &PathWorker::photoAvailable,
                         this,
                         &PathLoader::photoAvailable);
}

PathLoader::~PathLoader()
{
        pathWorker->stop();
        threadPool->clear();
        threadPool->waitForDone();
}

void PathLoader::stop()
{
        pathWorker->stop();
}

const QString& PathLoader::path() const
{
        return rootPath;
}

bool PathLoader::isRecursively() const
{
        return loadRecursively;
}
  
void PathLoader::setPath(const QString &path)
{
        rootPath = path;
        if (isSetPath)
                return;
        
        pathWorker->stop();
        threadPool->clear();
        DarkChamberApplication::releaseGuiSemaphore();
        threadPool->waitForDone();

        isSetPath = true;
        QApplication::processEvents();
        isSetPath = false;
        
        emit pathChanged(rootPath);

        DARKCHAMBER_LOG_DEBUG() << "set root path: " << rootPath;
        pathWorker->setPath(rootPath);
        threadPool->start(pathWorker);
}

void PathLoader::setRecursively(bool b)
{
        loadRecursively = b;
}
