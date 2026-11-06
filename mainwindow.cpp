#include "mainwindow.hpp"

#include <QListWidget>
#include <QListWidgetItem>
#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    currentPath = QDir::homePath();

    setupUI();
    refreshFiles();
}

void MainWindow::setupUI() {
    setWindowTitle("LFM - Light File Manager");
    resize(850, 550);

    fileList = new QListWidget(this);
    setCentralWidget(fileList);

    QToolBar *toolbar = addToolBar("Tools");

    QAction *openAction = toolbar->addAction("Open folder");
    QAction *upAction = toolbar->addAction("Go up");
    QAction *refreshAction = toolbar->addAction("Refresh");

    connect(openAction, &QAction::triggered,
            this, &MainWindow::openFolder);

    connect(upAction, &QAction::triggered,
            this, &MainWindow::goUp);

    connect(refreshAction, &QAction::triggered,
            this, &MainWindow::refreshFiles);

    // Dwukrotne kliknięcie elementu
    connect(fileList, &QListWidget::itemDoubleClicked,
            this, &MainWindow::openItem);
    
}

void MainWindow::refreshFiles() {
    fileList->clear();

    QDir dir(currentPath);

    QFileInfoList entries = dir.entryInfoList(
        QDir::AllEntries | QDir::NoDotAndDotDot,
        QDir::DirsFirst | QDir::Name
    );

    for (const QFileInfo &info : entries) {
        QString name = info.fileName();

        if(info.isDir()) {
            name = "[DIR] " + name;
        }

        QListWidgetItem *item = new QListWidgetItem(name);

        item->setData(Qt::UserRole, info.absoluteFilePath());

        fileList->addItem(item);
    }

    setWindowTitle("LFM - " + currentPath);
}

void MainWindow::openFolder() {
    QString path = QFileDialog::getExistingDirectory(
        this,
        "Select folder",
        currentPath
    );

    if (!path.isEmpty()) {
        currentPath = path;
        refreshFiles();
    }
}

void MainWindow::goUp() {
    QDir dir(currentPath);

    if(dir.cdUp()) {
        currentPath = dir.absolutePath();
        refreshFiles();
    }
}

void MainWindow::openItem(QListWidgetItem *item) {
    QString path = item->data(Qt::UserRole).toString();

    QFileInfo info(path);
    
    if(info.isDir()) {
        currentPath = path;
        refreshFiles();
    } else {
        QDesktopServices::openUrl(
            QUrl::fromLocalFile(path)
        );
    }
}