/******************************************************************************************
*-----First Created. time: 28-June-2020	3:30  P.m.
* @File name    :SensorAnalysis.cpp
* @Platform     :Hi3520D - linux-3.0.8 - Qt4.8.6 - arm-hisiv100nptl-linux-gcc
* @Author       :MacianYuan
* @E-mail       :macian@foxmail.com
* @Project      :农机作业远程监控终端
*******************************************************************************************
* @Description  :
* @Attention    :
* @Log          :
*****************29-June-2020(MacianYuan):
*****************
* @Copyright© @MacianYuan Anhui Xinjie Technology Co.,Ltd.  2010-2020  All rights reserved.
******************************************************************************************/
#include "sensoranalysis.h"
#include "mpi_sys.h"

static unsigned int get_oil_count;
static unsigned int get_S1_count = 0;
static unsigned int get_S2_count = 0;
static unsigned int get_S3_count = 0;
static unsigned int get_S4_count = 0;
static unsigned int sensor_judge_count = 0;
void SensorAnalysis::run()
{
    //标记可以运行
    m_isCanRun = true;
    //获取机具识别号功能  在for之前只获取一次机具识别号    for之内循环获取
    bool connect_DN = connect_devicenumber() ;
    for(;;){
        //获取到机具识别号之后
        if ( connect_DN == true ){
            //通过获取的作业模式，获取到具体使用哪些传感器和因传感器脱机作业模式下传感器的工作模式
            bool connect_SE = connect_sensor();
            while ( connect_SE == true ){

                if(get_oil_count >= 5){
                    get_sensor_oil_wear();
                    msleep(500);
                    get_oil_count = 0;
                }
                get_oil_count++;
                qDebug()<< "get_oil_count" << get_oil_count;

                switch (Device_public::Motype){
                    case 8:{//打捆作业  1输出打捆、深度   2输出深度   3输出旋转   4输出打捆、旋转   0不输出
                        switch(Device_public::Bunding8_WorkMode){
                            case 1:{
                                //获取深度传感器
                                if(Device_public::Sensor1_Online == 0){
                                    get_S1_count ++;
                                }else{
                                    get_S1_count = 0;
                                }

                                if(get_S1_count == 3){
                                   get_S1_count = 0;
                                   //传感器1 脱机
                                   emit tts_message(1,"4F20611F56684E008131673A4E258C284F5C4E1A");
                                }

                                get_sensor1_number();
                                msleep(500);

                                //获取打捆传感器
                                if(Device_public::Sensor3_Online == 0){
                                    get_S3_count ++;
                                }else{
                                    get_S3_count  = 0;
                                }

                                if(get_S3_count == 3){
                                   get_S3_count = 0;
                                   //传感器3 脱机
                                   emit tts_message(1,"4F20611F56684E098131673A4E258C284F5C4E1A");
                                }

                                //获取打捆传感器
                                get_sensor3_number();
                                msleep(500);
                            }break;
                            case 2:{
                                //获取深度传感器
                                if(Device_public::Sensor1_Online == 0){
                                    get_S1_count ++;
                                }else{
                                    get_S1_count = 0;
                                }

                                if(get_S1_count == 3){
                                   get_S1_count = 0;
                                   //传感器1 脱机
                                   emit tts_message(1,"4F20611F56684E008131673A4E258C284F5C4E1A");
                                }
                                get_sensor1_number();
                                msleep(500);
                            }break;
                            case 3:{
                                //获取旋转传感器
                                if(Device_public::Sensor2_Online == 0){
                                    get_S2_count ++;
                                }else{
                                    get_S2_count = 0;
                                }

                                if(get_S2_count == 3){
                                   get_S2_count = 0;
                                   //传感器1 脱机
                                   emit tts_message(1,"4F20611F56684E8C8131673A4E258C284F5C4E1A");
                                }
                                get_sensor2_number();
                                msleep(500);
                            }break;
                            case 4:{
                                //获取旋转传感器
                                if(Device_public::Sensor2_Online == 0){
                                    get_S2_count ++;
                                }else{
                                    get_S2_count = 0;
                                }

                                if(get_S2_count == 3){
                                   get_S2_count = 0;
                                   //传感器1 脱机
                                   emit tts_message(1,"4F20611F56684E8C8131673A4E258C284F5C4E1A");
                                }

                                get_sensor2_number();
                                msleep(500);

                                //获取打捆传感器

                                if(Device_public::Sensor3_Online == 0){
                                    get_S3_count ++;
                                }else{
                                    get_S3_count  = 0;
                                }

                                if(get_S3_count == 3){
                                   get_S3_count = 0;
                                   //传感器3 脱机
                                   emit tts_message(1,"4F20611F56684E098131673A4E258C284F5C4E1A");
                                }

                                get_sensor3_number();
                                msleep(500);
                            }break;
                            default:{

                            }break;
                        }
                    }break;
                    case 3:{//深翻作业  1输出深度最大值  2输出深度1  3输出深度2  0不输出
                        switch(Device_public::SpulchnianiePodglebia3_WorkMode){
                            case 1:{
                                //获取深度传感器1
                                if(Device_public::Sensor1_Online == 0){
                                    get_S1_count ++;
                                }else{
                                    get_S1_count = 0;
                                }

                                if(get_S1_count == 3){
                                   get_S1_count = 0;
                                   //传感器1 脱机
                                   emit tts_message(1,"4F20611F56684E008131673A4E258C284F5C4E1A");
                                }
                                get_sensor1_number();
                                msleep(500);

                                //获取深度传感器2
                                if(Device_public::Sensor2_Online == 0){
                                    get_S2_count ++;
                                }else{
                                    get_S2_count = 0;
                                }

                                if(get_S2_count == 3){
                                   get_S2_count = 0;
                                   //传感器1 脱机
                                   emit tts_message(1,"4F20611F56684E8C8131673A4E258C284F5C4E1A");
                                }
                                get_sensor2_number();
                                msleep(500);

                                //比较大小
                            }break;
                            case 2:{
                                //获取深度传感器1
                                if(Device_public::Sensor1_Online == 0){
                                    get_S1_count ++;
                                }else{
                                    get_S1_count = 0;
                                }

                                if(get_S1_count == 3){
                                   get_S1_count = 0;
                                   //传感器1 脱机
                                   emit tts_message(1,"4F20611F56684E008131673A4E258C284F5C4E1A");
                                }
                                get_sensor1_number();
                                msleep(500);
                            }break;
                            case 3:{
                                //获取深度传感器2
                                if(Device_public::Sensor2_Online == 0){
                                    get_S2_count ++;
                                }else{
                                    get_S2_count = 0;
                                }

                                if(get_S2_count == 3){
                                   get_S2_count = 0;
                                   //传感器1 脱机
                                   emit tts_message(1,"4F20611F56684E8C8131673A4E258C284F5C4E1A");
                                }
                                get_sensor2_number();
                                msleep(500);
                            }break;
                            default:{

                            }break;
                        }
                    }break;
                    case 4://免耕播种  1输出深度 2输出旋转 0不输出
                    case 5://稻麦收获
                    case 6://玉米收获
                    case 10://植保作业
                    case 13://抛肥作业
                    case 14:{//收获作业
                        switch(Device_public::Wheat4_WorkMode){
                            case 1:{
                                //获取深度传感器
                                if(Device_public::Sensor1_Online == 0){
                                    get_S1_count ++;
                                }else{
                                    get_S1_count = 0;
                                }

                                if(get_S1_count == 3){
                                   get_S1_count = 0;
                                   //传感器1 脱机
                                   emit tts_message(1,"4F20611F56684E008131673A4E258C284F5C4E1A");
                                }
                                get_sensor1_number();
                                msleep(500);
                            }break;
                            case 2:{
                                //获取旋转传感器
                                if(Device_public::Sensor2_Online == 0){
                                    get_S2_count ++;
                                }else{
                                    get_S2_count = 0;
                                }

                                if(get_S2_count == 3){
                                   get_S2_count = 0;
                                   //传感器1 脱机
                                   emit tts_message(1,"4F20611F56684E8C8131673A4E258C284F5C4E1A");
                                }
                                get_sensor2_number();
                                msleep(500);
                            }break;
                            default:{

                            }break;
                        }
                    }break;
                    case 1://深松作业  传感器1计算
                    case 2://环田作业
                    case 9://中耕作业
                    case 11://插秧作业
                    case 12://旋耕作业
                    case 15:{//中耙作业
                        //获取深度传感器
                        if(Device_public::Sensor1_Online == 0){
                            get_S1_count ++;
                        }else{
                            get_S1_count = 0;
                        }

                        if(get_S1_count == 3){
                           get_S1_count = 0;
                           //传感器1 脱机
                           emit tts_message(1,"4F20611F56684E008131673A4E258C284F5C4E1A");
                        }
                        get_sensor1_number();
                        msleep(500);
                    }break;
                    case 7:{//位置服务   不需要传感器
                        Device_public::wodeep =15;
                    }break;
                    default:
                    break;
                }
            }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
            {
                QMutexLocker locker(&m_lock);
                //在每次循环判断是否可以运行，如果不行就退出循环
                if(!m_isCanRun){
                    qDebug()<<"m_isCanRun";
                    return;
                }
            }
        }//connect_DN == true
        //机具号获取失败之后
        //需要测量的传感器有油量、
        else if ( connect_DN == false ) {
            //仍然进行油量信息的检测

            if(get_oil_count >= 3){// 5s
                //set_sensor_oil_wear();
                //msleep(1000);
                get_sensor_oil_wear();
                get_oil_count = 0;
            }else{
                get_oil_count++;
                msleep(1000);
            }

        }
    }
}
/******************************************************************************************
* @Function         : connect_devicenumber             20200831
*
* @Purpose          : 开机连接机具传感器
* @Description      :
********************* N次机会获取外置机具号，如果获取不到开始获取内置机具号
********************* 内置机具号从配置文件中读取，如果获取不到内置机具号则判为未获取机具识别号
********************* End
* @Grammar&Demand   :
********************* <QThread>
********************* 利用线程msleep秒睡眠
********************* End
* @Param            : None
* @Retval           : Bool  true 成功  false失败
* @Others           : 需在线程run()中使用  需在获取作业模式后运行
* @E.g.             : None
* @Log          :
*****************31-August-2020(MacianYuan):修改语音播报内容
*****************
******************************************************************************************/
bool SensorAnalysis::connect_devicenumber()
{

    unsigned int get_DN_count;      //获取次数计数
    QString device_num_tts;         //tts字符串

    //机具号为空 获取机具号  获取到跳出循环
    get_device_number();
    //足够的延时  机具传感器反馈具有延时  使得获取机具号确保成功
    msleep(1000);

    //获取 GET_DEVICE_NUMBER_TIMES 次机具号
    for( get_DN_count = 0; get_DN_count < GET_DEVICE_NUMBER_TIMES; get_DN_count++ ) {

        qDebug()<<"get_device_number"<<get_DN_count<<Device_public::device_number;
        device_num_tts = Device_public::device_number;
        //获取到数据
        if ( Device_public::device_number.startsWith("20") ) {                      //"20200701001"

            //添加空字符 播报字符串更加清晰悦耳
            for( int i = 1; i <= 20; ) {
                device_num_tts =  device_num_tts.insert(i," ");   //"2 0 2 0 0 7 0 1 0 0 1"
                i = i+2;
            }
            device_num_tts =  device_num_tts.insert(7,"  ");      //"2 0 2 0   0 7 0 1 0 0 1"
            device_num_tts =  device_num_tts.insert(18,"  ");     //"2 0 2 0   0 7 0 1   0 0 1"
            emit tts_message(2,device_num_tts);

            //获取机具识别号后 发射获取阿里云物联网平台秘钥 --> void MainWindow::on_get_aliyun_key()
            emit get_aliyun_key();
            return true;
        }

        //获取机具号   -->Mainwindow显示机具号  -->TTS播报机具号
        get_device_number();
        //足够的延时  机具传感器反馈具有延时  使得获取机具号确保成功
        msleep(1000);

    }//通过判断Device_public::device_number.isEmpty不为空，跳出循环，说明正常获取机具识别号
////////////////////////////////////////////////////////////////////////////////////
//////N次机具号获取不成功 跳出for循环  主动获取内置机具号
/////////////////////////////////////////////////////////////////////////////////////
    //播报 正在获取内置机具号
    emit tts_message(1,"6B63572883B753D651857F6E673A517753F7");
    //读取配置文件中的特殊机具号(内置机具号)
//    IniParse ini(AMIOT_CONFIG);
//    Device_public::device_number = ini.GetString("Device.SpecificDeviceNumber").c_str();
    device_num_tts = Device_public::device_number;

    //如果获取的机具号是以20为开始的字符串，说明获取成功
    if( device_num_tts.startsWith("20") ){

        //内置机具号标识符置位   置1 使用了内置机具号   默认0 使用外置机具号
        Device_public::Internal_DN = 1;
        qDebug()<<"Device_public::Internal_DN"<<Device_public::Internal_DN;
        //内置机具号播报；添加空字符，播报字符串更加清晰悦耳
        for( int i = 1; i <= 20; ){
            device_num_tts = device_num_tts.insert(i," ");//"2 0 2 0 0 7 0 1 0 0 1"
            i = i+2;
        }
        device_num_tts =  device_num_tts.insert(7,"  ");//"2 0 2 0   0 7 0 1 0 0 1"
        device_num_tts =  device_num_tts.insert(18,"  ");//"2 0 2 0   0 7 0 1   0 0 1"
        emit tts_message(2,device_num_tts);

        //获取机具识别号后 发射获取阿里云物联网平台秘钥 --> void MainWindow::on_get_aliyun_key()
        emit get_aliyun_key();
        return true;
    }else{
        qDebug()<<"connect_devicenumber failed";
        //语音播报 未获取机具识别号
        emit tts_message(1,"672A83B753D6673A51778BC6522B53F7");
        return false;//异常跳出循环，说明未获取机具识别号
    }
}

