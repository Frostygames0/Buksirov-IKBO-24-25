#include "history.h"

#include <core/stringutils.h>
#include <string_view>

namespace EmulatorMirea::Core
{
namespace
{

} // namespace

void History::add(std::string_view line)
{
    if (StringUtils::trim(line).empty())
    {
        return;
    }

    if (m_lines.empty() || m_lines.back() != line)
    {
        m_lines.emplace_back(line);
    }
}
} // namespace EmulatorMirea::Core