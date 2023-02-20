/**
 * File name: PhotoBrowser.h
 * Project: DarkChamber (A photo management software)
 *
 * Copyright (C) 2022 Iurie Nistor
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

#ifndef PHOTO_BROWSER_H
#define PHOTO_BROWSER_H

#include <QGraphicsView>

class PhotoBrowserModel;
class PhotoModel;

class PhotoBrowser : public QGraphicsView
{
        Q_OBJECT

public:
        explicit PhotoBrowser(PhotoModel *model, QWidget *parent = nullptr);

public slots:
        void nextItem();
        void previousItem();

protected:
        void resizeEvent(QResizeEvent *event) override;
        void keyPressEvent(QKeyEvent *event) override;
        void updateFilters();
        int getColumns() const;

private:
        QSize thumbnailSize;
        int thumbnailPadding;
        PhotoBrowserModel *browserModel;
};

#endif // PHOTO_BROWSER_H
