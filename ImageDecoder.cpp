/**
 * File name: ImageDecoder.cpp
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

#include "ImageDecoder.h"

ImageDecoder::ImageDecoder(const QString &path)
        : imagePath{path}
        , rawImageInfo{nullptr}
{
}

void ImageDecoder::setPath(const QString &path)
{
        imagePath = path;
}

const QString& ImageDecoder::path() const
{
        return imagePath;
}

const RawImageInfo* ImageDecoder::imageInfo()
{
        if (!getImageInfo()) {
                auto img = loadImageInfo();
                setImageInfo(std::move(img));
        }
        return rawImageInfo.get();
}

void ImageDecoder::setImageInfo(std::unique_ptr<RawImageInfo> info)
{
        rawImageInfo = std::move(info);
}

RawImageInfo* ImageDecoder::getImageInfo() const
{
        return rawImageInfo.get();
}
