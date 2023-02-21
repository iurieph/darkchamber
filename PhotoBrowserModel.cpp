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
        , itemColumns{0}
        , thumbnailPadding{0}
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
        thumbnailSize = size;
        for (auto &item: modelItems)
                item.first->setGeometry(0, 0, thumbnailSize.width(), thumbnailSize.height());
        updatePositions();
}

void PhotoBrowserModel::setThumbnailPadding(int padding)
{        
        thumbnailPadding = padding;
        updatePositions();
}

void PhotoBrowserModel::setColumns(int columns)
{
        itemColumns = columns;
        updatePositions();
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
                sceneItem->setGeometry(0, 0, thumbnailSize.width(), thumbnailSize.height());
                modelItems.push_back({sceneItem, item});
                modelScene->addItem(sceneItem);
        }
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

void PhotoBrowserModel::updatePositions()
{
        if (itemColumns < 1)
                return;

        auto itemSize = QSize{thumbnailSize.width() + thumbnailPadding / 2,
                         thumbnailSize.height() + thumbnailPadding / 2};
        int nRows = getRows();
        for (int row = 0; row < nRows; row++) {
                for (int col = 0; col < itemColumns; col++) {
                        auto index = itemColumns * row + col;
                        if (index < modelItems.size()) {
                                auto sceneItem = modelItems[itemColumns * row + col].first;
                                sceneItem->setPos(col * itemSize.width(), row * itemSize.height());
                        }
                }
        }
        modelScene->setSceneRect(QRect(0, 0,
                                       itemColumns * itemSize.width(),
                                       nRows * itemSize.height()));
}

int PhotoBrowserModel::getRows() const
{
        if (itemColumns > 0)
                return modelItems.size() / itemColumns
                        + (modelItems.size() % itemColumns ? 1 : 0);
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
                        nextItem = (i + n < modelItems.size()) ? modelItems[i + n] : modelItems.back();
                else
                        nextItem = (i - n > -1) ? modelItems[i - n] : modelItems.front();
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
        selectNext(false, itemColumns);
}

void PhotoBrowserModel::selectDown()
{
        selectNext(true, itemColumns);
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
                if (i + 1 < modelItems.size()) {
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
        for (auto i = 0; i < modelItems.size(); i++) {
                if (modelItems[i].first == item)
                        return i;
        }
        return -1;
}

void PhotoBrowserModel::viewImage()
{
        auto sceneSelectedItems = modelScene->selectedItems();
        if (!sceneSelectedItems.isEmpty())
                emit photoModel->viewImage(static_cast<Thumbnail*>(sceneSelectedItems.first())->getPhotoItem());
}
