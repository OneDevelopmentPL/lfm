#pragma once

#include <QMainWindow>
#include <QString>

class QListWidget;
class QListWidgetItem;

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(QWidget *parent = nullptr);
private:
    QListWidget *fileList;

    QString currentPath;

    void setupUI();
    void refreshFiles();
    void openFolder();
    void goUp();
    void openItem(QListWidgetItem *item);
};