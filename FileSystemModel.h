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
