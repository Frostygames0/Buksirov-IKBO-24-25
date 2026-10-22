#ifndef COMMANDHISTORY_H
#define COMMANDHISTORY_H

#include <optional>

#include <QString>
#include <QStringList>
#include <QtTypes>

namespace EmulatorMirea::Core
{

class CommandHistory
{
public:
    void record(const QString& line);

    [[nodiscard]] std::optional<QString> moveUp();
    [[nodiscard]] std::optional<QString> moveDown();

private:
    QStringList m_entries;
    qsizetype m_cursor = 0;
};

} // namespace EmulatorMirea::Core

#endif // COMMANDHISTORY_H