#ifndef ENUM_H
#define ENUM_H

// 枚举常数，与服务器数据库内一致
namespace biotech {
    namespace osla {
        enum Enum {
            UNKNOWN_USER_ID = 0,

            USER_STATUS_ACTIVE = 104,
            USER_STATUS_FROZEN = 105,
            USER_STATUS_INVALID = 106,

            ACCOUNT_STATUS_ACTIVE = 161,
            ACCOUNT_STATUS_FROZEN = 162,
            ACCOUNT_STATUS_INVALID = 163,

            SESSION_TYPE_TRAINING = 201,
            SESSION_TYPE_MAINTENANCE = 202,
            SESSION_TYPE_NORMAL = 203,
            SESSION_TYPE_ASSISTANCE = 204,
            SESSION_TYPE_EDUCATION = 205,      /// 教学
            SESSION_TYPE_ENTRUST = 206,        /// 项目委托

            INSTRUMENT_STATUS_ACTIVE = 231,
            INSTRUMENT_STATUS_FAULTY = 232,
            INSTRUMENT_STATUS_INACTIVE = 233,
            INSTRUMENT_STATUS_OUTOFSERVICE = 234,

            USER_RIGHT_UNAUTH = 251,
            USER_RIGHT_NORMAL = 252,
            USER_RIGHT_SENIOR = 253,
            USER_RIGHT_ADMIN = 254,
            USER_RIGHT_ADMIN_MAIN = 255,

            CARD_TYPE_ADMIN = 331,
            CARD_TYPE_USER = 332,
            CARD_TYPE_TEMPORARY = 333,
            CARD_TYPE_UNKNOWN = 334,

            BOOKING_STATUS_VALID = 1105,
            BOOKING_STATUS_CANCELED = 1107,
            BOOKING_STATUS_COMPLETED = 1108,
            BOOKING_STATUS_ABSENT = 1109,
            BOOKING_STATUS_TERMINATED = 1110,
            BOOKING_STATUS_OCCUPIED = 1111,

            USER_ACTION_NULL_OP = 1401,
            USER_ACTION_START_SESSION = 1402,
            USER_ACTION_START_TRAINING = 1403,
            USER_ACTION_START_MAINTENANCE = 1404,
            USER_ACTION_START_ASSISTANCE = 1405,
            USER_ACTION_TRAINING_CHECKIN = 1406,
            USER_ACTION_END_SESSION = 1407,
            USER_ACTION_CONFIG_SYSTEM = 1408,
            USER_ACTION_START_EDUCATION = 1409, /// new
            USER_ACTION_START_ENTRUST = 1410, /// new
            USER_ACTION_EDUCATION_CHECKIN = 1411, /// new

            USER_ACTION_NULL_RESULT = 1421,
            USER_ACTION_SUCCEED = 1422,
            USER_ACTION_FAILED_UNKNOWN_USER = 1423,
            USER_ACTION_FAILED_INVALID_TEMP_CARD = 1424,
            USER_ACTION_FAILED_INSTRUMENT_FAULTY = 1425,
            USER_ACTION_FAILED_INSTRUMENT_INACTIVE = 1426,
            USER_ACTION_FAILED_INSTRUMENT_OUTOFSERVICE = 1427,
            USER_ACTION_FAILED_INSTRUMENT_INUSE = 1428,
            USER_ACTION_FAILED_BOOKING_HOLDING = 1429,
            USER_ACTION_FAILED_RESERVATION_HOLDING = 1430,
            USER_ACTION_FAILED_INSUFFICIENT_RIGHT = 1431,
            USER_ACTION_FAILED_ACCOUNT_MISSING = 1432,
            USER_ACTION_FAILED_INVALID_ACCOUNT = 1433,
            USER_ACTION_FAILED_INSUFFICIENT_BALANCE = 1434,
            USER_ACTION_FAILED_CHECKIN_CLOSED = 1435,
            USER_ACTION_FAILED_CHECKIN_ITERATIVELY = 1436,
            USER_ACTION_FAILED_NO_BOOK_IN_STRICT = 1437,    //严格模式未预约用户刷卡事件,added by lwp, 2018-1-15
            USER_ACTION_FAILED_IN_BLACK_LIST =1438,         //黑名单刷卡事件,added by lwp, 2018-1-24
            USER_ACTION_SET_INSTRUMENT_ACTIVE = 1450,
            USER_ACTION_SET_INSTRUMENT_FAULTY = 1451,
            USER_ACTION_SET_INSTRUMENT_INACTIVE = 1452,
            USER_ACTION_SET_INSTRUMENT_OUTOFSERVICE = 1453,
            USER_ACTION_NETWORKSETTINGS_CHANGED = 1454,
            USER_ACTION_UNBIND_INSTRUMENT = 1455,
            USER_ACTION_FAILED_UNBIND_INSTRUMENT = 1456,
            USER_ACTION_FAILED_PENALTYSTOP = 1457,
            USER_ACTION_FAILED_BOOKINGMODELID = 1458,

