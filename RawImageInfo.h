/**
 * File name: RawImageInfo.h
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

#ifndef RAW_IMAGE_INFO_H
#define RAW_IMAGE_INFO_H

#include "DarkChamber.h"

class RawImageInfo
{
public:
        RawImageInfo();
        void setISO(double val);
        void setAperture(double val);
        void setShutter(double val);
        double getISO() const;
        double getAperture() const;
        double getShutter() const;
        void setTakenDate(const std::filesystem::file_time_type& date);
        const std::filesystem::file_time_type& takenDate() const;
private:
        double isoVal;
        double apertureVal;
        double shutterVal;
        std::filesystem::file_time_type m_takenDate;
};

#endif // RAW_IMAGE_INFO
