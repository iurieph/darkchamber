/**
 * File name: PhotoModel.h
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

#ifndef PHOTO_MODEL_H
#define PHOTO_MODEL_H

#include "DarkChamber.h"

#include <QObject>

class QFileSystemModel;
class PhotoFactory;
class PhotoItem;

class PhotoModel: public QObject
{
        Q_OBJECT

public:
        explicit PhotoModel(QObject *parent = nullptr);
        void setLocationPath(const QString &path);
        PhotoItem* currentSelectedItem() const;
public slots:
        void addPhoto(PhotoItem *item);
        void deletePermanelty(const std::vector<PhotoItem*> &items, bool trash);
        void moveSelectedFiles(const QString destPath, bool copy = true);
        void protect(const std::vector<PhotoItem*> &items, bool p);
        void clearModel();

signals:
        void itemsAdded(const std::vector<PhotoItem*> &items);
        void itemsRemoved(const std::vector<PhotoItem*> &items);
        void modelCleared();
        void viewImage(PhotoItem *image);
        
private:
        QFileSystemModel *fileSystemModel;
        PhotoFactory *photoFactory;
        std::vector<PhotoItem*> photoItemList;
        bool isLoadingPhotos;
};

#endif // PHOTO_MODEL_H
