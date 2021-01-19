/******************************************************************************************
*-----First Created. time: 11-Jan-2021	10:30  A.m.
* @File name    :iniconfig.cpp
* @Platform     :Hi3520DV300 - linux-3.0.8 - Qt5.12.7 - arm-hisiv300-linux-gcc
* @Author       :MacianYuan
* @E-mail       :macian@foxmail.com
* @Project      :农机作业远程监控终端
*******************************************************************************************
* @Description  : ini格式配置文件定义实现文件
* @Attention    :
* @Log          :
*****************
*****************
* @Copyright© @MacianYuan Anhui Xinjie Technology Co.,Ltd.   All rights reserved.
******************************************************************************************/
#include <QTextStream>

#include "iniconfig.h"

namespace ns_train {

static const int c_bufLen = 10240;
CIniConfig::CIniConfig() :m_bOpened(false) {
}

CIniConfig::~CIniConfig() {
}

bool CIniConfig::setFileName(const QString& strFileName) {
    if (strFileName.length() == 0)
        return false;

    QString tStr;

    m_file.setFileName(strFileName);

    if (!m_file.open(QFile::ReadOnly))		// 文件不存在则返回false
    {
        m_file.close();
        return false;
    }

    // 文件长度=0则返回false
    int t_nFileLen = m_file.size();

    if (t_nFileLen <= 0) {
        m_file.close();
        return false;
    }
    m_bOpened = true;
    m_vecString.clear();

	QTextStream in(&m_file);
	in.setCodec("UTF-8");
	// 将文件内容读入行链表
	while (!in.atEnd())	{
		tStr = in.readLine();
		tStr = tStr.trimmed();

		m_vecString.push_back(tStr);
	}

    m_file.close();

    return true;
}

bool CIniConfig::removeSubKey(const QString& strKey, const QString& strSubKey) {
    if (strKey.length() == 0 || strSubKey.length() == 0)
        return false;

    int nIndex = findSubKey(strKey, strSubKey);

    if (nIndex > 0)
        m_vecString.removeAt(nIndex);

    return save2File();
}

bool CIniConfig::removeKey(const QString& strKey) {
    if (strKey == NULL)
        return false;

    int nBegin = -1;
    int nEnd = -1;
    bool bFindKey = false;
	QString	strTmp;
	strTmp = "[";
    strTmp += strKey;
    strTmp += "]";

    for (int i = 0; i < m_vecString.size(); ++i) {
        if (m_vecString[i].left(1) == ";")			// 注释
            continue;

        if (!bFindKey) {// 未找到主键
        
            if (m_vecString[i].indexOf(strTmp, Qt::CaseInsensitive) == 0) {
            
                bFindKey = true;
                nEnd = nBegin = i; //如果主键没有子键，则删除主键不成功。
            }
        }
        else {			// 已经找到主键
        
            if (m_vecString[i].indexOf("[") == 0) {	// 子键不存在
            
                nEnd = i;
                break;
            }
            if (i == m_vecString.size() - 1)
                nEnd = i + 1;
        }
    }

    if (nBegin >= 0 && nEnd >= 0) {
    
        for (int j = nBegin; j < nEnd; j++)
            m_vecString.removeAt(nBegin);

        return save2File();
    }
    return false;
}

bool CIniConfig::deleteAllKeys() {
    if (m_bOpened)  {
        m_file.open(QFile::WriteOnly | QFile::Truncate);
        m_file.close();
        m_vecString.clear();
        return true;
    }
    else
        return false;
}

bool CIniConfig::getBoolean(const QString& strKey, const QString& strSubKey, bool i_bDefault, bool* o_bRet) {
    QString t_strResult;

    if (o_bRet != NULL)
        *o_bRet = true;

    // 抽取结果字符串
    if (!getValue(findSubKey(strKey, strSubKey), t_strResult)) {
        if (o_bRet != NULL)
            *o_bRet = false;
    }

    // 将结果字符串转换为整形数
    if (t_strResult == "Y" || t_strResult == "y")
        return true;
    else if (t_strResult == "N" || t_strResult == "n")
        return false;
    else {
        if (o_bRet != NULL)
            *o_bRet = false;
        return i_bDefault;
    }
}

bool CIniConfig::setBoolean(const QString& strKey, const QString& strSubKey, bool i_bValue) {
    if (strKey == NULL || strSubKey == NULL)
        return false;

    int nLine = findSubKey(strKey, strSubKey);

    char  t_szBool[2];

    if (i_bValue)
        sprintf(t_szBool, "%s", "Y");
    else
        sprintf(t_szBool, "%s", "N");

    if (nLine > 0) {// 存在主键和子键
    
        if (!setValue(nLine, t_szBool))
            return false;
    }
    else {
        if (nLine == (-m_vecString.size() - 1))
            nLine = 0;
        addValue(strKey, strSubKey, t_szBool, nLine);
    }

    return save2File();
}

int CIniConfig::getInteger(const QString& strKey, const QString& strSubKey, int i_nDefault, bool* o_bRet) {
    QString t_strResult;

    if (o_bRet != NULL)
        *o_bRet = true;

    // 抽取结果字符串
    if (!getValue(findSubKey(strKey, strSubKey), t_strResult)) {
        if (o_bRet != NULL)
            *o_bRet = false;
        return i_nDefault;
    }

    // 将结果字符串转换为整形数
    return atoi(t_strResult.toLocal8Bit().data());
}

bool CIniConfig::setInteger(const QString& strKey, const QString& strSubKey, int i_nValue) {
    if (strKey == NULL || strSubKey == NULL)
        return false;

    char  t_szInt[64];
    sprintf(t_szInt, "%d", i_nValue);

    int nLine = findSubKey(strKey, strSubKey);

    if (nLine > 0) {// 存在主键和子键
    
        if (!setValue(nLine, t_szInt))
            return false;
    }
    else {
        if (nLine == (-m_vecString.size() - 1))
            nLine = 0;
        addValue(strKey, strSubKey, t_szInt, nLine);
    }

    return save2File();
}

double CIniConfig::getDouble(const QString& strKey, const QString& strSubKey, double i_fDefault, bool* o_bRet) {
    QString t_strResult;

    if (o_bRet != NULL)
        *o_bRet = true;

    // 抽取结果字符串
    if (!getValue(findSubKey(strKey, strSubKey), t_strResult)) {
        if (o_bRet != NULL)
            *o_bRet = false;
        return i_fDefault;
    }

    // 将结果字符串转换为浮点数
    return atof(t_strResult.toLocal8Bit().data());
}

bool CIniConfig::setDouble(const QString& strKey, const QString& strSubKey, double i_fValue) {
    if (strKey == NULL || strSubKey == NULL)
        return false;

    char  t_szReal[64];
    sprintf(t_szReal, "%.3f", i_fValue);

    int nLine = findSubKey(strKey, strSubKey);

    if (nLine > 0)	{// 存在主键和子键
    
        if (!setValue(nLine, t_szReal))
            return false;
    }
    else {
        if (nLine == (-m_vecString.size() - 1))
            nLine = 0;
        addValue(strKey, strSubKey, t_szReal, nLine);
    }

    return save2File();
}

QString CIniConfig::getString(const QString& strKey, const QString& strSubKey, const QString& strDefault, bool* o_bRet) {
    QString tStr;

    if (o_bRet != NULL)
        *o_bRet = true;

    // 抽取结果字符串
    if (!getValue(findSubKey(strKey, strSubKey), tStr)) {
        if (o_bRet != NULL)
            *o_bRet = false;
        return strDefault;
    }
    return tStr;
}
QString CIniConfig::getStringMultiline(const QString& strKey, const QString& strSubKey, const QString& strDefault, bool* o_bRet/*=NULL*/) {
    QString tStr;

    if (o_bRet != NULL)
        *o_bRet = true;

    int nIndex = findSubKey(strKey, strSubKey);
    // 抽取结果字符串
    if (!getValue(nIndex, tStr)) {
        if (o_bRet != NULL)
            *o_bRet = false;
        return strDefault;
    }

    // 满值再读一行
    QString tNextLine = m_vecString[nIndex];
    while (tNextLine.length() == (c_bufLen - 1)) {
        tNextLine = m_vecString[++nIndex];
        tStr += tNextLine;
    }

    return tStr;
}

bool CIniConfig::setString(const QString& strKey, const QString& strSubKey, const QString& strValue) {
    if (strKey == NULL || strSubKey == NULL)
        return false;

    if (strValue.length() == 0)
        return removeSubKey(strKey, strSubKey);

    int nLine = findSubKey(strKey, strSubKey);

    if (nLine > 0) {// 存在主键和子键
    
        if (!setValue(nLine, strValue))
            return false;
    }
    else {   
        if (nLine == (-m_vecString.size() - 1))
            nLine = 0;
        addValue(strKey, strSubKey, strValue, nLine);
    }

    return save2File();
}

QString CIniConfig::getAllKeys(const QString& strKey) {
    bool bFindKey = false;
	QString	strTmp;
	strTmp = "[";
    strTmp += strKey;
    strTmp += "]";
    bool bFirstTime = true;

    QString tStr;
    for (int i = 0; i < m_vecString.size(); ++i) {
        if (m_vecString[i][0] == ';')			// 注释
            continue;

        if (!bFindKey) {// 未找到主键
        
            if (m_vecString[i].indexOf(strTmp, Qt::CaseInsensitive) == 0)
                bFindKey = true;
        }
        else {		// 已经找到主键
        
            if (m_vecString[i].indexOf("[") == 0)				// 子键的搜索到头
                return tStr;

            if (bFirstTime) {
            
                tStr = m_vecString[i];
                bFirstTime = false;
            }
            else {            
                tStr += "\r\n";
                tStr += m_vecString[i];
            }
        }
    }
    return tStr;
}

bool CIniConfig::setAllKeys(const QString& strKey, const QString& i_pStr) {
	QString	strTmp;
	strTmp = "[";
    strTmp += strKey;
    strTmp += "]";
    int tBeginPos = -1;
    QString tStr = i_pStr;

    for (int i = 0; i < m_vecString.size(); ++i) {
        if (m_vecString[i][0] == ';')			// 注释
            continue;

        if (m_vecString[i].indexOf(strTmp, Qt::CaseInsensitive) == 0) {
            tBeginPos = i;
            break;
        }
    }

    if (tBeginPos < 0) {// 未找到主键
    
        m_vecString.push_front(strTmp);
        tBeginPos = 1;
    }
    else {
        tBeginPos += 1;
        while (tBeginPos < m_vecString.size() && m_vecString[tBeginPos].indexOf("[") != 0) {
            m_vecString.removeAt(tBeginPos);
        }
    }

    // 插入处理,
    QStringList strList = tStr.split("\n");
    QStringList::iterator iteStr = strList.begin();
    while (iteStr != strList.end()) {
        tStr = *iteStr;
        m_vecString.insert(tBeginPos, tStr);
        iteStr++;
    }

    return save2File();
}

//=======================================================
// 私有函数
//=======================================================
int CIniConfig::findSubKey(const QString& strKey, const QString& strSubKey) {
    int nRet = -m_vecString.size() - 1;
    bool bFindKey = false;
    int nTmpPos = 0;
    QString tStr;
    QString strTmp;
    strTmp = "[";
    strTmp += strKey;
    strTmp += "]";

    for (int i = 0; i < m_vecString.size(); ++i) {
        if (m_vecString[i][0] == ';')			// 注释
            continue;

        if (!bFindKey)	{// 未找到主键
        
            if (m_vecString[i].indexOf(strTmp, Qt::CaseInsensitive) == 0)
            {
                bFindKey = true;
                nRet = -i - 1;										// 有主键
            }
        }
        else { // 已经找到主键
            nTmpPos = m_vecString[i].indexOf("=");
            tStr = m_vecString[i].left(nTmpPos);
            tStr = tStr.trimmed();

            if (tStr == strSubKey)	{// 找到子键
            
                nRet = i;
                break;
            }

            if (m_vecString[i][0] == '[')							// 子键不存在
                break;
        }
    }
    return nRet;
}

bool CIniConfig::save2File() {
    if (!m_file.open(QFile::WriteOnly))
        return false;

    // 将缓存区写入文件
    m_file.seek(0);
	QTextStream out(&m_file);
	out.setCodec("UTF-8");
    QString str;
    QString str1 = "\r";
    QString str2 = "\n";
    for (int i = 0; i < m_vecString.size(); ++i) {
        str = m_vecString[i];
        if ((str.right(1) != str1) && (str.right(1) != str2)) {
            str += "\r\n";
        }
		out << str;
 //       m_file.write(str.toUtf8());
    }
    m_file.close();
    return true;
}

bool CIniConfig::getValue(int i_nIndex, QString& o_pValue) {
    if (i_nIndex < 0 || i_nIndex >= m_vecString.size())
        return false;

    int nBegin = m_vecString[i_nIndex].indexOf("=") + 1;

    if (nBegin < 0)				// 没有"="号则错误
        return false;

    o_pValue = m_vecString[i_nIndex].mid(nBegin);
    o_pValue = o_pValue.trimmed();
    return true;
}

bool CIniConfig::setValue(int i_nIndex, const QString& strValue) {
    if (i_nIndex < 0 || i_nIndex >= m_vecString.size())
        return false;

    int nBegin = m_vecString[i_nIndex].indexOf("=") + 1;

    if (nBegin < 0)				// 没有"="号则错误
        return false;

    QString tStr = m_vecString[i_nIndex].mid(nBegin);
    m_vecString[i_nIndex].replace(nBegin, tStr.length(), strValue);
    return true;
}

void CIniConfig::addValue(const QString& strKey, const QString& strSubKey, const QString& strValue, int i_nRet) {
	QString strTmp;
    if (i_nRet == 0)	{// 无主键
    
        // 增加主键
		strTmp = "[";
        strTmp += strKey;
        strTmp += "]";
        m_vecString.push_back("");
        m_vecString.push_back(strTmp);

        // 增加子键
        strTmp = strSubKey;
        strTmp += "=";
        strTmp += strValue;
        m_vecString.push_back(strTmp);
    }
    else	{// 有主键无子键
    
        // 增加子键
        strTmp = strSubKey;
        strTmp += "=";
        strTmp += strValue;
        m_vecString.insert(-i_nRet, strTmp);
    }
}
}
