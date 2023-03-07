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

#include <QGraphicsScene>
#include <QSize>

class QGraphicsItem;
class QGraphicsScene;
class PhotoModel;
class PhotoFilter;
class Thumbnail;
class PhotoItem;

Q_DECLARE_OPAQUE_POINTER(PhotoItem*)

class PhotoBrowserModel : public QGraphicsScene
{
        Q_OBJECT

public:
        using ModelItem = std::pair<Thumbnail*, PhotoItem*>;
        using ModelItems = std::vector<ModelItem>;
        explicit PhotoBrowserModel(PhotoModel *model);
        void setThumbnailSize(const QSize &size);
        void setThumbnailPadding(int padding);
        void selectNext(int n = 1);
        void selectPrevious();
        void selectUp();
        void selectDown();
        void selectAll();
        void rejectSelectedItems();
        bool selectThumbnailAt(const QPointF &p);
        void deleteSelectedItems(bool trash = true);
        void protectSelectedItems(bool protect = true);
        void moveSelectedFiles(const QString &path, bool copy = true);
        bool isMultiSelect() const;

public slots:
        void addPhotoItems(const std::vector<PhotoItem*> &photoItems);
        void removePhotoItems(const std::vector<PhotoItem*> &photoItems);
        void clearModel();
        void updateColumns();
        void viewImage();
        void setMultiSelect(bool b = true);
//        void setFilters(const QList<PhotoFilter*> &filters);

protected:
        void updatePositions();
        int columnWidth() const;
        int rowHeight() const;
        int getRows() const;
        int getColumns(int width) const;
        int indexOf(const QGraphicsItem *item) const;
        int indexAt(const QPointF &pos) const;
        ModelItems::iterator findBySceneItem(const QGraphicsItem *item);
        ModelItems::iterator findByPhotoItem(const PhotoItem *item);
        const QSize& thumbnailSize() const;
        int thumbnailPadding() const;
        
        void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
        PhotoModel *photoModel;
        int m_beginSelectionIndex;
        int m_currentIndex;
        bool m_multiSelect;
        QSize m_thumbnailSize;
        double m_thumbnailPadding;
        int m_nColumns;
        //        std::vector<PhotoFilter> photoFilters;
        ModelItems modelItems;
};

#endif // PHOTO_BROWSER_MODEL_H
