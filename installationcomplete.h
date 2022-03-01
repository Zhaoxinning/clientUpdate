#ifndef INSTALLATIONCOMPLETE_H
#define INSTALLATIONCOMPLETE_H

#include <QWidget>

namespace Ui {
class InstallationComplete;
}

class InstallationComplete : public QWidget
{
    Q_OBJECT

public:
    explicit InstallationComplete(QString installPath, bool isInstallSucceed, QWidget *parent = nullptr);
    ~InstallationComplete();
private slots:
    void buttonFinish();
    void buttonCancel();
private:
    void addLinkToDeskTop(const QString& filename);
private:
    Ui::InstallationComplete *ui;
    QString installPath;
    bool isInstallSucceed;
};

#endif // INSTALLATIONCOMPLETE_H
