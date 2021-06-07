#ifndef TIMERANGECALCULATOR_APPCONFIG_HPP
#define TIMERANGECALCULATOR_APPCONFIG_HPP

#include "programConfigQtgso/programConfigGUI.hpp"

#include <QString>

class QJsonObject;
class mainWindow_c;

class appConfig_c : public programConfigGUI_c
{
    Q_OBJECT

    mainWindow_c* mainWindow_pri = nullptr;

    void derivedReadJSON_f(const QJsonObject& ) override
    {}
    void derivedWriteJSON_f(QJsonObject& ) const override
    {}
    void derivedWriteJSONDocumented_f(QJsonObject&) const override
    {}
    bool firstPositionalArgumentCanBeConfig_f() const override
    {
        return false;
    }
    bool translationCreatePlacerholders_f() const override
    {
        return true;
    }
    bool loggingSaveLogFileRequired_f() const override
    {
        return true;
    }
    QString programName_f() const override
    {
        return "Time Range Calculator Qtg";
    }
    text_c programDescription_f() const override
    {
        return "Time Range Calculator Qtg description\nCreated by Jouven";
    }
    QString programVersion_f() const override
    {
        return "1.0";
    }
    //only for the programconfigQt stuff but nothing to implement here
    bool requiresJSONDocumented_f() const override
    {
        return true;
    }
    void derivedConfigureCommandLineParser_f(QCommandLineParser& parser_par) const override;
    void derivedCheckCommandLineParser_f(QCommandLineParser& parser_par) override;
    void derivedStart_f() override;
    void derivedQuit_f() override;
public:
    appConfig_c(QObject* parent_par, mainWindow_c* mainWindow_par);
};

//needs to be initialized
extern appConfig_c* appConfig_ptr_ext;

#endif // TIMERANGECALCULATOR_APPCONFIG_HPP
