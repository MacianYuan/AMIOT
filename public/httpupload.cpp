#include "httpupload.h"
#include <QtNetwork/QNetworkReply>

HttpUpload::HttpUpload(QObject *parent) : QObject(parent)
{



}


void HttpUpload::UpLoadfile(QString Path,QString newFileName)
{
    QString path="http://192.168.100.128:8080/5--FTP/"; //服务器的url
    QMap<QString,QString> params_send; //上传的普通参数 在本程序中 需要上传一个普通参数为"username"
    params_send.insert("username","10005");

    QString fileFormName="upload_file"; //上传文件表单中的名
    QFile *file=new QFile(":/images/1.bmp");
    qDebug()<<path<<newFileName;
    UpLoadForm(path,params_send,fileFormName,file,newFileName);
}

/*
 * Path: 服务器的url地址
 * params: 普通的表单参数
 * fileFormName: 需要上传文件表单中的名字
 * uploadFile: 要上传的文件
 * newFileName: 服务器保存该文件时文件名
 */

void HttpUpload::UpLoadForm(QString Path,QMap<QString,QString> params,QString fileFormName,QFile *uploadFile,QString newFileName){
    QString BOUNDARY=QUuid::createUuid().toString();
    QByteArray sb=QByteArray();

    //先上传普通的表单数据
    for(QMap<QString,QString>::Iterator t=params.begin();t!=params.end();t++){
        sb.append("--"+BOUNDARY+"\r\n");
        sb.append(QString("Content-Disposition: form-data; name=\"")+t.key()+QString("\"")+QString("\r\n"));
        sb.append("\r\n");
        sb.append(t.value()+"\r\n");
    }

    //上传文件的头部
    sb.append("--"+BOUNDARY+"\r\n");
    sb.append(QString("Content-Disposition: form-data; name=\"")+fileFormName+QString("\"; filename=\"")+newFileName+QString("\"")+QString("\r\n"));
    sb.append("\r\n");
    //上传文件内容
    if(!uploadFile->open(QIODevice::ReadOnly)){
        qDebug()<< "open fail!!!!!";
        return;
    }

    sb.append(uploadFile->readAll());
    sb.append("\r\n");
    sb.append("--"+BOUNDARY+"\r\n");
    //编辑HTTP头部
    QNetworkAccessManager *_uploadManager=new QNetworkAccessManager();
    QNetworkRequest request=QNetworkRequest(QUrl(Path));
    request.setRawHeader(QString("Content-Type").toLatin1(),QString("multipart/form-data; boundary="+BOUNDARY).toLatin1());
    request.setRawHeader(QString("Content-Length").toLatin1(),QString::number(sb.length()).toLatin1());

   //执行post请求
   QNetworkReply *replay = _uploadManager->post(request,sb);

   qDebug()<<"QNetworkReply"<<replay->error();
}






QFileInfoList HttpUpload::GetFileList(QString path)
{
    QDir dir(path);
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (int i = 0; i != folder_list.size(); i++) {
        QString name = folder_list.at(i).absoluteFilePath();
        QFileInfoList child_file_list = GetFileList(name);
        file_list.append(child_file_list);
    }

    return file_list;
}

bool HttpUpload::DeletePath(QString dirName)
{
    QFileInfo fileInfo(dirName);
    if (true == fileInfo.isDir()) {
        QDir dir(dirName);
        if (!dir.exists()) {
            return false;
        }

        QFileInfoList fileInfoList = dir.entryInfoList();
        foreach(QFileInfo fileInfo, fileInfoList){
            if (fileInfo.fileName() == "." || fileInfo.fileName() == "..") {
                continue;
            }
            if (fileInfo.isDir()){
                QString tmpdir = dirName + ("/") + fileInfo.fileName();
                DeletePath(tmpdir);
                dir.rmdir(fileInfo.fileName()); /**< 移除子目录 */
            } else if (fileInfo.isFile()){
                QFile tmpFile(fileInfo.fileName());
                dir.remove(tmpFile.fileName()); /**< 删除临时文件 */
            }
        }

        dir.cdUp();            /**< 返回上级目录，因为只有返回上级目录，才可以删除这个目录 */
        if (dir.exists(dirName)){
            if (!dir.rmdir(dirName))
                return false;
        }
    } else if (true == fileInfo.isFile()) {
        QFile file(dirName);
        file.remove();
    }

    return true;
}

qint64 HttpUpload::GetFileSize(QString path)
{
    QFileInfo info(path);
    return info.size();
}

qint64 HttpUpload::GetDirSize(QString dir)
{
    qint64 size = 0;

    QFileInfoList fileListInfo = GetFileList(dir);
    for (int i=0;i<fileListInfo.size(); i++) {
        size += fileListInfo.at(i).size();
        //size += v.size();
    }

    return size;
}

bool HttpUpload::CreateDir(QString dir)
{
    QDir d(dir);
    if (false == d.exists()) {
        d.mkdir(dir);
    }
    return d.exists();
}

void HttpUpload::PrintMd5(unsigned char* val, int len)
{
    for (int i = 0; i < 32; ++i) {
        if (i % 2 == 0) {
            printf("%x", (val[i / 2] >> 4) & 0xf);
        }
        else {
            printf("%x", (val[i / 2]) & 0xf);
        }
    }
    printf("\n");
}


//将 &%#= 这些文件名中允许的字符但url特殊意义的字符转义
//% 转义符为 %25
//# 转义符为 %23
//& 转义符为 %26
//= 转义符为 %3D
void HttpUpload::convertSpecialChar(QString &str)
{
   str.replace("%", "%25");
   str.replace("#", "%23");
   str.replace("&", "%26");
   str.replace("=", "%3D");
   str.replace(" ", "%20");
}
