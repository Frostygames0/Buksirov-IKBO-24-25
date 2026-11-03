#include "lineparser.h"

#include <cctype>
#include <cstddef>
#include <string_view>
#include <vector>
#include <string>

#include "core/envprovider.h"

namespace EmulatorMirea::Core::LineParser
{

namespace
{
bool isDoubleQuoteEscapable(char c)
{
    return c == '"' || c == '\\' || c == '$';
}

bool isNameStart(char c)
{
    return (std::isalpha(static_cast<unsigned char>(c)) != 0) || c == u'_';
}

bool isNameChar(char c)
{
    return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_';
}

bool isSpace(char c)
{
    return std::isspace(static_cast<unsigned char>(c)) != 0;
}

std::size_t expandDollar(std::string_view in, std::size_t i, const IEnvProvider& env, std::string& out)
{
    const std::size_t n = in.size();

    if (i + 1 < n && in[i + 1] == '{')
    {
        const std::size_t end = in.find('}', i + 2);
        if (end != std::string_view::npos)
        {
            out += env.getEnv(in.substr(i + 2, end - (i + 2)));
            return end;
        }
    }
    else if (i + 1 < n && isNameStart(in[i + 1]))
    {
        std::size_t j = i + 1;
        while (j < n && isNameChar(in[j]))
        {
            ++j;
        }
        out += env.getEnv(in.substr(i + 1, j - (i + 1)));
        return j - 1;
    }

    out += u'$';
    return i;
}
} // namespace

std::vector<std::string> parse(std::string_view in, const IEnvProvider& env)
{
    std::vector<std::string> tokens;
    std::string word;
    bool inToken = false;
    bool quoted = false; // token used quotes, so an empty value is kept
    char quote = 0;

    auto finishToken = [&] {
        // An unquoted token that expanded to nothing disappears; "" stays.
        if (!word.empty() || quoted)
        {
            tokens.push_back(word);
        }
        word.clear();
        inToken = false;
        quoted = false;
    };

    for (std::size_t i = 0; i < in.size(); ++i)
    {
        const char c = in[i];
        const bool hasNext = i + 1 < in.size();

        // If it's a single quote, then parse it verbatim, no expansion or anything
        if (quote == '\'')
        {
            if (c == quote)
            {
                quote = 0;
            }
            else
            {
                word += c;
            }
        }
        // If it's a double quote, then parse it verbatim BUT expand dollars and escape chars
        else if (quote == u'"')
        {
            if (c == quote)
            {
                quote = 0;
            }
            else if (c == '\\' && hasNext && isDoubleQuoteEscapable(in[i + 1]))
            {
                word += in[++i];
            }
            else if (c == '$')
            {
                i = expandDollar(in, i, env, word);
            }
            else
            {
                word += c;
            }
        }
        // If char is a quote, then start to process according to quote rules
        else if (c == '\'' || c == '"')
        {
            quote = c;
            inToken = true;
            quoted = true;
        }
        // if it's a slash, read it verbatim
        else if (c == '\\' && hasNext)
        {
            word += in[++i];
            inToken = true;
        }
        // if it's a dollar, then expand dollar
        else if (c == '$')
        {
            i = expandDollar(in, i, env, word);
            inToken = true;
        }
        // if it's a space and nothing else, then just finish the token
        else if (isSpace(c))
        {
            if (inToken)
            {
                finishToken();
            }
        }
        // else process
        else
        {
            word += c;
            inToken = true;
        }
    }

    if (inToken)
    {
        finishToken();
    }

    return tokens;
}

} // namespace EmulatorMirea::Core::LineParser