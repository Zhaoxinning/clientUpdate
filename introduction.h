#ifndef INTRODUCTION_H
#define INTRODUCTION_H

#include <QWidget>

#include "main.h"
#include "licensecheck.h"

namespace Ui {
class Introduction;
}

class Introduction : public QWidget
{
    Q_OBJECT

public:
    explicit Introduction(QWidget *parent = nullptr);
    ~Introduction();

public slots:
    void showIntroduction(QPoint point);
private slots:
    void buttonNext();
    void buttonCancel();
private:
    Ui::Introduction *ui;
    LicenseCheck* licenseCheck;
};

#endif // INTRODUCTION_H
