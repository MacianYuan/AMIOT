#ifndef HTTPGETKEY_H
#define HTTPGETKEY_H

#include <QObject>
#include <QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include "device_public.h"

class HttpGetKey : public QObject
{
    Q_OBJECT
public:
    explicit HttpGetKey(QObject *parent = 0);
    void init();

signals:

public slots:
    void slot_replayFinished(QNetworkReply *reply);
private:
    QNetworkAccessManager * m_manager;
};

#endif // HTTPGETKEY_H
