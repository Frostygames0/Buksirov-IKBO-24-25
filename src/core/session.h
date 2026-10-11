#ifndef SESSION_H
#define SESSION_H

#include <QString>
#include <QStringList>
#include <QSysInfo>
#include <QtTypes>

#include "core/envprovider.h"

namespace EmulatorMirea::Core
{

class ISession : public IEnvProvider
{
public:
    [[nodiscard]] virtual QString userName() const = 0;
    [[nodiscard]] virtual QString machineName() const = 0;
    [[nodiscard]] virtual QString fancyName() const = 0;

    virtual void addHistory(const QString& line) = 0;
    virtual QString previousHistoryItem() = 0;
    virtual QString nextHistoryItem() = 0;
    virtual void resetHistoryIndex() = 0;
};

class Session : public ISession
{
public:
    explicit Session();

    [[nodiscard]] QString userName() const override;
    [[nodiscard]] QString machineName() const override;
    [[nodiscard]] QString fancyName() const override;

    void addHistory(const QString& line) override;
    QString previousHistoryItem() override;
    QString nextHistoryItem() override;
    void resetHistoryIndex() override;

    [[nodiscard]] QString getEnv(const QString& name, const QString& defaultValue = {}) const override;

private:
    QString m_currentUserName;
    QString m_currentMachineName;

    QStringList m_history;
    qsizetype m_historyIndex = 0;
};

} // namespace EmulatorMirea::Core

#endif // SESSION_H
