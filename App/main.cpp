/*! \copyright 康林工作室版权所有 (c)，保留所有权力。
 * \author 康　林 <kl222@126.com>
 */

#include "MainWindow.h"
#include <QApplication>
#include <QFile>
#include <QTranslator>
#include <QDir>
#include <QScreen>
#include <QLoggingCategory>
#if defined(Q_OS_ANDROID)
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    
#elif QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    #include <QtAndroid>
#endif
#else
    
#endif
#ifdef RABBITCOMMON
    #include "RabbitCommonTools.h"
    #include "FrmUpdater.h"
    #include "RabbitCommonDir.h"
#endif
#include <QLocale>
#include "LunarCalendar.h"

static Q_LOGGING_CATEGORY(Logger, "App.main")

/*!
 * \defgroup Example 示例程序
 */

/*!
 * \ingroup Example
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationVersion(LunarCalendar_VERSION);
    a.setApplicationName("LunarCalendar");

#ifdef RABBITCOMMON
    RabbitCommon::CTools::Instance()->Init();
#endif
    RabbitCommon::CTools::Instance()->InstallTranslator("LunarCalendarApp");

    a.setApplicationDisplayName(QObject::tr("Calendar"));
    
    qInfo(Logger) << QObject::tr("Calendar") + " " + CLunarCalendar::Version()
                  << QObject::tr("Rabbit Common: ")
                         + RabbitCommon::CTools::Version();

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

    if(a.arguments().size() > 1) {
        if(!pUpdate->GenerateUpdateJson())
            return 0;
        if(!pUpdate->GenerateUpdateXml())
            return 0;
    }
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

    return nRet;
}
