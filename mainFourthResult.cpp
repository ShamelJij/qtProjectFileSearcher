#include <QMainWindow>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

class MyMainWindow : public QMainWindow {
    Q_OBJECT

public:
    MyMainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        setupUI();
        connectSignalsAndSlots();
    }

private slots:
    void onBrowseButtonClicked() {
        QString filePath = QFileDialog::getOpenFileName(this, "Open File");
        if (!filePath.isEmpty()) {
            filePathLineEdit->setText(filePath);
            loadFileContent();
        }
    }

    void onNextButtonClicked() {
        findNextTargetWord();
    }

    void onPreviousButtonClicked() {
        findPreviousTargetWord();
    }

private:
    QLineEdit *filePathLineEdit;
    QLineEdit *targetWordLineEdit;
    QPushButton *browseButton;
    QPushButton *nextButton;
    QPushButton *previousButton;
    QTextEdit *fileContentTextEdit;

    void setupUI() {
        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        filePathLineEdit = new QLineEdit(this);
        targetWordLineEdit = new QLineEdit(this);
        browseButton = new QPushButton("Browse", this);
        nextButton = new QPushButton("Next", this);
        previousButton = new QPushButton("Previous", this);
        fileContentTextEdit = new QTextEdit(this);

        QVBoxLayout *layout = new QVBoxLayout(centralWidget);
        layout->addWidget(filePathLineEdit);
        layout->addWidget(targetWordLineEdit);
        layout->addWidget(browseButton);
        layout->addWidget(nextButton);
        layout->addWidget(previousButton);
        layout->addWidget(fileContentTextEdit);
    }

    void connectSignalsAndSlots() {
        connect(browseButton, &QPushButton::clicked, this, &MyMainWindow::onBrowseButtonClicked);
        connect(nextButton, &QPushButton::clicked, this, &MyMainWindow::onNextButtonClicked);
        connect(previousButton, &QPushButton::clicked, this, &MyMainWindow::onPreviousButtonClicked);
    }

    void loadFileContent() {
        QString filePath = filePathLineEdit->text();
        QFile file(filePath);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            // Handle error opening the file
            return;
        }

        QTextStream in(&file);
        fileContentTextEdit->setPlainText(in.readAll());

        file.close();
    }

    void findNextTargetWord() {
        QString targetWord = targetWordLineEdit->text();
        QTextCursor cursor(fileContentTextEdit->textCursor());
        QRegularExpression re(targetWord, QRegularExpression::CaseInsensitiveOption);

        while (cursor.find(re)) {
            cursor.removeSelectedText();  // Clear previous selection
            cursor.insertHtml("<span style=\"background-color: yellow;\">" + cursor.selectedText() + "</span>");
        }
    }

    void findPreviousTargetWord() {
        QString targetWord = targetWordLineEdit->text();
        QTextCursor cursor(fileContentTextEdit->textCursor());
        QRegularExpression re(targetWord, QRegularExpression::CaseInsensitiveOption);

        cursor.setPosition(cursor.selectionStart());

        while (cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor) && cursor.selectedText().contains(re)) {
            cursor.removeSelectedText();  // Clear previous selection
            cursor.insertHtml("<span style=\"background-color: yellow;\">" + cursor.selectedText() + "</span>");
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MyMainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}

#include "main.moc"
