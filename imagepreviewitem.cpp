#include "imagepreviewitem.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QFileInfo>

ImagePreviewItem::ImagePreviewItem(const QString &imagePath, QWidget *parent)
    : QWidget{parent}
{
    QLabel *imageLabel = new QLabel(this);
    pixmap = QPixmap(imagePath);

    int width = parent->width();
    pixmap = pixmap.scaledToWidth(width, Qt::SmoothTransformation);

    imageLabel->setPixmap(pixmap);
    imageLabel->setAlignment(Qt::AlignCenter);

    QLabel *fileNameLabel = new QLabel(QFileInfo(imagePath).fileName(), this);
    fileNameLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(imageLabel);
    layout->addWidget(fileNameLabel);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    setLayout(layout);
}