/******************************************************************************************
* @Function         : connect_sensor
*
* @Purpose          : 开机连接传感器，判断模式
* @Description      :
********************* 通过作业模式判断可能使用的传感器是否脱机，从而进一步根据传感器状态计算工作数据
********************* 传感器采集使用波特率9600采集，大约1ms传输1字节，在发送获取之后需等待足够获取完成数据包的时间
********************* End
* @Grammar&Demand   :
********************* <QThread>
********************* 利用线程msleep毫秒睡眠
********************* End
* @Param            : None
* @Retval           : Bool  true 成功  false失败
* @Others           : 需在线程run()中使用  需在获取作业模式后运行
* @E.g.             : None
******************************************************************************************/
bool SensorAnalysis::connect_sensor()
{
    static unsigned int count = 0;
    while(Device_public::Motype > 0){
        //qDebug()<<"connect_sensor";
        //最高2250ms的时间初始化所有传感器
        for(; count < 3; count ++ ){
            switch (Device_public::Motype){
                case 8:{            //打捆作业，特有的打捆传感器（传感器3）；亦会用到传感器1、传感器2
                    get_sensor3_number();
                    msleep(500);
                }
                case 4:{            //免耕播种，特有的播种传感器（传感器4）；亦会用到传感器1、传感器2
                    if(Device_public::Motype != 8){
                        get_sensor4_number();
                        msleep(500);
                    }
                }
                case 5:             //稻麦收获，有传感器1输出传感器1（深度）；无传感器1输出传感器2（旋转）
                case 6:             //玉米收获，有传感器1输出传感器1（深度）；无传感器1输出传感器2（旋转）
                case 10:            //植保作业，有传感器1输出传感器1（深度）；无传感器1输出传感器2（旋转）
                case 13:            //抛肥作业，有传感器1输出传感器1（深度）；无传感器1输出传感器2（旋转）
                case 14:            //收获作业，有传感器1输出传感器1（深度）；无传感器1输出传感器2（旋转）
                case 3:{            //深翻作业，传感器1、传感器2有其一输出在线传感器，都在线比较输出最大值
                    get_sensor2_number();
                    msleep(500);
                }
                case 1:             //深松作业，只使用传感器1
                case 2:             //环田作业，只使用传感器1
                case 9:             //中耕作业，只使用传感器1
                case 11:            //插秧作业，只使用传感器1
                case 12:            //旋耕作业，只使用传感器1
                case 15:{           //中耙作业，只使用传感器1
                    get_sensor1_number();
                    msleep(500);
                }
                case 7:{            //位置服务，不需要传感器
                }break;
                default:{
                    //作业模式异常 无法判断作业模式
                }break;
            }
        }
        if(count >= 3)
            count = 3;
        //3次之后跳出循环，暂时结束获取传感器数据
        //开始判断不同模式下 具体使用哪些传感器
        for(int i =0; i <=1;){
            qDebug()<< "switch Device_public::Motype";
            switch (Device_public::Motype){
                case 8:{            //打捆作业，特有的打捆传感器（传感器3）；亦会用到传感器1、传感器2
                    if((Device_public::Sensor1_Online ==1)&&(Device_public::Sensor2_Online ==1)&&(Device_public::Sensor3_Online ==1)){
                        // 1  1  1   深度传感器输出  旋转传感器未脱机不输出   打捆传感器输出
                        Device_public::Bunding8_WorkMode = 1;
                        Device_public::state_Sersor = 1;
                        return true;
                        //播报传感器正常

                    }else if((Device_public::Sensor1_Online ==1)&&(Device_public::Sensor2_Online ==0)&&(Device_public::Sensor3_Online ==1)){
                        // 1  0  1   深度传感器输出  旋转传感器脱机          打捆传感器输出
                        Device_public::Bunding8_WorkMode = 1;
                        Device_public::state_Sersor = 1;
                        return true;
                        //播报旋转传感器脱机 正常工作

                    }else if((Device_public::Sensor1_Online ==1)&&(Device_public::Sensor2_Online ==1)&&(Device_public::Sensor3_Online ==0)){
                        // 1  1  0   深度传感器输出  旋转传感器未脱机不输出   打捆传感器脱机
                        Device_public::Bunding8_WorkMode = 2;
                        Device_public::state_Sersor = 1;
                        return true;
                        //播报打捆传感器脱机 正常工作

                    }else if((Device_public::Sensor1_Online ==1)&&(Device_public::Sensor2_Online ==0)&&(Device_public::Sensor3_Online ==0)){
                        // 1  0  0   深度传感器输出  旋转传感器脱机          打捆传感器脱机
                        Device_public::Bunding8_WorkMode = 2;
                        Device_public::state_Sersor = 1;
                        return true;
                        //播报旋转传感器、打捆传感器脱机 正常工作

                    }else if((Device_public::Sensor1_Online ==0)&&(Device_public::Sensor2_Online ==1)&&(Device_public::Sensor3_Online ==0)){
                        // 0  1  0   深度传感器脱机  旋转传感器输出          打捆传感器脱机
                        Device_public::Bunding8_WorkMode = 3;
                        Device_public::state_Sersor = 1;
                        return true;
                        //播报深度传感器、打捆传感器脱机 正常工作

                    }else if((Device_public::Sensor1_Online ==0)&&(Device_public::Sensor2_Online ==1)&&(Device_public::Sensor3_Online ==1)){
                        // 0  1  1   深度传感器脱机  旋转传感器输出          打捆传感器输出
                        Device_public::Bunding8_WorkMode = 4;
                        Device_public::state_Sersor = 1;
                        return true;
                        //播报深度传感器脱机 正常工作
                    }else {
                        // 0  0  1   深度传感器脱机  旋转传感器脱机          打捆传感器未脱机不输出
                        // 0  0  0   深度传感器脱机  旋转传感器脱机          打捆传感器脱机
                        Device_public::Bunding8_WorkMode = 0;

                        sensor_judge_count ++;
                        if(sensor_judge_count == 300){
                            //传感器脱机严谨作业
                            emit tts_message(1,"4F20611F56688131673A4E258C284F5C4E1A");
                            Device_public::state_Sersor = 2;
                            //播报深度传感器、旋转传感器 无法工作
                            i = 2;//异常跳出，传感器故障
                            sensor_judge_count = 0;
                        }
                    }
                    qDebug()<<" Device_public::Bunding8_WorkMode"<< Device_public::Bunding8_WorkMode;
                }break;
                case 4:{            //免耕播种，特有的播种传感器（传感器4）；亦会用到传感器1、传感器2
                    if(Device_public::Sensor4_Online ==1){
                        //播报播种传感器在线
                    }else{
                        //播报播种传感器脱机
                    }
                }
                case 5:             //稻麦收获，有传感器1输出传感器1（深度）；无传感器1输出传感器2（旋转）
                case 6:             //玉米收获，有传感器1输出传感器1（深度）；无传感器1输出传感器2（旋转）
                case 10:            //植保作业，有传感器1输出传感器1（深度）；无传感器1输出传感器2（旋转）
                case 13:            //抛肥作业，有传感器1输出传感器1（深度）；无传感器1输出传感器2（旋转）
                case 14:{           //收获作业，有传感器1输出传感器1（深度）；无传感器1输出传感器2（旋转）
                    if((Device_public::Sensor1_Online ==1)&&(Device_public::Sensor2_Online ==1)){
                        Device_public::Wheat4_WorkMode = 1;
                        Device_public::state_Sersor = 1;
                        //播报传感器正常
                         return true;
                    }else if((Device_public::Sensor1_Online ==1)&&(Device_public::Sensor2_Online ==0)){
                        Device_public::Wheat4_WorkMode = 1;
                        Device_public::state_Sersor = 1;
                        //播报旋转传感器脱机 正常工作
                         return true;
                    }else if((Device_public::Sensor1_Online ==0)&&(Device_public::Sensor2_Online ==1)){
                        Device_public::Wheat4_WorkMode = 2;
                        Device_public::state_Sersor = 1;
                        //播报深度传感器脱机 正常工作
                         return true;
                    }else if((Device_public::Sensor1_Online ==0)&&(Device_public::Sensor2_Online ==0)){
                        Device_public::Wheat4_WorkMode = 0;
                        //播报深度传感器脱机  旋转传感器脱机 无法工作
                        //传感器脱机严谨作业
                        sensor_judge_count ++;
                        if(sensor_judge_count == 300){
                            emit tts_message(1,"4F20611F56688131673A4E258C284F5C4E1A");
                            Device_public::state_Sersor = 2;
                            i = 2;//异常跳出，传感器故障
                            sensor_judge_count = 0;
                        }
                    }
                    qDebug()<<" Device_public::Wheat4_WorkMode"<< Device_public::Wheat4_WorkMode;
                }break;
                case 3:{            //深翻作业，传感器1、传感器2有其一输出在线传感器，都在线比较输出最大值
                    if((Device_public::Sensor1_Online == 1)&&(Device_public::Sensor2_Online == 1)){
                        Device_public::SpulchnianiePodglebia3_WorkMode = 1;
                        Device_public::state_Sersor = 1;
                        //播报传感器正常 输出最大值
                        return true;
                    }else if((Device_public::Sensor1_Online == 1)&&(Device_public::Sensor2_Online == 0)){
                        Device_public::SpulchnianiePodglebia3_WorkMode = 2;
                        Device_public::state_Sersor = 1;
                        //播报深度2传感器脱机  正常工作
                        return true;
                    }else if((Device_public::Sensor1_Online == 0)&&(Device_public::Sensor2_Online == 1)){
                        Device_public::SpulchnianiePodglebia3_WorkMode = 3;
                        Device_public::state_Sersor = 1;
                        //播报深度1传感器脱机  正常工作
                        return true;
                    }else if((Device_public::Sensor1_Online == 0)&&(Device_public::Sensor2_Online == 0)){
                        Device_public::SpulchnianiePodglebia3_WorkMode = 0;
                        //播报深度1传感器脱机  深度2传感器脱机 无法工作

                        //传感器脱机严谨作业
                        sensor_judge_count ++;
                        if(sensor_judge_count == 300){
                            emit tts_message(1,"4F20611F56688131673A4E258C284F5C4E1A");
                            Device_public::state_Sersor = 2;
                            i = 2;//异常跳出，传感器故障
                            sensor_judge_count = 0;
                        }
                    }
                     qDebug()<<" Device_public::SpulchnianiePodglebia3_WorkMode"<< Device_public::SpulchnianiePodglebia3_WorkMode;
                }break;
                case 1:             //深松作业，只使用传感器1
                case 2:             //环田作业，只使用传感器1
                case 9:             //中耕作业，只使用传感器1
                case 11:            //插秧作业，只使用传感器1
                case 12:            //旋耕作业，只使用传感器1
                case 15:{           //中耙作业，只使用传感器1
                    if(Device_public::Sensor1_Online == 1){
                        Device_public::Deep_sigh_homework1_WorkMode = 1;
                        Device_public::state_Sersor = 1;
                        //播报深度传感器正常 输出
                        return true;
                    }else {
                        Device_public::Deep_sigh_homework1_WorkMode = 2;
                        //播报深度传感器脱机 无法工作
                        //传感器脱机严谨作业
                        sensor_judge_count ++;
                        if(sensor_judge_count == 300){
                            emit tts_message(1,"4F20611F56688131673A4E258C284F5C4E1A");
                            Device_public::state_Sersor = 2;
                            i = 2;//异常跳出，传感器故障
                            sensor_judge_count = 0;
                        }
                    }
                    qDebug()<<" Device_public::Deep_sigh_homework1_WorkMode"<< Device_public::Deep_sigh_homework1_WorkMode;
                }break;
                case 7:{            //位置服务，不需要传感器


                }break;
                default:{
                    //作业模式异常 无法判断作业模式
                    //传感器脱机严谨作业
                    //emit tts_message(1,"4F20611F56688131673A4E258C284F5C4E1A");
                    return false;//异常跳出，传感器故障
                }
            }
        }
        //正常跳出，传感器识别到，并成功判断作业模式下传感器工作模式
        return true;
    }
}

