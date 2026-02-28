#ifndef KLAUNCHER_DIR_H
#define KLAUNCHER_DIR_H
#include <QString>

namespace Klauncher
{
    class Dir
    {
    public:
        static QString getDirectoryPath();
        static QString getIconsLocation();
        static QString getHomeDirectory();

        static QString getDataPath();
    };
} // Klauncher

#endif //KLAUNCHER_DIR_H