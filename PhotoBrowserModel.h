/**
 * File name: PhotoBrowserModel.h
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

#ifndef PHOTO_BROWSER_MODEL_H
#define PHOTO_BROWSER_MODEL_H

#include "DarkChamber.h"

#include <QObject>
#include <QSize>

class QGraphicsItem;
class QGraphicsScene;
class PhotoModel;
class PhotoFilter;
class Thumbnail;
class PhotoItem;

Q_DECLARE_OPAQUE_POINTER(PhotoItem*)

class PhotoBrowserModel : public QObject
{
        Q_OBJECT

public:
        using ModelItem = std::pair<Thumbnail*, PhotoItem*>;
        using ModelItems = std::vector<ModelItem>;
        explicit PhotoBrowserModel(QGraphicsScene *parent, PhotoModel *model);
        void setThumbnailSize(const QSize &size);
        void setThumbnailPadding(int padding);
        void setColumns(int columns);
        void selectNext(bool direction = true, int n = 1);
        void selectPrevious();
        void selectUp();
        void selectDown();
        void rejectSelectedItems();
        bool selectItemAt(const QPointF &p);
        void deleteSelectedItems(bool trash = true);
        void protectSelectedItems(bool protect = true);
        void moveSelectedFiles(const QString &path, bool copy = true);

public slots:
        void addItems(const std::vector<PhotoItem*> &items);
        void removeItems(const std::vector<PhotoItem*> &items);
        void clear();
        void viewImage();
//        void setFilters(const QList<PhotoFilter*> &filters);

protected:
        void updatePositions();
        int getRows() const;
        int indexOf(const QGraphicsItem *item) const;
        ModelItems::iterator findBySceneItem(const QGraphicsItem *item);
        ModelItems::iterator findByPhotoItem(const PhotoItem *item);
        const QSize& thumbnailSize() const;
        int thumbnailPadding() const;

private:
        QGraphicsScene *modelScene;
        PhotoModel *photoModel;
        QSize m_thumbnailSize;
        int m_thumbnailPadding;
        int itemColumns;
        //        std::vector<PhotoFilter> photoFilters;
        ModelItems modelItems;
};

#endif // PHOTO_BROWSER_MODEL_H
