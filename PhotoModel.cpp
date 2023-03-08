/**
 * File name: PhotoModel.cpp
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

#include "PhotoModel.h"
#include "PhotoItem.h"
#include "Application.h"

PhotoModel::PhotoModel(QObject *parent)
        : QObject(parent)
{
}

void PhotoModel::addPhoto(PhotoItem *item)
{
        item->setParent(this);
        photoItemList.emplace_back(item);
        QObject::connect(item,
                         &PhotoItem::viewImage,
                         this,
                         &PhotoModel::viewImage);
        emit itemsAdded({item});
        emit addedPhotos(photoItemList.size());
        DarkChamberApplication::guiSemaphore().release();
}

void PhotoModel::deletePermanelty(const std::vector<PhotoItem*> &items, bool trash)
{
        if (items.empty())
                return;
        std::vector<PhotoItem*> removedItems;
        for (auto &item: items) {
                if (item->deleteFile(trash)) {
                        auto it = std::find_if(photoItemList.begin(),
                                               photoItemList.end(),
                                               [item](const auto &i) { return i == item; });
                        if (it != photoItemList.end()) {
                                photoItemList.erase(it);
                                removedItems.emplace_back(item);
                        }
                }
        }
        
        if (!removedItems.empty()) {
                emit itemsRemoved(removedItems);
                for (auto &item: removedItems)
                        delete item;
        }
}

void PhotoModel::moveSelectedFiles(const QString destPath, bool copy)
{
        std::vector<PhotoItem*> removedItems;
        for (auto it = photoItemList.begin(); it != photoItemList.end();) {
                if ((*it)->isSelected() && (*it)->moveFile(destPath, copy)) {
                        if (!copy)
                                removedItems.emplace_back(*it);
                        it = photoItemList.erase(it);
                } else {
                        ++it;
                }
        }
        
        if (!removedItems.empty()) {
                emit itemsRemoved(removedItems);
                for (auto &item: removedItems)
                        delete item;
        }
}

void PhotoModel::protect(const std::vector<PhotoItem*> &items, bool p)
{
        if (items.empty())
                return;
        for (auto &item: items)
                item->protect(p);
        
}

void PhotoModel::clearModel()
{
        for (auto &item: photoItemList)
                delete item;
        photoItemList.clear();
        emit modelCleared();
}

PhotoItem* PhotoModel::currentSelectedItem() const
{
        for (const auto &item: photoItemList) {
                if (item->isSelected())
                        return item;
        }
        return nullptr;
}
