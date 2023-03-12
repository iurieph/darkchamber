/**
 * File name: PhotoBrowserModel.cpp
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

#include "PhotoBrowserModel.h"
#include "PhotoModel.h"
#include "PhotoItem.h"
#include "Thumbnail.h"

#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>

PhotoBrowserModel::PhotoBrowserModel(PhotoModel *model)
        : photoModel{model}
        , m_beginSelectionIndex{0}
        , m_currentIndex{0}
        , m_multiSelect{false}
        , m_thumbnailPadding{0.0}
        , m_nColumns{0}
{
        QObject::connect(photoModel,
                         &PhotoModel::itemsAdded,
                         this,
                         &PhotoBrowserModel::addPhotoItems);
        QObject::connect(photoModel,
                         &PhotoModel::itemsRemoved,
                         this,
                         &PhotoBrowserModel::removePhotoItems);
        QObject::connect(photoModel,
                         &PhotoModel::modelCleared,
                         this,
                         &PhotoBrowserModel::clearModel);
}

void PhotoBrowserModel::setThumbnailSize(const QSize &size)
{
        m_thumbnailSize = size;
        for (auto &item: modelItems)
                item.first->setImageSize(m_thumbnailSize);
        updateLayout();
}

void PhotoBrowserModel::setThumbnailPadding(int padding)
{        
        m_thumbnailPadding = padding;
        updateLayout();
}
  
bool PhotoBrowserModel::selectThumbnailAt(const QPointF &p)
{
        auto sceneItems = items(p);
        for (auto& sceneItem : sceneItems) {
                auto thumb = dynamic_cast<Thumbnail*>(sceneItem);
                if (thumb) {
                        thumb->setSelected(true);
                        return true;
                }
        }
        return false;
}

void PhotoBrowserModel::addPhotoItems(const std::vector<PhotoItem*> &photoItems)
{
        auto rejectedFilter = [](const PhotoItem *photoItem) -> bool {
                return !photoItem->isRejected();
        };

        for (const auto& photoItem: photoItems | std::views::filter(rejectedFilter)) {
                auto thumbnail = new Thumbnail(photoItem);
                thumbnail->setImageSize(m_thumbnailSize);
                auto newItem = std::make_pair(thumbnail, photoItem);
                auto pos = std::lower_bound(modelItems.begin(),
                                            modelItems.end(),
                                            newItem,
                                            [](const auto &i, const auto &newItem) {
                                                    return i.second->imageData()->takenDate()
                                                            > newItem.second->imageData()->takenDate();
                                            });
                if (modelItems.empty())
                        modelItems.push_back(newItem);
                else 
                        modelItems.insert(pos, newItem);
                addItem(thumbnail);
        }

        updateLayout();
}

void PhotoBrowserModel::removePhotoItems(const std::vector<PhotoItem*> &photoItems)
{
        for (const auto& photoItem: photoItems) {
                auto it = findByPhotoItem(photoItem);
                if (it != modelItems.end()) {
                        removeItem((*it).first);
                        delete (*it).first;
                        modelItems.erase(it);
                }
        }

        if (modelItems.empty())
                m_currentIndex = 0;
                
        updateLayout();
}

void PhotoBrowserModel::clearModel()
{
        clear();
        modelItems.clear();
        m_currentIndex = 0;
}

//void PhotoBrowserModel::setFilters(const std::vector<PhotoFilter> &filters)
//{
//}

void PhotoBrowserModel::updateLayout()
{
        if (views().empty())
                return;

        double xOffset = 0.0;
        double yOffset = 0.0;
        double rowHeight = 0.0;
        double sceneWidth = views().first()->rect().width();
        double sceneHeight = 0.0;
        int nColumns = 0;
        for (auto &item : modelItems) {
                m_nColumns = std::max(m_nColumns, nColumns);
                rowHeight = std::max(item.first->height() + m_thumbnailPadding, rowHeight);
                if (xOffset + item.first->width() + m_thumbnailPadding > sceneWidth) {
                        xOffset = 0.0;
                        yOffset += rowHeight;
                        rowHeight = 0.0;
                        nColumns = 0;
                }
                item.first->setPos(xOffset, yOffset);
                xOffset += item.first->width() + m_thumbnailPadding;
                sceneHeight = yOffset + item.first->height();
                nColumns++;
        }
        
        setSceneRect(QRect(0, 0, sceneWidth, sceneHeight));
        emit sceneRectChanged(sceneRect());
}

void PhotoBrowserModel::selectNext(int n)
{
    if (modelItems.empty())
        return;

    clearSelection();
    
    m_currentIndex = std::clamp(m_currentIndex + n,
                                0,
                                static_cast<int>(modelItems.size() - 1));
    if (isMultiSelect()) {
            auto startIndex = std::min(m_beginSelectionIndex, m_currentIndex);
            auto endIndex = std::max(m_beginSelectionIndex, m_currentIndex);
            for (decltype(startIndex) i = startIndex; i <= endIndex; i++)
                    modelItems[i].second->setSelected(true);
    } else {
            modelItems[m_currentIndex].second->setSelected(true);
    }

    for (auto &view : views()) {
            if (m_currentIndex >= 0 && m_currentIndex < static_cast<int>(modelItems.size()))
                    view->ensureVisible(modelItems[m_currentIndex].first);
    }
}

void PhotoBrowserModel::selectPrevious()
{
        selectNext(-1);
}

void PhotoBrowserModel::selectUp()
{
        selectNext(-m_nColumns);
}

void PhotoBrowserModel::selectDown()
{
        selectNext(m_nColumns);
}

void PhotoBrowserModel::selectAll()
{
        for (auto &item : modelItems)
                item.second->setSelected();
}

void PhotoBrowserModel::rejectSelectedItems()
{
        if (modelItems.empty())
                return;

        QGraphicsItem *sceneNextItem = nullptr;
        auto sceneSelectedItems = selectedItems();
        if (!sceneSelectedItems.isEmpty()) {
                auto sceneLastDeleteItem = sceneSelectedItems.last();
                auto i = indexOf(sceneLastDeleteItem);
                if (static_cast<decltype(modelItems.size())>(i + 1) < modelItems.size()) {
                        auto nextItem = modelItems[i + 1];
                        nextItem.second->setSelected(true);
                        sceneNextItem = nextItem.first;
                }
        }

        for (auto &sceneItem: sceneSelectedItems) {
                removeItem(sceneItem);
                auto it = findBySceneItem(sceneItem);
                if (it != modelItems.end()) {
                        (*it).second->setRejected();
                        modelItems.erase(it);
                }
                delete sceneItem;
        }
        
        updateLayout();
        if (sceneNextItem) {
                for (auto &view : views())
                        view->ensureVisible(sceneNextItem);
        }
}

void PhotoBrowserModel::deleteSelectedItems(bool trash)
{
        auto sceneSelectedItems = selectedItems();
        if (sceneSelectedItems.empty())
                return;

        std::vector<PhotoItem*> selectedItems;
        for (const auto &item: sceneSelectedItems) {
                auto thumb = dynamic_cast<Thumbnail*>(item);
                if (thumb)
                        selectedItems.emplace_back(thumb->getPhotoItem());
        }
        photoModel->deletePermanelty(selectedItems, trash);
        
}

void PhotoBrowserModel::moveSelectedFiles(const QString &path, bool copy)
{
        photoModel->moveSelectedFiles(path, copy);
}

PhotoBrowserModel::ModelItems::iterator
PhotoBrowserModel::findBySceneItem(const QGraphicsItem *item)
{
        return std::find_if(modelItems.begin(),
                                modelItems.end(),
                                [item](const ModelItem &i) { return i.first == item; });
}

PhotoBrowserModel::ModelItems::iterator
PhotoBrowserModel::findByPhotoItem(const PhotoItem *item)
{
        return std::find_if(modelItems.begin(),
                                modelItems.end(),
                                [item](const ModelItem &i) { return i.second == item; });
}

int PhotoBrowserModel::indexOf(const QGraphicsItem *item) const
{
        if (!item)
                return -1;
        
    auto it = std::find_if(modelItems.begin(), modelItems.end(),
                           [item](const auto& p) { return p.first == item; });
    if (it != modelItems.end())
            return std::distance(modelItems.begin(), it);

    return -1;
}

void PhotoBrowserModel::viewImage()
{
        auto sceneSelectedItems = selectedItems();
        if (!sceneSelectedItems.isEmpty())
                emit photoModel->viewImage(static_cast<Thumbnail*>(sceneSelectedItems.first())->getPhotoItem());
}

const QSize& PhotoBrowserModel::thumbnailSize() const
{
        return m_thumbnailSize;
}

int PhotoBrowserModel::thumbnailPadding() const
{
        return m_thumbnailPadding;
}

void PhotoBrowserModel::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
        auto index = indexAt(event->scenePos());
        if (index > -1) {
                m_currentIndex = index;
                m_beginSelectionIndex = m_currentIndex;
        }

        if (event->button() != Qt::LeftButton) {
                event->accept();
                return;
        }

        QGraphicsScene::mousePressEvent(event);
}

void PhotoBrowserModel::setMultiSelect(bool b)
{
        if (m_multiSelect != b) {
                m_beginSelectionIndex = std::clamp(m_currentIndex,
                                                   0,
                                                   static_cast<int>(modelItems.size() - 1));
                m_multiSelect = b;
                dkch_debug() << ": " << b;
        }
}

bool PhotoBrowserModel::isMultiSelect() const
{
        return m_multiSelect;
}

int PhotoBrowserModel::indexAt(const QPointF &pos) const
{
        auto sceneItems = items(pos);
        for (const auto& sceneItem: sceneItems) {
                auto thumbnail = dynamic_cast<Thumbnail*>(sceneItem);
                if (thumbnail)
                        return indexOf(thumbnail);
        }
        return -1;
}

