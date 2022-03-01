#include "performinstallation.h"
#include "ui_performinstallation.h"
#include "installthread.h"
#include "installationcomplete.h"

PerformInstallation::PerformInstallation(QString installPath,QPoint point, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PerformInstallation),
    installPath(installPath),
    filesSize(0),
    msgBox(nullptr)
{
    ui->setupUi(this);
    this->move(point);

    this->setWindowIcon(QIcon(":/resourceFile/cnc.ico"));
    this->setWindowTitle(tr("%1 %2 更新").arg(cnc::exeName,cnc::versions));

    ui->labelIco->setPixmap(QPixmap(":/resourceFile/cnc.ico"));
    ui->labelIco->setScaledContents(true); //自适应

    ui->labelSelectInstall->setText(tr("正在更新"));
    ui->labelSelectInstall->setFont(QFont("Times", 10, QFont::Bold));
    ui->labelSelectDir->setText(QString(" \"%1 %2 \" %3").arg(cnc::exeName,cnc::versions,tr("正在更新,请稍候...")));

    ui->textEdit->setReadOnly(true);
    //设置光标形状
    ui->textEdit->setCursor(QCursor(Qt::CursorShape::ArrowCursor));

    ui->pushButtonCancel->setText(tr("取消(C)"));
    ui->pushButtonCancel->setShortcut(Qt::Key_C);
    ui->pushButtonNext->setText(tr("下一步(N) >"));
    ui->pushButtonNext->setShortcut(Qt::Key_N);
    ui->pushButtonLastStep->setText(tr("< 上一步(B)"));
    ui->pushButtonLastStep->setShortcut(Qt::Key_B);

    connect(ui->pushButtonNext, SIGNAL(clicked(bool)), this, SLOT(pushButtonNext()));
    connect(ui->pushButtonCancel, SIGNAL(clicked(bool)), this, SLOT(buttonCancel()));
    connect(ui->pushButtonLastStep, SIGNAL(clicked(bool)), this, SLOT(buttonLastStep()));

    ui->pushButtonCancel->setEnabled(false);
    ui->pushButtonNext->setEnabled(false);
    ui->pushButtonLastStep->setEnabled(false);

    files = getFiles(cnc::qrcName);//QStringList
    initData();
}

PerformInstallation::~PerformInstallation()
{
    delete ui;
}

//这个文件列表中的格式如下:file-file->dir1_file-dir1_file-Dir1->file-file,抽取文件时从后向前
QStringList PerformInstallation::getFiles(QString path)
{
    if (!path.endsWith('/')) {
        path.append('/');
    }
    QDir dir(path);//":/cnc/"
    QStringList files;

    //entrylist:返回目录中所有文件和目录的名称列表
    foreach (QString name, dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
        QString newPath = path + name;
        if (!QFileInfo(newPath).isSymLink() && QFileInfo(newPath).isDir()) {
            files.append(getFiles(newPath));
        }
        files.append(newPath);
    }
    return files;//文件路径字符串的列表
}

void PerformInstallation::initData()
{
    //在文本编辑的结尾追加一个新段落。
    ui->textEdit->append(tr("输出目录: ") + installPath);

    //files.size()：列表项数
    InstallThread* installThread = new InstallThread(installPath,files.size());
    QThread* thread = new QThread();

    installThread->moveToThread(thread);

    connect(this, SIGNAL(signalStartCopy(QString)), installThread, SLOT(startCopy(QString)));
    connect(installThread, SIGNAL(signalData(double)), this, SLOT(updateData(double)));
    connect(installThread, SIGNAL(signalErrorData(QString)), this, SLOT(errorData(QString)));

    thread->start();
    if (!files.isEmpty()) {
        emit signalStartCopy(files.last());
        files.removeLast();//删除列表最后一项
    }
}

//initData->thread->updateData/errorData
void PerformInstallation::updateData(double data)
{
    ui->progressBar->setValue(data);
    if (!files.isEmpty()) {
        //QFileInfo:获取文件信息
        //fileName():返回文件名称，不包括路径
        ui->labelProgress->setText(tr("抽取: ") +
                                   QFileInfo(files.last()).fileName() + "..."
                                   + QString("%1%").arg((int)data));
        ui->textEdit->append(tr("抽取: ") + QFileInfo(files.last()).fileName() + "...");
        emit signalStartCopy(files.last()); //返给线程
        files.removeLast();
    }
    else {
        isInstallSucceed(true);
    }
}

void PerformInstallation::errorData(QString error)
{
    ui->pushButtonLastStep->setEnabled(true);
    if (msgBox == nullptr) {
        msgBox = new QMessageBox(this);
        msgBox->setText(error);
        msgBox->exec();
    }
    isInstallSucceed(false);
}

void PerformInstallation::isInstallSucceed(bool isSucceed)
{
    if (isSucceed) {
        setRegedit();//改写版本号
        ui->labelProgress->setText(tr("已完成%1 %2的更新").arg(cnc::exeName,cnc::versions));
        ui->textEdit->append(tr("完成"));
        ui->progressBar->setValue(100);
        InstallationComplete* installationComplete = new InstallationComplete(installPath, isSucceed);
        installationComplete->show();
        this->hide();
    }
    else {
        ui->labelProgress->setText(tr("%1 %2 的更新已失败").arg(cnc::exeName,cnc::versions));
        ui->textEdit->append(tr("更新失败"));
        InstallationComplete* installationComplete = new InstallationComplete(installPath, isSucceed);
        QDesktopWidget* desktop = QApplication::desktop();
        installationComplete->move((desktop->width() - installationComplete->width())/2, (desktop->height() - installationComplete->height())/2-30);
        installationComplete->show();
        this->hide();
    }
}

void PerformInstallation::setRegedit()
{
    QSettings settingsVersions("HKEY_CLASSES_ROOT\\" + cnc::exeName +  "Versions", QSettings::NativeFormat);
    settingsVersions.setValue("Versions", cnc::versions);
}


void PerformInstallation::buttonCancel()
{
    QMessageBox* box = new QMessageBox(QMessageBox::Question, tr("%1 %2的更新").arg(cnc::exeName,cnc::versions) ,
                                       tr("你确定要退出%1 %2 的更新吗?").arg(cnc::exeName,cnc::versions),
                                       QMessageBox::Yes | QMessageBox::No);

    box->button(QMessageBox::Yes)->setText(tr("是"));
    box->button(QMessageBox::No)->setText(tr("否"));
    box->button(QMessageBox::No)->setFocus(); //设置焦点

    box->move(this->x() + (((this->width() - 310) / 2)), this->y() + ((this->height() - 120) / 2));
    int ret = box->exec();
    if (ret == QMessageBox::Yes) {
        exit(1);
    }
}

void PerformInstallation::buttonLastStep()
{
    emit signalShowTargetDirectory();
    this->hide();
}
