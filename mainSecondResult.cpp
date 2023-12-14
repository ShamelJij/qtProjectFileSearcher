#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>

void searchInFile(const QString& filePath, const QString& targetWord) {
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening file:" << filePath;
        return;
    }

    QTextStream in(&file);
    int lineNum = 1;
    int occurrences = 0;

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.contains(targetWord, Qt::CaseInsensitive)) {
            qDebug() << "Found in" << filePath << "at line" << lineNum << ": " << line;
            occurrences++;
        }
        lineNum++;
    }

    qDebug() << "Total occurrences in" << filePath << ":" << occurrences;

    file.close();
}

void searchInDirectory(const QString& directoryPath, const QString& targetWord, const QStringList& filters) {
    QDir directory(directoryPath);

    if (!directory.exists()) {
        qDebug() << "Directory does not exist:" << directoryPath;
        return;
    }

    QStringList files;

    // Apply filters if specified
    if (!filters.isEmpty()) {
        files = directory.entryList(filters, QDir::Files);
    } else {
        files = directory.entryList(QDir::Files);
    }

    foreach (const QString& file, files) {
        searchInFile(directory.filePath(file), targetWord);
    }
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    if (argc < 4) {
        qDebug() << "Usage: searchApp <directory> <targetWord> [filter]";
        return 1;
    }

    QString directoryPath = argv[1];
    QString targetWord = argv[2];
    QStringList filters;

    if (argc == 4) {
        filters << argv[3];
    }

    try {
        searchInDirectory(directoryPath, targetWord, filters);
    } catch (const std::exception& e) {
        qDebug() << "Exception:" << e.what();
        return 1;
    }

    return a.exec();
}
