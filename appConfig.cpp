#include "appConfig.hpp"

#include "mainWindow.hpp"

#include <QJsonObject>
#include <QCommandLineParser>

void appConfig_c::derivedConfigureCommandLineParser_f(QCommandLineParser& ) const
{
    //if i ever serialize the time range rows of the different tables and implement a save/load
    //parser_par.addPositionalArgument("action file paths", "Optional, path/s to action files to be loaded, they must be compatible-valid");
}

void appConfig_c::derivedCheckCommandLineParser_f(QCommandLineParser& )
{

}

void appConfig_c::derivedStart_f()
{
    appConfig_ptr_ext = this;

    QObject::connect(this, &appConfig_c::started_signal, mainWindow_pri, &mainWindow_c::start_f);
    QObject::connect(mainWindow_pri, &mainWindow_c::closeWindow_signal, this, &appConfig_c::quit_signal);

    loadConfigFile_f();
    tryLoadTranslations_f();
    loadLogging_f();

    if (configLoaded_f())
    {

    }
    else
    {
        //messageUser_f({"Couldn't load {0} config file, pass \"-g\" argument to generate a config file documentation file", configFilePath_f()}, messageType_ec::information);
        Q_EMIT quit_signal();
    }
}

void appConfig_c::derivedQuit_f()
{
    saveConfigFile_f();
    saveTranslationFile_f();
}

appConfig_c::appConfig_c(QObject* parent_par, mainWindow_c* mainWindow_par)
    : programConfigGUI_c(parent_par), mainWindow_pri(mainWindow_par)
{}

appConfig_c* appConfig_ptr_ext = nullptr;
