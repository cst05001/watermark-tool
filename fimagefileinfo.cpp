#include "fimagefileinfo.h"
#include <QDir>

FImageFileInfo::FImageFileInfo(QFileInfo file, QString openDir, QString destRootDir)
{
    this->file = file;
    this->openDir = openDir;
    this->destRootDir = destRootDir;
}

QFileInfo FImageFileInfo::getFileInfo()
{
    return this->file;
}

QString FImageFileInfo::getOpenDir()
{
    return openDir;
}

void FImageFileInfo::setDestRootDir(QString destRootDir)
{
    this->destRootDir = destRootDir;
}

QString FImageFileInfo::getDestRootPath()
{
    return destRootDir;
}

QString FImageFileInfo::getDestPath()
{
    QString relativePath = QDir(openDir).relativeFilePath(this->file.absoluteFilePath());
    return destRootDir + QDir::separator() + relativePath;
}
