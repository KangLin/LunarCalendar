#include "MainWindow.h"
#include <QApplication>
#include <QFile>
#include <QTranslator>
#include <QDir>
#include <QScreen>
#if defined(Q_OS_ANDROID)
#include <QtAndroid>
#endif
#ifdef RABBITCOMMON
    #include "RabbitCommonTools.h"
    #include "FrmUpdater/FrmUpdater.h"
    #include "RabbitCommonDir.h"
#endif
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationVersion(BUILD_VERSION);

#if defined(Q_OS_ANDROID) && QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    QtAndroid::hideSplashScreen();
#endif

    QTranslator tApp;
    tApp.load(RabbitCommon::CDir::Instance()->GetDirTranslations()
              + "/LunarCalendarApp_" + QLocale::system().name() + ".qm");
    a.installTranslator(&tApp);

    CLunarCalendar::InitResource();

#ifdef RABBITCOMMON
    RabbitCommon::CTools::Instance()->Init();
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

#ifdef RABBITCOMMON
    CFrmUpdater *pUpdate = new CFrmUpdater();
    pUpdate->SetTitle(QPixmap(":/image/Calendar"));
    if(!pUpdate->GenerateUpdateXml())
        return 0;
#endif

    MainWindow w;

#if defined (Q_OS_ANDROID)
    w.showMaximized();
#else
    #if defined (Q_OS_UNIX)
        QRect rect = qApp->primaryScreen()->geometry();
        int left = (rect.width() - 500) >> 1;
        int top = (rect.height() - 500) >> 1;
        w.move(left, top);
    #endif
    w.show();
#endif
    return a.exec();
}
