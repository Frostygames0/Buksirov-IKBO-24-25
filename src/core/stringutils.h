#pragma once

#include <concepts>
#include <cstddef>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

namespace EmulatorMirea::Core::StringUtils
{

template <std::ranges::range R>
    requires std::convertible_to<std::ranges::range_value_t<R>, std::string_view>
[[nodiscard]] inline std::string joinStrings(const R& parts, std::string_view delim)
{
    if (parts.empty())
    {
        return {};
    }

    std::size_t totalSize{};
    for (const auto& part : parts)
    {
        totalSize += part.size();
    }
    totalSize += delim.size() * (parts.size() - 1);

    std::string result;
    result.reserve(totalSize);

    result += parts[0];
    for (std::size_t i{1}; i < parts.size(); ++i)
    {
        result.append(delim);
        result.append(parts[i]);
    }

    return result;
}

[[nodiscard]] inline std::vector<std::string_view> split(std::string_view str, char delim)
{
    std::vector<std::string_view> result;
    std::size_t start{};
    std::size_t end = str.find(delim);

    while (end != std::string_view::npos)
    {
        auto part = str.substr(start, end - start);
        if (!part.empty())
        {
            result.push_back(part);
        }

        start = end + 1;
        end = str.find(delim, start);
    }

    auto lastPart = str.substr(start);
    if (!lastPart.empty())
    {
        result.push_back(str.substr(start));
    }

    return result;
}

inline std::string_view trim(std::string_view sv)
{
    const char* spaces = " \t\n\r\f\v";

    std::size_t start = sv.find_first_not_of(spaces);
    if (start == std::string_view::npos)
    {
        return "";
    }

    std::size_t end = sv.find_last_not_of(spaces);

    return sv.substr(start, end - start + 1);
}
}; // namespace EmulatorMirea::Core::StringUtils