void SensorAnalysis::on_sensor_data_analysis(QByteArray str)
{

    QString OriginalData = QString(str.toHex().toUpper());

    qDebug()<<"OriginalData"<<OriginalData;

    QString CaptureData;
    //55 0A 00 00 00 00 00 00 00 00 00 00 00 00 32 30 31 39 33 31 36 38 33 37 33 76 03
    //机具传感器 机具号解析
    if(OriginalData.startsWith("550A")){
        //截取机具号对应数据段
        CaptureData= OriginalData.mid(28,22);
        //原始机具号清空
        Device_public::DeviceName.clear();
        //数据存储到字节数组中
        for(int i =1;i<CaptureData.size();i+=2){
           Device_public::DeviceName.append(CaptureData.at(i));
        }
        Device_public::device_number = Device_public::DeviceName.data();
        //qDebug()<<"device_number"<<Device_public::device_number;
        //机具号保存到配置文件中
//        IniParse ini(AMIOT_CONFIG);
//        ini.Set("Device.DeviceNumber",Device_public::device_number.toStdString());
//        ini.SaveFile();
        //识别到外置机具号，未使用内置机具号
        Device_public::Internal_DN =0;
    }
    //55 01 14 E7 F1 3B 26 5F 8D BC 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 76 03
    //传感器1数据解析
    if(OriginalData.startsWith("5501")){

        QByteArray ConversionData;
        ConversionData.append(OriginalData.mid(10,2));
        ConversionData.append(OriginalData.mid(8,2));
        ConversionData.append(OriginalData.mid(6,2));
        ConversionData.append(OriginalData.mid(4,2));
        //截取机具号对应数据段
        CaptureData = ConversionData.data();
        qDebug()<<CaptureData;

        emit sensor_capturedata(CaptureData);

        int hex =CaptureData.toUInt(0,16);
        Device_public::sensor1_data=*(float*)&hex;   //角度值
        qDebug()<<"5501"<<Device_public::sensor1_data;


        Device_public::Sensor1_Online = 1;
        qDebug()<<"Device_public::Sensor1_Online"<<Device_public::Sensor1_Online;
    }
    //55 02 82 32 A4 BE 18 09 BD 3E 79 9B 9D BF 00 00 00 00 00 00 00 00 00 00 00 76 03
    //传感器2数据解析
    if(OriginalData.startsWith("5502")){

        QByteArray ConversionData;
        ConversionData.append(OriginalData.mid(10,2));
        ConversionData.append(OriginalData.mid(8,2));
        ConversionData.append(OriginalData.mid(6,2));
        ConversionData.append(OriginalData.mid(4,2));
        //截取机具号对应数据段
        CaptureData = ConversionData.data();
        qDebug()<<CaptureData;
        int hex =CaptureData.toUInt(0,16);
        Device_public::sensor2_data=*(int*)&hex;  //角度值
        qDebug()<<"5502"<<Device_public::sensor2_data;

        Device_public::Sensor2_Online = 1;
        qDebug()<<"Device_public::Sensor2_Online"<<Device_public::Sensor2_Online;
    }
    //55 03 01 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 76 03
    //55 03 02 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 76 03
    //55 03 02 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 76 03
    //传感器3数据解析  打捆
    if(OriginalData.startsWith("5503")){
        QString ValidData;
        static QString OldData;
        QString NewData;
        QByteArray ConversionData;
        //判断有效位   截取有效位对应数据段
        ValidData.clear();
        ValidData.append(OriginalData.mid(9,1));
        //qDebug()<<"ValidData"<<ValidData;
        //有效为为1 为有效数据
        if(ValidData.contains("1")){
            NewData.clear();
            NewData.append(OriginalData.mid(5,1));
            //qDebug()<<"NewData:"<<NewData;
            //如果新老数据不相同
            if(NewData.compare(OldData) != 0){
               Device_public::binding_data ++;
               if(Device_public::binding_data >200)
                    Device_public::binding_data =200;
               //qDebug()<<"Device_public::binding_data:"<<Device_public::binding_data;
            }else{

            }
            OldData.clear();
            OldData.append(NewData);
            //qDebug()<<"OldData:"<<OldData;
        }else{

        }
        Device_public::Sensor3_Online = 1;
        qDebug()<<"Device_public::Sensor3_Online"<<Device_public::Sensor3_Online;
    }

    //FF FC 00 11 19 02 3C 00 00 00 01 00 00 00 01 65 FD
    //传感器4数据解析  播种
    if(OriginalData.startsWith("FFFC")&&(OriginalData.endsWith("FD"))){
        Device_public::plant_distance = OriginalData.mid(8,2).toInt(0,16);//株距
        Device_public::line_number = OriginalData.mid(10,2).toInt(0,16);//行数
        Device_public::line_distance = OriginalData.mid(12,2).toInt(0,16);//行距
        //暂不实现
        Device_public::Sensor4_Online = 1;
        qDebug()<<"Device_public::Sensor4_Online"<<Device_public::Sensor4_Online;
    }
    //24 21 44 4F 30 31 33 39 0D 0A
    //2A 52 46 56 30 31 30 34 34 2E 35 39 41 44 0D 0A
    //2A 52 46 56 30 31 30 35 39 2E 38 38 42 35 0D 0A
    //2A 52 46 56 30 31 30 35 39 2E 38 39 42 36 0D 0A

    //传感器油量解析
    if( (OriginalData.startsWith("2A43")) && (OriginalData.endsWith("0D0A")) ){

        unsigned int oil_data = str.mid(8,4).toUInt(0,16);
        qDebug()<<"oil wear:"<<oil_data;
        Device_public::sensor_oil_data = (float)oil_data*100/65535;
        qDebug()<<"oil wear:"<<Device_public::sensor_oil_data;
    }
}
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
/// \brief SensorAnalysis::get_device_number
/// \return 0
/////获取传感器信息方法
/////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////
int SensorAnalysis::get_device_number()
{
    // FF FF FF 55 0A 36 00 FF FF FF
    //获取机具号指令
    QByteArray senddata ;
    senddata[0]=0xff;
    senddata[1]=0xff;
    senddata[2]=0xff;
    senddata[3]=0x55;
    senddata[4]=0x0A;
    senddata[5]=0x36;
    senddata[6]=0x00;
    senddata[7]=0xff;
    senddata[8]=0xff;
    senddata[9]=0xff;
    //设置485 发送模式
    HI_MPI_SYS_SetReg(0x200F004C,0);
    HI_MPI_SYS_SetReg(0x201A0400,1);
    HI_MPI_SYS_SetReg(0x201A0004,1);

    //发送获取机具号指令
    WriteCom(Device_public::sensor_port,senddata);

    //短暂的延时
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<20);

    //设置485 接收模式
    HI_MPI_SYS_SetReg(0x200F004C,0);
    HI_MPI_SYS_SetReg(0x201A0400,1);
    HI_MPI_SYS_SetReg(0x201A0004,0);

    return 0;
}

