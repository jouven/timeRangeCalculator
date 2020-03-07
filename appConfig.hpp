#ifndef TIMERANGECALCULATOR_APPCONFIG_HPP
#define TIMERANGECALCULATOR_APPCONFIG_HPP

#include "translatorJSONQtso/translator.hpp"
#include "logsinJSONQtso/logDataHub.hpp"
#include "essentialQtso/essentialQt.hpp"

#include <QString>
#include <QByteArray>

#include <QHash>
#include <QMap>

#include <vector>

class QJsonObject;
class action_c;
class check_c;

//don't call this macro inside the appConfig_c class (like calling from the ctor and hanging the process... without errors...)
//inside use MACRO_ADDMESSAGE
#define MACRO_FILEMETA MACRO_FILENAME, __func__, __LINE__
#define MACRO_ADDACTONQTGLOG(...) appConfig_ptr_ext->addLogMessage_f(__VA_ARGS__, MACRO_FILEMETA)
class appConfig_c
{
    bool configLoaded_pri = false;

    QHash<QString, QByteArray> widgetGeometryUMap_pri;
    //first key is dialog name/id, nested qmap key is datetime and value is the directory path
    QHash<QString, QMap<int_fast64_t, QString>> fileDialogNameToDirectoryNameAndTimeMap_pri;

    //to prevent resaving if the file data is the same as the data being saved
    QByteArray loadChecksum_pri;

    //right now, translationConfigFile_pri and logsDirectoryPath_pri
    //can't be set from-inside the program (TODO),
    //the set variables will confirm when a legit SET of these variables happens.
    //When not set the program will use default values but
    //still it won't SET the variables so they don't get saved with the default values
    //because the default path nature is dynamic
    //or the default value is an absolute path (if the program gets moved it will use the old path still)
    //right now they only get SET if they come from the json config file
    QString translationConfigFile_pri;
    bool translationConfigFileSet_pri = false;

    translator_c translator_pri;

    QString logsDirectoryPath_pri;
    bool logsDirectoryPathSet_pri = false;

    logDataHub_c logDataHub_pri;

    QCommandLineParser commandLineParser_pri;

    void tryLoadTranslations_f();

    void read_f(const QJsonObject &json_par_con);
    void write_f(QJsonObject &json_par) const;
public:
    appConfig_c();

    bool saveConfigFile_f();

    bool configLoaded_f() const;

    //I would use a pair but restoring states with an empty QByteArray doesn't do anything... so it's ok for now
    QByteArray widgetGeometry_f(const QString& key_par_con) const;
    void setWidgetGeometry_f(const QString& key_par_con, const QByteArray& windowGeometry_par_con);

    std::vector<QString> directoryHistory_f(const QString& fileDialogStringId_par_con) const;
    void addDirectoryHistory_f(const QString& fileDialogStringId_par_con, const QString& directory_par_con);

    QString translate_f(const QString& key_par_con);
    QString translateAndReplace_f(const text_c& text_par_con);
    QString translateAndReplace_f(const textCompilation_c& textCompilation_par_con);
    //FUTURE allow to change language using translationFromToPairs_f (the "to" part)
    //Making a program like "translationParserAvidcalm" (which is another program made by me, that parses the source js/cpp files for translations)
    //but to generate skeleton json language files
    //(from language will always be hardcoded because the initial source-language is always harcoded)

    bool addLogMessage_f(
            const text_c& message_par_con
            , const logItem_c::type_ec logType_par_con
            , const QString& sourceFile_par_con
            , const QString& sourceFunction_par_con
            , const int_fast32_t line_par_con
    );

    std::vector<std::pair<const logItem_c* const, const QDateTime* const>> getLogs_f(const logFilter_c& logFilter_par_con = logFilter_c()) const;

    QStringList commandLinePositionalArguments_f() const;

    logDataHub_c* logDataHub_f();
};

//needs to be initialized
extern appConfig_c* appConfig_ptr_ext;

#endif // TIMERANGECALCULATOR_APPCONFIG_HPP
