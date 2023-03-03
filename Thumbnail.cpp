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
#include "Application.h"

#include <QPainter>
#include <QFileInfo>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsLinearLayout>

Thumbnail::Thumbnail(PhotoItem *item)
        : QGraphicsWidget(nullptr)
        , photoItem{item}
        , m_borderWidth{1}
        , m_padding{1}
        , m_exposureInfo{new QGraphicsTextItem(photoItem->imageData()->getExposureInfo(), this)}
        , m_thumbnailImage{new QGraphicsPixmapItem(QPixmap::fromImage(photoItem->thumbnail()), this)}
{
        setSize(DarkChamberApplication::getAppInstance()->thumbnailsSize());        
        setFlag(QGraphicsItem::ItemIsSelectable);
        setSelected(photoItem->isSelected());
        
        QObject::connect(photoItem,
                         &PhotoItem::photoSelected,
                         this,
                         &Thumbnail::selectItem);
        
        updatePositions();
}

void Thumbnail::updatePositions()
{
        m_thumbnailImage->setPos((rect().width() - m_thumbnailImage->boundingRect().width()) / 2,
                                 m_borderWidth + m_padding);
        if (m_exposureInfo->boundingRect().width() > rect().width() - 2 * (m_borderWidth + m_padding))
                m_exposureInfo->setScale(static_cast<double>(rect().width() - 2 * (m_borderWidth + m_padding)) / m_exposureInfo->boundingRect().width());
        m_exposureInfo->setPos(m_borderWidth + m_padding,
                               m_borderWidth + m_padding + m_thumbnailImage->boundingRect().height() + 2);
}

void Thumbnail::paint(QPainter *painter,
                      [[maybe_unused]]const QStyleOptionGraphicsItem *option,
                      [[maybe_unused]]QWidget *widget)
{
        auto pen = painter->pen();
        pen.setWidth(m_borderWidth);
        if (isSelected())
                pen.setColor({0, 0, 255});
        else
                pen.setColor(Qt::gray);
        painter->setPen(pen);
        painter->drawRect(rect());
}

void Thumbnail::drawFileName(int yOffset, QPainter *painter)
{
//        painter->drawText(QRectF(0, yOffset, rect().width(), 20/*painter->font().pixelSize()*/),
//                          Qt::AlignHCenter, QFileInfo(photoItem->path()).fileName());
}

void Thumbnail::selectItem(bool b)
{
        if (b != isSelected())
                setSelected(b);
}

void Thumbnail::setSize(const QSize &size)
{
        auto currentGeom = geometry();
        setGeometry(currentGeom.x(),
                    currentGeom.y(),
                    size.width() + 2 * (m_borderWidth + m_padding),
                    size.height() + 2 * (m_borderWidth + m_padding));

        auto exposureFont = m_exposureInfo->font();
        exposureFont.setPixelSize(12);
        m_exposureInfo->setFont(exposureFont);
       
        auto pm = QPixmap::fromImage(photoItem->thumbnail());
        if (pm.size() != size) {
                pm = pm.scaled(rect().width() - 2 * (m_borderWidth + m_padding) + 1,
                               rect().height() - m_exposureInfo->boundingRect().height() - 3);
        }
        m_thumbnailImage->setPixmap(pm);
        updatePositions();
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

void Thumbnail::resizeEvent(QGraphicsSceneResizeEvent *event)
{
        int yOffset = m_borderWidth + m_padding;
        m_thumbnailImage->setPos(m_borderWidth + m_padding, yOffset);
        yOffset += m_thumbnailImage->boundingRect().height();
        m_exposureInfo->setPos(m_borderWidth + m_padding, yOffset);
}
