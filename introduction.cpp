#include "introduction.h"
#include "ui_introduction.h"
#include "main.h"

Introduction::Introduction(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Introduction),
    licenseCheck(nullptr)
{
    //构造函数中this，指向对象
    ui->setupUi(this);

    this->setWindowIcon(QIcon(":/resourceFile/cnc.ico"));
    this->setWindowTitle(tr("%1 %2 安装").arg(cnc::exeName,cnc::versions));


    ui->labelWallpaper->setPixmap(QPixmap(":/resourceFile/picture.png"));
    ui->labelWallpaper->setScaledContents(true);//自适应

    ui->labelTitleIntroduction->setFont(QFont("Times", 10, QFont::Bold));
    //wordWrap用来控制视图中数据项文本的换行策略。为true,则在文本中分词的适当处进行换行。
    ui->labelTitleIntroduction->setWordWrap(true);
    ui->labelTitleIntroduction->setText(tr("\n 欢迎使用\"%1 %2\"安装向导").arg(cnc::exeName,cnc::versions));

    ui->textEditcontent->setFrameShape(QFrame::Shape::NoFrame); //设置无边框
    ui->textEditcontent->setReadOnly(true);//只读模式
    //设置光标成箭头形状
    ui->textEditcontent->setCursor(QCursor(Qt::CursorShape::ArrowCursor)); //如果不显示该鼠标!一定属性里面在点一下!
    ui->textEditcontent->setText(tr("\n  这个向导将指引您完成 %1 %2 的安装。\n\n  建议在继续之前先退出其他运行的程序。\n\n  单击 [下一步] 继续. \n\n  或者单击 [取消] 退出本安装程序。")
                                 .arg(cnc::exeName,cnc::versions));

    ui->pushButtonNext->setText(tr("下一步(N) >"));
    ui->pushButtonNext->setShortcut(Qt::Key_N); //设置快捷键
    ui->pushButtonCancel->setText(tr("取消(C)"));
    ui->pushButtonCancel->setShortcut(Qt::Key_C);
    connect(ui->pushButtonNext, SIGNAL(clicked(bool)), this, SLOT(buttonNext()));
    connect(ui->pushButtonCancel, SIGNAL(clicked(bool)), this, SLOT(buttonCancel()));
}

Introduction::~Introduction()
{
    delete ui;
}

void Introduction::showIntroduction(QPoint point)
{
    this->move(point);
    this->show();
}

void Introduction::buttonNext()
{
    if (licenseCheck == nullptr) {
        licenseCheck = new LicenseCheck();
        /*
         * 信号和槽函数:
         *   信号：
         *      就是在特定情况下被发射的事件(对象状态改变)，
         *      例如，pushButton最常见的信号就是鼠标单击时发射的clicked()信号，
         *      一个ComboBox最常见的信号是选择的列表项变化是发射的CurrentIndexChanged()信号。
         *      只要知道什么情况下发射哪些信号，合理地去响应和处理这些信号就行了。
         *      信号只能声明，不能定义。参数会传递到槽函数。
         *      一个控件的信号可以连接到另外一个控件————
         *      如：connect(pb3,&QPushButton::pressed,pb2,&QPushButton::pressed);
         *   发送信号：
         *      只需要在关键字emit后面加上要发的信号。一旦参数发生改变，则发射信号，执行槽函数。
         *   槽：
         *      就是对信号响应的函数。槽函数可以与一个信号关联。
         *      当信号被发射时，关联的槽函数被自动执行。
         *      参数从信号那里来。
         *   两者的返回值类型任何时候都可以不同。
         *   关联函数：
         *      信号可以看作是特殊函数，有参数时也要指明参数。
         *      QObject::connect(sender, SIGNAL(signal()), receiver, SLOT(slot()));
         * */
        connect(licenseCheck, SIGNAL(signalShowIntroduction(QPoint)), this, SLOT(showIntroduction(QPoint)));
    }
    //得到窗口左上角的坐标，使下一个窗口和该窗口位置一样
    QPoint point = this->frameGeometry().topLeft();
    licenseCheck->setMoveWindows(point);
    licenseCheck->show();
    this->hide();
}

void Introduction::buttonCancel()
{    
    QMessageBox* box = new QMessageBox(QMessageBox::Question, tr("%1 %2的安装").arg(cnc::exeName,cnc::versions) ,
                                      tr("你确定要退出%1 %2 的安装吗?").arg(cnc::exeName,cnc::versions),
                                       QMessageBox::Yes | QMessageBox::No);    
    box->button(QMessageBox::Yes)->setText(tr("是"));
    box->button(QMessageBox::No)->setText(tr("否"));
    box->button(QMessageBox::No)->setFocus(); //设置焦点

    //对话框将位于窗口的正中央
    box->move(this->x() + (((this->width() - 310) / 2)), this->y() + ((this->height() - 120) / 2));
    int ret = box->exec();
    if (ret == QMessageBox::Yes) {
        exit(1);
    }
}
