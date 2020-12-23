#ifndef SQLWORKER_H
#define SQLWORKER_H

#include <time.h>
#include <QtCore/QtCore>
#include <QtSql/QtSql>
#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtCore/QDate>
#include <QtSql/QSqlDatabase>
#include "public/device_public.h"

class SqlWorker : public QObject
{
    Q_OBJECT
public:
    explicit SqlWorker(QObject *parent = 0);
    ~SqlWorker();
private:


public:
    enum SqlError {
        DATABASEOPENERROR
    };

    bool isReady() const;
    void closeDatabase();
    void removeDatabase();

    void createTables();
    bool createConnection();
    void removeDatabase_file();
    static SqlWorker* getInstance();

signals:
    void error(QString errorString);
    void adminUpdated();
    void ready();
    void error(const SqlError error);

private:
    static SqlWorker* instance;

};

#endif // SQLWORKER_H
