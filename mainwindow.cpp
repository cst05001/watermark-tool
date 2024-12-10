#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QList>
#include <QDebug>
#include <QDirIterator>
#include <QFileDialog>
#include <QStringListModel>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QBrush>
#include <QFont>
#include <QMessageBox>
#include "imagepreviewitem.h"
#include <QResizeEvent>

#include "about.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // init Text and Background Color ComboBox
    // 初始化文字颜色和背景颜色下拉框
    // colorList = 16中基础色
    QStringList colorList = QStringList({
        "#000000", "#FFFFFF", "#FF0000", "#008000",
        "#0000FF", "#FFFF00", "#00FFFF", "#FF00FF",
        "#808080", "#C0C0C0", "#800000", "#808000",
        "#800080", "#008080", "#000080", "#00FF00"
    });

    foreach (QString color, colorList) {
        QColor qColor = QColor(color);
        QPixmap pixmap(20, 20);
        pixmap.fill(qColor);
        QIcon icon(pixmap);
        ui->comboBoxTextColor->addItem(icon, qColor.name());
        ui->comboBoxBgColor->addItem(icon, qColor.name());
    }
    ui->comboBoxBgColor->setCurrentIndex(0);
    ui->comboBoxTextColor->setCurrentIndex(1);
    ui->lineEditBgColorAlpha->setText(QString::number(ui->sliderBgColorAlpha->value()));
    ui->lineEditTextColorAlpha->setText(QString::number(ui->sliderTextColorAlpha->value()));


    // init Font Rocate Slider
    // 初始化字体旋转角度滑块
    QRegExp regExp("-?\\d{1,3}");
    QRegExpValidator *validator = new QRegExpValidator(regExp, this);
    ui->lineEditRotate->setValidator(validator);

    // init Watermark Margin Slider
    // 初始化水印间距滑块
    ui->lineEditMargin->setText(QString::number(ui->sliderMargin->value()));
    ui->lineEditRotate->setText(QString::number(ui->sliderDegree->value()));

    // init Text Size Editor
    // 初始化文字大小编辑框
    ui->lineEditSize->setText(QString::number(ui->sliderSize->value()));

    // set fontComboBox default value to system default font
    // 把系统默认字体设置为 fontComboBox 默认值
    QFont defaultFont = QApplication::font();
    QString defaultFontFamily = defaultFont.family();
    int defauotFontIndex = ui->fontComboBox->findText(defaultFontFamily, Qt::MatchExactly);
    if (defauotFontIndex != -1) {
        ui->fontComboBox->setCurrentIndex(defauotFontIndex);
    }


    connect(ui->actionOpenFile, SIGNAL(triggered()), this, SLOT(onActionOpenFileClicked()));
    connect(ui->actionOpenDirectory, SIGNAL(triggered()), this, SLOT(onActionOpenDirectoryClicked()));
    connect(ui->buttonSave, SIGNAL(clicked()), this, SLOT(onButtonSave()));
    connect(ui->sliderDegree, SIGNAL(valueChanged(int)), this, SLOT(onSliderDegreeChanged(int)));
    connect(ui->sliderMargin, SIGNAL(valueChanged(int)), this, SLOT(onSliderMarginChanged(int)));
    connect(ui->lineEditRotate, SIGNAL(editingFinished()), this, SLOT(onLineEditRotateEditingFinished()));
    connect(ui->lineEditMargin, SIGNAL(editingFinished()), this, SLOT(onLineEditMarginEditingFinished()));
    connect(ui->listWidgetSrcPreview, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onListWidgetSrcPreviewItemClicked(QListWidgetItem*)));
    connect(ui->sliderSize, SIGNAL(valueChanged(int)), this, SLOT(onSliderSizeChanged(int)));
    connect(ui->lineEditSize, SIGNAL(editingFinished()), this, SLOT(onLineEditSizeEditingFinished()));
    connect(ui->comboBoxTextColor, SIGNAL(currentIndexChanged(int)), this, SLOT(updateGraphicsView()));
    connect(ui->comboBoxBgColor, SIGNAL(currentIndexChanged(int)), this, SLOT(updateGraphicsView()));
    connect(ui->lineEditText, SIGNAL(editingFinished()), this, SLOT(updateGraphicsView()));
    connect(ui->lineEditText, SIGNAL(textChanged(QString)), this, SLOT(updateGraphicsView()));
    connect(ui->lineEditSize, SIGNAL(editingFinished()), this, SLOT(updateGraphicsView()));
    connect(ui->actionHelpAbout, SIGNAL(triggered()), this, SLOT(onActionHelpAboutClicked()));
    connect(ui->sliderBgColorAlpha, SIGNAL(valueChanged(int)), this, SLOT(onSliderBgColorAlphaChanged(int)));
    connect(ui->sliderTextColorAlpha, SIGNAL(valueChanged(int)), this, SLOT(onSliderFgColorAlphaChanged(int)));
    connect(ui->lineEditTextColorAlpha, SIGNAL(editingFinished()), this, SLOT(onLineEditFgColorAlphaEditingFinished()));
    connect(ui->lineEditBgColorAlpha, SIGNAL(editingFinished()), this, SLOT(onLineEditBgColorAlphaEditingFinished()));
    connect(ui->fontComboBox, SIGNAL(currentFontChanged(QFont)), this, SLOT(onFontComboBoxFontChanged()));
    connect(this, SIGNAL(windowResized(QSize)), this, SLOT(onWindowResized(QSize)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    QSize size = event->size();
    emit windowResized(size);
}

void MainWindow::onActionOpenFileClicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter("Images(*.png *.jpg, *.jpeg)");
    dialog.exec();
    if (!dialog.result()) {
        return;
    }
    QStringList files = dialog.selectedFiles();

    openPath = new QFileInfo(QFileInfo(files.at(0)).path());

    imageInfoList = QList<FImageFileInfo*>();

    foreach (QString filename, files) {
        QFileInfo fi = QFileInfo(filename);
        imageInfoList.append(new FImageFileInfo(fi.absoluteFilePath(), fi.absolutePath(), ""));
    }

    ui->listWidgetSrcPreview->clear();
    // ui->listWidgetSrcPreview->setIconSize(QSize(300, 300));

    foreach (FImageFileInfo *fi, imageInfoList) {
        QListWidgetItem *item = new QListWidgetItem(ui->listWidgetSrcPreview);
        ImagePreviewItem *imageItem = new ImagePreviewItem(fi->getFileInfo().absoluteFilePath(), ui->listWidgetSrcPreview);
        item->setSizeHint(imageItem->sizeHint());
        ui->listWidgetSrcPreview->setItemWidget(item, imageItem);
    }


    if (imageInfoList.size() > 0) {
        QListWidgetItem *item = ui->listWidgetSrcPreview->item(0);
        ui->listWidgetSrcPreview->setCurrentItem(item);
        onListWidgetSrcPreviewItemClicked(item);
    }
    ui->lineEditText->setFocus();
}

void MainWindow::onActionOpenDirectoryClicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);

    QStringList files;
    QString directory = dialog.getExistingDirectory(this, tr("Open a Directory"), NULL, QFileDialog::ShowDirsOnly);
    // if cancel clicked then return
    if (directory.isEmpty()) {
        return;;
    }

    openPath = new QFileInfo(directory);

    QDirIterator it(directory, {"*.jpg", "*.jpeg", "*.png"}, QDir::Files, QDirIterator::Subdirectories);

    imageInfoList = QList<FImageFileInfo*>();

    while (it.hasNext()) {
        it.next();
        QFileInfo fi = it.fileInfo();
        QString destPath = fi.absolutePath().replace(0, openPath->absoluteFilePath().size(), "");
        imageInfoList.append(new FImageFileInfo(fi.absoluteFilePath(), openPath->absoluteFilePath(), ""));
    }

    ui->listWidgetSrcPreview->clear();

    foreach (FImageFileInfo *fi, imageInfoList) {
        QListWidgetItem *item = new QListWidgetItem(ui->listWidgetSrcPreview);
        ImagePreviewItem *imageItem = new ImagePreviewItem(fi->getFileInfo().absoluteFilePath(), ui->listWidgetSrcPreview);
        item->setSizeHint(imageItem->sizeHint());
        ui->listWidgetSrcPreview->setItemWidget(item, imageItem);
    }

    if (imageInfoList.size() > 0) {
        QListWidgetItem *item = ui->listWidgetSrcPreview->item(0);
        ui->listWidgetSrcPreview->setCurrentItem(item);
        onListWidgetSrcPreviewItemClicked(item);
    }

    foreach (FImageFileInfo *fi, imageInfoList) {
        qDebug() << "fi.getFileInfo().absoluteFilePath():" << fi->getFileInfo().absoluteFilePath() << "\tfi.getOpenDir():" << fi->getOpenDir() << "\tfi.getDestRootPath():" << fi->getDestRootPath() << "\tfi.getDestPath():" << fi->getDestPath() << "\n";
    }
    ui->lineEditText->setFocus();
}

