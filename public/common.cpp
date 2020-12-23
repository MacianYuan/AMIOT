#include <QtCore/QByteArray>
#include <QtCore/QTime>
#include <QtSql/QtSql>
#include "public/common.h"

// int转bytes，低字节在前
QByteArray intToBytes(int num, int size) {
    QByteArray byteArray;

    if (size <= 0) {
        size = sizeof(num);
    }

    for (int i = 0; i < size; i++) {
        byteArray.append((uchar) (num >> (i * 8) & 0xff));
    }

    return byteArray;
}

// bytes转int，低字节在前
int bytesToInt(QByteArray byteArray, int size) {
    if (size <= 0)
        return 0;

    if (size > byteArray.size())
        size = byteArray.size();

    unsigned int num = 0;
    for (int i = 0; i < size; i++)
    {
        unsigned int temp = byteArray[i];
        num |= (temp & 0xff) << (i * 8);
    }

    return num;
}

// int转bytes, 高字节在前
QByteArray intToBytes2(int num, int size) {
    QByteArray byteArray;

    if (size <= 0)
        size = sizeof(num);

    for (int i = size - 1; i >= 0; i--)
        byteArray.append((uchar) (num >> (i * 8) & 0xff));

    return byteArray;
}

// bytes转int，高字节在前
int bytesToInt2(QByteArray byteArray, int size) {
    if (size <= 0)
        return 0;
    if (size > byteArray.size())
        size = byteArray.size();

    int num = 0;
    for (int i = 0; i < size; i++)
        num |= (byteArray[size - 1 - i] << (i * 8));

    return num;
}

void msleep(uint msecs) {
    QTime dieTime = QTime::currentTime().addMSecs(msecs);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

char ConvertHexChar(char ch) {
    if ((ch >= '0') && (ch <= '9')) {
        return ch - 0x30;
    } else if ((ch >= 'A') && (ch <= 'F')) {
        return ch - 'A' + 10;
    } else if ((ch >= 'a') && (ch <= 'f')) {
        return ch - 'a' + 10;
    }
    return (-1);
}
