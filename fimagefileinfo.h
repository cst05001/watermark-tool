#ifndef FIMAGEFILEINFO_H
#define FIMAGEFILEINFO_H
#include <QFileInfo>

class FImageFileInfo
{
public:
    FImageFileInfo(QFileInfo file, QString openDir, QString destRootDir);

    QFileInfo getFileInfo();
    QString getOpenDir();
    void setDestRootDir(QString destRootDir);
    QString getDestRootPath();
    QString getDestPath();

private:
    QFileInfo file;
    QString openDir;
    QString destRootDir;
};

#endif // FIMAGEFILEINFO_H
