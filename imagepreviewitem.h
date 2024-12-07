#ifndef IMAGEPREVIEWITEM_H
#define IMAGEPREVIEWITEM_H

#include <QWidget>
#include <QPixmap>
#include <QResizeEvent>

class ImagePreviewItem : public QWidget
{
    Q_OBJECT

private:
    QPixmap pixmap;

public:
    explicit ImagePreviewItem(const QString &imagePath, QWidget *parent = nullptr);

signals:
};

#endif // IMAGEPREVIEWITEM_H
