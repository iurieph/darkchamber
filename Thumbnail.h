/**
 * File name: Thumbnail.h
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

#ifndef PHOTO_THUMBNAIL_H
#define PHOTO_THUMBNAIL_H

#include <QGraphicsWidget>

class QPainter;
class QStyleOptionGraphicsItem;
class PhotoItem;
class QGraphicsTextItem;
class QGraphicsImageItem;

class Thumbnail : public QGraphicsWidget
{
        Q_OBJECT

public:
        explicit Thumbnail(PhotoItem *item);
        PhotoItem* getPhotoItem() const;
        void setSize(const QSize &size);

protected slots:
        void selectItem(bool b = true);

protected:
        void paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option,
                   QWidget *widget = nullptr) override;
        void drawFileName(int yOffset, QPainter *painter);
        QVariant itemChange(QGraphicsItem::GraphicsItemChange change,
                            const QVariant &value) override;
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
        void resizeEvent(QGraphicsSceneResizeEvent *event) override;
private:
        PhotoItem *photoItem;
        QString myId;
        int m_borderWidth;
        int m_padding;
        QGraphicsTextItem *m_exposureInfo;
        QGraphicsPixmapItem *m_thumbnailImage;
};

#endif // PHOTO_THUMBNAIL_H
