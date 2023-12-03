#ifndef CFRMLUNARCALENDARPLUGIN_H
#define CFRMLUNARCALENDARPLUGIN_H

#include <QObject>

#include <QDesignerCustomWidgetInterface>
class CFrmLunarCalendarPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT

    Q_INTERFACES(QDesignerCustomWidgetInterface)
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface")
#endif // QT_VERSION >= 0x050000

public:
    explicit CFrmLunarCalendarPlugin(QObject *parent = nullptr);

    // QDesignerCustomWidgetInterface interface
    virtual QString name() const override;
    virtual QString group() const override;
    virtual QString toolTip() const override;
    virtual QString whatsThis() const override;
    virtual QString includeFile() const override;
    virtual QIcon icon() const override;
    virtual bool isContainer() const override;
    virtual QWidget *createWidget(QWidget *parent) override;
    virtual bool isInitialized() const override;
    virtual void initialize(QDesignerFormEditorInterface *core) override;
    virtual QString domXml() const override;
    virtual QString codeTemplate() const override;
    
private:
    bool m_initialized;
};

#endif // CFRMLUNARCALENDARPLUGIN_H