int SensorAnalysis::get_sensor1_number()
{
     //获取机具号指令
    QByteArray senddata ;
    senddata[0]=0xff;
    senddata[1]=0xff;
    senddata[2]=0xff;
    senddata[3]=0x55;
    senddata[4]=0x01;
    senddata[5]=0x36;
    senddata[6]=0x00;
    senddata[7]=0xff;
    senddata[8]=0xff;
    senddata[9]=0xff;
    //设置485 发送模式
    HI_MPI_SYS_SetReg(0x200F004C,0);
    HI_MPI_SYS_SetReg(0x201A0400,1);
    HI_MPI_SYS_SetReg(0x201A0004,1);

    //发送获取机具号指令
    WriteCom(Device_public::sensor_port,senddata);

    //短暂的延时
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<20);

    //设置485 接收模式
    HI_MPI_SYS_SetReg(0x200F004C,0);
    HI_MPI_SYS_SetReg(0x201A0400,1);
    HI_MPI_SYS_SetReg(0x201A0004,0);
    qDebug()<<"get_sensor1_number";
    return 0;
}

int SensorAnalysis::get_sensor2_number()
{
    //获取机具号指令
    QByteArray senddata ;
    senddata[0]=0xff;
    senddata[1]=0xff;
    senddata[2]=0xff;
    senddata[3]=0x55;
    senddata[4]=0x02;
    senddata[5]=0x36;
    senddata[6]=0x00;
    senddata[7]=0xff;
    senddata[8]=0xff;
    senddata[9]=0xff;
    //设置485 发送模式
    HI_MPI_SYS_SetReg(0x200F004C,0);
    HI_MPI_SYS_SetReg(0x201A0400,1);
    HI_MPI_SYS_SetReg(0x201A0004,1);

    //发送获取机具号指令
    WriteCom(Device_public::sensor_port,senddata);

    //短暂的延时
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<20);

    //设置485 接收模式
    HI_MPI_SYS_SetReg(0x200F004C,0);
    HI_MPI_SYS_SetReg(0x201A0400,1);
    HI_MPI_SYS_SetReg(0x201A0004,0);
    qDebug()<<"get_sensor2_number";
    return 0;
}

