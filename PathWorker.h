/**
 * File name: PathWorker.h
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

#ifndef PATH_WORKER_H
#define PATH_WORKER_H

#include "DarkChamber.h"

#include <QObject>
#include <QRunnable>

#include <atomic>

class PhotoItem;

class PathWorker : public QObject, public QRunnable
{
        Q_OBJECT
public:
        PathWorker(QObject *parent);
        void setPath(const QString path);
        void stop();
        void run() override;

signals:
        void photoAvailable(PhotoItem *item);
private:
        QString rootPath;
        std::atomic<bool> isRunning;
};

#endif // PATH_WORKER_H
