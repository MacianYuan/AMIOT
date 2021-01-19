///******************************************************************************************
//*-----First Created. time: 11-Jan-2021	10:30  A.m.
//* @File name    :mainwindow.cpp
//* @Platform     :Hi3520DV300 - linux-3.0.8 - Qt5.12.7 - arm-hisiv300-linux-gcc
//* @Author       :MacianYuan
//* @E-mail       :macian@foxmail.com
//* @Project      :农机作业远程监控终端
//*******************************************************************************************
//* @Description  :
//* @Attention    :
//* @Log          :
//*****************
//*****************
//* @Copyright© @MacianYuan Anhui Xinjie Technology Co.,Ltd.   All rights reserved.
//******************************************************************************************/

//#pragma once


//#include <QColor>
//#include <QString>

//namespace ns_train {

//    // == 颜色相关 =============================================================================================
//    /**
//    * @brief 解析颜色, 将字符串转换为QColor.
//             格式:r,g,b[,a], alpha可选

//    * @param[in] strColor  待解析的字符串
//    * @return 解析所得的颜色
//    */
//    QColor parseColor(const QString& strColor);

//    /**
//    * @brief 将QColor格式化为字符串,输出的格式:r,g,b,a

//    * @param[in] clr  待转换的颜色
//    * @return 解析所得的颜色字符串
//    */
//    QString getColorRgbValue(const QColor& clr);

//    // == 文件相关 =============================================================================================

//    /**
//    * @brief 获取指定path的字符串,
//	*		 如果使用环境变量，格式必须为:"$环境变量名/xxx/xxx"
//    *         接口内部负责:
//    *           1. 将"\\"转换为"/"
//    *           2. 自动将环境变量替换为实际路径，环境变量使用$XXX的格式，
//	*				比如， 输入："$TRAINDEVHOME/src/"，
//	*					   输出："d:/xingdianketang/project/gui/src/"
//    *
//    * @param[in] strPath 指定路径
//    * @return 文件名, 全路径。
//	*		  如果传入的是目录，那么返回值是否带最后的斜杠由传入参数决定。
//	*		  也就是说，传入时带最后的斜杠，那么返回值也带最后的斜杠，反之则不带。
//    */
//    QString getPath(const QString& strPath);

//    /**
//    * @brief 获取指定path所在的全目录名,
//	*		 如果使用环境变量，格式必须为:"$环境变量名/xxx/xxx.yy"
//    *        接口内部负责:
//    *          1. 将"\\"转换为"/"
//    *          2. 自动将环境变量替换为实际路径，环境变量使用$XXX的格式，
//	*			  比如， 输入："$TRAINDEVHOME/src/a.txt"，
//	*				     输出："d:/xingdianketang/project/gui/src/"
//	*
//    * @param[in] strPath 指定路径(可以是文件的全路径)，
//	*					 比如:/usr/local/a.txt，也可以是目录：/usr/local。
//    * @return 文件名所在目录
//    */
//    QString getDirectory(const QString& strPath);
//    /**
//    * @brief 获取指定文件名的名称，如果使用环境变量，格式必须为:"$环境变量名/xxx/xxx.yy"
//    *        接口内部负责:
//    *          1. 将"\\"转换为"/"
//    *          2. 自动将环境变量替换为实际路径，环境变量使用$XXX的格式，
//	*			  比如， 输入："$TRAINDEVHOME/src/a.txt"，输出："a.txt"
//    *
//    * @param[in] strFilePath 指定文件（全路径）
//    * @return 文件名(a.txt)
//    */
//    QString getFileName(const QString& strFilePath);

//	/**
//	* @brief 获取指定strDirectory的当前子目录名,
//	*		 如果使用环境变量，格式必须为:"$环境变量名/xxx/xxx/xxx/"
//	*		 接口内部负责:
//	*			1. 将"\\"转换为"/"
//	*			2. 自动将环境变量替换为实际路径，环境变量使用$XXX的格式，
//	*			   比如， 输入："$TRAINDEVHOME/src/exchange"，输出："exchange"
//	*
//	* @param[in] strDirectory 指定路径
//	* @return 当前子目录名
//	*/
//    QString getNameOfDirectory(const QString& strDirectory);

//    /**
//    * @brief 获取指定目录下的所有文件名

//    * @param[in] strPath 指定路径, 内部会将"\\"转换为"/"。支持环境变量，比如"$TRAINDEVHOME/temp"
//    * @param[in] nameFilters 文件名过滤符,比如:"*.h"
//    * @param[in] bRecursive true:递归. false:仅根目录
//    * @return 文件名列表, 全路径
//    */
//    QStringList getFileList(const QString& strPath, const QStringList& nameFilters, bool bRecursive);
//	/**
//	* @brief 获取指定文件对于指定目录的相对路径,
//	*		 比如，"d:/temp/file/a.txt",相对于 "d:/temp/"的相对路径为"/file/a.txt"。
//	*
//	* @param[in] strFileName 指定文件（带绝对路径）
//	* @param[in] strDirectory 指定路径（带绝对路径），可以不带最后的“/”。
//	* @return 相对路径,带最后的斜杠。
//	*/
//    QString getReleativePath(const QString& strFileName, const QString& strDirectory);

//	/**
//	* @brief 获取指定文件的md5码。
//	* @param[in] strFileName 指定文件（带绝对路径）
//	* @return md5码
//	*/
//    QByteArray getMd5(const QString& strFileName);
//}
