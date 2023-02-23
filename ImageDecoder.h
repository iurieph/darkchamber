/**
 * File name: ImageDecoder.h
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

#ifndef IMAGE_DECODER_H
#define IMAGE_DECODER_H

#include "RawImageInfo.h"

#include <QImage>

class ImageDecoder
{
public:
        ImageDecoder(const QString &path);
        void setPath(const QString &path);
        const QString& path() const;
        virtual QImage thumbnail() const = 0;
        virtual QImage image() const = 0;
        const RawImageInfo* imageInfo();

protected:
        void setImageInfo(std::unique_ptr<RawImageInfo> info);
        RawImageInfo* getImageInfo() const;
        virtual std::unique_ptr<RawImageInfo> loadImageInfo() = 0;

 private:
        QString imagePath;
        std::unique_ptr<RawImageInfo> rawImageInfo;
};

#endif // IMAGE_DECODER_H
