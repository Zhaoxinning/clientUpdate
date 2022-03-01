#ifndef PERFORMINSTALLATION_H
#define PERFORMINSTALLATION_H

#include <QWidget>

#include "main.h"

namespace Ui {
class PerformInstallation;
}

class PerformInstallation : public QWidget
{
    Q_OBJECT

public:
    explicit PerformInstallation(QString installPath,QPoint point, QWidget *parent = nullptr);
    ~PerformInstallation();
signals:
    void signalShowTargetDirectory();
    void signalStartCopy(QString installPath);
public slots:
    void errorData(QString error);
    void updateData(double data);
private slots:
    //void pushButtonNext();
    void buttonCancel();
    void buttonLastStep();
private:
    QStringList getFiles(QString path);
    void linuxRoot();
    void setRegedit();
    void initData();
    void isInstallSucceed(bool isSucceed);
private:
    Ui::PerformInstallation *ui;
    QString installPath;
    QStringList files;
    //bool ifupdate;
    qint64 filesSize; 
    QMessageBox* msgBox;
};

#endif // PERFORMINSTALLATION_H
