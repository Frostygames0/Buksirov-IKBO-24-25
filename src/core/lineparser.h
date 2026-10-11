#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <QStringList>
#include <QStringView>

namespace EmulatorMirea::Core
{

class IEnvProvider;

class LineParser
{
public:
    [[nodiscard]] QStringList parse(QStringView line, const IEnvProvider& session) const;
};

} // namespace EmulatorMirea::Core

#endif // TOKENIZER_H