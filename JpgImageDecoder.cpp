/**
 * File name: JpgImageDecoder.cpp
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

#include "JpgImageDecoder.h"
#include "Application.h"

#include <QImage>
#include <QFile>
#include <QImageReader>
#include <QDateTime>

JpgImageDecoder::JpgImageDecoder(const QString &path)
        : ImageDecoder(path)
{
        DARKCHAMBER_LOG_DEBUG() << path;
}

QImage JpgImageDecoder::thumbnail() const
{
        DARKCHAMBER_LOG_DEBUG() << path();
        auto size = DarkChamberApplication::getAppInstance()->thumbnailsSize();
        return QImage(path()).scaled(size,
                                     Qt::KeepAspectRatio/*,
                                                          Qt::SmoothTransformation*/);
}

QImage JpgImageDecoder::image() const
{
        return QImage(path());
}

std::unique_ptr<RawImageInfo> JpgImageDecoder::loadImageInfo()
{
        auto info = std::make_unique<RawImageInfo>();
        info->setTakenDate(takenDate());
        return info;
}

std::filesystem::file_time_type JpgImageDecoder::takenDate() const
{
        QFile file(path());
        if (!file.open(QIODevice::ReadOnly))
                return std::filesystem::file_time_type();

        QImageReader reader(&file);
        reader.setDecideFormatFromContent(true);
        if (!reader.canRead()) {
                file.close();
                return std::filesystem::file_time_type();
        }
        
        const QStringList keys = reader.textKeys();
        QDateTime creationTime;
        if (keys.contains("DateTimeOriginal")) {
                creationTime = QDateTime::fromString(reader.text("DateTime"), Qt::ISODate);
        } else if (keys.contains("DateTimeDigitized")) {
                creationTime = QDateTime::fromString(reader.text("DateTimeOriginal"), Qt::ISODate);
        } else if (keys.contains("DateTime")) {
                creationTime = QDateTime::fromString(reader.text("DateTimeDigitized"), Qt::ISODate);
        } else {
                file.close();
                return std::filesystem::file_time_type();
        }

        file.close();
        auto seconds = std::chrono::seconds(creationTime.toSecsSinceEpoch());   
        auto duration = std::chrono::duration_cast<std::filesystem::file_time_type::duration>(seconds);
        return std::filesystem::file_time_type(duration);
}
