/**
 * File name: Thumbnail.cpp
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

#include "Thumbnail.h"
#include "PhotoItem.h"

#include <QPainter>

Thumbnail::Thumbnail(PhotoItem *item)
        : QGraphicsWidget(nullptr)
        , photoItem{item}
{
        setFlag(QGraphicsItem::ItemIsSelectable);
        setSelected(photoItem->isSelected());
        QObject::connect(photoItem,
                         &PhotoItem::photoSelected,
                         this,
                         &Thumbnail::selectItem);
}

void Thumbnail::paint(QPainter *painter,
                      [[maybe_unused]]const QStyleOptionGraphicsItem *option,
                      [[maybe_unused]]QWidget *widget)
{
        if (photoItem->thumbnail().isNull())
                return;
        int x = (rect().width() - photoItem->thumbnail().width()) / 2;
        int y = rect().height() - photoItem->thumbnail().height();
        painter->drawImage(x, y, photoItem->thumbnail());
        auto pen = painter->pen();
        pen.setWidth(2);
        pen.setColor({255, 255, 255});
        painter->setPen(pen);

        if (isSelected()) {
                auto pen = painter->pen();
                pen.setWidth(2);
                pen.setColor({0, 0, 255});
                painter->setPen(pen);
                painter->drawRect(rect());
        }
}

void Thumbnail::selectItem(bool b)
{
        if (b != isSelected())
                setSelected(b);
}

QVariant Thumbnail::itemChange(QGraphicsItem::GraphicsItemChange change,
                               const QVariant &value)
{
        if (change == QGraphicsItem::ItemSelectedHasChanged)
                photoItem->setSelected(value.toBool());
        return QGraphicsWidget::itemChange(change, value);
}

PhotoItem* Thumbnail::getPhotoItem() const
{
        return photoItem;
}

void Thumbnail::mouseDoubleClickEvent([[maybe_unused]]QGraphicsSceneMouseEvent *event)
{
        emit photoItem->viewImage(photoItem);
}
