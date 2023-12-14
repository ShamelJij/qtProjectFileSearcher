#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>

void searchWordInFiles(const QString& directoryPath, const QString& targetWord)
{
    QDir directory(directoryPath);

    if (!directory.exists()) {
        qDebug() << "Directory does not exist.";
        return;
    }

    QStringList filters;
    filters << "*.txt"; // You can modify the filter based on the file types you want to search

    directory.setNameFilters(filters);
    directory.setFilter(QDir::Files | QDir::NoSymLinks);

    QFileInfoList fileList = directory.entryInfoList();

    foreach (QFileInfo fileInfo, fileList) {
        QFile file(fileInfo.absoluteFilePath());
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
                if (line.contains(targetWord, Qt::CaseInsensitive)) {
                    qDebug() << "Found in file:" << fileInfo.fileName();
                    break;  // You can choose to continue searching in the same file or remove the break to find all occurrences.
                }
            }
            file.close();
        } else {
            qDebug() << "Unable to open file:" << fileInfo.fileName();
        }
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString directoryPath = "/path/to/your/directory";
    QString targetWord = "yourWord";

    searchWordInFiles(directoryPath, targetWord);

    return a.exec();
}
