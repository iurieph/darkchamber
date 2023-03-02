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

#include <QGraphicsScene>
#include <QGraphicsView>

PhotoBrowserModel::PhotoBrowserModel(QGraphicsScene *parent, PhotoModel *model)
        : QObject{parent}
        , modelScene{parent}
        , photoModel{model}
        , m_thumbnailPadding{0}
        , m_nColumns{0}
{
        QObject::connect(photoModel,
                         &PhotoModel::itemsAdded,
                         this,
                         &PhotoBrowserModel::addItems);
        QObject::connect(photoModel,
                         &PhotoModel::itemsRemoved,
                         this,
                         &PhotoBrowserModel::removeItems);
        QObject::connect(photoModel,
                         &PhotoModel::modelCleared,
                         this,
                         &PhotoBrowserModel::clear);
}

void PhotoBrowserModel::setThumbnailSize(const QSize &size)
{
        m_thumbnailSize = size;
        for (auto &item: modelItems)
                item.first->setSize(m_thumbnailSize);
        updateColumns();
}

void PhotoBrowserModel::setThumbnailPadding(int padding)
{        
        m_thumbnailPadding = padding;
        updateColumns();
}
  
bool PhotoBrowserModel::selectItemAt(const QPointF &p)
{
        auto items = modelScene->items(p);
        if (!items.isEmpty()) {
                auto thumb = dynamic_cast<Thumbnail*>(items.first());
                if (thumb) {
                        thumb->setSelected(true);
                        return true;
                }
        }
        return false;
}

void PhotoBrowserModel::addItems(const std::vector<PhotoItem*> &items)
{
        auto rejectedFilter = [](const PhotoItem *item) -> bool { return !item->isRejected(); };
        for (const auto& item: items | std::views::filter(rejectedFilter)) {
                auto sceneItem = new Thumbnail(item);
                sceneItem->setSize(m_thumbnailSize);
                auto newItem = std::make_pair(sceneItem, item);
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
                modelScene->addItem(sceneItem);
        }

        if (modelItems.size() == 1)
                updateColumns();
        else
                updatePositions();
}

void PhotoBrowserModel::removeItems(const std::vector<PhotoItem*> &items)
{
        DARKCHAMBER_LOG_DEBUG();
        for (const auto& item: items) {
                auto it = findByPhotoItem(item);
                if (it != modelItems.end()) {
                        modelScene->removeItem((*it).first);
                        delete (*it).first;
                        modelItems.erase(it);
                }
        }
        updatePositions();
}

void PhotoBrowserModel::clear()
{
        modelScene->clear();
        modelItems.clear();
}

//void PhotoBrowserModel::setFilters(const std::vector<PhotoFilter> &filters)
//{
//}

void PhotoBrowserModel::updateColumns()
{
        int nColumns = getColumns(columnWidth());
        DARKCHAMBER_LOG_DEBUG() << "columnWidth(): " << columnWidth();
        DARKCHAMBER_LOG_DEBUG() << "nColumns" << nColumns;
        if (m_nColumns != nColumns) {
                m_nColumns = nColumns;
                updatePositions();
                return;
        }
        
        // The QGraphicsScene caches the scene rect.
        emit modelScene->sceneRectChanged(modelScene->sceneRect());
}

void PhotoBrowserModel::updatePositions()
{
        if (m_nColumns < 1)
                return;
        
        auto itemSize = QSizeF(columnWidth(), rowHeight());
        DARKCHAMBER_LOG_DEBUG() << itemSize.width() << "|" <<  itemSize.height();
        int nRows = getRows();
        for (int row = 0; row < nRows; row++) {
                for (int col = 0; col < m_nColumns; col++) {
                        decltype(modelItems.size()) index = m_nColumns * row + col;
                        if (index < modelItems.size()) {
                                auto sceneItem = modelItems[m_nColumns * row + col].first;
                                sceneItem->setPos(col * itemSize.width(), row * itemSize.height());
                        }
                }
        }
        modelScene->setSceneRect(QRect(0, 0,
                                       m_nColumns * itemSize.width(),
                                       nRows * itemSize.height()));
}

int PhotoBrowserModel::columnWidth() const
{
        if (modelItems.empty())
                return 0;
        return modelItems.front().first->boundingRect().width() + thumbnailPadding() / 2;
}

int PhotoBrowserModel::rowHeight() const
{
        if (modelItems.empty())
                return 0;
        return modelItems.front().first->boundingRect().height() + thumbnailPadding() / 2;
}

int PhotoBrowserModel::getColumns(int width) const
{
        if (width == 0)
                return 0;
        
        auto viewWidth = modelScene->views().first()->width();
        if (viewWidth < width)
                return 1;
        return viewWidth / width;
}

int PhotoBrowserModel::getRows() const
{
        if (m_nColumns > 0)
                return modelItems.size() / m_nColumns
                        + (modelItems.size() % m_nColumns ? 1 : 0);
        return 0;
}

void PhotoBrowserModel::selectNext(bool direction, int n)
{
        if (modelItems.empty())
                return;
        
        auto sceneSelectedItems = modelScene->selectedItems();
        if (sceneSelectedItems.isEmpty()) {
                auto item = direction ? modelItems.front() : modelItems.back();
                item.second->setSelected(true);
                return;
        }

        auto sceneSelectedItem = direction ? sceneSelectedItems.first() : sceneSelectedItems.last();
        if (modelItems.size() < 2)
                return;

        auto i = indexOf(sceneSelectedItem);
        if (i > -1) {
                ModelItem nextItem;
                if (direction)
                        nextItem = (static_cast<decltype(modelItems.size())>(i + n)
                                    < modelItems.size()) ? modelItems[i + n] : modelItems.back();
                else
                        nextItem = static_cast<decltype(modelItems.size())>(i - n > -1)
                                ? modelItems[i - n] : modelItems.front();
                static_cast<Thumbnail*>(sceneSelectedItem)->getPhotoItem()->setSelected(false);
                nextItem.second->setSelected(true);
                for (auto &view : modelScene->views())
                        view->ensureVisible(nextItem.first);
        }        
}

void PhotoBrowserModel::selectPrevious()
{
        selectNext(false, 1);
}

void PhotoBrowserModel::selectUp()
{
        selectNext(false, m_nColumns);
}

void PhotoBrowserModel::selectDown()
{
        selectNext(true, m_nColumns);
}

#include <QDebug>

void PhotoBrowserModel::rejectSelectedItems()
{
        if (modelItems.empty())
                return;

        QGraphicsItem *sceneNextItem = nullptr;
        auto sceneSelectedItems = modelScene->selectedItems();
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
                modelScene->removeItem(sceneItem);
                auto it = findBySceneItem(sceneItem);
                if (it != modelItems.end()) {
                        (*it).second->setRejected();
                        modelItems.erase(it);
                }
                delete sceneItem;
        }
        updatePositions();
        if (sceneNextItem) {
                for (auto &view : modelScene->views())
                        view->ensureVisible(sceneNextItem);
        }
}

void PhotoBrowserModel::deleteSelectedItems(bool trash)
{
        auto sceneSelectedItems = modelScene->selectedItems();
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
    auto it = std::find_if(modelItems.begin(), modelItems.end(),
                           [item](const auto& p) { return p.first == item; });

    if (it != modelItems.end())
            return std::distance(modelItems.begin(), it);
    return -1;
}

void PhotoBrowserModel::viewImage()
{
        auto sceneSelectedItems = modelScene->selectedItems();
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
