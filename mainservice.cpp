#include "mainservice.h"

MainService::MainService(QObject *parent) : QObject(parent)
{
    thread_tts = new QThread;
    text_to_speech = new TextToSpeech;
    mqttlogic_processing = new MqttLogicProcessing;

}

void MainService::init(){
    mainTimer = new QTimer();
    mainTimer->start(1000);
    connect(mainTimer,SIGNAL(timeout()),this,SLOT(on_mainTimeOut()));
    connect(this,SIGNAL(service_flow(unsigned int)),this,SLOT(on_service_flow(unsigned int)));
}

void MainService::on_service_flow(unsigned int flow)
{
    qDebug()<<"on_service_flow";
    switch(flow){
        case START_SYSTEM_FLOW_TTS:{
            //与4G EC20通信 语音播报功能
            text_to_speech->moveToThread(thread_tts);
            connect(this,SIGNAL(tts_message(unsigned int ,QString)),text_to_speech,SLOT(on_tts_message(unsigned int ,QString)),Qt::BlockingQueuedConnection);
            thread_tts->start();
        }break;
        case START_SYSTEM_FLOW_KEY:{
            //与单片机串口通信 创建一个线程实现与单片机串口通信，用于处理单片机发送接受相关事件 获取按键信息
            QThread * thread_serial = new QThread;
            SerialCommunication* serial_communication = new SerialCommunication;
            serial_communication->moveToThread(thread_serial);

            thread_serial->start();

            //红外检测到菜单键，menuwidget显示隐藏
            connect(serial_communication,SIGNAL(menushowSelect()),this,SLOT(on_menushowSelect()));
            //红外检测到上下键，stackedwidget翻页
            connect(serial_communication,SIGNAL(stackedshowSelect(bool)),this,SLOT(on_stackedshowSelect(bool)));

            connect(serial_communication,SIGNAL(stream_change(bool)),this,SLOT(on_stream_change(bool)));
        }break;
        case START_SYSTEM_FLOW_IAR:{
            //红外键盘解码  创建一个线程实现红外键盘解码，用于处理红外按键相关事件
            QThread * thread_Irda  = new QThread;
            IrdaAnalysis *irdaanalysis = new IrdaAnalysis;
            irdaanalysis->moveToThread(thread_Irda);

            thread_Irda->start();
            //红外检测到菜单键，menuwidget显示隐藏
            connect(irdaanalysis,SIGNAL(menushowSelect()),this,SLOT(on_menushowSelect()));
            //红外检测到上下键，stackedwidget翻页
            connect(irdaanalysis,SIGNAL(stackedshowSelect(bool)),this,SLOT(on_stackedshowSelect(bool)));

            connect(irdaanalysis,SIGNAL(stream_change(bool)),this,SLOT(on_stream_change(bool)));

        }break;
        case START_SYSTEM_FLOW_GPS:{
            //与单片机串口通信 GPS 获取定位信息
            QThread * thread_gps = new QThread;
            GpsAnalysis* gps_analysis= new GpsAnalysis;
            gps_analysis->moveToThread(thread_gps);

            thread_gps->start();
            connect(gps_analysis,SIGNAL(tts_message(unsigned int ,QString)),text_to_speech,SLOT(on_tts_message(unsigned int ,QString)),Qt::BlockingQueuedConnection);

        }break;
        case START_SYSTEM_FLOW_SENSOR:{
            //传感器信息获取
            sensor_analysis = new SensorAnalysis;
            sensor_analysis->init();
            sensor_analysis->start();
            // 传感器线程中 语音播报
            connect(sensor_analysis,SIGNAL(tts_message(unsigned int ,QString)),text_to_speech,SLOT(on_tts_message(unsigned int ,QString)),Qt::BlockingQueuedConnection);
        }break;
        case START_SYSTEM_FLOW_GETKEY:{
            //HTTP获取密钥和产品key
            HttpGetKey * httpgetkey = new HttpGetKey;
            httpgetkey->init();
        }break;
        case START_SYSTEM_FLOW_ALIYUN:{
            //(1.b)获取机具识别号 传感器初始化    (2.a)HTTP获取密钥和产品key (2.b)触发阿里云登录信号，登录aliyun服务器
            //QThread * thread_Aliyun = new QThread;
            AliyunClient *aliyunclient = new AliyunClient;

            connect(aliyunclient,SIGNAL(tts_message(unsigned int ,QString)),text_to_speech,SLOT(on_tts_message(unsigned int ,QString)),Qt::BlockingQueuedConnection);
            //aliyunclient->moveToThread(thread_Aliyun);
            //thread_Aliyun->start();
            //获取工作模式
            mqttlogic_processing->mqtt_logic_get_mode();
            qDebug()<<"  mqttlogic_processing->mqtt_logic_get_mode()";
        }break;
        case START_SYSTEM_FLOW_UPDATE:{
        //推送版本号，等待更新版本
           // mqttlogic_processing->mqtt_logic_post_version();
            if(Device_public::Update_flag == 1){
                //远程升级
                HttpGetFile * getfile = new HttpGetFile;
                QUrl url(Device_public::file_Url);
                if (!getfile->getFile(url)){
                    //下载错误
                   break ;
                }
                connect(getfile, SIGNAL(done()), getfile, SLOT(copy_update_file()));
            }
        }break;
        case START_SYSTEM_FLOW_UPLOAD:{
            httpService = new HttpService;
            httpService->start();

            qDebug()<<"httpService->UpLoadJpgResume()";
            //httpService->UpLoadJpgResume();
        }break;
       default:
        break;
    }
}


