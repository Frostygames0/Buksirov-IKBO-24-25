#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace EmulatorMirea::Core
{
class History
{
public:
    void add(std::string_view line);

    [[nodiscard]] std::string_view at(std::size_t index) const { return m_lines.at(index); }

    [[nodiscard]] std::size_t size() const { return m_lines.size(); }

private:
    std::vector<std::string> m_lines;
};
} // namespace EmulatorMirea::Core