#include "FrmLunarCalendarPlugin.h"
#include "LunarCalendar.h"
#include "RabbitCommonTools.h"

CFrmLunarCalendarPlugin::CFrmLunarCalendarPlugin(QObject *parent)
    : QObject{parent},
    QDesignerCustomWidgetInterface(),
    m_initialized(false)
{}

QString CFrmLunarCalendarPlugin::name() const
{
    return tr("LunarCalendar");
}

QString CFrmLunarCalendarPlugin::group() const
{
    return tr("Display Widgets");
}

QString CFrmLunarCalendarPlugin::toolTip() const
{
    return name();
}

QString CFrmLunarCalendarPlugin::whatsThis() const
{
    return name();
}

QString CFrmLunarCalendarPlugin::includeFile() const
{
    return ("LunarCalendar.h");
}

QIcon CFrmLunarCalendarPlugin::icon() const
{
    return QIcon(":/Image/Today");
    return QIcon::fromTheme("calendar");
}

bool CFrmLunarCalendarPlugin::isContainer() const
{
    return false;
}

QWidget *CFrmLunarCalendarPlugin::createWidget(QWidget *parent)
{
    return new CLunarCalendar(parent);
}

bool CFrmLunarCalendarPlugin::isInitialized() const
{
    return m_initialized;
}

void CFrmLunarCalendarPlugin::initialize(QDesignerFormEditorInterface *core)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    RabbitCommon::CTools::Instance()->Init();

    m_initialized = true;
}

QString CFrmLunarCalendarPlugin::domXml() const
{
    return QLatin1String("<widget class=\"CFrmLunarCalendar\" name=\"FrmLunarCalendar\">\n</widget>\n");
}

QString CFrmLunarCalendarPlugin::codeTemplate() const
{
    return QString();
}
