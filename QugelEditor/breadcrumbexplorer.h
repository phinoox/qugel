#ifndef BREADCRUMBEXPLORER_H
#define BREADCRUMBEXPLORER_H

#include <QWidget>
#include <QFileSystemModel>
#include <QList>
#include <QPushButton>
#include <QListWidgetItem>

class QQuickView;

namespace Ui {
class BreadCrumbExplorer;
}

class BreadCrumbExplorer : public QWidget
{
    Q_OBJECT

public:
    explicit BreadCrumbExplorer(QWidget *parent = nullptr);
    ~BreadCrumbExplorer();

private slots:
    void on_treeView_clicked(const QModelIndex &index);
    void onPathChanged(QString newPath);
    void onFileSelected(QString newPath);
    void onFileActivated(QString newPath);

    void on_breadcrumbs_itemClicked(QListWidgetItem *item);

private:
    Ui::BreadCrumbExplorer *ui;
    QFileSystemModel *treeModel;
    QFileSystemModel *tableModel;
    QStringList breadCrumbs;
    void createBreadCrumbs(const QString &filePath);
    QQuickView *view;
    void setCurrentPath(QString path);
};

#endif // BREADCRUMBEXPLORER_H
