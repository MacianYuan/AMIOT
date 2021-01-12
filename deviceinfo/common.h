#ifndef COMMON_H
#define COMMON_H

#include <QByteArray>

// int转bytes，低字节在前，bytes长度为size
QByteArray intToBytes(int num, int size = 4);
// bytes转int，低字节在前，bytes长度为size
int bytesToInt(QByteArray byteArray, int size = 4);
// int转bytes，高字节在前，bytes长度为size
QByteArray intToBytes2(int num, int size = 4);
// bytes转int，高字节在前，bytes长度为size
int bytesToInt2(QByteArray byteArray, int size = 4);
// 毫秒级延时
void msleep(uint msecs);
// 16禁止转换成数字
char ConvertHexChar(char ch);

#endif // COMMON_H
