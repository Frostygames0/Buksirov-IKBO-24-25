#pragma once

#include <QString>

namespace EmulatorMirea::Core
{

class IOutput
{
public:
    virtual ~IOutput() = default;
    virtual void writeLine(const QString& line) = 0;
};
}; // namespace EmulatorMirea::Core