#include "MainWindow.h"
#include <QApplication>
#include <QFile>
#include <QTranslator>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString szPre;    
#if defined(Q_OS_ANDROID) || _DEBUG
    szPre = ":/Translations";
#else
    szPre = qApp->applicationDirPath() + QDir::separator() + ".." + QDir::separator() + "translations";
#endif
    
    QTranslator tApp;
    tApp.load(szPre + "/LunarCalendarApp_" + QLocale::system().name() + ".qm");
    a.installTranslator(&tApp);
    
    CLunarCalendar::InitTranslator();
    
//    QFile file("d:/Source/Tasks/Src/Resource/sink/dark/style.qss");  
//    if(file.open(QFile::ReadOnly))
//    {
//        QString stylesheet= file.readAll();
//        qApp->setStyleSheet(stylesheet);
//        file.close();
//    }
    
    MainWindow w;
#if defined (Q_OS_ANDROID)
    w.showMaximized();
#else
    w.show();
#endif
    return a.exec();
}
