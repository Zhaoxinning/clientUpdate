#include "installationcomplete.h"
#include "ui_installationcomplete.h"
#include "main.h"

InstallationComplete::InstallationComplete(QString installPath, bool isInstallSucceed, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InstallationComplete),
    installPath(installPath),
    isInstallSucceed(isInstallSucceed)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("%1 %2 更新").arg(cnc::exeName,cnc::versions));
    ui->labelWallpaper->setScaledContents(true);
    ui->labelWallpaper->setPixmap(QPixmap(":/resourceFile/picture.png"));
    ui->labelTitleComplete->setWordWrap(true);
    ui->labelTitleComplete->setFont(QFont("Times", 12, QFont::Bold));
    if (isInstallSucceed) {
        ui->labelTitleComplete->setText(tr("   %1 %2 已经完成更新").arg(cnc::exeName,cnc::versions));
        ui->textEditcontent->setText(tr("    %1 %2 已安装在您的系统. \n\n    单击 [完成] 关闭此向导").arg(cnc::exeName,cnc::versions));

        ui->pushButtonCancel->setText(tr("取消(C)"));
        ui->pushButtonCancel->setShortcut(Qt::Key_C);
        ui->pushButtonfinish->setText(tr("完成(F)"));
        ui->pushButtonfinish->setShortcut(Qt::Key_F);
        ui->pushButtonLastStep->setText(tr("< 上一步(B)"));
        ui->pushButtonLastStep->setShortcut(Qt::Key_B);
        ui->pushButtonLastStep->setEnabled(false);

        ui->checkBox->setChecked(true);
        ui->checkBox->setText(tr("运行 ") + cnc::exeName + cnc::versions);
        ui->checkBoxIsDesktop->setChecked(true);
        ui->checkBoxIsDesktop->setText(tr("创建桌面快捷方式"));

    }
    else {
        ui->labelTitleComplete->setText(tr("%1 %2 更新失败").arg(cnc::exeName,cnc::versions));
        ui->textEditcontent->setText(tr("%1 %2 更新失败,请检查后重新安装. \n\n单击 [重新安装] 继续此向导").arg(cnc::exeName,cnc::versions));

        ui->pushButtonCancel->setText(tr("取消(C)"));
        ui->pushButtonCancel->setShortcut(Qt::Key_C);
        ui->pushButtonfinish->setText(tr("重新安装(R)"));
        ui->pushButtonfinish->setShortcut(Qt::Key_R);
        ui->pushButtonLastStep->setText(tr("< 上一步(B)"));
        ui->pushButtonLastStep->setShortcut(Qt::Key_B);
        ui->pushButtonLastStep->setEnabled(false);

        ui->checkBoxIsDesktop->hide();
        ui->checkBox->hide();
    }

    ui->textEditcontent->setFrameShape(QFrame::Shape::NoFrame); //设置无边框
    ui->textEditcontent->setReadOnly(true);
    ui->textEditcontent->setCursor(QCursor(Qt::CursorShape::ArrowCursor));

    connect(ui->pushButtonCancel, SIGNAL(clicked(bool)), this, SLOT(buttonCancel()));
    connect(ui->pushButtonfinish, SIGNAL(clicked(bool)), this, SLOT(buttonFinish()));
}

InstallationComplete::~InstallationComplete()
{
    delete ui;
}

void InstallationComplete::buttonFinish()
{
    if (isInstallSucceed) {
        //checkState():返回复选框的检查状态
        if (ui->checkBox->checkState()) {
            //installPath以'/'结尾
            //str:安装包下载后的可执行文件的路径
            QString str = QString("\"%1%2\"").arg(installPath,cnc::executableExeName);
            QProcess *p = new QProcess(this);
            p->start(str);//启动cnc
        }
        if (ui->checkBoxIsDesktop->checkState()) {
            addLinkToDeskTop(installPath + cnc::executableExeName);
        }
        exit(0);
    }
    else {
        //获取当前程序路径,重新启动!!
        QProcess* p = new QProcess();
        //QCoreApplication::applicationFilePath():返回应用程序可执行文件的文件路径。
        /*void QProcess::start(const QString &program, const QStringList &arguments):
         *   在新进程中启动给定程序，在参数中传递命令行参数。QProcess对象将立即进入启动状态。
         */
        p->start(QCoreApplication::applicationFilePath(), QStringList() << "  ");
        exit(1);
    }
}

void InstallationComplete::addLinkToDeskTop(const QString &filename)
{
    //建立桌面快捷方式
    QString strAppPath = filename;
    QString strDesktopLink = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/";
    strDesktopLink += cnc::exeName + ".lnk";
    QFile fApp(strAppPath);
    fApp.link(strDesktopLink);

    //建立开始菜单快捷方式
    QString strMenuLink = QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation) + "/";
    strMenuLink += cnc::exeName + "/";
    QDir pathDir;
    pathDir.mkpath(strMenuLink);
    strMenuLink += cnc::exeName + ".lnk";
    fApp.link(strMenuLink);

}

void InstallationComplete::buttonCancel()
{
    exit(1);
}
