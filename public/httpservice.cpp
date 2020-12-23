#include "httpservice.h"



HttpService::HttpService(QObject *parent) : QThread(parent) { }

void HttpService::run()
{

    m_isCanRun = true;
    static int thread_change = 0;
    while(1){
        qDebug()<<"HttpService::run";

        switch(thread_change){
            case 0:{
                    save_thread();
            }break;
            case 1:{
                    upload_thread("TSnapUpdate");
            }break;
            case 2:{
                    upload_thread("TStreamUpdate");
            }break;
            default:
            break;
        }
        thread_change ++;
        if(thread_change == 3)
            thread_change = 0;
        msleep(1500);

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////
        {
            QMutexLocker locker(&m_lock);
            //在每次循环判断是否可以运行，如果不行就退出循环
            if(!m_isCanRun){
                qDebug()<<"m_isCanRun";
                return;
            }
        }
    }
}


void HttpService::upload_thread(QString table)
{
    SqlWorker *sqlwork = SqlWorker::getInstance();
    bool existingData = QFile::exists(DATABASE_NAME);
    sqlwork->createConnection();
    if (!existingData){
       createFakeData();
       qDebug()<<"cretae db.....";
    }

    QSqlQuery query(Device_public::db);

    Device_public::db.transaction();
    QString select_cmd =QString("SELECT * FROM '%1' WHERE SyncFlag=0").arg(table);

    query.exec(select_cmd);
    while(query.next()){
        QString id = query.value(0).toString();
        QString FileName = query.value(1).toString();
        QString FileSize = query.value(2).toString();
        QString deviceNumber = query.value(0).toString();
        QString x_data = query.value(1).toString();
        QString y_data = query.value(2).toString();
        QString work_mode = query.value(0).toString();
        QString EventDate = query.value(1).toString();
        QString CameraId = query.value(2).toString();
        qDebug()<<id<<FileName<<FileSize;

        //QString http_path="http://192.168.100.128:8080/5--FTP/"; //服务器的url
        QString http_path = QString("http://47.92.125.82:8000/api/file/%1").arg(FileName); //服务器的url

        QMap<QString,QString> params_send; //上传 表单 参数
        params_send.insert("deviceNumber",deviceNumber);
        params_send.insert("x",x_data);
        params_send.insert("y",y_data);
        params_send.insert("work_mode",work_mode);
        params_send.insert("eventData",EventDate);
        params_send.insert("ID",CameraId);

        QString file_send=table; //上传 表单 文件名
        QString file_local_path = QString(DIR_JPG_CACHE_TEMP+FileName);
        qDebug()<<"upload_thread....."<<file_local_path;
        QNetworkReply *replay;

        QFile *file_local=new QFile(file_local_path);
        replay = UpLoadForm(http_path,params_send,file_send,file_local,FileName);
        if(replay->error() == QNetworkReply::NoError){
            QString updata_cmd = QString("UPDATE TSnapUpdate SET SyncFlag=1 WHERE FileName ='%1' AND SyncFlag=0").arg(FileName);
            if(!query.exec(updata_cmd)){
                Device_public::db.rollback();
            }
            file_local->remove();
        }
     }
     Device_public::db.commit();
}



