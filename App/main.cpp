// 作者：康林 <kl222@126.com>

#include "MainWindow.h"
#include <QApplication>
#include <QFile>
#include <QTranslator>
#include <QDir>
#include <QScreen>
#if defined(Q_OS_ANDROID) && QT_VERSION >= QT_VERSION_CHECK(5, 7, 0) && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
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
    a.setApplicationVersion(LunarCalendar_VERSION);
    a.setApplicationName("LunarCalendar");
    
#if defined(Q_OS_ANDROID) && QT_VERSION >= QT_VERSION_CHECK(5, 7, 0) && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QtAndroid::hideSplashScreen();
#endif

    QTranslator tApp;
    bool bRetTranslator = tApp.load(RabbitCommon::CDir::Instance()->GetDirTranslations()
              + "/LunarCalendarApp_" + QLocale::system().name() + ".qm");
    if(bRetTranslator)
        a.installTranslator(&tApp);

    CLunarCalendar::InitResource();

#ifdef RABBITCOMMON
    RabbitCommon::CTools::Instance()->Init();
#endif

    a.setApplicationDisplayName(QObject::tr("Calendar"));

#ifdef RABBITCOMMON
    CFrmUpdater *pUpdate = new CFrmUpdater();
    QIcon icon = QIcon::fromTheme("calendar");
    if(!icon.isNull()) {
        auto sizeList = icon.availableSizes();
        if(!sizeList.isEmpty()) {
            QPixmap p = icon.pixmap(*sizeList.begin());
            pUpdate->SetTitle(p.toImage());
        }
    }
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
    int nRet = a.exec();

    if(bRetTranslator) a.removeTranslator(&tApp);

    return nRet;
}