int SensorAnalysis::get_sensor3_number()
{
     //获取机具号指令
    QByteArray senddata ;
    senddata[0]=0xff;
    senddata[1]=0xff;
    senddata[2]=0xff;
    senddata[3]=0x55;
    senddata[4]=0x03;
    senddata[5]=0x36;
    senddata[6]=0x00;
    senddata[7]=0xff;
    senddata[8]=0xff;
    senddata[9]=0xff;
    //设置485 发送模式
    HI_MPI_SYS_SetReg(0x200F004C,0);
    HI_MPI_SYS_SetReg(0x201A0400,1);
    HI_MPI_SYS_SetReg(0x201A0004,1);

    //发送获取机具号指令
    WriteCom(Device_public::sensor_port,senddata);

    //短暂的延时
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<20);

    //设置485 接收模式
    HI_MPI_SYS_SetReg(0x200F004C,0);
    HI_MPI_SYS_SetReg(0x201A0400,1);
    HI_MPI_SYS_SetReg(0x201A0004,0);
    qDebug()<<"get_sensor3_number";
    return 0;
}


int SensorAnalysis::get_sensor4_number()
{
     //获取机具号指令
    QByteArray senddata ;
    senddata[0]=0xff;
    senddata[1]=0xff;
    senddata[2]=0xff;
    senddata[3]=0xff;
    senddata[4]=0x03;
    senddata[5]=0xff;
    senddata[6]=0xff;
    senddata[7]=0xff;
    senddata[8]=0xff;
    senddata[9]=0xff;
    //设置485 发送模式
    HI_MPI_SYS_SetReg(0x200F004C,0);
    HI_MPI_SYS_SetReg(0x201A0400,1);
    HI_MPI_SYS_SetReg(0x201A0004,1);

    //发送获取机具号指令
    WriteCom(Device_public::sensor_port,senddata);

    //短暂的延时
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<20);

    //设置485 接收模式
    HI_MPI_SYS_SetReg(0x200F004C,0);
    HI_MPI_SYS_SetReg(0x201A0400,1);
    HI_MPI_SYS_SetReg(0x201A0004,0);
    qDebug()<<"get_sensor4_number";
    return 0;
}

