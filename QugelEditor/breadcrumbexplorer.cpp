#include "breadcrumbexplorer.h"
#include "ui_breadcrumbexplorer.h"
#include <QDir>
#include <QtDebug>
#include <QTableView>
#include <QPushButton>
#include <QListWidget>
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickItem>
#include <QSizePolicy>


BreadCrumbExplorer::BreadCrumbExplorer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BreadCrumbExplorer)
{
    ui->setupUi(this);
    treeModel = new QFileSystemModel(this);
    tableModel = new QFileSystemModel(this);

    QString currentPath = QDir::currentPath()+"/../../Assets/";
    treeModel->setFilter(QDir::Dirs|QDir::NoDotAndDotDot);

    ui->treeView->setModel(treeModel);

    ui->treeView->setRootIndex(treeModel->setRootPath(currentPath));
    for(int i=1;i<treeModel->columnCount();i++){
        ui->treeView->hideColumn(i);
    }
    //connect(ui->pushButton, SIGNAL(clicked()), qApp, SLOT(quit()));

    view = new QQuickView();
    QWidget *container = QWidget::createWindowContainer(view, this);

    container->setFocusPolicy(Qt::TabFocus);
    view->setSource(QUrl("qrc:/qml/UitestForm.ui.qml"));
    QObject* root = view->rootObject();
    QObject *rect = root->findChild<QObject*>("fileBrowser",Qt::FindChildrenRecursively);
    if (rect){
        rect->setProperty("shown",true);
        rect->setProperty("folder",currentPath);
    }
    QObject::connect(rect, SIGNAL(pathChanged(QString)),
                     this, SLOT(onPathChanged(QString)));
    QObject::connect(rect, SIGNAL(fileSelected(QString)),
                     this, SLOT(onFileSelected(QString)));
    QObject::connect(rect, SIGNAL(fileActivated(QString)),
                     this, SLOT(onFileActivated(QString)));

    ui->gridParent->addWidget(container);

}

BreadCrumbExplorer::~BreadCrumbExplorer()
{
    delete ui;
}

void BreadCrumbExplorer::on_treeView_clicked(const QModelIndex &index)
{
    QString res=treeModel->filePath(index);
    createBreadCrumbs(res);
    setCurrentPath(res);

}

void BreadCrumbExplorer::onPathChanged(QString newPath)
{
    createBreadCrumbs(newPath);
}

void BreadCrumbExplorer::onFileSelected(QString newPath)
{

}

void BreadCrumbExplorer::onFileActivated(QString newPath)
{
qDebug()<<newPath;
}

void BreadCrumbExplorer::createBreadCrumbs(const QString &newFilePath)
{
    QString filePath = newFilePath;
    if(filePath.startsWith("file:///"))
        filePath=filePath.mid(7);
    ui->breadcrumbs->clear();
    QString path=filePath.mid(treeModel->rootPath().length());
   // qDebug()<<treeModel->rootPath();
   // qDebug()<<filePath;
   // qDebug()<<path;
    breadCrumbs =path.split("/");
    breadCrumbs[0]="/";
    for(auto dir : breadCrumbs){
        ui->breadcrumbs->addItem(dir);
    }
}

void BreadCrumbExplorer::setCurrentPath(QString path)
{
     if(!path.startsWith("file:///"))
         path="file:///"+path;
    QObject* root = view->rootObject();
    QObject *rect = root->findChild<QObject*>("fileBrowser",Qt::FindChildrenRecursively);
    if (rect){

        rect->setProperty("folder",path);
    }

}

void BreadCrumbExplorer::on_breadcrumbs_itemClicked(QListWidgetItem *item)
{
    QString path=treeModel->rootPath();
    for(auto dir : breadCrumbs){
        if(dir!="/"&&dir!="")
            path+="/"+dir;
        if(item->text()==dir)
            break;
    }
    qDebug()<<path;
    createBreadCrumbs(path);
    setCurrentPath(path);
}
