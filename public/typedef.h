#ifndef TYPEDEF_H
#define TYPEDEF_H

typedef struct AccountInfo {
    uint id;
    uint status;
    uint groupId;
    int deposit;
    int credit;
} Account;

typedef struct CardInfo {
    std::string serial;
    uint type;
    uint status; // 0-有效卡，1-未生效卡，2-过期卡（仅对临时卡有效）
} Card;

typedef struct UserInfo {
    uint id;
    uint status;
    QString firstName;
    QString lastName;
    QString fullName;
    QString phone;
    bool isInBlacklist;
    uint right;
    uint longTimeRight;
    uint rightExpireTime;
    Account account;
    Card card;
    uint timestamp;
    bool isInPenaltyStop;
    int inPenaltyStopStart;
    int inPenaltyStopEnd;
} User;


struct ConditionNode {
    bool status;
    QString info;
    QString soundPath;
    int errorCode;
};

#endif // TYPEDEF_H