int SensorAnalysis::get_sensor_oil_wear()
{
     //获取机具号指令
    //QByteArray senddata = "$!DO0139\r\n";
    QByteArray senddata = "$!RY0151\r\n";
//    senddata[0]=0x24;
//    senddata[1]=0x21;
//    senddata[2]=0x44;
//    senddata[3]=0x4f;
//    senddata[4]=0x30;
//    senddata[5]=0x31;
//    senddata[6]=0x33;
//    senddata[7]=0x39;
//    senddata[8]=0x0D;
//    senddata[9]=0x0A;
    //设置485 发送模式
    HI_MPI_SYS_SetReg(0x200F004C,0);
    HI_MPI_SYS_SetReg(0x201A0400,1);
    HI_MPI_SYS_SetReg(0x201A0004,1);

    //发送获取机具号指令
    WriteCom(Device_public::sensor_port,senddata);

    //短暂的延时
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<20);

    //设置485 接收模式
    HI_MPI_SYS_SetReg(0x200F004C,0);
    HI_MPI_SYS_SetReg(0x201A0400,1);
    HI_MPI_SYS_SetReg(0x201A0004,0);
    qDebug()<<"get_sensor_oil_wear";
    return 0;
}

int SensorAnalysis::set_sensor_oil_wear()
{
     //获取机具号指令
    //QByteArray senddata = "$!DO0139\r\n";
    QByteArray senddata = "$!Z10131\r\n";
//    senddata[0]=0x24;
//    senddata[1]=0x21;
//    senddata[2]=0x44;
//    senddata[3]=0x4f;
//    senddata[4]=0x30;
//    senddata[5]=0x31;
//    senddata[6]=0x33;
//    senddata[7]=0x39;
//    senddata[8]=0x0D;
//    senddata[9]=0x0A;
    //设置485 发送模式
    HI_MPI_SYS_SetReg(0x200F004C,0);
    HI_MPI_SYS_SetReg(0x201A0400,1);
    HI_MPI_SYS_SetReg(0x201A0004,1);

    //发送获取机具号指令
    WriteCom(Device_public::sensor_port,senddata);

    //短暂的延时
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<20);

    //设置485 接收模式
    HI_MPI_SYS_SetReg(0x200F004C,0);
    HI_MPI_SYS_SetReg(0x201A0400,1);
    HI_MPI_SYS_SetReg(0x201A0004,0);
    qDebug()<<"set_sensor_oil_wear";
    return 0;
}
//驱动相关
/******************************************************************************************
* @Function         : init
*
* @Purpose          : 传感器驱动初始化
* @Description      :
********************* 串口驱动 增加485RE控制引脚
********************* 硬件端口设备描述符  /dev/ttyAMA3    波特率9600
********************* End
* @Grammar&Demand   :
********************* <QThread>
********************* 利用线程msleep秒睡眠
********************* End
* @Param            : None
* @Retval           : Bool  true 成功  false失败
* @Others           :
* @E.g.             : None
* @Log          :
*****************
*****************
******************************************************************************************/
void SensorAnalysis::init()
{
    //485RE 引脚
    HI_MPI_SYS_SetReg(0x200F004C,0);
    HI_MPI_SYS_SetReg(0x201A0400,1);
    HI_MPI_SYS_SetReg(0x201A0004,0);
    //串口初始化
    Device_public::sensor_port = new QextSerialPort("/dev/ttyAMA3",QextSerialPort::EventDriven);
    //注意：得要先打开串口，然后再设置串口的参数，不然设置无效！！！
    int m_fd = Device_public::sensor_port->open(QIODevice::ReadWrite);

    //定义串口对象，并传递参数，在构造函数里对其进行初始化
    if(m_fd){
        //Device_public::sensor_port->setDtr(true);
        //Device_public::sensor_port->setRts(false);
        //设置波特率
        Device_public::sensor_port->setBaudRate(BAUD9600);
        //设置数据位
        Device_public::sensor_port->setDataBits(DATA_8);
        //设置奇偶校验
        Device_public::sensor_port->setParity(PAR_NONE);
        //设置停止位
        Device_public::sensor_port->setStopBits(STOP_1);
        //设置数据流控制，我们使用无数据流的默认设置
        Device_public::sensor_port->setFlowControl(FLOW_OFF);
        //设置延时      --Modify 改小点
        Device_public::sensor_port->setTimeout(10);
        qDebug() <<"Device_public::sensor_port /dev/ttyAMA3 success!";
    }else{
        qDebug () << tr("open serial failed");
    }

    //定时接收100ms的数据
    timer = new QTimer(this);
    timer->setInterval(GET_DATA_MS_TIME);
    //在有数据到达时开启计时
    connect(timer, SIGNAL(timeout()), this,SLOT(on_timeout()));

    //有数据读取到，进入数据分析槽函数，分析出数据
    connect(Device_public::sensor_port, SIGNAL(readyRead()), this,SLOT(ReadCom()));
    //连接分析函数
    connect(this,SIGNAL(sensor_data_analysis(QByteArray)),this,SLOT(on_sensor_data_analysis(QByteArray)));
}


void SensorAnalysis::stopImmediately()
{
    QMutexLocker locker(&m_lock);
    m_isCanRun = false;
}

//读数据 对数据进行解析，定时100ms的数据
void SensorAnalysis::ReadCom()
{
    //开始接受下面100ms的数据
    timer->start();
    baRcvData.append(Device_public::sensor_port->readAll());

}
void SensorAnalysis::on_timeout()
{
    timer->stop();
    qDebug()<<"baRcvData"<<baRcvData;

    if ( baRcvData.length() != 0 ){
        emit sensor_data_analysis(baRcvData);
        baRcvData.clear();
    }

}

int SensorAnalysis::WriteCom(QextSerialPort *write_com, QByteArray data)
{
    if(write_com == 0  || !write_com->isOpen()){
        qDebug() << "SensorAnalysis write_com Open Failed!";
        return -1;
    }
    //qDebug()<<data;
    if(write_com->write(data) <= 0){
        //QMessageBox::information(0,tr("错误"),tr("通讯中断"),QMessageBox::Ok);
        write_com->close();
        return -1;
    }
    return 0;
}

SensorAnalysis::SensorAnalysis(QObject *parent) : QThread(parent) { }

SensorAnalysis::~SensorAnalysis() { }
