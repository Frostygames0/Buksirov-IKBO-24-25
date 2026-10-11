#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <QStringList>

class QStringView;

namespace EmulatorMirea
{

class Tokenizer
{
public:
    QStringList Tokenize(QStringView line) const;
};

}

#endif // TOKENIZER_H
