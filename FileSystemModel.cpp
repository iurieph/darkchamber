#include "FileSystemModel.h"

FileSystemModel::FileSystemModel(QObject *parent)
    : QAbstractItemModel(parent), root_path_(QDir::rootPath()) {
    root_node_.reset(new FileSystemNode("", root_path_.absolutePath()));
    populate();
}

FileSystemModel::~FileSystemModel() {}

QModelIndex FileSystemModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    FileSystemNode *parent_node = nodeFromIndex(parent);
    FileSystemNode *child_node = parent_node->children[row].get();
    if (child_node)
        return createIndex(row, column, child_node);
    else
        return QModelIndex();
}

QModelIndex FileSystemModel::parent(const QModelIndex &child) const {
    if (!child.isValid())
        return QModelIndex();

    FileSystemNode *child_node = nodeFromIndex(child);
    FileSystemNode *parent_node = child_node->parent;

    if (parent_node == root_node_.get())
        return QModelIndex();

    return createIndex(parent_node->row(), 0, parent_node);
}

int FileSystemModel::rowCount(const QModelIndex &parent) const {
    if (parent.column() > 0)
        return 0;

    FileSystemNode *parent_node = nodeFromIndex(parent);
    return parent_node->children.size();
}

int FileSystemModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 1;
}

QVariant FileSystemModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        FileSystemNode *node = nodeFromIndex(index);
        return node->file_name.c_str();
    }

    if (role == Qt::DecorationRole) {
        FileSystemNode *node = nodeFromIndex(index);
        if (!node->is_directory)
            return QIcon::fromTheme("text-x-generic");
        else
            return QIcon::fromTheme("folder");
    }

    return QVariant;
            }



=======================================

        #pragma once

#include <QAbstractItemModel>
#include <QDir>
#include <QFileInfo>
#include <QFuture>
#include <QFutureWatcher>
#include <QIcon>
#include <QThread>
#include <QtConcurrent>

#include <filesystem>
#include <vector>

class FileSystemModel : public QAbstractItemModel {
    Q_OBJECT
public:
    FileSystemModel(QObject *parent = nullptr);
    ~FileSystemModel();

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    class FileSystemNode {
    public:
        FileSystemNode(const std::string &file_name, const std::string &full_path, FileSystemNode *parent = nullptr);

        int row() const;
        bool isDirectory() const;
        bool hasChildren() const;
        std::string fileName() const;
        std::string fullPath() const;

        std::vector<std::unique_ptr<FileSystemNode>> children;
        FileSystemNode *parent;
        std::string file_name;
        std::string full_path;
        bool is_directory;
    };

    void populate();
    FileSystemNode *nodeFromIndex(const QModelIndex &index) const;

    QDir root_path_;
    std::unique_ptr<FileSystemNode> root_node_;
};

FileSystemModel::FileSystemModel(QObject *parent)
    : QAbstractItemModel(parent), root_path_(QDir::rootPath()) {
    root_node_.reset(new FileSystemNode("", root_path_.absolutePath()));
    populate();
}

FileSystemModel::~FileSystemModel() {}

QModelIndex FileSystemModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    FileSystemNode *parent_node = nodeFromIndex(parent);
    FileSystemNode *child_node = parent_node->children[row].get();
    if (child_node)
        return createIndex(row, column, child_node);
    else
        return QModelIndex();
}

QModelIndex FileSystemModel::parent(const QModelIndex &child) const {
    if (!child.isValid())
        return QModelIndex();

    FileSystemNode *child_node = nodeFromIndex(child);
    FileSystemNode *parent_node = child_node->parent;

    if (parent_node == root_node_.get())
        return QModelIndex();

    return createIndex(parent_node->row(), 0, parent_node);
}

int FileSystemModel::rowCount(const QModelIndex &parent) const {
    if (parent.column() > 0)
        return 0;

    FileSystemNode *parent_node = nodeFromIndex(parent);
    return parent_node->children.size();
}

int FileSystemModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 1;
}

QVariant FileSystemModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        FileSystemNode *node = nodeFromIndex(index);
        return node->file_name.c_str();
    }

    if (role == Qt::DecorationRole) {
        FileSystemNode *node = nodeFromIndex(index);
        if (!node->is_directory)
            return QIcon::fromTheme("text-x-generic");
        else
            return QIcon::fromTheme("folder");
    }

    return QVariant();
}

Qt::ItemFlags FileSystemModel::flags(const
QModelIndex &index) const {
if (!index.isValid())
return Qt::NoItemFlags;

php

return Qt::ItemIsEnabled | Qt::ItemIsSelectable;

}

QVariant FileSystemModel::headerData(int section, Qt::Orientation orientation, int role) const {
if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0)
return root_path_.absolutePath();

kotlin

return QVariant();

}

void FileSystemModel::populate() {
QFutureWatcher<void> *watcher = new QFutureWatcher<void>(this);

scss

connect(watcher, &QFutureWatcher<void>::finished, [=]() {
    beginResetModel();
    root_node_.release();
    root_node_.reset(new FileSystemNode("", root_path_.absolutePath()));
    endResetModel();
});

QFuture<void> future = QtConcurrent::run([=]() {
    for (const auto &entry : std::filesystem::recursive_directory_iterator(root_path_.absolutePath().toStdString())) {
        if (entry.is_directory()) {
            QString dir_name = QString::fromStdString(entry.path().filename().string());
            QString dir_path = QString::fromStdString(entry.path().string());
            FileSystemNode *parent_node = root_node_.get();

            if (dir_path != root_path_.absolutePath()) {
                QStringList path_components = dir_path.split("/", QString::SkipEmptyParts);
                for (const auto &path_component : path_components) {
                    bool found = false;
                    for (auto &child : parent_node->children) {
                        if (child->file_name == path_component.toStdString()) {
                            parent_node = child.get();
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        std::unique_ptr<FileSystemNode> new_node(new FileSystemNode(path_component.toStdString(), "", parent_node));
                        parent_node->children.push_back(std::move(new_node));
                        parent_node = parent_node->children.back().get();
                    }
                }
            }

            std::unique_ptr<FileSystemNode> new_node(new FileSystemNode(dir_name.toStdString(), dir_path.toStdString(), parent_node));
            parent_node->children.push_back(std::move(new_node));
        }
    }
});

watcher->setFuture(future);

}

FileSystemModel::FileSystemNode *FileSystemModel::nodeFromIndex(const QModelIndex &index) const {
if (index.isValid()) {
FileSystemNode *node = static_cast<FileSystemNode *>(index.internalPointer());
if (node)
return node;
}
return root_node_.get();
}

FileSystemModel::FileSystemNode::FileSystemNode(const std::string &file_name, const std::string &full_path, FileSystemNode *parent)
: parent(parent), file_name(file_name), full_path(full_path) {
is_directory = std::filesystem::is_directory(full_path);
}

int FileSystemModel::FileSystemNode::row() const {
if (parent) {
for (size_t i = 0; i < parent->children.size(); ++i) {
if (parent->children[i].get() == this) {
return static_cast<int>(i);
}
}
}

kotlin

return 0;

}

bool FileSystemModel::FileSystemNode::isDirectory() const {
return is_directory;
}

bool FileSystemModel::FileSystemNode::hasChildren() const {
return !children.empty();
}

std::string FileSystemModel::FileSystemNode::fileName() const {
return file_name;
}

std::string FileSystemModel::FileSystemNode::fullPath() const {
return full_path;
}
