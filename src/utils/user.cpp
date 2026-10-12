#include "user.h"

#ifdef Q_OS_WIN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <lmcons.h>
#include <windows.h>
#else
#include <pwd.h>
#include <unistd.h>
#endif

#include <QString>
#include <QSysInfo>
#include <QtEnvironmentVariables>

namespace EmulatorMirea::UserUtils
{

QString getCurrentUsername()
{
#ifdef Q_OS_WIN
    wchar_t buf[UNLEN + 1];
    DWORD size = UNLEN + 1;
    if (GetUserNameW(buf, &size)) // NOLINT
        return QString::fromWCharArray(buf);
#else
    if (struct passwd* pw = getpwuid(getuid()))
        return QString::fromLocal8Bit(pw->pw_name);
#endif
    return qEnvironmentVariable("USER", qEnvironmentVariable("USERNAME"));
}

} // namespace EmulatorMirea::UserUtils