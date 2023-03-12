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
        , m_padding{2}
        , exposureInfo{new QGraphicsTextItem(photoItem->imageData()->getExposureInfo(), this)}
        , thumbnailImage{new QGraphicsPixmapItem(QPixmap::fromImage(photoItem->thumbnail()), this)}
{
        setFlag(QGraphicsItem::ItemIsSelectable);
        setSelected(photoItem->isSelected());       
        QObject::connect(photoItem,
                         &PhotoItem::photoSelected,
                         this,
                         &Thumbnail::selectItem);
        
        initUi();
}

void Thumbnail::initUi()
{
        auto exposureFont = exposureInfo->font();
        exposureFont.setPixelSize(12);
        exposureInfo->setFont(exposureFont);

        auto pixmap = thumbnailImage->pixmap();
        auto imageFrameWidth = std::max(thumbnailImage->boundingRect().width(),
                                        thumbnailImage->boundingRect().height());
        QRectF imageFrameRect(m_borderWidth + m_padding,
                              m_borderWidth + m_padding,
                              imageFrameWidth,
                              imageFrameWidth);
        thumbnailImage->setPos(imageFrameRect.topLeft()
                               + QPointF((imageFrameRect.width() - pixmap.width()) / 2,
                                         (imageFrameRect.height() - pixmap.height()) / 2));
        exposureInfo->setPos(imageFrameRect.bottomLeft() + QPointF(0, 2));
        setGeometry(0, 0,
                    imageFrameRect.width() + 2.0 * (m_borderWidth + m_padding),
                    imageFrameRect.height() + 2.0 * (m_borderWidth + m_padding)
                    + exposureInfo->boundingRect().height() + 4);
}

void Thumbnail::paint(QPainter *painter,
                      [[maybe_unused]]const QStyleOptionGraphicsItem *option,
                      [[maybe_unused]]QWidget *widget)
{
        auto pen = painter->pen();
        pen.setWidth(m_borderWidth);
        if (isSelected()) {
                pen.setColor({0, 0, 255});
                painter->fillRect(rect(), {200, 200, 200});
                thumbnailImage->setOpacity(0.8);
        } else {
                thumbnailImage->setOpacity(1.0);
                pen.setColor(Qt::gray);
        }
        painter->setPen(pen);
        painter->drawRect(rect());
}

void Thumbnail::selectItem(bool b)
{
        if (b != isSelected())
                setSelected(b);
}

void Thumbnail::setImageSize(const QSizeF &newSize)
{
        auto imageSize = thumbnailImage->pixmap().size();
        if (imageSize.width() >= imageSize.height())
                setScale(newSize.width() / imageSize.width());
        else
                setScale(newSize.height() / imageSize.height());
}

QSizeF Thumbnail::size() const
{
        return mapRectToParent(rect()).size();
}

double Thumbnail::width() const
{
        return mapRectToParent(rect()).width();
}

double Thumbnail::height() const
{
        //        dark_debug() << "H: " << thumbnailImage->pixmap().size().height();
        //        dark_debug() << "rect(): " << rect().height();
        return mapRectToParent(rect()).height();
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

void Thumbnail::resizeEvent([[maybe_unused]]QGraphicsSceneResizeEvent *event)
{
}
