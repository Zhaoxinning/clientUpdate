#ifndef MAIN_H
#define MAIN_H

#include <QStandardPaths>
#include <QDesktopWidget>
#include <QSharedMemory>
#include <QStorageInfo>
#include <QLocalSocket>
#include <QLocalServer>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QTranslator>
#include <QSettings>
#include <QFileInfo>
#include <QShortcut>
#include <QProcess>
#include <QThread>
#include <QString>
#include <QCursor>
#include <QDebug>
#include <QFrame>
#include <QFont>
#include <QDir>


namespace cnc {

static const QString qrcName = ":/cnc/";
static const int nameSize = qrcName.size();//记录该字符串字符数，方便后续去掉
static const QString iconName = "cnc.ico";
static const QString exeName = "CNC";
static const QString executableExeName = "CNC.exe";
//static const QString runExeName1 = "NOVICK_SERIES_REGISTER.ex";
//static const QString runExeName2 = "NOVICK_SERIES_REGISTER.exe";
static const QString versions = "1.0.1";

}

#endif // MAIN_H
