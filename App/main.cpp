#include "MainWindow.h"
#include <QApplication>
#include <QFile>
#include <QTranslator>
#include <QDir>
#ifdef RABBITCOMMON
    #include "RabbitCommonTools.h"
    #include "FrmUpdater/FrmUpdater.h"
#endif
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
    
    CLunarCalendar::InitResource();
    
#ifdef RABBITCOMMON
    CRabbitCommonTools::Instance()->Init();
#endif
    
    a.setApplicationName("LunarCalendar");
    a.setApplicationDisplayName(QObject::tr("Calendar"));
    
//    QFile file("d:/Source/Tasks/Src/Resource/sink/dark/style.qss");  
//    if(file.open(QFile::ReadOnly))
//    {
//        QString stylesheet= file.readAll();
//        qApp->setStyleSheet(stylesheet);
//        file.close();
//    }
    
    MainWindow w;
    
#ifdef RABBITCOMMON
    CFrmUpdater u;
    u.SetTitle(qApp->applicationDisplayName(),
                 w.windowIcon().pixmap(w.windowIcon().availableSizes().at(0)));
    if(!u.GenerateUpdateXml())
        return 0;
#endif
    
#if defined (Q_OS_ANDROID)
    w.showMaximized();
#else
    w.show();
#endif
    return a.exec();
}
