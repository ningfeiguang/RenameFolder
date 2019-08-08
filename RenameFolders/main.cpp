
//#include <QtCore/QCoreApplication>
#include <iostream>
#include <QString>
#include <QDir>
#include <QDirIterator>
#include <QDebug>

void printHelpMsg() {
    std::cout << "Please input 4 parameters" << std::endl;
    std::cout << "RenameFolders <parent path> <alignment count> <row> <col>" << std::endl;
}

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

    //return a.exec();
    if (argc < 5) {
        printHelpMsg();
        return -1;
    }

    QString strFolder(argv[1]);
    int alignmentCount = strtol(argv[2], 0, 10);
    const int ROWS = strtol(argv[3], 0, 10);
    const int COLS = strtol(argv[4], 0, 10);
    {
        QDirIterator iter(strFolder, QDir::Dirs | QDir::NoDotAndDotDot);

        int count = 0;
        while (iter.hasNext())
        {
            ++ count;
            qDebug() << iter.next();
        }

        int requiredFolderCount = alignmentCount + ROWS * COLS;
        if (count != requiredFolderCount) {
            std::cout << "Current subfolder count " << count << " not match with required count " << requiredFolderCount << std::endl;
            return -1;
        }
    }

    if (!strFolder.endsWith('/') && !strFolder.endsWith('\\')) {
        strFolder += "/";
    }

    {
        QDirIterator iter(strFolder, QDir::Dirs | QDir::NoDotAndDotDot);

        int count = 0;
        while (iter.hasNext())
        {
            QString strNewName;
            if (count < alignmentCount)  {
                strNewName = QString("alignment_%1").arg(count);
            }
            else {
                int row = (count - alignmentCount) / COLS;
                int index = (count - alignmentCount) - (row * COLS);

                int col = index;
                if (row % 2 != 0)
                    col = COLS - index - 1;

                strNewName = QString("%1").arg(row * COLS + col);
            }
              
            QDir dir(iter.next());
            auto newPath = strFolder + strNewName;
            std::cout << "Rename " << dir.path().toStdString() << " to " << newPath.toStdString() << std::endl;
            dir.rename(dir.path(), newPath);
            ++count;
        }
    }

    return 0;
}
