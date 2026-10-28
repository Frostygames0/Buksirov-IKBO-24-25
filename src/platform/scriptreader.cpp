#include "scriptreader.h"

#include <expected>

#include <QFile>
#include <QString>
#include <QStringList>
#include <QTextStream>

namespace EmulatorMirea::Platform::ScriptReader
{
std::expected<QStringList, QString> readScript(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return std::unexpected(QStringLiteral("Cannot open script: %1 (%2)").arg(path, file.errorString()));
    }

    QStringList list;

    QTextStream in(&file);
    while (!in.atEnd())
    {
        const QString line = in.readLine();
        const QString trimmed = line.trimmed();
        if (trimmed.isEmpty() || trimmed.startsWith(u'#'))
        {
            continue;
        }

        list << line;
    }

    return list;
};

} // namespace EmulatorMirea::Platform::ScriptReader