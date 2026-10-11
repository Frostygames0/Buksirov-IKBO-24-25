#ifndef SESSION_H
#define SESSION_H

#include <QString>
#include <QStringList>
#include <QSysInfo>
#include <QtTypes>

namespace EmulatorMirea
{

class Session
{
public:
    explicit Session();

    [[nodiscard]] QString userName() const;
    [[nodiscard]] QString machineName() const;
    [[nodiscard]] QString fancyName() const;

    void addHistory(const QString& line);
    QString previousHistoryItem();
    QString nextHistoryItem();
    void resetHistoryIndex();

    [[nodiscard]] QString getEnv(const QString& name, const QString& defaultValue = {}) const;

private:
    QString m_currentUserName;
    QString m_currentMachineName;

    QStringList m_history;
    qsizetype m_historyIndex;
};

} // namespace EmulatorMirea

#endif // SESSION_H
