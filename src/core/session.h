#ifndef SESSION_H
#define SESSION_H

#include <QString>
#include <QStringList>
#include <QSysInfo>

namespace EmulatorMirea
{

class Session
{
public:
    explicit Session();

    QString userName() const;
    QString machineName() const;
    QString fancyName() const;

    void addHistory(const QString& line);
    QString previousHistoryItem();
    QString nextHistoryItem();
    void resetHistoryIndex();

    QString getEnv(const QString& name, const QString& defaultValue = {}) const;
private:
    QString m_currentUserName;
    QString m_currentMachineName;

    QStringList m_history;
    int m_historyIndex;
};

}

#endif // SESSION_H
