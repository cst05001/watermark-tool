#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "fimagefileinfo.h"
#include <QLineEdit>
#include <QSlider>
#include <QListWidgetItem>
#include <QFileInfo>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onActionOpenFileClicked();
    void onActionOpenDirectoryClicked();
    void onButtonSave();
    void onSliderDegreeChanged(int value);
    void onSliderMarginChanged(int value);
    void onLineEditRotateEditingFinished();
    void onLineEditMarginEditingFinished();
    void onSliderBgColorAlphaChanged(int value);
    void onSliderFgColorAlphaChanged(int value);
    void onLineEditFgColorAlphaEditingFinished();
    void onLineEditBgColorAlphaEditingFinished();
    void onListWidgetSrcPreviewItemClicked(QListWidgetItem *item);
    void onSliderSizeChanged(int value);
    void onLineEditSizeEditingFinished();
    void onActionHelpAboutClicked();
    void onFontComboBoxFontChanged();
    void updateGraphicsView();
    void updateListWidgetSrcPreview();
    void onWindowResized(QSize size);

signals:
    void windowResized(QSize size);

private:
    Ui::MainWindow *ui;

    QList<FImageFileInfo*> imageInfoList;
    QFileInfo *openPath;

    void generateWatermark(QImage *image);
};
#endif // MAINWINDOW_H
