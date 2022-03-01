#ifndef INSTALLTHREAD_H
#define INSTALLTHREAD_H

#include <QObject>
class InstallThread : public QObject
{
    Q_OBJECT
public:
    explicit InstallThread(QString installPath, qint64 num, QObject *parent = nullptr);
signals:
    void signalErrorData(QString error);
    void signalData(double num);
public slots:
    void startCopy(QString file);
private:
    double value;
    double size;
    QString installPath;
};

#endif // INSTALLTHREAD_H
