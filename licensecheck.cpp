#include "licensecheck.h"
#include "ui_licensecheck.h"

#include "main.h"

LicenseCheck::LicenseCheck(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LicenseCheck),
    targetDirectory(nullptr)
{
    ui->setupUi(this);

    this->setWindowIcon(QIcon(":/resourceFile/cnc.ico"));
    this->setWindowTitle(tr("%1 %2 安装").arg(cnc::exeName,cnc::versions));

    ui->labelTitle->setFont(QFont("Times", 9, QFont::Bold));
    ui->labelTitle->setText(tr("许可协议"));

    ui->label->setText(QString("  %1\"%2%3\"%4")
                       .arg(tr("在安装"),cnc::exeName,cnc::versions,tr("之前,请阅读授权协议.")));

    ui->labelIco->setPixmap(QPixmap(":/resourceFile/cnc.ico"));
    ui->labelIco->setScaledContents(true); //自适应

    ui->label1->setText(tr("授权协议!"));

    ui->labelExplain->setWordWrap(true);
    ui->labelExplain->setText(tr("如果你接受协议中的调款,单击[我接受]继续安装.如果你选定[取消],\n安装程序将会关闭.必须接受协议才能安装") + cnc::exeName + cnc::versions);

    QFile f(":/resourceFile/LicenseAgreementDocument.txt");
    f.open(QIODevice::ReadOnly);
    ui->textEdit->setText(f.readAll());

    ui->pushButtonAgree->setText(tr("我接受(N)"));
    ui->pushButtonAgree->setShortcut(Qt::Key_N);
    ui->pushButtonCancel->setText(tr("取消(C)"));
    ui->pushButtonCancel->setShortcut(Qt::Key_C);
    ui->pushButtonLastStep->setText(tr("< 上一步(B)"));
    ui->pushButtonLastStep->setShortcut(Qt::Key_B);

    connect(ui->pushButtonAgree, SIGNAL(clicked(bool)), this, SLOT(buttonAgree()));
    connect(ui->pushButtonCancel, SIGNAL(clicked(bool)), this, SLOT(buttonCancel()));
    connect(ui->pushButtonLastStep, SIGNAL(clicked(bool)), this, SLOT(buttonLastStep()));

}

LicenseCheck::~LicenseCheck()
{
    delete ui;
}

void LicenseCheck::setMoveWindows(QPoint point)
{
    this->move(point);
}

void LicenseCheck::showLicenseCheck(QPoint point)
{
    this->move(point);
    this->show();
}

/*与函数buttonNext相同*/
void LicenseCheck::buttonAgree()
{
    if (targetDirectory == nullptr) {
        targetDirectory = new TargetDirectory();
        connect(targetDirectory, SIGNAL(signalShowLicenseCheck(QPoint)), this, SLOT(showLicenseCheck(QPoint)));
    }

    QPoint point = this->frameGeometry().topLeft();
    targetDirectory->setMoveWindows(point);
    targetDirectory->show();
    this->hide();
}

void LicenseCheck::buttonCancel()
{
    QMessageBox* box = new QMessageBox(QMessageBox::Question, tr("%1 %2的安装").arg(cnc::exeName,cnc::versions) ,
                                      tr("你确定要退出%1 %2 的安装吗?").arg(cnc::exeName,cnc::versions),
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

void LicenseCheck::buttonLastStep()
{
    //当参数值发生改变时，发射信号，执行槽函数
    emit signalShowIntroduction(this->frameGeometry().topLeft());
    this->hide();
}