void HttpService::save_thread()
{
    SqlWorker *sqlwork = SqlWorker::getInstance();
    bool existingData = QFile::exists(DATABASE_NAME);
    sqlwork->createConnection();
    if (!existingData){
       createFakeData();
       qDebug()<<"cretae db.....";
    }

    QFileInfoList file_snap_list = GetFileList(DIR_JPG_CACHE);
    QString filter_snap;
    unsigned int snap_uid = readTableRow("TSnapUpdate")+1;
    unsigned int stream_uid = readTableRow("TStreamUpdate")+1;
    qDebug()<<snap_uid<<stream_uid;
    foreach(QFileInfo fileinfo, file_snap_list)
    {
       filter_snap = fileinfo.suffix();    //后缀名
       if( filter_snap == "jpg" )
       {
             qDebug()<<fileinfo.fileName()<<fileinfo.size();
             QString http_path=QString("http://47.92.125.82:8000/api/file/%1").arg(fileinfo.fileName()); //服务器的url

             //  获取当前设备系统时间
             QByteArray time_bytearray =  Device_public::timeString.toString("yyyy-MM-dd hh:mm:ss").toLatin1();
             QString x_data = QString("%1 (%2)").arg(Device_public::gps_information.GPS_Longitude);

             QString y_data = QString("%1 (%2)").arg(Device_public::gps_information.GPS_Latitude)
                     .arg(Device_public::gps_information.GPS_Latitude_direction);
             QString carmer_id = fileinfo.fileName().at(5);//0 \ 1
             //数据库服务
             createTsnapData("TSnapUpdate",snap_uid,fileinfo.fileName(),fileinfo.size(),
                             Device_public::device_number,x_data,y_data,
                             Device_public::Motype,time_bytearray,carmer_id.toInt(),0);

             snap_uid++;
             //在备份文件夹创建相同的文件
             createFile(DIR_JPG_CACHE_TEMP, fileinfo.fileName());
             QString new_stream = QString(DIR_JPG_CACHE_TEMP+fileinfo.fileName());
             //打开文件
             QFile writefile;
             writefile.setFileName(new_stream);
             writefile.open(QIODevice::WriteOnly|QIODevice::Append);

             //  创建JSON 包
             cJSON *event_data = cJSON_CreateObject();
             //  获取当前设备机具号
             QByteArray device_name = Device_public::device_number.toLatin1();
             cJSON_AddStringToObject(event_data, "deviceNumber",device_name.data());
             cJSON_AddNumberToObject(event_data,"x",Device_public::gps_information.GPS_Longitude);
             cJSON_AddNumberToObject(event_data,"y",Device_public::gps_information.GPS_Latitude);
             cJSON_AddNumberToObject(event_data, "work_mode", Device_public::Motype);
             cJSON_AddStringToObject(event_data, "eventDate", time_bytearray.data());
             cJSON_AddNumberToObject(event_data, "ID", carmer_id.toInt());
             //  字节数组转化
             QByteArray pub_payload_string(cJSON_Print(event_data));
             cJSON_Delete(event_data);

             //向文件中写入位置数据
             QTextStream stream_new(&writefile);
             stream_new << QString(pub_payload_string.data());

             //关闭文件，不关闭出现换行等影响图片数据的bug
             writefile.close();
             //打开原视频流文件
             QFile readFile;
             readFile.setFileName(fileinfo.absoluteFilePath());//原视频流
             readFile.open(QIODevice::ReadOnly);
             //拿出图片数据
             uchar *buffer = (uchar *)malloc(fileinfo.size());
             memset(buffer,0,fileinfo.size());
             readFile.seek(0);
             int readfilesize = readFile.read((char *)buffer,fileinfo.size());
             qDebug()<<"readfilesize ="<<readfilesize<<fileinfo.size();
             //向文件中写入视频数据
             writefile.setFileName(new_stream);
             writefile.open(QIODevice::WriteOnly|QIODevice::Append);
             writefile.write((char *)buffer,fileinfo.size());
             //关闭文件
             writefile.close();
             readFile.close();
             //删除源文件
             QFile(fileinfo.absoluteFilePath()).remove();
       }
    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    QFileInfoList file_stream_list = GetFileList(DIR_264_CACHE);
    QString filter_stream;

    foreach(QFileInfo fileinfo, file_stream_list)
    {
       filter_stream = fileinfo.suffix();    //后缀名
       if( filter_stream == "h264" )
       {
             qDebug()<<fileinfo.fileName()<<fileinfo.size();

             //  获取当前设备系统时间
             QByteArray time_bytearray =  Device_public::timeString.toString("yyyy-MM-dd hh:mm:ss").toLatin1();
             QString x_data = QString("%1 (%2)").arg(Device_public::gps_information.GPS_Longitude);

             QString y_data = QString("%1 (%2)").arg(Device_public::gps_information.GPS_Latitude)
                     .arg(Device_public::gps_information.GPS_Latitude_direction);

             QString carmer_id = fileinfo.fileName().at(5);
             createTsnapData("TStreamUpdate",stream_uid,fileinfo.fileName(),fileinfo.size(),
                             Device_public::device_number,x_data,y_data,
                             Device_public::Motype,time_bytearray,carmer_id.toInt(),0);

             stream_uid++;

             //在备份文件夹创建相同的文件
             createFile(DIR_264_CACHE_TEMP, fileinfo.fileName());
             QString new_stream = QString(DIR_264_CACHE_TEMP+fileinfo.fileName());
             //打开文件
             QFile writefile;
             writefile.setFileName(new_stream);
             writefile.open(QIODevice::WriteOnly|QIODevice::Append);

             //  创建JSON 包
             cJSON *event_data = cJSON_CreateObject();
             //  获取当前设备机具号
             QByteArray device_name = Device_public::device_number.toLatin1();
             cJSON_AddStringToObject(event_data, "deviceNumber",device_name.data());
             cJSON_AddNumberToObject(event_data,"x",Device_public::gps_information.GPS_Longitude);
             cJSON_AddNumberToObject(event_data,"y",Device_public::gps_information.GPS_Latitude);
             cJSON_AddNumberToObject(event_data, "work_mode", Device_public::Motype);
             cJSON_AddStringToObject(event_data, "eventDate", time_bytearray.data());
             cJSON_AddNumberToObject(event_data, "ID", carmer_id.toInt());
             //  字节数组转化
             QByteArray pub_payload_string(cJSON_Print(event_data));
             cJSON_Delete(event_data);

             //向文件中写入位置数据
             QTextStream stream_new(&writefile);
             stream_new << QString(pub_payload_string.data());

             //关闭文件，不关闭出现换行等影响视频数据的bug
             writefile.close();
             //打开原视频流文件
             QFile readFile;
             readFile.setFileName(fileinfo.absoluteFilePath());//原视频流
             readFile.open(QIODevice::ReadOnly);
             //拿出视频数据
             uchar *buffer = (uchar *)malloc(fileinfo.size());
             memset(buffer,0,fileinfo.size());
             readFile.seek(0);
             int readfilesize = readFile.read((char *)buffer,fileinfo.size());
             qDebug()<<"readfilesize ="<<readfilesize<<fileinfo.size();
             //向文件中写入视频数据
             writefile.setFileName(new_stream);
             writefile.open(QIODevice::WriteOnly|QIODevice::Append);
             writefile.write((char *)buffer,fileinfo.size());
             //关闭文件
             writefile.close();
             readFile.close();
             //删除源文件
             QFile(fileinfo.absoluteFilePath()).remove();
       }
    }
    sqlwork->removeDatabase();
}

void HttpService::stopImmediately()
{
    QMutexLocker locker(&m_lock);
    m_isCanRun = false;
}


HttpService::~HttpService() {}

void HttpService::createFakeData()
{
    QSqlQuery query;
    //query->exec("DROP TABLE THttpEvent");
   query.exec("DROP TABLE TWorkData");
    //query->exec("DROP TABLE TSensorData");
    //query->exec("DROP TABLE TTextToSpeech");
   query.exec("DROP TABLE TSnapUpdate");
   query.exec("DROP TABLE TStreamUpdate");
    //id  name
   query.exec("CREATE TABLE TWorkData ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "EventDate VARCHAR(40) NOT NULL, "
                    "Cmd INTEGER NOT NULL, "
                    "Type INTEGER NOT NULL, "
                    "DeviceName VARCHAR(40) NOT NULL,"
                    "x VARCHAR(40) NOT NULL,"
                    "y VARCHAR(40) NOT NULL, "
                    "DepthSymbol INTEGER NOT NULL, "
                    "WorkDeep INTEGER NOT NULL, "
                    "Speed INTEGER NOT NULL, "
                    "Distance INTEGER NOT NULL, "
                    "Status INTEGER NOT NULL, "
                    "WorkMode INTEGER NOT NULL, "
                    "Binding INTEGER NOT NULL, "
                    "SyncFlag INTEGER NOT NULL) ");

   query.exec("CREATE TABLE TSnapUpdate ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "FileName VARCHAR(40) NOT NULL, "
                    "FileSize INTEGER NOT NULL, "
                    "DeviceName VARCHAR(40) NOT NULL,"
                    "x VARCHAR(40) NOT NULL,"
                    "y VARCHAR(40) NOT NULL, "
                    "WorkMode INTEGER NOT NULL, "
                    "EventDate VARCHAR(40) NOT NULL, "
                    "CameraId INTEGER NOT NULL, "
                    "SyncFlag INTEGER NOT NULL) ");

   query.exec("CREATE TABLE TStreamUpdate ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "FileName VARCHAR(40) NOT NULL, "
                    "FileSize INTEGER NOT NULL, "
                    "DeviceName VARCHAR(40) NOT NULL,"
                    "x VARCHAR(40) NOT NULL,"
                    "y VARCHAR(40) NOT NULL, "
                    "WorkMode INTEGER NOT NULL, "
                    "EventDate VARCHAR(40) NOT NULL, "
                    "CameraId INTEGER NOT NULL, "
                    "SyncFlag INTEGER NOT NULL) ");


}
void HttpService::createTsnapData(QString Table,int uid,QString uFileName,int uFileSize,QString uDeviceName,QString ux,QString uy,int uWorkMode,QByteArray uEventDate,int uCameraId,int uSyncFlag)
{
    QSqlQuery query;
    QString data_value = QString("INSERT INTO '%1' (id, FileName, FileSize, DeviceName,x,y,WorkMode,EventDate,CameraId,SyncFlag)"
                                 "VALUES ( '%2', '%3' , '%4' , '%5' ,'%6' , '%7' ,'%8', '%9' ,'%10' , '%11')")
            .arg(Table).arg(uid).arg(uFileName).arg(uFileSize).arg(uDeviceName).arg(ux)
            .arg(uy).arg(uWorkMode).arg(uEventDate.data()).arg(uCameraId).arg(uSyncFlag);
    if(query.exec(data_value)){
        qDebug()<<"query prepare uccess";
    }

}
unsigned int HttpService::readTableRow(QString table)
{
     QSqlQuery query ;
     int seq ;
     QString data_value = QString("SELECT seq FROM sqlite_sequence WHERE name = '%1'").arg(table);
     if(query.exec(data_value)){
        seq  = query.value(0).toInt();
        return seq;
     }
    return 0;
}

//void HttpService::UpLoadfileRealTime()
//{
//    QString path="http://192.168.100.128:8080/5--FTP/"; //服务器的url
//    QMap<QString,QString> params_send; //上传的普通参数 在本程序中 需要上传一个普通参数为"username"
//    params_send.insert("deviceNumber","10005");
//    params_send.insert("x","10005");
//    params_send.insert("y","10005");
//    params_send.insert("work_mode","10005");
//    params_send.insert("eventData","10005");
//    params_send.insert("ID","10005");

//    QString fileFormName="upload_stream"; //上传文件表单中的名

//    QFileInfoList file_info_list = GetFileList("/storage/stream");
//    QString filter;
//    foreach(QFileInfo fileinfo, file_info_list)
//    {
//       filter = fileinfo.suffix();    //后缀名
//       if( filter == "")
//       {
//           QFile *file=new QFile(fileinfo.absoluteFilePath());
//           UpLoadForm(path,params_send,fileFormName,file,fileinfo.fileName());
//       }
//    }
//}


void HttpService::createFile(QString filePath,QString fileName)
{
    QDir tempDir;
    if(!tempDir.exists(filePath))
    {
        qDebug()<<QObject::tr("不存在该路径")<<endl;
        tempDir.mkpath(filePath);
    }
    QFile *tempFile = new QFile;
    if(tempFile->exists(fileName))
    {
        qDebug()<<QObject::tr("文件存在");
        return ;
    }
    tempFile->setFileName(fileName);
    if(!tempFile->open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qDebug()<<QObject::tr("打开失败");
    }
    tempFile->close();
}


/*
 * Path: 服务器的url地址
 * params: 普通的表单参数
 * fileFormName: 需要上传文件表单中的名字
 * uploadFile: 要上传的文件
 * newFileName: 服务器保存该文件时文件名
 */

QNetworkReply* HttpService::UpLoadForm(QString Path,QMap<QString,QString> params,QString fileFormName,QFile *uploadFile,QString newFileName){
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

   return replay;
}


QFileInfoList HttpService::GetFileList(QString path)
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

bool HttpService::DeletePath(QString dirName)
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

qint64 HttpService::GetFileSize(QString path)
{
    QFileInfo info(path);
    return info.size();
}

qint64 HttpService::GetDirSize(QString dir)
{
    qint64 size = 0;

    QFileInfoList fileListInfo = GetFileList(dir);
    for (int i=0;i<fileListInfo.size(); i++) {
        size += fileListInfo.at(i).size();
        //size += v.size();
    }

    return size;
}

bool HttpService::CreateDir(QString dir)
{
    QDir d(dir);
    if (false == d.exists()) {
        d.mkdir(dir);
    }
    return d.exists();
}

//void HttpService::PrintMd5(unsigned char* val, int len)
//{
//    for (int i = 0; i < 32; ++i) {
//        if (i % 2 == 0) {
//            printf("%x", (val[i / 2] >> 4) & 0xf);
//        }
//        else {
//            printf("%x", (val[i / 2]) & 0xf);
//        }
//    }
//    printf("\n");
//}

//将 &%#= 这些文件名中允许的字符但url特殊意义的字符转义
//% 转义符为 %25
//# 转义符为 %23
//& 转义符为 %26
//= 转义符为 %3D
void HttpService::convertSpecialChar(QString &str)
{
   str.replace("%", "%25");
   str.replace("#", "%23");
   str.replace("&", "%26");
   str.replace("=", "%3D");
   str.replace(" ", "%20");
}
