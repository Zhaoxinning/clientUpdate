#ifndef TARGETDIRECTORY_H
#define TARGETDIRECTORY_H

#include <QWidget>

#include "main.h"
#include "performinstallation.h"

namespace Ui {
class TargetDirectory;
}

class TargetDirectory : public QWidget
{
    Q_OBJECT

public:
    explicit TargetDirectory(QWidget *parent = nullptr);
    ~TargetDirectory();
    void setMoveWindows(QPoint point);
signals:
    void signalShowLicenseCheck(QPoint point);
private:
    void initTargetDirectory();
    QString bytesToSize(qint64 mByte);
    qint64 getFilesSize(QString path);

    void mssageBoxShow(QString text);
public slots:
    void showTargetDirectory();
private slots:
    void buttonInstall();
    void buttonBrowse();
    void buttonCancel();
    void textChanged(QString text);

private:
    Ui::TargetDirectory *ui;
    PerformInstallation* performInstallation;
    QString installPath;
    qint64 filesSize;
    //bool ifupdate;
    QStorageInfo storage;
};

#endif // TARGETDIRECTORY_H
