#include <QApplication>
#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>

class FileViewerWidget : public QWidget {
    Q_OBJECT

public:
    FileViewerWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setupUI();
    }

public slots:
    void displayFileContent(const QString& filePath, const QString& targetWord) {
        clearTable();

        QFile file(filePath);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Error opening file:" << filePath;
            return;
        }

        QTextStream in(&file);
        int lineNum = 1;

        while (!in.atEnd()) {
            QString line = in.readLine();

            // Highlight target word in the line
            QString highlightedLine = highlightTargetWord(line, targetWord);

            // Add line to the table
            addRowToTable(QString::number(lineNum), highlightedLine);
            lineNum++;
        }

        file.close();
    }

private:
    QTableWidget *tableWidget;

    void setupUI() {
        QVBoxLayout *layout = new QVBoxLayout(this);
        tableWidget = new QTableWidget(this);
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget->setColumnCount(2);
        tableWidget->setHorizontalHeaderLabels({"Line Number", "Content"});
        layout->addWidget(tableWidget);
    }

    void clearTable() {
        tableWidget->setRowCount(0);
    }

    void addRowToTable(const QString& lineNumber, const QString& content) {
        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);

        QTableWidgetItem *itemLineNumber = new QTableWidgetItem(lineNumber);
        QTableWidgetItem *itemContent = new QTableWidgetItem(content);

        tableWidget->setItem(row, 0, itemLineNumber);
        tableWidget->setItem(row, 1, itemContent);
    }

    QString highlightTargetWord(const QString& line, const QString& targetWord) {
        QString highlightedLine = line;

        // Use regular expression to find and highlight target word
        QRegularExpression re(targetWord, QRegularExpression::CaseInsensitiveOption);
        highlightedLine.replace(re, "<font color=\"red\">" + targetWord + "</font>");

        return highlightedLine;
    }
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    if (argc < 3) {
        qDebug() << "Usage: fileViewerApp <filePath> <targetWord>";
        return 1;
    }

    FileViewerWidget viewer;
    viewer.resize(800, 600);
    viewer.show();

    QString filePath = argv[1];
    QString targetWord = argv[2];
    viewer.displayFileContent(filePath, targetWord);

    return a.exec();
}

#include "main.moc"
