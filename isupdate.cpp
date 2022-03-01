#include "isupdate.h"
#include "ui_isupdate.h"
#include "targetdirectory.h"
#include "unistd.h"
#include "main.h"

isupdate::isupdate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::isupdate)
{
    ui->setupUi(this);

    this->setWindowIcon(QIcon(":/resourceFile/cnc.ico"));
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    this->setWindowTitle(tr("更新"));

    ui->labelSelectLanguage->setText(tr("请选择是否开始更新？\n\n如果是，请单击[确定]，否则单击[取消]。 "));
    ui->labelIco->setPixmap(QPixmap(":/resourceFile/novick.jpg"));
    ui->labelIco->setScaledContents(true); //自适应，缩放来填充所用空间

    ui->pushButtonOK->setText(tr("确定"));
    ui->pushButtonCancel->setText(tr("取消"));
    connect(ui->pushButtonOK, SIGNAL(clicked(bool)), this, SLOT(buttonOK()));
    connect(ui->pushButtonCancel, SIGNAL(clicked(bool)), this, SLOT(buttonCancel()));
}

isupdate::~isupdate()
{
    delete ui;
}

void isupdate::buttonCancel()
{
    //按钮在默认情况下，点击后都会退出对话框，无论Yes or NO
    QMessageBox* box = new QMessageBox(QMessageBox::Question, tr("%1 %2的更新").arg(cnc::exeName,cnc::versions) ,
                                       tr("你确定要退出%1 %2 的更新吗?").arg(cnc::exeName,cnc::versions),
                                       QMessageBox::Yes | QMessageBox::No);

    box->button(QMessageBox::Yes)->setText(tr("是"));
    box->button(QMessageBox::No)->setText(tr("否"));
    box->button(QMessageBox::No)->setFocus();

    int ret = box->exec();
    if (ret == QMessageBox::Yes) {
        exit(1);
    }
}

void isupdate::buttonOK()
{
    TargetDirectory* targetDirectory;

    isInstall(); //检测是否安装
    isStart(); //检测是否启动

    targetDirectory= new TargetDirectory();
    QDesktopWidget* desktop = QApplication::desktop();
    targetDirectory->move((desktop->width()- targetDirectory->width() )/2, (desktop->height() - targetDirectory->height())/2-30);
    targetDirectory->show();
    this->hide();
}



/*检测是否安装*/
void isupdate::isInstall()
{
    QSettings* settingsVersions;
    settingsVersions = new QSettings(QString("HKEY_CLASSES_ROOT\\%1Versions").arg(cnc::exeName), QSettings::NativeFormat);

    if (settingsVersions->contains("Versions")) {
        QString oldVersions = settingsVersions->value("Versions").toString();
        oldVersions.remove('.');
        QString newVersions = cnc::versions;
        newVersions.remove('.');

        if (newVersions.toInt() > oldVersions.toInt()) {
           return;
        }
        if (newVersions.toInt() == oldVersions.toInt()) {
            QMessageBox* box = new QMessageBox(QMessageBox::Question, tr("%1 %2的更新").arg(cnc::exeName,cnc::versions) ,
                                               tr("检测到%1版本和已安装版本相同,\n请问是否继续?").arg(cnc::exeName),
                                               QMessageBox::Yes | QMessageBox::No);

            box->button(QMessageBox::Yes)->setText(tr("是"));
            box->button(QMessageBox::No)->setText(tr("否"));
            box->button(QMessageBox::No)->setFocus();

            int ret = box->exec();
            if (ret == QMessageBox::Yes) {
                return;
            }
            else {
                exit(1);
            }
        }
        if (newVersions.toInt() < oldVersions.toInt()) {
            QMessageBox* box = new QMessageBox(QMessageBox::Question, tr("%1 %2的安装").arg(cnc::exeName,cnc::versions) ,
                                               tr("检测到当前%1的版本较旧,\n请问是否替换当前新版本?").arg(cnc::exeName),
                                               QMessageBox::Yes | QMessageBox::No);

            box->button(QMessageBox::Yes)->setText(tr("是"));
            box->button(QMessageBox::No)->setText(tr("否"));
            box->button(QMessageBox::Yes)->setFocus();

            int ret = box->exec();
            if (ret == QMessageBox::Yes) {
                return;
            }
            else {
                exit(1);
            }
        }
    }
}

void isupdate::isStart()
{
    process = new QProcess;
    //cmd命令：tasklist
    process->start("tasklist");
    //waitForFinished():阻塞,直到进程完成并发出finished()信号。
    process->waitForFinished();
    //readAllStandardOutput():得到程序运行后的标准输出。
    //fromLocal8Bit():返回QString类型。
    QString outputStr = QString::fromLocal8Bit(process->readAllStandardOutput());
    if (outputStr.contains(cnc::executableExeName)) {
        QMessageBox* box = new QMessageBox(QMessageBox::Question, tr("%1 %2的更新").arg(cnc::exeName,cnc::versions) ,
                                           tr("您的%1正在运行\n请问是否关闭立即更新?").arg(cnc::exeName),
                                           QMessageBox::Yes | QMessageBox::No);

        box->button(QMessageBox::Yes)->setText(tr("立即更新"));
        box->button(QMessageBox::No)->setText(tr("下次再说"));

        box->move(this->x() + (((this->width() - 310) / 2)), this->y() + ((this->height() - 120) / 2));
        int ret = box->exec();
        if (ret == QMessageBox::Yes) {
            QProcess p;
            //execute():在新进程中启动程序命令，等待它完成，然后返回退出代码。
            /* taskkill /f:强制关闭名称为指定名称的程序的。
             * taskkill /t:终止进程和它启用的子进程。
             * taskkill /im:指定要终止的进程的映像名称。
            */
            p.execute("taskkill /f /t /im " + cnc::executableExeName);
        }
        else {
            exit(1);
        }
    }
}

int entry(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file(":/resourceFile/ui.qss");
    if (file.open(QIODevice::ReadOnly)) {
        //setStyleSheet:此属性保存应用程序样式表
        a.setStyleSheet(file.readAll());
        file.close();
    }

    isupdate* w = new isupdate();
    w->show();
    return a.exec();
}
