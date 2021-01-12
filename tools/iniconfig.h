/******************************************************************************************
*-----First Created. time: 11-Jan-2021	10:30  A.m.
* @File name    :iniconfig.h
* @Platform     :Hi3520DV300 - linux-3.0.8 - Qt5.12.7 - arm-hisiv300-linux-gcc
* @Author       :MacianYuan
* @E-mail       :macian@foxmail.com
* @Project      :农机作业远程监控终端
*******************************************************************************************
* @Description  : ini格式配置文件定义头文件, 保存的配置文件采用GB2312编码
* @Attention    :
* @Log          :
*****************
*****************
* @Copyright© @MacianYuan Anhui Xinjie Technology Co.,Ltd.   All rights reserved.
******************************************************************************************/

#pragma once

#include <QFile>
#include <QString>
#include <QVector>


namespace ns_train {

// ini格式配置文件处理类
//-----------------------------------------


class CIniConfig {
public:
	CIniConfig();
	~CIniConfig();

    /**
    * @brief 设置配置文件的文件名，使用全路径，不支持环境变量.
    * @param[in] strFileName 文件名
    * @return true:成功,false:失败
    */
	bool  setFileName(const QString& strFileName);

	/**
	* @brief 读取bool类型的键值
	* @param[in] strKey 主键
	* @param[in] strSubKey 子键
	* @param[in] i_nDefault 默认值
	* @param[out] o_bRet true:成功, false:失败
	* @return 数据
	*/
	bool  getBoolean(const QString& strKey, const QString& strSubKey, bool i_nDefault, bool* o_bRet = NULL);
	/**
	* @brief 读取int类型的键值
	* @param[in] strKey 主键
	* @param[in] strSubKey 子键
	* @param[in] i_nDefault 默认值
	* @param[out] o_bRet true:成功, false:失败
	* @return 数据
	*/
	int  getInteger(const QString& strKey, const QString& strSubKey, int i_nDefault, bool* o_bRet = NULL);
	/**
	* @brief 读取浮点类型的键值
	* @param[in] strKey 主键
	* @param[in] strSubKey 子键
	* @param[in] i_fDefault 默认值
	* @param[out] o_bRet true:成功, false:失败
	* @return 数据
	*/
	double  getDouble(const QString& strKey, const QString& strSubKey, double i_fDefault, bool* o_bRet = NULL);
	/**
	* @brief 读取string类型的键值
	* @param[in] strKey 主键
	* @param[in] strSubKey 子键
	* @param[in] strDefault 默认值
	* @param[out] o_bRet true:成功, false:失败
	* @return 数据
	*/
	QString getString(const QString& strKey, const QString& strSubKey, const QString& strDefault, bool* o_bRet = NULL);
	/**
	* @brief 读取string类型的键值(扩展接口),支持多行读取
	* @param[in] strKey 主键
	* @param[in] strSubKey 子键
	* @param[in] strDefault 默认值
	* @param[out] o_bRet true:成功, false:失败
	* @return 数据
	*/
	QString getStringMultiline(const QString& strKey, const QString& strSubKey, const QString& strDefault, bool* o_bRet = NULL);
	/**
	* @brief 设置bool类型的键值
	* @param[in] strKey 主键
	* @param[in] strSubKey 子键
	* @param[in] i_nValue 子键的值
	* @return true:成功, false:失败
	*/
	bool  setBoolean(const QString& strKey, const QString& strSubKey, bool i_nValue);
	/**
	* @brief 设置int类型的键值
	* @param[in] strKey 主键
	* @param[in] strSubKey 子键
	* @param[in] i_nValue 子键的值
	* @return true:成功, false:失败
	*/
	bool  setInteger(const QString& strKey, const QString& strSubKey, int i_nValue);
	/**
	* @brief 设置浮点类型的键值
	* @param[in] strKey 主键
	* @param[in] strSubKey 子键
	* @param[in] i_fValue 子键的值
	* @return true:成功, false:失败
	*/
	bool  setDouble(const QString& strKey, const QString& strSubKey, double i_fValue);
	/**
	* @brief 设置字符串类型的键值, 比如，配置文件内容如下:
	*           [config]
	*           x=xx
	*           y=yy
	*           z=zz
	* @param[in] strKey 主键=config
	* @param[in] strSubKey 子键=x
	* @param[in] strValue 子键的值, 为""表示删除该子健。
	* @return true:成功, false:失败
	*/
	bool  setString(const QString& strKey, const QString& strSubKey, const QString& strValue);
    /**
    * @brief 删除全部键值
    * @return true:成功,false:失败
    */
	bool  deleteAllKeys();
    /**
    * @brief 删除指定主键
    * @param[in] strKey 主键
    * @return true:成功,false:失败
    */
	bool  removeKey(const QString& strKey);	
	/**
    * @brief 删除指定子键
    * @param[in] strKey 主键
    * @param[in] strSubKey 子键
    * @return true:成功,false:失败
    */
	bool  removeSubKey(const QString& strKey, const QString& strSubKey);


 
    /**
    * @brief 读取键值列表, ，比如传入("config"")，得到, "x=xx\ny=yy\nz=zz"
    * @param[in] strKey 主键
    * @return 键值列表
    */
    QString getAllKeys(const QString& strKey);

    /**
    * @brief 设置键值列表, ，比如传入("config", "x=xx\ny=yy\nz=zz")
    *           执行后的结果:
    *           [config]
    *           x=xx
    *           y=yy
    *           z=zz
    * @param[in] strKey 主键
    * @param[in] str 子键值值列表,\n分隔。比如: "x=xx\ny=yy\nz=zz"  
    * @return 结果，true:成功, false:失败
    */	
    bool setAllKeys(const QString& strKey, const QString& Str);

private:
	int findSubKey(const QString& strKey, const QString& strSubKey);
	bool save2File();

	bool getValue(int i_nIndex, QString& o_pValue);
	bool setValue(int i_nIndex, const QString& strValue);
	void addValue(const QString& strKey, const QString& strSubKey, const QString& strValue, int i_nRet);

	QFile				m_file;    // 文件对象
	QVector<QString>	m_vecString;
	bool				m_bOpened;  // 配置文件是否已打开
};

}