void MainWindow::onButtonSave()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Save to Directory"), NULL, QFileDialog::ShowDirsOnly);
    QFileInfo destDirectory = QFileInfo(directory);
    if (destDirectory.absoluteFilePath() == openPath->absoluteFilePath()) {
        QMessageBox::warning(this, tr("Error"), tr("Source and destination directory can't be same."));
        return;
    }

    foreach (FImageFileInfo *fi, imageInfoList) {
        fi->setDestRootDir(directory);
    }

    foreach (FImageFileInfo *fi, imageInfoList) {
        QImage image(fi->getFileInfo().absoluteFilePath());
        generateWatermark(&image);
        qDebug() << "fi.getFileInfo().absoluteFilePath():" << fi->getFileInfo().absoluteFilePath() << "\tfi.getOpenDir():" << fi->getOpenDir() << "\tfi.getDestRootPath():" << fi->getDestRootPath() << "\tfi.getDestPath():" << fi->getDestPath() << "\n";
        QDir destDir = QFileInfo(fi->getDestPath()).dir();
        if (!destDir.exists()) {
            destDir.mkpath(".");
        }

        image.save(fi->getDestPath());
    }
    QMessageBox::information(this, tr("Success"), tr("All images saved successfully to directory: ") + directory);
}

void MainWindow::onSliderDegreeChanged(int value)
{
    ui->lineEditRotate->setText(QString::number(value));
    updateGraphicsView();
}

void MainWindow::onSliderMarginChanged(int value)
{
    ui->lineEditMargin->setText(QString::number(value));
    updateGraphicsView();
}

void MainWindow::onLineEditRotateEditingFinished()
{
    if (ui->lineEditRotate->text().isEmpty()) {
        ui->lineEditRotate->setText(QString::number(ui->sliderDegree->value()));
    }

    if (ui->lineEditRotate->text().toInt() > ui->sliderDegree->maximum()) {
        ui->lineEditRotate->setText(QString::number(ui->sliderDegree->maximum()));
    } else if (ui->lineEditRotate->text().toInt() < ui->sliderDegree->minimum()) {
        ui->lineEditRotate->setText(QString::number(ui->sliderDegree->minimum()));
    }
    ui->sliderDegree->setValue(ui->lineEditRotate->text().toInt());
    updateGraphicsView();
}

void MainWindow::onLineEditMarginEditingFinished()
{
    if (ui->lineEditMargin->text().isEmpty()) {
        ui->lineEditMargin->setText(QString::number(ui->sliderMargin->value()));
    }

    if (ui->lineEditMargin->text().toInt() > ui->sliderMargin->maximum()) {
        ui->lineEditMargin->setText(QString::number(ui->sliderMargin->maximum()));
    } else if (ui->lineEditMargin->text().toInt() < ui->sliderMargin->minimum()) {
        ui->lineEditMargin->setText(QString::number(ui->sliderMargin->minimum()));
    }
    ui->sliderMargin->setValue(ui->lineEditMargin->text().toInt());
    updateGraphicsView();
}

void MainWindow::onSliderBgColorAlphaChanged(int value)
{
    ui->lineEditBgColorAlpha->setText(QString::number(value));
    updateGraphicsView();
}

void MainWindow::onSliderFgColorAlphaChanged(int value)
{
    ui->lineEditTextColorAlpha->setText(QString::number(value));
    updateGraphicsView();
}

void MainWindow::onLineEditFgColorAlphaEditingFinished()
{
    int value = ui->lineEditTextColorAlpha->text().toInt();
    if (value > 255) {
        value = 255;
    } else if (value < 0) {
        value = 0;
    }
    ui->sliderTextColorAlpha->setValue(value);
}

void MainWindow::onLineEditBgColorAlphaEditingFinished()
{
    int value = ui->lineEditBgColorAlpha->text().toInt();
    if (value > 255) {
        value = 255;
    } else if (value < 0) {
        value = 0;
    }
    ui->sliderBgColorAlpha->setValue(value);
    updateGraphicsView();
}

void MainWindow::onListWidgetSrcPreviewItemClicked(QListWidgetItem *item)
{
    updateGraphicsView();
}

