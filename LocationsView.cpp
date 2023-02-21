/**
 * File name: LocationView.cpp
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

#include "LocationsView.h"

#include <QTreeView>
#include <QFileSystemModel>
#include <QVBoxLayout>
#include <QDebug>

LocationsView::LocationsView(QWidget *parent)
  : QDockWidget(tr("Collections"), parent)
  , treeView{new QTreeView(this)}
{
        setWidget(treeView);
        QObject::connect(treeView, &QTreeView::doubleClicked,
                         this,
                         [&](const QModelIndex &index) {
                                 auto filePath = dynamic_cast<QFileSystemModel*>(treeView->model())->filePath(index);
                                 DARKCHAMBER_LOG_DEBUG() << "location path: " << filePath;
                                 emit pathSelected(filePath);
                         });  
        treeView->setAnimated(true);
}

void LocationsView::setModel(QFileSystemModel *model)
{
        treeView->setModel(model);
        treeView->setColumnHidden(1, true);
        treeView->setColumnHidden(2, true);
        treeView->setColumnHidden(3, true);
        treeView->setExpanded(treeView->rootIndex(), true);
}


