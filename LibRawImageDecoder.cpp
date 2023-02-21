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

#include "libraw/libraw.h"

LibRawImageDecoder::LibRawImageDecoder(const QString &path)
        : ImageDecoder(path)
{
}

QImage LibRawImageDecoder::thumbnail() const
{        
        auto rawProcessor = std::make_unique<LibRaw>();
        if (rawProcessor->open_file(path().toStdString().c_str()) != LIBRAW_SUCCESS)
                return QImage();

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
        auto rawProcessor = std::make_unique<LibRaw>();
        if (rawProcessor->open_file(path().toStdString().c_str()) != LIBRAW_SUCCESS)
                return QImage();

        if (rawProcessor->unpack() != LIBRAW_SUCCESS)
                return QImage();

        if (rawProcessor->dcraw_process() != LIBRAW_SUCCESS)
                return QImage();

        auto image = rawProcessor->dcraw_make_mem_image();
        if (!image)
                return QImage();
                
        QImage img(image->width, image->height, QImage::Format_RGB888);
        img.loadFromData(image->data, image->data_size);
        LibRaw::dcraw_clear_mem(image);
        return img;
}