            SYSTEM_EVENT_DEVICE_INIT_SETUP = 1471,
            SYSTEM_EVENT_DEVICE_FACTORY_RESET = 1472,
            SYSTEM_EVENT_DEVICE_COLD_BOOT = 1473,
            SYSTEM_EVENT_DEVICE_HOT_BOOT = 1474,
            SYSTEM_EVENT_INSTRUMENT_STATUS_CHANGED = 1475,
            SYSTEM_EVENT_DEVICE_SOFTWARE_UPDATED = 1476,
            SYSTEM_EVENT_BOOKING_COMPLETED = 1481,
            SYSTEM_EVENT_BOOKING_ABSENT = 1482,
            SYSTEM_EVENT_BOOKING_OCCUPIED = 1483,
            SYSTEM_EVENT_BOOKING_TERMINATED = 1484,
            SYSTEM_EVENT_BOOKING_LOCKED = 1485,
            SYSTEM_EVENT_BOOKING_LOCKED_IN_USED = 1486,
            SYSTEM_EVENT_BOOKING_LOCKED_IN_USED_SELF = 1487,
            SYSTEM_EVENT_EXCEPTIONAL_USING_TIME = 1491,
            SYSTEM_EVENT_LOOSE_MODE = 1492,

            //added by lwp, 刷卡机升级事件,2018-1-12
            SYSTEM_EVENT_UPGRADE_MANUAL        = 5000,    /*!< 执行手工升级*/
            SYSTEM_EVENT_UPGRADE_REMOTE        = 5001,    /*!< 执行远程升级*/
            SYSTEM_EVENT_UPGRADE_PLAN          = 5002,    /*!< 执行升级计划*/
            SYSTEM_EVENT_UPGRADE_DOWNLOADING   = 5003,    /*!< 开始下载*/
            SYSTEM_EVENT_UPGRADE_DOWNLOADED    = 5004,    /*!< 下载完成*/
            SYSTEM_EVENT_UPGRADE_UPDATING      = 5005,    /*!< 正在更新*/
            SYSTEM_EVENT_UPGRADE_UPDATED       = 5006,    /*!< 更新成功*/
            SYSTEM_EVENT_UPGRADE_USER_CANNEL   = 5007,    /*!< 更新失败-用户取消*/
            SYSTEM_EVENT_UPGRADE_PARAMS_ERR    = 5008,    /*!< 更新失败-升级参数不正确*/
            SYSTEM_EVENT_UPGRADE_DOWNLOAD_ERR  = 5009,    /*!< 更新失败-下载失败*/
            SYSTEM_EVENT_UPGRADE_FILE_ERR      = 5010,    /*!< 更新失败-文件检查错误*/
            SYSTEM_EVENT_UPGRADE_VER_NEWEST    = 5011,    /*!< 更新失败-终端已经是最新版本*/
            SYSTEM_EVENT_UPGRADE_VER_LOWER     = 5012,    /*!< 更新失败-终端版本过低*/
            SYSTEM_EVENT_UPGRADE_ANOTHER_PROC  = 5013,    /*!< 更新失败-另一个升级任务正在执行*/
            SYSTEM_EVENT_UPGRADE_USING         = 5014,    /*!< 更新失败-仪器正在使用中*/
            SYSTEM_EVENT_UPGRADE_UNSYNC        = 5015,    /*!< 更新失败-仪器有未同步记录*/
            SYSTEM_EVENT_UPGRADE_UPDATE        = 5016,    /*!< 更新失败-刷新终端程序时出错*/
            //end
        };

    } // namespace osla
} // namespace biotech

enum EntrustStatus {
    DONT_DO = 1, // 未执行
    DONT_OVER = 2, //未完成
    HAVA_OVER = 3, // 已完成
    TERMINAL = 4, //  终止
    //NOW_DO = 50   // 执行中  私有定义
};

enum BookingModelType {
    Book = 1 << 0,
    Project = 1 << 1,
};

#endif // ENUM_H
