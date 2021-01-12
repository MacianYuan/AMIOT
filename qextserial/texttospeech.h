#ifndef TEXTTOSPEECH_H
#define TEXTTOSPEECH_H

#include <QThread>
#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QDataStream>
#include <QMessageBox>
#include "device_public.h"
#include "qextserialport.h"
#include <qsocketnotifier.h>
#include "cJSON.h"

class TextToSpeech : public QObject
{
     Q_OBJECT
public:
    explicit TextToSpeech(QObject *parent = 0);
    int WriteCom(QString str);//return write whether successed

    ~TextToSpeech();
protected:

public slots:
    void on_tts_message(unsigned int ,QString str);
    void on_set_volnm(unsigned int Accuracy_Volnm);
private slots:
    void ReadCom();

private:

};

#endif // TEXTTOSPEECH_H
