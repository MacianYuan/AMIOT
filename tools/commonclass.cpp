///******************************************************************************************
//*-----First Created. time: 11-Jan-2021	10:30  A.m.
//* @File name    :commonclass.cpp
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

//#include "commonclass.h"

//#include <QCryptographicHash>
//#include <QDir>
//#include <QFileInfoList>

//namespace ns_train {

/////////////////////////////////////////////////////////////////////
//QColor parseColor(const QString& strColor) {
//    QColor clr(Qt::white);
//    if (strColor.length() > 0) {
//        if (strColor.count(',') == 2) {
//            int nRed = strColor.section(',', 0, 0).toInt();
//            int nGreen = strColor.section(',', 1, 1).toInt();
//            int nBlue = strColor.section(',', 2, 2).toInt();
//            clr.setRgb(nRed, nGreen, nBlue);
//        } else if (strColor.count(',') == 3) {
//            int nRed = strColor.section(',', 0, 0).toInt();
//            int nGreen = strColor.section(',', 1, 1).toInt();
//            int nBlue = strColor.section(',', 2, 2).toInt();
//            int nAlpha = strColor.section(',', 3, 3).toInt();
//            clr.setRgba(qRgba(nRed, nGreen, nBlue, nAlpha));
//        } else {
//            clr.setNamedColor(strColor); // name形式存储
//        }
//    }
//    return clr;
//}

//QString getColorRgbValue(const QColor& clr) {
//    QString szRgb = QString("%1,%2,%3,%4").arg(clr.red()).arg(clr.green()).arg(clr.blue()).arg(clr.alpha());
//    return szRgb;
//}

//QString getPath(const QString& strInputPath) {
//    QString strPath = strInputPath;
//    strPath.replace("\\", "/");
//    int idx = strPath.indexOf("$");
//    if (idx < 0) {
//        return strPath.toLower();
//    }
//    QString strRight = strPath.right(strPath.length() - idx);
//    idx = strRight.indexOf("/");
//    QString strEnvironmentVariable = strRight.left(idx);
//    strPath.remove(strEnvironmentVariable);
//    strEnvironmentVariable.remove("$");
//    QString str = qEnvironmentVariable(strEnvironmentVariable.toLocal8Bit().data());
//    if (str.length() > 0) {
//        str.replace("\\", "/");
//        if (str.right(1) == "/") { // 删除环境变量中最后的"/"
//            str = str.left(str.length() - 1);
//        }
//        strPath.insert(0, str);
//    }
//	strPath = strPath.toLower();
//    return strPath;
//}


//QString getFileName(const QString& strFilePath) {
//    QString strFileName = getPath(strFilePath);
//	int idx = strFileName.lastIndexOf("/");
//	strFileName.right(strFileName.length()-idx-1);
//	return strFileName;
//}


//QString getDirectory(const QString& strPath) {
//    QString strDirectory = getPath(strPath);
//    if (0 == strDirectory.length()) {
//        return "";
//    }


//    int idx = strDirectory.lastIndexOf("/");
//    if (idx < 0) {
//        return strDirectory;
//    }
//	int postFixIdx = strDirectory.lastIndexOf(".");
//	if (postFixIdx > idx) {

//	}
//	else if (strDirectory.toLower() == strPath.toLower()) {
//		return strDirectory;
//	}
	
//	strDirectory = strDirectory.left(idx);
//    QDir dir(strDirectory);
//    QString str = dir.absolutePath();
//    str = dir.canonicalPath();

//    return strDirectory;
//}

//QString getNameOfDirectory(const QString& strDirectory) {
//	QString strDirName = getPath(strDirectory);
//	if (strDirName.endsWith("/")) {
//		strDirName = strDirName.left(strDirName.length()-1);
//	}
//	int idx = strDirName.lastIndexOf("/");
//	if (idx < 0) {
//		return QString();
//	}
//	else {
//		strDirName = strDirName.right(strDirName.length() - idx-1);
//		return strDirName;
//	}
//}
//QString getReleativePath(const QString& strFileName, const QString& strDirectory)  {
//	QString strFile = ns_train::getPath(strFileName);
//	QString strPath = ns_train::getPath(strDirectory);
//	if (strFile.indexOf(strPath) < 0) {
//		return QString();
//	}

//	QString strReleativePath = strFile.remove(strPath);
//	if (strReleativePath[0] != '/') {
//		strReleativePath = "/" + strReleativePath;
//	}
//	return strReleativePath;
//}
//QStringList getFileList(const QString& strPath, const QStringList& nameFilters, bool bRecursive) {
//    QStringList strAllFileList;
//    QStringList strList;
//    QString strFileName;
//    QDir dir;
//	QString strFilePath = getPath(strPath);
//    dir.setPath(strFilePath);
//	QStringList::const_iterator iteFilter;
//	QStringList filter = (bRecursive ? QStringList() : nameFilters);
//    QFileInfoList fileInfoList = dir.entryInfoList(filter, QDir::Dirs | QDir::Files | QDir::NoSymLinks);
//    QFileInfoList::iterator iteFileInfo = fileInfoList.begin();
//    QFileInfo fileInfo;
//	QString str;
//    for (iteFileInfo = fileInfoList.begin(); iteFileInfo != fileInfoList.end(); iteFileInfo++) {
//        fileInfo = *iteFileInfo;
//        strFileName = fileInfo.fileName();
//        if ((strFileName == ".") || (strFileName == ".."))  { // 跳过本目录和上级目录指示符。
//            continue;
//        }
//        if (fileInfo.isDir()) {
//            if (bRecursive) {
//                strFileName = fileInfo.filePath();
//                strList = getFileList(strFileName, nameFilters, true);
//                strAllFileList += strList;
//            }
//        } else if (fileInfo.isFile()){
//			if (0 == nameFilters.size()) {
//	            strAllFileList += fileInfo.absoluteFilePath();
//			}
//			for (iteFilter = nameFilters.begin(); iteFilter != nameFilters.end(); iteFilter++) {
//				str = (*iteFilter).right((*iteFilter).length()-1);
//				if (strFileName.endsWith(str)) {
//					strAllFileList += fileInfo.absoluteFilePath();
//				}
//			}
//        }
//    }
//    return strAllFileList;
//}

//QByteArray getMd5(const QString& strFileName) {
//	QByteArray byteData;
//	QCryptographicHash cghash(QCryptographicHash::Md5);
//	QFile file(strFileName);
//	if (!file.open(QIODevice::ReadOnly)) {
//		return byteData;
//	}
//	cghash.addData(&file);
//	byteData = cghash.result();
//	return byteData;
//}

//} // namespace ns_train
