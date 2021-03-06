#include "image_browser.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QString>
#include <QHeaderView>
#include <QFileInfo>
#include <QStandardItemModel>

ImageBrowser::ImageBrowser(QWidget *parent)
    : QWidget(parent)
    , recent_directories_(0)
    , list_view_(0)
    , v_splitter_(0)
    , model_(0)
{
    initWidgets();
    initLayout();
}

ImageBrowser::~ImageBrowser()
{
}

void ImageBrowser::setImageDirTableModel(ImageDirTableModel* model)
{
    model_ = model;
    recent_directories_->setModel(model);
}

ImageDirTableModel *ImageBrowser::getImageDirTableModel()
{
    return model_;
}

ImageCanvas *ImageBrowser::getCanvas() const
{
    return list_view_->getCanvas();
}

ImageDisplayWidget *ImageBrowser::getDisplayWidget() const
{
    return list_view_->getDisplayWidget();
}

void ImageBrowser::onNewDirectory(const QString & name)
{
    if(model_ != 0) {
        model_->addImageDirRecord(QFileInfo(name));
        /*
        QStandardItemModel* m = (QStandardItemModel*) recent_directories_->model();
        for(int i = 0; i < m->rowCount(); ++i) {
            QString dir_name = m->data(m->index(i, 0)).toString();
            if(name.compare(dir_name) == 0)
                return;
        }
        m->appendRow(new QStandardItem(name));
        */
    }
    else {
        QStandardItemModel* m = (QStandardItemModel*) recent_directories_->model();
        for(int i = 0; i < m->rowCount(); ++i) {
            QString dir_name = m->data(m->index(i, 0)).toString();
            if(name.compare(dir_name) == 0)
                return;
        }
        m->appendRow(new QStandardItem(name));
    }
}

void ImageBrowser::onRecentDirClicked(const QModelIndex& idx)
{
    if(!idx.isValid() || idx.row() < 0)
        return;
    QStandardItemModel* m = (QStandardItemModel*) recent_directories_->model();
    QString dir =  m->data(idx).toString();
    list_view_->openDirectory(dir);
}

void ImageBrowser::initWidgets()
{
    v_splitter_ = new QSplitter(Qt::Vertical, this);

    QStandardItemModel* model = new QStandardItemModel(0, 1, this);
    model->setHeaderData(0, Qt::Horizontal, tr("Recent Directories"));
    recent_directories_ = new QTreeView(v_splitter_);
    recent_directories_->setModel(model);
    connect(recent_directories_, SIGNAL(clicked(const QModelIndex&)),
            this, SLOT(onRecentDirClicked(const QModelIndex&)));

    list_view_ = new ThumbnailList(v_splitter_);
    list_view_->layout()->setMargin(0);
    connect(list_view_, SIGNAL(newDirectory(const QString&)),
            this, SLOT(onNewDirectory(const QString&)));

    v_splitter_->addWidget(recent_directories_);
    v_splitter_->addWidget(list_view_);
    v_splitter_->setStretchFactor(0, 2);
    v_splitter_->setStretchFactor(1, 8);
}

void ImageBrowser::initLayout()
{
    QVBoxLayout* layout = new QVBoxLayout;

    layout->addWidget(v_splitter_);

    setLayout(layout);
}
