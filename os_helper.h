#ifndef OS_HELPER_H
#define OS_HELPER_H

#include <QString>
#include <QSysInfo>
#include <QtGlobal>

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

inline QString getCurrentUsername()
{
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

inline QString getUserPlusMachineName()
{
    return getCurrentUsername() + "@" + QSysInfo::machineHostName();
}

#endif // OS_HELPER_H
