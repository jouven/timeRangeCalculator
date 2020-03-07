#include "mainWindow.hpp"

#include "appConfig.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication qtapp(argc, argv);
    QApplication::setApplicationName("Time Range Calculator");
    QApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Time Range Calculator description\nCreated by Jouven");
    parser.addHelpOption();
    parser.addVersionOption();

    //process the actual command line arguments given by the user
    parser.process(qtapp);

    appConfig_c appConfigTmp;
    appConfig_ptr_ext = std::addressof(appConfigTmp);

    mainWindow_c mainWindowTmp;
    mainWindowTmp.show();

    return qtapp.exec();
}
