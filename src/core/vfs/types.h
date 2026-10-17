
#include <qtypes.h>

enum class VfsDataType
{
    File,
    Dir,
};

struct VfsEntry
{
    VfsDataType type;
    QString name;
    qsizetype size;
};

enum class VfsError
{
    None,
    NotFound,
    NotADirectory,
    AlreadyExists,
    InvalidArgument,
};