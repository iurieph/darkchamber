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
        /*        auto rawProcessor = std::make_unique<LibRaw>();
        if (rawProcessor->open_file(path().toStdString().c_str()) != LIBRAW_SUCCESS)
                return QImage();

        if (rawProcessor->unpack() != LIBRAW_SUCCESS)
                return QImage();

        auto channels = rawProcessor->imgdata.idata.colors;
        QImage img(rawProcessor->imgdata.sizes.iwidth, rawProcessor->imgdata.sizes.iheight, QImage::Format_ARGB32);
        rawProcessor->raw2image();
        for (auto y = 0; y < img.height(); y++) {
                auto line = static_cast<QRgb*>(img.scanLine(y));
                for (auto x = 0; x < img.width(); x++) {
                        int r = rawProcessor->imgdata.image[y][x][0];
                        int g = rawProcessor->imgdata.image[y][x][1];
                        int b = rawProcessor->imgdata.image[y][x][2];
                        int a = (channels == 4) ? rawProcessor->imgdata.image[y][x][3] : 255;
                        line[x] = qRgba(r, g, b, a);
                }
        }


        int width = rawProcessor->imgdata.sizes.iwidth;
        int height = rawProcessor->imgdata.sizes.iheight;
        auto img = QImage(width, height, QImage::Format_ARGB32);
        rawProcessor->raw2image();
        for (auto y = 0; y < height; y++) {
                for (atuo x = 0; x < width; x++) {
                        img.setPixelColor(x, y, QColor(iProcessor.imgdata.image[i][0],
                                                       iProcessor.imgdata.image[i][1],
                                                       iProcessor.imgdata.image[i][2],
                                                       iProcessor.imgdata.image[i][3]));
                }
        }

        QImage img(rawProcessor->imgdata.width, rawProcessor->imgdata.height, QImage::Format_ARGB32);
        for (auto y = 0; y < image.height(); y++) {
                auto line = static_cast<QRgb*>(img.scanLine(y));
                for (auto x = 0; x < image.width(); x++) {
                        int r = rawProcessor->imgdata.image[y][x][0];
                        int g = rawProcessor->imgdata.image[y][x][1];
                        int b = rawProcessor->imgdata.image[y][x][2];
                                        int a = (channels == 4) ? rawProcessor->imgdata.image[y][x][3] : 255;
                                        line[x] = qRgba(r, g, b, a);
                }
                }*/
        return QImage();

        //        return img;
}
