#pragma once

#include <string>

namespace EmulatorMirea::Core
{

class IOutput
{
public:
    virtual ~IOutput() = default;

    virtual void write(const std::string& line) = 0;
    virtual void writeLine(const std::string& line = "") = 0;

    virtual void clear() = 0;
};
}; // namespace EmulatorMirea::Core