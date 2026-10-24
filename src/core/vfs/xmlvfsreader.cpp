#include "xmlvfsreader.h"

#include <QByteArray>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QStringLiteral>
#include <QXmlStreamReader>
#include <QtTypes>

#include "core/vfs/vfs.h"

namespace EmulatorMirea::Core::XmlVfs
{
namespace
{
QString xmlError(const QXmlStreamReader& reader, const QString& fallback)
{
    if (reader.hasError())
    {
        return QStringLiteral("XML error at line %1: %2").arg(reader.lineNumber()).arg(reader.errorString());
    }
    return fallback;
}

std::expected<QByteArray, QString> decode(const QString& text, const QString& encoding)
{
    // If encoding is not specified or utf-8, just utf-8 it
    if (encoding.isEmpty() || encoding == u"utf8" || encoding == u"utf-8")
    {
        return text.toUtf8();
    }

    // if encoding is base64
    if (encoding == u"base64")
    {
        return QByteArray::fromBase64(text.toLatin1());
    }

    return std::unexpected(QStringLiteral("Unknown encoding '%1'").arg(encoding));
}

std::expected<void, QString> readTree(QXmlStreamReader& reader, IVfs& vfs)
{
    QList<VfsPath> parents{VfsPath{}};

    while (!parents.isEmpty())
    {
        if (!reader.readNextStartElement())
        {
            parents.removeLast();
            continue;
        }

        const QStringView tag = reader.name();

        // If not dir or file - skip it
        if (tag != u"dir" && tag != u"file")
        {
            reader.skipCurrentElement();
            continue;
        }

        const QXmlStreamAttributes attrs = reader.attributes();
        const qint64 lineNumber = reader.lineNumber();

        if (!attrs.hasAttribute(u"name"))
        {
            return std::unexpected(
                QStringLiteral("Tag %1 at line %2 has no 'name' attribute").arg(tag).arg(lineNumber));
        }

        const QString name = attrs.value(u"name").toString();
        const std::optional<VfsPath> path = parents.last().child(name);
        if (!path)
        {
            return std::unexpected(QStringLiteral("Invalid <%1> name '%2' at line %3").arg(tag, name).arg(lineNumber));
        }

        if (tag == u"dir")
        {
            const std::expected<void, VfsError> created = vfs.createDir(*path);
            if (!created)
            {
                return std::unexpected(QStringLiteral("Cannot create dir %1: %2")
                                           .arg(path->toString(), vfsErrorToString(created.error())));
            }

            parents.append(*path);
            continue;
        }

        const QString encoding = attrs.value(u"encoding").toString().toLower();
        const QString text = reader.readElementText(); // consumes end </file> tag

        if (reader.hasError())
        {
            return std::unexpected(xmlError(reader, {}));
        }

        const std::expected<QByteArray, QString> data = decode(text, encoding);
        if (!data)
        {
            return std::unexpected(QStringLiteral("%1 (%2)").arg(data.error(), path->toString()));
        }

        const std::expected<void, VfsError> created = vfs.createFile(*path, *data);
        if (!created)
        {
            return std::unexpected(
                QStringLiteral("Cannot create file %1: %2").arg(path->toString(), vfsErrorToString(created.error())));
        }
    }

    return {};
}
} // namespace

std::expected<void, QString> readFromFile(const QString& path, IVfs& vfs)
{
    QFile file{path};
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return std::unexpected(QStringLiteral("Cannot open '%1': %2").arg(path, file.errorString()));
    }

    QXmlStreamReader reader(&file);

    if (!reader.readNextStartElement())
    {
        return std::unexpected(xmlError(reader, QStringLiteral("Empty document")));
    }

    if (reader.name() != u"vfs")
    {
        return std::unexpected(QStringLiteral("Root element must be <vfs>"));
    }

    if (auto r = readTree(reader, vfs); !r)
    {
        return std::unexpected(r.error());
    }

    if (reader.hasError())
    {
        return std::unexpected(xmlError(reader, {}));
    }

    return {};
}

bool writeStub(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return false;
    }

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeEmptyElement(QStringLiteral("vfs"));
    writer.writeEndDocument();

    return true;
}
} // namespace EmulatorMirea::Core::XmlVfs