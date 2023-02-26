/**
 * File name: DarkChamber.h
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

#ifndef DARKCHAMBER_H
#define DARKCHAMBER_H

#define DARKCHAMBER_VERSION 0x010000
#define DARKCHAMBER_NAME "DarkChamber"
#define DARKCHAMBER_VERSION_STRING "1.0.0"

#include <vector>
#include <utility>
#include <ranges>
#include <limits>
#include <chrono>
#include <filesystem>
#include <algorithm>

#include <QDebug>

#define DARKCHAMBER_LOG_DEBUG() qDebug() << __PRETTY_FUNCTION__ << "[DEBUG] "

#endif // DARKCHAMBER_H

