/**
 * File name: PhotoItem.h
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

#ifndef PHOTO_ITEM_H
#define PHOTO_ITEM_H

#include "DarkChamber.h"
#include "PhotoStateFile.h"
#include "ImageData.h"

#include <QObject>
#include <QImage>
#include <QStringList>

class PhotoItem: public QObject
{
        Q_OBJECT

public:
        explicit PhotoItem(const QString &path, QObject *parent = nullptr);
        static QStringList formats();
        const QImage &thumbnail() const;
        QImage image() const;
        const QString &path() const;
        void setRejected(bool b = true);
        bool isRejected() const;
        void setSelected(bool b = true);
        bool isSelected() const;
        bool deleteFile(bool trash = true);
        bool protect(bool p = true);
        bool isProtected() const;
        ImageData imageData() const;

signals:
        void photoRejected(bool b);
        void photoSelected(bool b);
        void viewImage(PhotoItem *image);

private:
        QString photoPath;
        QImage photoThumbnail;
        PhotoStateFile photoStateFile;
        bool photoIsSelected;
};

#endif // PHOTO_ITEM_H
