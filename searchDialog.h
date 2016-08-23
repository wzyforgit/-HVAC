#ifndef SEARCH_DIALOG_H
#define SEARCH_DIALOG_H

#include <QDialog>

class OpenStatus;
class QGridLayout;
class QTextEdit;
class QPushButton;

class searchDialog : public QDialog
{
    Q_OBJECT

public:
    searchDialog(QWidget *parent = 0);
    void message(OpenStatus *devices[], int num, const QString &text);
    ~searchDialog();

private slots:
    void clearMessages();

private:
    QGridLayout *All;
    QTextEdit *messageBox;
    QPushButton *OK;
    QPushButton *Clear;
    OpenStatus **deviceGroup;
    int deviceNum;
};

#endif // DIALOG_H
