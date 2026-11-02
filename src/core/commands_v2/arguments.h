#pragma once

#include <cassert>
#include <charconv>
#include <concepts>
#include <expected>
#include <format>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace EmulatorMirea::Core
{

struct ArgumentSpec
{
    enum class Kind
    {
        Flag,
        Option,
        Positional
    };

    Kind kind = Kind::Flag;
    char shortName = 0;
    std::string longName;
    std::string description;
};

namespace Detail
{

template <class T>
struct IsOptional : std::false_type
{
};

template <class T>
struct IsOptional<std::optional<T>> : std::true_type
{
};

template <class T>
concept ValueType = std::same_as<T, std::string> || std::same_as<T, std::string_view> ||
                    (std::integral<T> && !std::same_as<T, bool> && !std::same_as<T, char>) ||
                    std::floating_point<T>;

template <ValueType T>
std::expected<T, std::string> convert(std::string_view text, std::string_view name)
{
    if constexpr (std::same_as<T, std::string_view>)
    {
        return text;
    }
    else if constexpr (std::same_as<T, std::string>)
    {
        return std::string(text);
    }
    else
    {
        T result{};
        const char* first = text.data();
        const char* last = first + text.size();
        auto [ptr, ec] = std::from_chars(first, last, result);

        if (ec == std::errc::result_out_of_range)
        {
            return std::unexpected(std::format("value out range for <{}>: {}", name, text));
        }
        if (ec != std::errc{} || ptr != last)
        {
            return std::unexpected(std::format(
                "invalid {} for <{}>: {}", (std::integral<T> ? "integer" : "number"), name, text));
        }
        return result;
    }
}

} // namespace Detail

template <class... Args>
class ArgumentSet
{
public:
    ArgumentSet()
        requires(sizeof...(Args) == 0)
    = default;

    [[nodiscard]] const std::vector<ArgumentSpec>& specs() const { return m_specs; }

    [[nodiscard]] std::expected<std::tuple<Args...>, std::string> parse(
        std::span<const std::string_view> args) const
    {
        RawSet raw(m_specs.size());
        const std::vector<std::size_t> positionals = positionalIndices();
        std::size_t nextPositional = 0;
        bool optionsDone = false;

        Cursor cursor{args};
        while (!cursor.atEnd())
        {
            const std::string_view tok = cursor.take();

            std::expected<void, std::string> status;
            if (!optionsDone && tok == "--")
            {
                optionsDone = true;
            }
            else if (!optionsDone && isLongToken(tok))
            {
                status = parseLong(tok, cursor, raw);
            }
            else if (!optionsDone && isShortToken(tok))
            {
                status = parseShortCluster(tok, cursor, raw);
            }
            else
            {
                status = assignPositional(tok, positionals, nextPositional, raw);
            }

            if (!status)
            {
                return std::unexpected(std::move(status.error()));
            }
        }

        if (nextPositional < positionals.size())
        {
            return std::unexpected(
                std::format("missing argument: <{}>", m_specs[positionals[nextPositional]].longName));
        }

        return make(std::index_sequence_for<Args...>{}, raw);
    }

    [[nodiscard]] std::string usage() const
    {
        std::string out;
        for (const ArgumentSpec& s : m_specs)
        {
            out += "  ";
            if (s.kind == ArgumentSpec::Kind::Positional)
            {
                out += "<" + s.longName + ">";
            }
            else
            {
                if (s.shortName != 0)
                {
                    out += '-';
                    out += s.shortName;
                    if (!s.longName.empty())
                    {
                        out += ", ";
                    }
                }
                if (!s.longName.empty())
                {
                    out += "--" + s.longName;
                }
                if (s.kind == ArgumentSpec::Kind::Option)
                {
                    out += " <value>";
                }
            }
            out += "  " + s.description + "\n";
        }
        return out;
    }

private:
    using RawSet = std::vector<std::optional<std::string_view>>;

    std::vector<ArgumentSpec> m_specs;

    explicit ArgumentSet(std::vector<ArgumentSpec> specs) : m_specs(std::move(specs))
    {
        assert(m_specs.size() == sizeof...(Args));
    }

    template <class... Us>
    friend class ArgumentBuilder;

    class Cursor
    {
    public:
        explicit Cursor(std::span<const std::string_view> tokens) : m_tokens(tokens) {}

        [[nodiscard]] bool atEnd() const { return m_pos >= m_tokens.size(); }

        std::string_view take() { return m_tokens[m_pos++]; }

    private:
        std::span<const std::string_view> m_tokens;
        std::size_t m_pos = 0;
    };

    [[nodiscard]] std::vector<std::size_t> positionalIndices() const
    {
        std::vector<std::size_t> result;
        for (std::size_t i = 0; i < m_specs.size(); ++i)
        {
            if (m_specs[i].kind == ArgumentSpec::Kind::Positional)
            {
                result.push_back(i);
            }
        }
        return result;
    }

    [[nodiscard]] std::optional<std::size_t> findShort(char c) const
    {
        for (std::size_t i = 0; i < m_specs.size(); ++i)
        {
            const ArgumentSpec& s = m_specs[i];
            if (s.kind != ArgumentSpec::Kind::Positional && s.shortName != 0 && s.shortName == c)
            {
                return i;
            }
        }
        return std::nullopt;
    }

    [[nodiscard]] std::optional<std::size_t> findLong(std::string_view name) const
    {
        if (name.empty())
        {
            return std::nullopt;
        }
        for (std::size_t i = 0; i < m_specs.size(); ++i)
        {
            const ArgumentSpec& s = m_specs[i];
            if (s.kind != ArgumentSpec::Kind::Positional && s.longName == name)
            {
                return i;
            }
        }
        return std::nullopt;
    }

    [[nodiscard]] bool isNegativeNumber(std::string_view tok) const
    {
        return tok.size() > 1 && tok[0] == '-' &&
               ((std::isdigit(static_cast<unsigned char>(tok[1])) != 0) || tok[1] == '.') &&
               !findShort(tok[1]);
    }

    [[nodiscard]] static bool isLongToken(std::string_view tok) { return tok.starts_with("--"); }

    [[nodiscard]] bool isShortToken(std::string_view tok) const
    {
        return tok.size() > 1 && tok[0] == '-' && !isNegativeNumber(tok);
    }

    static std::expected<std::string_view, std::string> consumeValue(std::string_view inlineValue,
                                                                     bool hasInline,
                                                                     Cursor& cursor,
                                                                     const std::string& displayName)
    {
        if (hasInline)
        {
            return inlineValue;
        }
        if (!cursor.atEnd())
        {
            return cursor.take();
        }
        return std::unexpected(std::format("option {} requires a value", displayName));
    }

    std::expected<void, std::string> parseLong(std::string_view tok, Cursor& cursor, RawSet& raw) const
    {
        std::string_view name = tok.substr(2);
        std::string_view inlineValue;
        const auto eq = name.find('=');
        const bool hasInline = eq != std::string_view::npos;
        if (hasInline)
        {
            inlineValue = name.substr(eq + 1);
            name = name.substr(0, eq);
        }

        const auto idx = findLong(name);
        if (!idx)
        {
            return std::unexpected("unknown option: --" + std::string(name));
        }

        const ArgumentSpec& spec = m_specs[*idx];
        if (spec.kind == ArgumentSpec::Kind::Flag)
        {
            if (hasInline)
            {
                return std::unexpected(std::format("flag --{} does not take a value", spec.longName));
            }
            raw[*idx] = std::string_view{};
            return {};
        }

        auto value = consumeValue(inlineValue, hasInline, cursor, "--" + spec.longName);
        if (!value)
        {
            return std::unexpected(std::move(value.error()));
        }
        raw[*idx] = *value;
        return {};
    }

    std::expected<void, std::string> parseShortCluster(std::string_view tok,
                                                       Cursor& cursor,
                                                       RawSet& raw) const
    {
        for (std::size_t k = 1; k < tok.size(); ++k)
        {
            const auto idx = findShort(tok[k]);
            if (!idx)
            {
                return std::unexpected(std::format("unknown option: -{}", tok[k]));
            }
            if (m_specs[*idx].kind == ArgumentSpec::Kind::Flag)
            {
                raw[*idx] = std::string_view{};
                continue;
            }

            std::string_view rest = tok.substr(k + 1);
            if (rest.starts_with('='))
            {
                rest.remove_prefix(1);
            }
            auto value = consumeValue(rest, !rest.empty(), cursor, std::string("-") + tok[k]);
            if (!value)
            {
                return std::unexpected(std::move(value.error()));
            }
            raw[*idx] = *value;
            return {};
        }
        return {};
    }

    static std::expected<void, std::string> assignPositional(std::string_view tok,
                                                             const std::vector<std::size_t>& positionals,
                                                             std::size_t& nextPositional,
                                                             RawSet& raw)
    {
        if (nextPositional >= positionals.size())
        {
            return std::unexpected(std::format("unexpected argument: {}", tok));
        }
        raw[positionals[nextPositional++]] = tok;
        return {};
    }

    template <class T>
    static T extractFromRaw(const std::optional<std::string_view>& raw,
                            const ArgumentSpec& spec,
                            std::string& error)
    {
        if constexpr (std::same_as<T, bool>)
        {
            return raw.has_value();
        }
        else if constexpr (Detail::IsOptional<T>::value)
        {
            if (!raw)
            {
                return std::nullopt;
            }
            return extractFromRaw<typename T::value_type>(raw, spec, error);
        }
        else
        {
            if (!raw || !error.empty())
            {
                return T{};
            }
            auto converted = Detail::convert<T>(*raw, spec.longName);
            if (!converted)
            {
                error = std::move(converted.error());
                return T{};
            }
            return std::move(*converted);
        }
    }

    template <std::size_t... I>
    std::expected<std::tuple<Args...>, std::string> make(std::index_sequence<I...>,
                                                         [[maybe_unused]] const RawSet& raw) const
    {
        std::string error;
        std::tuple<Args...> values{extractFromRaw<Args>(raw[I], m_specs[I], error)...};
        if (!error.empty())
        {
            return std::unexpected(std::move(error));
        }
        return values;
    }
};

template <class... Args>
class ArgumentBuilder
{
public:
    ArgumentBuilder() = default;

    [[nodiscard]] ArgumentBuilder<Args..., bool> flag(char shortName,
                                                      std::string longName,
                                                      std::string description) &&
    {
        ArgumentSpec spec{
            .kind = ArgumentSpec::Kind::Flag,
            .shortName = shortName,
            .longName = std::move(longName),
            .description = std::move(description),
        };
        return std::move(*this).template add<bool>(std::move(spec));
    }

    template <Detail::ValueType T = std::string>
    [[nodiscard]] ArgumentBuilder<Args..., std::optional<T>> option(char shortName,
                                                                    std::string longName,
                                                                    std::string description) &&
    {
        ArgumentSpec spec{
            .kind = ArgumentSpec::Kind::Option,
            .shortName = shortName,
            .longName = std::move(longName),
            .description = std::move(description),
        };
        return std::move(*this).template add<std::optional<T>>(std::move(spec));
    }

    template <Detail::ValueType T = std::string>
    [[nodiscard]] ArgumentBuilder<Args..., T> positional(std::string name, std::string description) &&
    {
        ArgumentSpec spec{
            .kind = ArgumentSpec::Kind::Positional,
            .shortName = 0,
            .longName = std::move(name),
            .description = std::move(description),
        };

        return std::move(*this).template add<T>(std::move(spec));
    }

    [[nodiscard]] ArgumentSet<Args...> buildSet() && { return ArgumentSet<Args...>(std::move(m_specs)); }

private:
    std::vector<ArgumentSpec> m_specs;

    explicit ArgumentBuilder(std::vector<ArgumentSpec> specs) : m_specs(std::move(specs)) {}

    template <class... Us>
    friend class ArgumentBuilder;

    template <class T>
    ArgumentBuilder<Args..., T> add(ArgumentSpec spec) &&
    {
        assert(
            (spec.kind == ArgumentSpec::Kind::Positional || spec.shortName != 0 || !spec.longName.empty()) &&
            "flags/options need a short or long name");
        for ([[maybe_unused]] const ArgumentSpec& s : m_specs)
        {
            assert(!sameShortName(s, spec) && "duplicate short name");
            assert(!sameLongName(s, spec) && "duplicate long name");
        }
        m_specs.push_back(std::move(spec));
        return ArgumentBuilder<Args..., T>(std::move(m_specs));
    }

    static bool sameShortName(const ArgumentSpec& a, const ArgumentSpec& b)
    {
        return a.shortName != 0 && a.shortName == b.shortName;
    }

    static bool sameLongName(const ArgumentSpec& a, const ArgumentSpec& b)
    {
        const bool bothNamed = a.kind != ArgumentSpec::Kind::Positional &&
                               b.kind != ArgumentSpec::Kind::Positional;
        return bothNamed && !a.longName.empty() && a.longName == b.longName;
    }
};
} // namespace EmulatorMirea::Core