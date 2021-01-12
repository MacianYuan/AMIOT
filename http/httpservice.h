//#ifndef HTTPSERVICE_H
//#define HTTPSERVICE_H

//#include <QObject>
//#include <QtCore/QString>
//#include <QtCore/QFileInfoList>
//#include <QDir>
//#include <QCoreApplication>
//#include <QtNetwork>
//#include <QDebug>
//#include <QtNetwork/QNetworkReply>
//#include <QThread>
//#include "device_public.h"
//#include "sqlworker/sqlworker.h"

//class HttpService : public QThread
//{
//    Q_OBJECT
//public:
//    HttpService(QObject *parent = 0);
//    void run();
//    void stopImmediately();
//    QFileInfoList GetFileList(QString path);
//    bool DeletePath(QString dir);
//    qint64 GetFileSize(QString dir);
//    qint64 GetDirSize(QString dir);
//    bool CreateDir(QString dir);
//    //void PrintMd5(unsigned char* val, int len);
//    void convertSpecialChar(QString &str);
//    QNetworkReply * UpLoadForm(QString Path,QMap<QString,QString> params,QString fileFormName,QFile *uploadFile,QString newFileName);
//    //void UpLoadfileRealTime();
//    //void UpLoadJpgResume();
//    void createFile(QString filePath,QString fileName);
//    ~HttpService();

//private:
//    void createFakeData();
//    void createTsnapData(QString Table,int uid,QString FileName,int FileSize,QString DeviceName,QString x,QString y,int WorkMode,QByteArray EventDate,int CameraId,int SyncFlag);
//    unsigned int readTableRow(QString table);
//    void save_thread();
//    void upload_thread(QString table);
//signals:

//public slots:

//private:
//    //互斥锁
//    bool m_isCanRun;
//    QMutex m_lock;

//    QSqlTableModel *snapModel;
//};

//#endif // HTTPSERVICE_H
