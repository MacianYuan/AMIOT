#include "sqlworker/sqlworker.h"

#define likely(x) __builtin_expect((x),1)
#define unlikely(x) __builtin_expect((x),0)

#define ConvertQ2CString(value) value.toLocal8Bit().data()
#define ConvertC2QString(value) QString(value.c_str())

SqlWorker::SqlWorker(QObject *parent)
    : QObject(parent) {
    Device_public::db = QSqlDatabase::addDatabase(DATABASE_DRIVER);
}

SqlWorker* SqlWorker::instance = new SqlWorker;

SqlWorker* SqlWorker::getInstance()
{
    return instance;
}

bool SqlWorker::createConnection()
{
    Device_public::db.setDatabaseName(DATABASE_NAME);
    //Device_public::db.open();
    //if (!isReady()) {
        if (!Device_public::db.open()) {
            qDebug()<<"SqlWorker: Database open error:" << ConvertQ2CString(Device_public::db.lastError().text());
            return false;
        }
    //}
    return true;
}

SqlWorker::~SqlWorker() {
    closeDatabase();
}

bool SqlWorker::isReady() const {
    return Device_public::Device_public::db.isOpen() && (!Device_public::db.isOpenError());
}

void SqlWorker::closeDatabase() {
    Device_public::db.close();
}

void SqlWorker::removeDatabase() {
    if (Device_public::db.isOpen()) {
        Device_public::db.close();
    }
    Device_public::db.removeDatabase(DATABASE_NAME);
}

void SqlWorker::removeDatabase_file() {
    if (Device_public::db.isOpen()) {
        Device_public::db.close();
    }
    QFile(DATABASE_NAME).remove();
}







