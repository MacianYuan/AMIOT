#ifndef HTTPUPLOAD_H
#define HTTPUPLOAD_H

#include <QtCore/QString>
#include <QtCore/QFileInfoList>
#include <QDir>
#include <QCoreApplication>
#include <QtNetwork>
#include <QDebug>


class HttpUpload : public QObject
{
    Q_OBJECT
public:
    explicit HttpUpload(QObject *parent = 0);
    QFileInfoList GetFileList(QString path);
    bool DeletePath(QString dir);
    qint64 GetFileSize(QString dir);
    qint64 GetDirSize(QString dir);
    bool CreateDir(QString dir);
    void PrintMd5(unsigned char* val, int len);
    void convertSpecialChar(QString &str);
    void UpLoadForm(QString Path,QMap<QString,QString> params,QString fileFormName,QFile *uploadFile,QString newFileName);
    void UpLoadfile(QString Path,QString newFileName);
signals:

public slots:
};

#endif // HTTPUPLOAD_H