void MainService::on_mainTimeOut()
{
    static unsigned int timeKeeping_init;
    static unsigned int timeKeeping_run;
    timeKeeping_init++;
    timeKeeping_run++;
    if(timeKeeping_init == 30){
        timeKeeping_init = 30;
    }
    //mqttlogic_processing->mqtt_logic_work_data();

    Device_public::timeString = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = Device_public::timeString.toString("yyyy-MM-dd hh:mm:ss "); //设置显示格式
    emit label_time_refresh(str);


    if(timeKeeping_run ==5){

    }
    tts_message_mode();
    emit gps_signal_update();

    switch (timeKeeping_init){
        case 1:{
            qDebug()<<"emit service_flow(START_SYSTEM_FLOW_TTS)";
            emit service_flow(START_SYSTEM_FLOW_TTS);

        }break;
        case 3:{
            emit service_flow(START_SYSTEM_FLOW_KEY);
            qDebug()<<"emit service_flow(START_SYSTEM_FLOW_KEY)";
        }break;
        case 5:{
            emit service_flow(START_SYSTEM_FLOW_IAR);
            emit  stackedshowSelect(true);//开机页面自动转换到系统检测页面
            qDebug()<<"emit service_flow(START_SYSTEM_FLOW_IAR)";
        }break;
        case 7:{
            emit service_flow(START_SYSTEM_FLOW_GPS);
            qDebug()<<"emit service_flow(START_SYSTEM_FLOW_GPS)";
        }break;
        case 9:{
            emit service_flow(START_SYSTEM_FLOW_SENSOR);
            qDebug()<<"emit service_flow(START_SYSTEM_FLOW_SENSOR)";
        }break;
        case 11:{
            if(!Device_public::device_number.isEmpty()){
                emit service_flow(START_SYSTEM_FLOW_GETKEY);
                qDebug()<<"emit service_flow(START_SYSTEM_FLOW_GETKEY)";
            }else{
                timeKeeping_init = 9;
            }
        }break;
        case 13:{
            if(!Device_public::ProductKey.isEmpty()){
                emit service_flow(START_SYSTEM_FLOW_ALIYUN);
                qDebug()<<"emit service_flow(START_SYSTEM_FLOW_ALIYUN)";
            }else{
                timeKeeping_init = 11;
            }
        }break;
        case 17:{
            emit service_flow(START_SYSTEM_FLOW_UPDATE);
            qDebug()<<"emit service_flow(START_SYSTEM_FLOW_UPDATE)";
        }break;
        case 20:{
            emit service_flow(START_SYSTEM_FLOW_UPLOAD);
            qDebug()<<"emit service_flow(START_SYSTEM_FLOW_UPLOAD)";
        }break;
        default:
         break;
    }
}
void MainService::tts_message_mode()
{
    static int Motype_old = 0;
    //语音播报作业模式  当前作业值：
    qDebug()<< "Device_public::Motype" << Device_public::Motype;
    if(Motype_old != Device_public::Motype){
        Motype_old = Device_public::Motype;
        qDebug()<< "Device_public::Motype   novoice_flag" << Device_public::Motype;
        switch( Device_public::Motype){
            case 1:emit tts_message(1,"5F53524D4F5C4E1A503C6DF1677E4F5C4E1A");//深松作业
            break;
            case 2:emit tts_message(1,"5F53524D4F5C4E1A503C73AF75304F5C4E1A");//环田作业
            break;
            case 3:emit tts_message(1,"5F53524D4F5C4E1A503C6DF17FFB4F5C4E1A");//深翻作业
            break;
            //{"mode":4,"deviceNumber":"20200701001","token":"20200701001","eventDate":1596417688008}
            //eyJtb2RlIjo0LCJkZXZpY2VOdW1iZXIiOiIyMDIwMDcwMTAwMSIsInRva2VuIjoiMjAyMDA3MDEwMDEiLCJldmVudERhdGUiOjE1OTY0MTc2ODgwMDh9
            case 4:emit tts_message(1,"5F53524D4F5C4E1A503C514D801564AD79CD");//免耕播种
            break;
            case 5:emit tts_message(1,"5F53524D4F5C4E1A503C7A3B9EA6653683B7");//稻麦收获
            break;
            case 6:emit tts_message(1,"5F53524D4F5C4E1A503C73897C73653683B7");//玉米收获
            break;
            case 7:emit tts_message(1,"5F53524D4F5C4E1A503C4F4D7F6E670D52A1");//位置服务
            break;
            case 8:emit tts_message(1,"5F53524D4F5C4E1A503C625363464F5C4E1A");//打捆作业
            break;
            case 9:emit tts_message(1,"5F53524D4F5C4E1A503C4E2D80154F5C4E1A");//中耕作业
            break;
            case 10:emit tts_message(1,"5F53524D4F5C4E1A503C690D4FDD4F5C4E1A");//植保作业
            break;
            case 11:emit tts_message(1,"5F53524D4F5C4E1A503C63D279E74F5C4E1A");//插秧作业
            break;
            case 12:emit tts_message(1,"5F53524D4F5C4E1A503C65CB80154F5C4E1A");//旋耕作业
            break;
            case 13:emit tts_message(1,"5F53524D4F5C4E1A503C629B80A54F5C4E1A");//抛肥作业
            break;
            case 14:emit tts_message(1,"5F53524D4F5C4E1A503C653683B74F5C4E1A");//收获作业
            break;
            case 15:emit tts_message(1,"5F53524D4F5C4E1A503C4E2D80194F5C4E1A");//中耙作业
            break;
            default:
                break;
        }
    }
}



void MainService::on_tts_message(unsigned int id,QString str)
{
    emit tts_message(id ,str);//-->tts
}
void MainService::on_stream_change(bool id)
{
    emit stream_change(id);//-->mainwindow
}

void MainService::on_stackedshowSelect(bool id)
{
    emit stackedshowSelect(id);//-->mainwindow
}

void MainService::on_menushowSelect()
{
    emit menushowSelect();//-->mainwindow
}




MainService::~MainService(){
    sensor_analysis->stopImmediately();
    sensor_analysis->wait();
    httpService->stopImmediately();
    httpService->wait();
}