void MainWindow::generateWatermark(QImage *image)
{
    QFont font = ui->fontComboBox->currentFont();
    int fontSize = ui->sliderSize->value();
    font.setPixelSize(fontSize);
    QString text = ui->lineEditText->text().isEmpty() ? QString("I'm Watermark.") : ui->lineEditText->text();

    QFontMetrics fm(font);
    int textWidth = fm.horizontalAdvance(text);
    int textHeight = fm.height();
    int ascent = fm.ascent();
    int descent = fm.descent();

    QImage textImage(textWidth, textHeight, QImage::Format_ARGB32);
    QColor bgColor = QColor(ui->comboBoxBgColor->currentText());
    bgColor.setAlpha(ui->sliderBgColorAlpha->value());
    textImage.fill(bgColor);

    QPainter painterWatermark(&textImage);
    QColor textColor = QColor(ui->comboBoxTextColor->currentText());
    textColor.setAlpha(ui->sliderTextColorAlpha->value());
    painterWatermark.setFont(font);
    painterWatermark.setPen(textColor);
    painterWatermark.setBrush(textColor);
    painterWatermark.drawText(0, ascent, text);
    painterWatermark.end();

    // 按照角度旋转 textImage
    QTransform transform;
    transform.rotate(ui->lineEditRotate->text().toInt());
    textImage = textImage.transformed(transform);

    // 把 textImage 重复铺满 image，每个 textImage 上下左右间距 sliderMargin->value()
    QPainter painterImage(image);
    int margin = ui->sliderMargin->value();
    for (int x = 0; x < image->width(); x += textImage.width() + margin) {
        for (int y = 0; y < image->height(); y += textImage.height() + margin) {
            painterImage.drawImage(x, y, textImage);
        }
    }

    painterImage.end();
}

void MainWindow::updateGraphicsView()
{
    if (ui->listWidgetSrcPreview->currentItem() == NULL) {
        return;
    }

    QListWidgetItem *item = ui->listWidgetSrcPreview->currentItem();
    FImageFileInfo fi = *imageInfoList.at(ui->listWidgetSrcPreview->currentRow());
    QImage image(fi.getFileInfo().absoluteFilePath());
    generateWatermark(&image);
    QPixmap pixmap = QPixmap::fromImage(image);
    QGraphicsScene *scene = new QGraphicsScene();
    scene->addPixmap(pixmap);
    ui->graphicsView->setScene(scene);

    if (image.width() > ui->graphicsView->width() || image.height() > ui->graphicsView->height()) {
        ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    }
    ui->graphicsView->show();
}



void MainWindow::onSliderSizeChanged(int value)
{
    if (value % 2 != 0) {
        value = value + 1;
    }
    if (value > ui->sliderSize->maximum()) {
        value = ui->sliderSize->maximum();
    } else if (value < ui->sliderSize->minimum()) {
        value = ui->sliderSize->minimum();
    }
    ui->sliderSize->setValue(value);
    ui->lineEditSize->setText(QString::number(value));
    updateGraphicsView();
}

void MainWindow::onLineEditSizeEditingFinished()
{
    if (ui->lineEditSize->text().isEmpty()) {
        ui->lineEditSize->setText(QString::number(ui->sliderSize->value()));
    }

    int size = ui->lineEditSize->text().toInt();
    if (size % 2 != 0) {
        size = size + 1;
    }

    if (size > ui->sliderSize->maximum()) {
        size = ui->sliderSize->maximum();
        ui->lineEditSize->setText(QString::number(size));
    } else if (size < ui->sliderSize->minimum()) {
        size = ui->sliderSize->minimum();
        ui->lineEditSize->setText(QString::number(size));
    }
    ui->sliderSize->setValue(size);
    updateGraphicsView();
}

void MainWindow::onActionHelpAboutClicked()
{
    // Open About Widget
    About *about = new About();
    about->show();
}

void MainWindow::onFontComboBoxFontChanged()
{
    updateGraphicsView();
}

void MainWindow::updateListWidgetSrcPreview()
{
    int currentIndex = ui->listWidgetSrcPreview->currentRow();
    ui->listWidgetSrcPreview->clear();
    foreach (FImageFileInfo *fi, imageInfoList) {
        QListWidgetItem *item = new QListWidgetItem(ui->listWidgetSrcPreview);
        ImagePreviewItem *imageItem = new ImagePreviewItem(fi->getFileInfo().absoluteFilePath(), ui->listWidgetSrcPreview);
        item->setSizeHint(imageItem->sizeHint());
        ui->listWidgetSrcPreview->setItemWidget(item, imageItem);
    }
    if (imageInfoList.size() > 0) {
        QListWidgetItem *item = ui->listWidgetSrcPreview->item(0);
        ui->listWidgetSrcPreview->setCurrentItem(item);
        onListWidgetSrcPreviewItemClicked(item);
    }
    ui->listWidgetSrcPreview->setCurrentRow(currentIndex);
}

void MainWindow::onWindowResized(QSize size)
{
    updateGraphicsView();
    updateListWidgetSrcPreview();
}
