/**
 * File name: PhotoItem.cpp
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

#include "PhotoItem.h"
#include "ImageData.h"

#include <QImage>
#include <QFile>
#include <QFileInfo>
#include <QDir>

PhotoItem::PhotoItem(const QString &path, QObject *parent)
        : QObject(parent)
        , photoPath{path}
        , fileName{QFileInfo(path).fileName()}
        , photoStateFile{PhotoStateFile(photoPath)}
        , m_imageData{std::make_unique<ImageData>(path)}
        , photoThumbnail{m_imageData->thumbnail()}
        , photoIsSelected{false}
{
}

QStringList PhotoItem::formats()
{
        return {"*.jpg", "*.png", "*.cr3"};
}

const QImage& PhotoItem::thumbnail() const
{
        return photoThumbnail;
}

QImage PhotoItem::image() const
{
        return ImageData(path()).image();
}

const QString& PhotoItem::path() const
{
        return photoPath;
}

const QString& PhotoItem::photoName() const
{
        return fileName;
}

void PhotoItem::setRejected(bool b)
{
        if (b != photoStateFile.isRejected()) {
                photoStateFile.setRejected(b);
                emit photoRejected(b);
        }
}

bool PhotoItem::isRejected() const
{
        return photoStateFile.isRejected();
}

void PhotoItem::setSelected(bool b)
{
        if ( photoIsSelected != b) {
                photoIsSelected = b;
                emit photoSelected(b);
        }
}

bool PhotoItem::isSelected() const
{
        return photoIsSelected;
}

bool PhotoItem::protect(bool p)
{
        if (isProtected())
                return QFile::setPermissions(path(), QFileDevice::ReadOwner);
        return true;
}

bool PhotoItem::isProtected() const
{
        QFileInfo info(path());
        return !info.isWritable();
}

bool PhotoItem::deleteFile(bool trash)
{
        if (isProtected())
                return false;
                
        if (trash)
                //QFile::moveToTrash(path());
                return false;
        else
                return QFile::remove(path());
}

bool PhotoItem::moveFile(const QString &destPath, bool copy)
{
        if (!copy && isProtected())
                return false;

        auto destFile = destPath
                + QDir::separator()
                + QFileInfo(path()).fileName();
        if (copy)
                return QFile::copy(path(), destFile);
        else
                return QFile::rename(path(), destFile);
}

const ImageData* PhotoItem::imageData() const
{
        return m_imageData.get();
}
