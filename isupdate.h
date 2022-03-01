#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QWidget>
#include "main.h"

namespace Ui {
class isupdate;
}

class isupdate : public QWidget
{
    Q_OBJECT

public:
    explicit isupdate(QWidget *parent = nullptr);
    ~isupdate();
private slots:
    void buttonOK();
    void buttonCancel();
private:
    void isInstall();
    void isStart();
private:
    Ui::isupdate *ui;
    QProcess* process;
};
int entry(int argc, char *argv[]);
#endif // LANGUAGE_H
