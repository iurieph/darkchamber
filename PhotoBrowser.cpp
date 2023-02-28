
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

#include "PhotoBrowser.h"
#include "PhotoBrowserModel.h"
#include "PhotoModel.h"

#include <QKeyEvent>
#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>

PhotoBrowser::PhotoBrowser(PhotoModel *model, QWidget *parent)
        : QGraphicsView(parent)
        , thumbnailSize{150, 150}
        , thumbnailPadding{40}
        , deleteAct{nullptr}
        , trashAct{nullptr}
        , copyAct{nullptr}
        , moveAct{nullptr}
        , protectAct{nullptr}
{
        auto scene = new QGraphicsScene(this);
        setScene(scene);
        browserModel = new PhotoBrowserModel(scene, model);
        browserModel->setThumbnailSize(thumbnailSize);
        browserModel->setThumbnailPadding(thumbnailPadding);
        browserModel->setColumns(getColumns());
        setMinimumSize(thumbnailSize + QSize(thumbnailPadding, thumbnailPadding));
        setAlignment(Qt::AlignLeft | Qt::AlignTop);
        createActions();
}

void PhotoBrowser::createActions()
{
        // Delete action, i.e. permanently delete.
        deleteAct = new QAction(tr("Delete"), this);
        deleteAct->setStatusTip(tr("Delete permanently the files"));
        connect(deleteAct, &QAction::triggered, this, &PhotoBrowser::deletePermanently);

        // Trash action.
        trashAct = new QAction(tr("Move to Trash"), this);
        trashAct->setStatusTip(tr("Move files to trash"));
        connect(trashAct, &QAction::triggered, this, &PhotoBrowser::moveToTrash);

        // Copy files action.
        copyAct = new QAction(tr("Copy to..."), this);
        trashAct->setStatusTip(tr("Copy to a destination"));
        connect(copyAct, &QAction::triggered, this, &PhotoBrowser::copyTo);

        // Move files action.
        moveAct = new QAction(tr("Move to..."), this);
        trashAct->setStatusTip(tr("Move files to a destination"));
        connect(moveAct, &QAction::triggered, this, &PhotoBrowser::moveTo);

        // Protect file action
        protectAct = new QAction(tr("Protect"), this);
        protectAct->setStatusTip(tr("Protect the file(s) from deletion or change"));
        connect(protectAct, &QAction::triggered, this, &PhotoBrowser::protectFile);
}

void PhotoBrowser::resizeEvent(QResizeEvent *event)
{
        browserModel->setColumns(getColumns());
}

void PhotoBrowser::keyPressEvent(QKeyEvent *event)
{
        switch (event->key()) {
        case Qt::Key_Delete:
                browserModel->rejectSelectedItems();
                break;
        case Qt::Key_Right:
                qDebug() << "Qt::Key_Right";
                browserModel->selectNext();
                break;
        case Qt::Key_Left:
                browserModel->selectPrevious();
                break;
	case Qt::Key_Up:
                browserModel->selectUp();
                break;
	case Qt::Key_Down:
                browserModel->selectDown();
                break;
        case Qt::Key_Return:
                browserModel->viewImage();
                break;
        }
}

void PhotoBrowser::contextMenuEvent(QContextMenuEvent *event)
{
        auto res = browserModel->selectItemAt(mapToScene({event->x(), event->y()}));
        if (res) {
                QMenu menu(this);
                menu.addAction(deleteAct);
                menu.addAction(trashAct);
                menu.addAction(copyAct);
                menu.addAction(moveAct);
                menu.addAction(moveAct);
                menu.addSeparator();
                menu.addAction(protectAct);
                menu.exec(event->globalPos());
        }
}

void PhotoBrowser::deletePermanently()
{
        auto res = QMessageBox::warning(this, tr("Delete permanetly"),
                                        tr("Are you shure you want to permanently delete the file(s)?"),
                                        QMessageBox::Yes,
                                        QMessageBox::No);
        if (res == QMessageBox::Yes)
                browserModel->deleteSelectedItems(false);
}

void PhotoBrowser::moveToTrash()
{
        //        browserModel->deleteSelectedItems();
}

void PhotoBrowser::copyTo()
{
        auto destPath = QFileDialog::getExistingDirectory(
                           this,
                           tr("Select Destination Directory"), ".");
        if (!destPath.isEmpty())
                browserModel->moveSelectedFiles(destPath);
}

void PhotoBrowser::moveTo()
{
        auto destPath = QFileDialog::getExistingDirectory(
                this,
                tr("Select Destination Directory"), ".");
        if (!destPath.isEmpty())
                browserModel->moveSelectedFiles(destPath, false);
}

void PhotoBrowser::protectFile()
{
        //        browserModel->protectSelectedItems();
}

int PhotoBrowser::getColumns() const
{
        auto itemSize = QSize{thumbnailSize.width() + thumbnailPadding / 2,
                thumbnailSize.height() + thumbnailPadding / 2};
        if (width() < itemSize.width())
                return 1;
        return width() / itemSize.width();
}

void PhotoBrowser::updateFilters()
{
        //QVector<std::unique_ptr<PhotoFilters*>> filters;
        //browserModel->setFilters(filters);
}

void PhotoBrowser::nextItem()
{
        browserModel->selectNext();        
}

void PhotoBrowser::previousItem()
{
        browserModel->selectPrevious();
}
