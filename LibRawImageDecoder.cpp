/**
 * File name: LibRawImage.cpp
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

#include "LibRawImageDecoder.h"

LibRawImageDecoder::LibRawImageDecoder(const QString &path)
        : ImageDecoder(path)
{
        //        DARKCHAMBER_LOG_DEBUG() << path;
}

std::unique_ptr<LibRaw> LibRawImageDecoder::getProcessor() const
{
        auto processor = std::make_unique<LibRaw>();
        if (processor->open_file(path().toStdString().c_str()) != LIBRAW_SUCCESS)
                return nullptr;
        return processor;
}


QImage LibRawImageDecoder::thumbnail() const
{        
        auto rawProcessor = getProcessor();
        if (!rawProcessor)
                return QImage();

//        if (!getImageInfo())
        //               setImageInfo(loadRawInfo(rawProcessor));

        if (rawProcessor->unpack_thumb() != LIBRAW_SUCCESS)
                return QImage();

        int width = rawProcessor->imgdata.thumbnail.twidth;
        int height = rawProcessor->imgdata.thumbnail.theight;
        auto thumb = QImage(width, height, QImage::Format_RGB888);
        thumb.loadFromData(QByteArray(rawProcessor->imgdata.thumbnail.thumb,
                                      rawProcessor->imgdata.thumbnail.tlength));
        return thumb;
}

QImage LibRawImageDecoder::image() const
{
        auto rawProcessor = getProcessor();
        if (!rawProcessor)
                return QImage();

//        if (!getImageInfo())
//                setImageInfo(loadRawInfo(rawProcessor));

        if (rawProcessor->unpack() != LIBRAW_SUCCESS)
                return QImage();

        if (rawProcessor->dcraw_process() != LIBRAW_SUCCESS)
                return QImage();

        auto image = rawProcessor->dcraw_make_mem_image();
        if (!image)
                return QImage();

        // TODO: use 16bit images.
        QImage img(image->data,
                   image->width,
                   image->height,
                   QImage::Format_RGB888,
                   [](void* img) {
                           LibRaw::dcraw_clear_mem(reinterpret_cast<decltype(image)>(img));
                   },
                   image);
        return img;
}

std::unique_ptr<RawImageInfo> LibRawImageDecoder::loadImageInfo()
{
        auto processor = getProcessor();
        if (!processor)
                return std::make_unique<RawImageInfo>();
        return loadRawInfo(processor);
}

std::unique_ptr<RawImageInfo> LibRawImageDecoder::loadRawInfo(const std::unique_ptr<LibRaw> &processor)
{
        auto info = std::make_unique<RawImageInfo>();
        info->setISO(processor->imgdata.other.iso_speed);
        info->setShutter(processor->imgdata.other.shutter);
        info->setAperture(processor->imgdata.other.aperture);
        
        auto seconds = std::chrono::seconds(processor->imgdata.other.timestamp);
        auto duration = std::chrono::duration_cast<std::filesystem::file_time_type::duration>(seconds);
        info->setTakenDate(std::filesystem::file_time_type(duration));
        
        return info;
}


