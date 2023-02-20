/**
 * File name: PathLoader.h
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

#ifndef PATH_LOADER_H
#define PATH_LOADER_H

#include <QObject>

class PhotoItem;
class PathWorker;
class QThreadPool;

class PathLoader : public QObject
{
  Q_OBJECT

 public:

  explicit PathLoader(QObject *parent = nullptr);
  ~PathLoader();
  const QString& path() const;
  bool isRecursively() const;
  
 public slots:

  void setPath(const QString &path);
  void setRecursively(bool b = true);
  void stop();

 signals:
  void pathChanged(const QString &path);
  void photoAvailable(PhotoItem *item);

 private:
  QString rootPath;
  PathWorker *pathWorker;
  bool loadRecursively;
  QThreadPool *threadPool;
  bool isSetPath;
};

#endif // PATH_LOADER_H
