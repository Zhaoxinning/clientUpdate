#ifndef LICENSECHECK_H
#define LICENSECHECK_H

#include <QWidget>

#include "targetdirectory.h"

namespace Ui {
class LicenseCheck;

}

class LicenseCheck : public QWidget
{
    Q_OBJECT

public:
    explicit LicenseCheck(QWidget *parent = nullptr);
    ~LicenseCheck();
    void setMoveWindows(QPoint point);
signals:
    void signalShowIntroduction(QPoint point);

public slots:
    void showLicenseCheck(QPoint point);
private slots:
    void buttonAgree();
    void buttonCancel();
    void buttonLastStep();

private:
    Ui::LicenseCheck *ui;
    TargetDirectory* targetDirectory;
};

#endif // LICENSECHECK_H
