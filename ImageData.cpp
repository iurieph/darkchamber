/**
 * File name: ImageData.cpp
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

#include "ImageData.h"

#include "DefaultImageDecoder.h"
#include "LibRawImageDecoder.h"
#include "JpgImageDecoder.h"
#include "Application.h"

#include <QFileInfo>

ImageData::ImageData(const QString &path)
        : imageDecoder{nullptr}
{
        DARKCHAMBER_LOG_DEBUG() << path;
        QFileInfo info(path);
        if (info.suffix().toUpper() == "CR3")
                imageDecoder = std::make_unique<LibRawImageDecoder>(path);
        else if (info.suffix().toUpper() == "JPG" || info.suffix().toUpper() == "JPEG")
                imageDecoder = std::make_unique<JpgImageDecoder>(path);
        else
                imageDecoder = std::make_unique<DefaultImageDecoder>(path);
}

const QString& ImageData::path() const
{
        return imageDecoder->path();
}

QImage ImageData::thumbnail() const
{
        DARKCHAMBER_LOG_DEBUG() << imageDecoder->path(); 
        auto img = imageDecoder->thumbnail();
        if (!img.isNull() && img.size().isValid()) {
                auto imgSize = DarkChamberApplication::getAppInstance()->thumbnailsSize();
                return imageDecoder->thumbnail().scaled(imgSize,
                                                        Qt::KeepAspectRatioByExpanding/*,
                                                                             Qt::SmoothTransformation*/);
        }
        return QImage();
}

QImage ImageData::image() const
{
        return imageDecoder->image();
}

double ImageData::getISO() const
{
        return imageDecoder->imageInfo()->getISO();
}

double ImageData::getAperture() const
{
        if (imageDecoder->imageInfo())
                return imageDecoder->imageInfo()->getAperture();
        return 0;
}

double ImageData::getShutter() const
{
        return imageDecoder->imageInfo()->getShutter();
}

QString ImageData::getExposureInfo() const
{
        auto rawInfo = imageDecoder->imageInfo();
        QString isoStr;
        if (rawInfo->getISO() < 0.0)
                isoStr = "-";
        else
                isoStr = QString::number(rawInfo->getISO(), 'f', 0);

        QString apertureStr;
        if (rawInfo->getAperture() < 0.0)
                apertureStr = "-";
        else
                apertureStr = QString::number(rawInfo->getAperture(), 'f', 1);

        QString shutterStr;
        if (rawInfo->getShutter() < 0.0
            || (rawInfo->getShutter() - 0.0) < std::numeric_limits<double>::epsilon()) {
                shutterStr = "-";
        } else {
                shutterStr = QString::number( 1.0 / rawInfo->getShutter(), 'f', 0);
        }

        return QString("ISO %1  f/%2  1/%3").arg(isoStr).arg(apertureStr).arg(shutterStr);
}

const std::filesystem::file_time_type& ImageData::takenDate() const
{
        return imageDecoder->imageInfo()->takenDate();
}
