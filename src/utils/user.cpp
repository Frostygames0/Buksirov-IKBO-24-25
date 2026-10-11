#include "utils/user.h"

#include <QSysInfo>
#include <QString>
#include <QtEnvironmentVariables>

#ifdef Q_OS_WIN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <lmcons.h>
#else
#include <unistd.h>
#include <pwd.h>
#endif

namespace EmulatorMirea::UserUtils
{

QString getCurrentUsername() {
#ifdef Q_OS_WIN
    wchar_t buf[UNLEN + 1];
    DWORD size = UNLEN + 1;
    if (GetUserNameW(buf, &size))
        return QString::fromWCharArray(buf);
#else
    if (struct passwd *pw = getpwuid(getuid()))
        return QString::fromLocal8Bit(pw->pw_name);
#endif
    return qEnvironmentVariable("USER", qEnvironmentVariable("USERNAME"));
}

}