/**
 * File name: ImageData.h
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

#ifndef IMAGE_DATA_H
#define IMAGE_DATA_H

#include "DarkChamber.h"
#include "ImageDecoder.h"

class ImageData
{
 public:
        ImageData(const QString &path);
        const QString& path() const;
        QImage thumbnail() const;
        QImage image() const;
        double getISO() const;
        double getAperture() const;
        double getShutter() const;
        QString getExposureInfo() const;
        const std::filesystem::file_time_type& takenDate() const;
        
 private:
        std::unique_ptr<ImageDecoder> imageDecoder;
};

#endif // IMAGE_DATA_H
