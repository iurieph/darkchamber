/**
 * File name: RawImageInfo.cpp
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

#include "RawImageInfo.h"

RawImageInfo::RawImageInfo()
        : isoVal{-1.0}
        , apertureVal{-1.0}
        , shutterVal{-1.0}        
{
}

void RawImageInfo::setISO(double val)
{
        isoVal = val;
}

void RawImageInfo::setAperture(double val)
{
        apertureVal = val;
}

void RawImageInfo::setShutter(double val)
{
        shutterVal = val;
}

double RawImageInfo::getISO() const
{
        return isoVal;
}

double RawImageInfo::getAperture() const
{
        return apertureVal;
}

double RawImageInfo::getShutter() const
{
        return shutterVal;
}

void RawImageInfo::setTakenDate(const std::filesystem::file_time_type& date)
{
        m_takenDate = date;
}

const std::filesystem::file_time_type& RawImageInfo::takenDate() const
{
        return m_takenDate;
}
        

