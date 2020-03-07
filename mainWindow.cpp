#include "mainWindow.hpp"

#include "appConfig.hpp"

#include "essentialQtgso/messageBox.hpp"

#include "comuso/practicalTemplates.hpp"

#include <QtWidgets>
#include <QTime>

#include <cstdlib>
#include <set>

#ifdef DEBUGJOUVEN
#include <QDebug>
#endif

void mainWindow_c::saveTableGeometry_f(QTableWidget* table_par) const
{
    appConfig_ptr_ext->setWidgetGeometry_f(this->objectName() + table_par->objectName() + table_par->horizontalHeader()->objectName(), table_par->horizontalHeader()->saveState());
    appConfig_ptr_ext->setWidgetGeometry_f(this->objectName() + table_par->objectName(), table_par->saveGeometry());
}

void mainWindow_c::loadTableGeometry_f(QTableWidget* table_par)
{
    table_par->restoreGeometry(appConfig_ptr_ext->widgetGeometry_f(this->objectName() + table_par->objectName()));
    table_par->horizontalHeader()->restoreState(appConfig_ptr_ext->widgetGeometry_f(this->objectName() + table_par->objectName() + table_par->horizontalHeader()->objectName()));
}

void mainWindow_c::createTableEditControlsWidgets_f(QVBoxLayout* layout_par)
{
    //1st row add from, add to, remove selected, about and clear all buttons
    QHBoxLayout* firstRowLayoutTmp(new QHBoxLayout);

    QPushButton* addTimeFromButtonTmp(new QPushButton(appConfig_ptr_ext->translate_f("Add &From time")));
    addTimeFromButtonTmp->setToolTip(appConfig_ptr_ext->translate_f("Also works pressing 'a' while the input is focused"));
    firstRowLayoutTmp->addWidget(addTimeFromButtonTmp);
    QObject::connect(addTimeFromButtonTmp, &QPushButton::clicked, this, &mainWindow_c::addTimeFrom_f);

    QPushButton* addTimeToButtonTmp(new QPushButton(appConfig_ptr_ext->translate_f("Add &To time")));
    addTimeToButtonTmp->setToolTip(appConfig_ptr_ext->translate_f("A row must be selected, also works pressing 's' while the input is focused"));
    firstRowLayoutTmp->addWidget(addTimeToButtonTmp);
    QObject::connect(addTimeToButtonTmp, &QPushButton::clicked, this, &mainWindow_c::addTimeTo_f);

    QPushButton* removeSelectionButtonTmp(new QPushButton(appConfig_ptr_ext->translate_f("&Remove time")));
    firstRowLayoutTmp->addWidget(removeSelectionButtonTmp);
    QObject::connect(removeSelectionButtonTmp, &QPushButton::clicked, this, &mainWindow_c::removeSelected_f);

    //about button
    QIcon aboutIcon = QIcon::fromTheme("help-about", QIcon(":/images/about.png"));
    QPushButton* aboutButtonTmp = new QPushButton(aboutIcon, appConfig_ptr_ext->translate_f("About"));
    firstRowLayoutTmp->addWidget(aboutButtonTmp);
    QObject::connect(aboutButtonTmp, &QPushButton::clicked, this, &mainWindow_c::showAboutMessage_f);

    QIcon clearAllIcon(QIcon::fromTheme("document-new", QIcon(":/images/new.png")));
    QPushButton* newConfigFileButtonTmp(new QPushButton(clearAllIcon, appConfig_ptr_ext->translate_f("Clear All")));
    newConfigFileButtonTmp->setToolTip(appConfig_ptr_ext->translate_f("Clears all the tables"));
    firstRowLayoutTmp->addWidget(newConfigFileButtonTmp);
    QObject::connect(newConfigFileButtonTmp, &QPushButton::clicked, this, &mainWindow_c::clearAllTables_f);

    firstRowLayoutTmp->insertStretch(-1, 1);

    //2nd row radios to select which table is being edited
    QHBoxLayout* secondRowLayoutTmp(new QHBoxLayout);

    editValuesTableRadio_pri = new QRadioButton(appConfig_ptr_ext->translate_f("&Values"));
    editChangesTableRadio_pri = new QRadioButton(appConfig_ptr_ext->translate_f("&Changes"));

    secondRowLayoutTmp->addWidget(new QLabel(appConfig_ptr_ext->translate_f("Time table to edit")));
    secondRowLayoutTmp->addWidget(editValuesTableRadio_pri);
    secondRowLayoutTmp->addWidget(editChangesTableRadio_pri);

    secondRowLayoutTmp->insertStretch(-1, 1);

    QButtonGroup* editTableRadiosButtonGroupTmp = new QButtonGroup;
    editTableRadiosButtonGroupTmp->addButton(editValuesTableRadio_pri);
    editTableRadiosButtonGroupTmp->addButton(editChangesTableRadio_pri);

    editValuesTableRadio_pri->setChecked(true);

    //3rd row time input, sign (for time change) radios
    QHBoxLayout* thirdRowLayoutTmp = new QHBoxLayout;

    changeValuePositiveRadio_pri = new QRadioButton("&+");
    changeValueNegativeRadio_pri = new QRadioButton("&-");

    thirdRowLayoutTmp->addWidget(changeValuePositiveRadio_pri);
    thirdRowLayoutTmp->addWidget(changeValueNegativeRadio_pri);

    QButtonGroup* signRadiosButtonGroupTmp = new QButtonGroup(this);
    signRadiosButtonGroupTmp->addButton(changeValuePositiveRadio_pri);
    signRadiosButtonGroupTmp->addButton(changeValueNegativeRadio_pri);

    changeValuePositiveRadio_pri->setChecked(true);

    timeInputLineEdit_pri = new timeInput_c;
    timeInputLineEdit_pri->setToolTip(appConfig_ptr_ext->translate_f(
R"(While the input is focused there are the following hotkeys:
'a' same pushing Adds From time button
's' same pushing Adds To time button
'd' adds a ':' character
'f' same a pressing backspace)"
));
    //that's the "string" length of max 64bit signed number
    /////////////////////////////////////"9223372036854775808 (length 19)
    QRegExpValidator *a64bitValidatorTmp = new QRegExpValidator(QRegExp(R"([0-9\.:\s-]{19})"), this);
    timeInputLineEdit_pri->setValidator(a64bitValidatorTmp);
    //timerValueLineEdit_pri->setInputMask("0000000000000000009");

    thirdRowLayoutTmp->addWidget(new QLabel(appConfig_ptr_ext->translate_f("Time input")));
    thirdRowLayoutTmp->addWidget(timeInputLineEdit_pri);

    thirdRowLayoutTmp->insertStretch(-1, 1);

    layout_par->addLayout(firstRowLayoutTmp);
    layout_par->addLayout(secondRowLayoutTmp);
    layout_par->addLayout(thirdRowLayoutTmp);
}

void mainWindow_c::createTableExportOptionsWidgets_f(QVBoxLayout* layout_par)
{
    QHBoxLayout* firstRowTmp(new QHBoxLayout);

    exportPresetCombo_pri = new QComboBox();
    exportPresetCombo_pri->setEditable(true);
    exportPresetCombo_pri->setInsertPolicy(QComboBox::NoInsert);
    exportPresetCombo_pri->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    exportPresetCombo_pri->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    firstRowTmp->addWidget(new QLabel(appConfig_ptr_ext->translate_f("Export preset")));
    firstRowTmp->addWidget(exportPresetCombo_pri);

    exportPresetCombo_pri->insertItem(exportPresetCombo_pri->count(), appConfig_ptr_ext->translate_f("All columns"), "All columns");
    exportPresetCombo_pri->insertItem(exportPresetCombo_pri->count(), appConfig_ptr_ext->translate_f("Milliseconds"), "Milliseconds");
    exportPresetCombo_pri->insertItem(exportPresetCombo_pri->count(), appConfig_ptr_ext->translate_f("Time ranges"), "Time ranges");

    QPushButton* exportButtonTmp(new QPushButton(appConfig_ptr_ext->translate_f("Export to clipboard")));
    exportButtonTmp->setToolTip(appConfig_ptr_ext->translate_f("Exporting also works pressing the \"copy\" key sequence (usually ctrl+c), a table must focused"));
    firstRowTmp->addWidget(exportButtonTmp);
    QObject::connect(exportButtonTmp, &QPushButton::clicked, this, &mainWindow_c::exportToClipboardButtonPushed_f);

    QHBoxLayout* secondRowTmp(new QHBoxLayout);

    tableToExportCombo_pri = new QComboBox();
    tableToExportCombo_pri->setEditable(true);
    tableToExportCombo_pri->setInsertPolicy(QComboBox::NoInsert);
    tableToExportCombo_pri->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    tableToExportCombo_pri->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    secondRowTmp->addWidget(new QLabel(appConfig_ptr_ext->translate_f("Table to export")));
    secondRowTmp->addWidget(tableToExportCombo_pri);

    tableToExportCombo_pri->insertItem(tableToExportCombo_pri->count(), appConfig_ptr_ext->translate_f("Values"), "Values");
    tableToExportCombo_pri->insertItem(tableToExportCombo_pri->count(), appConfig_ptr_ext->translate_f("Changes"), "Changes");
    tableToExportCombo_pri->insertItem(tableToExportCombo_pri->count(), appConfig_ptr_ext->translate_f("Results"), "Results");

    //include elapsed time check
    resultExportIncludeElapsed_pri = new QCheckBox(appConfig_ptr_ext->translate_f("Include elapsed time"));
    resultExportIncludeElapsed_pri->setToolTip(appConfig_ptr_ext->translate_f("Only applies to results, add an extra column with elapsed time (milliseconds, timestamp or both, depending on the preset)"));
    secondRowTmp->addWidget(resultExportIncludeElapsed_pri);

    //include elapsed time check
    resultExportIncludeSign_pri = new QCheckBox(appConfig_ptr_ext->translate_f("Include sign"));
    resultExportIncludeSign_pri->setToolTip(appConfig_ptr_ext->translate_f("Include sign column when exporting changes table rows"));
    resultExportIncludeSign_pri->setChecked(true);
    secondRowTmp->addWidget(resultExportIncludeSign_pri);

    //time range separator radios
    QHBoxLayout* thirdRowTmp(new QHBoxLayout);

    timeRangeSeparatorStringRadio_pri = new QRadioButton(appConfig_ptr_ext->translate_f("String"));
    timeRangeSeparatorNewlineRadio_pri = new QRadioButton(appConfig_ptr_ext->translate_f("Newline"));

    QButtonGroup* timeRangeSeparatoButtonGroupTmp = new QButtonGroup(this);
    timeRangeSeparatoButtonGroupTmp->addButton(timeRangeSeparatorStringRadio_pri);
    timeRangeSeparatoButtonGroupTmp->addButton(timeRangeSeparatorNewlineRadio_pri);

    timeRangeSeparatorStringRadio_pri->setChecked(true);

    exportTimeRangeSeparatorStringLineEdit_pri = new QLineEdit;
    exportTimeRangeSeparatorStringLineEdit_pri->setText("-");
    exportTimeRangeSeparatorStringLineEdit_pri->setToolTip(
                appConfig_ptr_ext->translate_f(
                    "Character or string to separate time range, i.e. '-' 01:00-02:00, '_' 01:00_02:00"
                    )
                );

    thirdRowTmp->addWidget(new QLabel(appConfig_ptr_ext->translate_f("Time range separator")));
    thirdRowTmp->addWidget(timeRangeSeparatorNewlineRadio_pri);
    thirdRowTmp->addWidget(timeRangeSeparatorStringRadio_pri);
    thirdRowTmp->addWidget(exportTimeRangeSeparatorStringLineEdit_pri);

    //time row separator radios
    QHBoxLayout* fourthRowTmp(new QHBoxLayout);

    timeRowSeparatorStringRadio_pri = new QRadioButton(appConfig_ptr_ext->translate_f("String"));
    timeRowSeparatorNewlineRadio_pri = new QRadioButton(appConfig_ptr_ext->translate_f("Newline"));

    QButtonGroup* timeRowSeparatoButtonGroupTmp = new QButtonGroup(this);
    timeRowSeparatoButtonGroupTmp->addButton(timeRowSeparatorStringRadio_pri);
    timeRowSeparatoButtonGroupTmp->addButton(timeRowSeparatorNewlineRadio_pri);

    timeRowSeparatorNewlineRadio_pri->setChecked(true);

    exportTimeRowSeparatorStringLineEdit_pri = new QLineEdit;
    exportTimeRowSeparatorStringLineEdit_pri->setText("|");
    exportTimeRowSeparatorStringLineEdit_pri->setToolTip(
                appConfig_ptr_ext->translate_f(
                    "Character or string to separate time range, i.e. '|' 01:00-02:00|03:00-04:00"
                    )
                );

    fourthRowTmp->addWidget(new QLabel(appConfig_ptr_ext->translate_f("Time row separator")));
    fourthRowTmp->addWidget(timeRowSeparatorNewlineRadio_pri);
    fourthRowTmp->addWidget(timeRowSeparatorStringRadio_pri);
    fourthRowTmp->addWidget(exportTimeRowSeparatorStringLineEdit_pri);

    //allow export selection using control+c
    //presets to export the result table: all columns, "milliseconds ranges" columns, "time ranges" columns
    //add check to include elapsed column
    //from-to separator: newline, input a string
    //separator between range pairs: newline, input a string
    //
    //widgets:table to copy combo, preset to export combo, include elapsed check (only for the result table)
    //separator combos
    //qlineedit for input string separator
    //button to export
    layout_par->addLayout(firstRowTmp);
    layout_par->addLayout(secondRowTmp);
    layout_par->addLayout(thirdRowTmp);
    layout_par->addLayout(fourthRowTmp);
}

void mainWindow_c::closeEvent(QCloseEvent* event)
{
    saveTableGeometry_f(timeValuesTable_pri);
    saveTableGeometry_f(timeChangesTable_pri);
    saveTableGeometry_f(timeResultsTable_pri);
    appConfig_ptr_ext->setWidgetGeometry_f(this->objectName(), saveGeometry());

    appConfig_ptr_ext->saveConfigFile_f();

    event->accept();
}

int mainWindow_c::getRowFromTimeBase_f(const int_fast64_t time_par_con) const
{
    int resultTmp(-1);
    for (int rowIndexTimeBase = 0, l = timeValuesTable_pri->rowCount(); rowIndexTimeBase<l; ++rowIndexTimeBase)
    {
        if (timeValuesTable_pri->item(rowIndexTimeBase, 1)->data(Qt::DisplayRole).toLongLong() == time_par_con)
        {
            resultTmp = rowIndexTimeBase;
            break;
        }
    }
    return resultTmp;
}

void mainWindow_c::removeSelectedTimeBaseTable_f()
{
    QList<QTableWidgetItem *> selectionTmp = timeValuesTable_pri->selectedItems();
    if (not selectionTmp.isEmpty())
    {
        std::set<int> rowIndexSetTmp;
        //get the selected row (indexes)
        for (const QTableWidgetItem* item_ite_con : selectionTmp)
        {
            rowIndexSetTmp.emplace(item_ite_con->row());
        }

        std::vector<int> rowsToRemoveTmp(rowIndexSetTmp.begin(), rowIndexSetTmp.end());
        std::reverse(rowsToRemoveTmp.begin(), rowsToRemoveTmp.end());
        for (const int row_ite_con : rowsToRemoveTmp)
        {
            int_fast64_t timeChangeValueTmp(timeValuesTable_pri->item(row_ite_con, 0)->data(Qt::DisplayRole).toLongLong());
            timeBaseValues_pri.erase(timeChangeValueTmp);
            timeValuesTable_pri->removeRow(row_ite_con);
        }
        //remove results too
        for (const int row_ite_con : rowsToRemoveTmp)
        {
            timeResultsTable_pri->removeRow(row_ite_con);
        }
    }
}

void mainWindow_c::removeSelectedTimeChangeTable_f()
{
    QList<QTableWidgetItem *> selectionTmp = timeChangesTable_pri->selectedItems();
    if (not selectionTmp.isEmpty())
    {
        std::set<int> rowIndexSetTmp;
        //get the selected row (indexes)
        for (const QTableWidgetItem* item_ite_con : selectionTmp)
        {
            rowIndexSetTmp.emplace(item_ite_con->row());
        }

        std::vector<int> rowsToRemoveTmp(rowIndexSetTmp.begin(), rowIndexSetTmp.end());
        std::reverse(rowsToRemoveTmp.begin(), rowsToRemoveTmp.end());
        for (const int row_ite_con : rowsToRemoveTmp)
        {
            int_fast64_t timeChangeValueTmp(timeChangesTable_pri->item(row_ite_con, 1)->data(Qt::DisplayRole).toLongLong());
            timeChangeValues_pri.erase(timeChangeValueTmp);
            timeChangesTable_pri->removeRow(row_ite_con);
        }

        if (timeValuesTable_pri->rowCount() > 0)
        {
            updateResults_f();
        }
    }
}

void mainWindow_c::keyPressEvent(QKeyEvent* event_par)
{

     if (event_par->matches(QKeySequence::Copy))
     {
         QTableWidget* focusedTable(nullptr);
         if (timeValuesTable_pri->hasFocus())
         {
             focusedTable = timeValuesTable_pri;
         }
         if (timeChangesTable_pri->hasFocus())
         {
             focusedTable = timeChangesTable_pri;
         }
         if (timeResultsTable_pri->hasFocus())
         {
             focusedTable = timeResultsTable_pri;
         }
         exportToClipboard_f(focusedTable);
     }
     if (event_par->key() == Qt::Key_A)
     {
         addTimeFrom_f();
     }
     if (event_par->key() == Qt::Key_S)
     {
         addTimeTo_f();
     }
     if (event_par->key() == Qt::Key_D)
     {
         timeInputLineEdit_pri->setText(timeInputLineEdit_pri->text() + ":");
     }
     if (event_par->key() == Qt::Key_F)
     {
         //simulate backspace
         QKeyEvent* backspacePressTmp(new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier));
         QApplication::sendEvent(timeInputLineEdit_pri, backspacePressTmp);
     }
}

void mainWindow_c::updateResults_f()
{
    //iterate from the last time (biggest time in milliseconds) to the first time
    auto reverseIteratorValueTmp(timeBaseValues_pri.rbegin());
    //get the last row index
    int_fast64_t reverseRowIndexTmp(timeValuesTable_pri->rowCount() - 1);
    while (reverseIteratorValueTmp not_eq timeBaseValues_pri.rend())
    {
        int_fast64_t currentFromTimeTmp(reverseIteratorValueTmp->first);
        int_fast64_t currentToTimeTmp(0);
        bool includeToTmp(reverseIteratorValueTmp->second > 0);
        if (includeToTmp)
        {
            currentToTimeTmp = reverseIteratorValueTmp->first + reverseIteratorValueTmp->second;
        }
        //it doesn't matter if its rbegin or just begin
        auto reverseIteratorFromChangeTmp(timeChangeValues_pri.rbegin());
        //once found start adding or substracting change values previous to the "time base" value found
        //then insert the result as the first row always
        while (reverseIteratorFromChangeTmp not_eq timeChangeValues_pri.rend())
        {
            //the "change from" timestamp must be smaller than the "value from" timestamp
            if (reverseIteratorFromChangeTmp->first < currentFromTimeTmp)
            {
                //if change range is NOT set ignore
                if (reverseIteratorFromChangeTmp->second == 0)
                {
                }
                else
                {
                    currentFromTimeTmp = currentFromTimeTmp + reverseIteratorFromChangeTmp->second;
                    if (includeToTmp)
                    {
                        currentToTimeTmp = currentToTimeTmp + reverseIteratorFromChangeTmp->second;
                    }
                }
            }
            ++reverseIteratorFromChangeTmp;
        }

        timeResultsTable_pri->item(reverseRowIndexTmp, 0)->setData(Qt::DisplayRole, currentFromTimeTmp);
        timeResultsTable_pri->item(reverseRowIndexTmp, 1)->setData(Qt::DisplayRole, formatedTime_f(currentFromTimeTmp));
        if (includeToTmp)
        {
            timeResultsTable_pri->item(reverseRowIndexTmp, 2)->setData(Qt::DisplayRole, currentToTimeTmp);
            timeResultsTable_pri->item(reverseRowIndexTmp, 3)->setData(Qt::DisplayRole, formatedTime_f(currentToTimeTmp));
        }

        ++reverseIteratorValueTmp;
        --reverseRowIndexTmp;
    }
}

int_fast64_t mainWindow_c::parseInput_f() const
{
    int_fast64_t resultTmp(0);
    QString inputTextTmp(timeInputLineEdit_pri->text());
    while (not inputTextTmp.isEmpty())
    {
        //case number of milliseconds
        bool goodNumberConversion(false);
        int_fast64_t integerFromTextTmp(inputTextTmp.toLongLong(&goodNumberConversion));
        if (goodNumberConversion)
        {
            resultTmp = integerFromTextTmp;
            break;
        }

        //try to parse different time formats
        std::vector<QString> formatsTmp(
        {"mm:ss"
         , "m:s"
         , "hh:mm:ss"
         , "h:m:s"
         , "hh:mm:ss.zzz"
         ,  "h:m:s.z"
         , "ss.zzz"
         , "s.z"
         , "mm:ss.zzz"
         , "m:s.z"}
        );
        QTime timeTmp;
        bool calculateResultTmp(false);
        for (const QString& format_ite_con : formatsTmp)
        {
            timeTmp = QTime::fromString(inputTextTmp, format_ite_con);
            if (timeTmp.isValid())
            {
                calculateResultTmp = true;
                break;
            }
        }
        if (calculateResultTmp)
        {
            resultTmp = (((timeTmp.hour() * 3600) + (timeTmp.minute() * 60) + timeTmp.second()) * 1000) + timeTmp.msec();
            break;
        }

        //case custom format
        inputTextTmp.replace(':',' ').replace('/', ' ').replace('-',' ');
        QStringList splitTmp(inputTextTmp.split(" ", QString::SkipEmptyParts));
        for (const QString& something_par_con : splitTmp)
        {
            QChar lastCharacterTmp(something_par_con.at(something_par_con.size() - 1));
            if (equalOnce_ft(lastCharacterTmp, 'z', 's', 'm', 'h', 'd'))
            {
                QString somethingCopyTmp(something_par_con);
                //remove the character at the end and try number conversion
                somethingCopyTmp.chop(1);
                bool goodNumberConversion(false);
                int_fast64_t integerFromTextTmp(somethingCopyTmp.toLongLong(&goodNumberConversion));
                if (goodNumberConversion)
                {
                    if (lastCharacterTmp == 'z')
                    {
                        resultTmp = resultTmp + integerFromTextTmp;
                        continue;
                    }

                    if (lastCharacterTmp == 's')
                    {
                        resultTmp = resultTmp + (integerFromTextTmp * 1000);
                        continue;
                    }

                    if (lastCharacterTmp == 'm')
                    {
                        resultTmp = resultTmp + (integerFromTextTmp * 1000 * 60);
                        continue;
                    }

                    if (lastCharacterTmp == 'h')
                    {
                        resultTmp = resultTmp + (integerFromTextTmp * 1000) * 60 * 60;
                        continue;
                    }

                    if (lastCharacterTmp == 'd')
                    {
                        resultTmp = resultTmp + (integerFromTextTmp * 1000 * 60 * 60 * 24);
                        continue;
                    }
                }
            }
        }

        break;
    }

    return resultTmp;
}

QString mainWindow_c::formatedTime_f(const int_fast64_t milliseconds_par_con) const
{
    QString resultTmp("00:00");
    while (milliseconds_par_con not_eq 0)
    {
        QString millisecondsStrTmp(QString::number(milliseconds_par_con));
        QString secondsStrTmp;
        QString minutesStrTmp;
        QString hoursStrTmp;
        QString daysStrTmp;

        int_fast64_t secondsTmp(0);
        if (milliseconds_par_con < 1000)
        {
            resultTmp = millisecondsStrTmp;
            break;
        }
        else
        {
            resultTmp.clear();
            millisecondsStrTmp = millisecondsStrTmp.mid(millisecondsStrTmp.size() - 3);
            secondsTmp = milliseconds_par_con / 1000;
        }

        int_fast64_t quotTmp(0);
        if (secondsTmp > 0)
        {
            auto divisionSecondsResultTmp(std::lldiv(secondsTmp, 60));
            secondsStrTmp = QString::number(divisionSecondsResultTmp.rem);
            quotTmp = divisionSecondsResultTmp.quot;
            if (secondsStrTmp.size() == 1)
            {
                secondsStrTmp.prepend('0');
            }
        }

        if (quotTmp > 0)
        {
            auto divisionMinutesResultTmp(std::lldiv(quotTmp, 60));
            minutesStrTmp = QString::number(divisionMinutesResultTmp.rem);
            quotTmp = divisionMinutesResultTmp.quot;
            if (minutesStrTmp.size() == 1)
            {
                minutesStrTmp.prepend('0');
            }
        }

        if (quotTmp > 0)
        {
            auto divisionHoursResultTmp(std::lldiv(quotTmp, 24));
            hoursStrTmp = QString::number(divisionHoursResultTmp.rem);
            if (divisionHoursResultTmp.quot > 0)
            {
                daysStrTmp = QString::number(divisionHoursResultTmp.quot);
            }
        }

        //23 23:24:00.000
        if (millisecondsStrTmp not_eq "000")
        {
            resultTmp = millisecondsStrTmp;
            if (not secondsStrTmp.isEmpty())
            {
                resultTmp.prepend('.');
            }
        }
        if (not secondsStrTmp.isEmpty())
        {
            resultTmp.prepend(secondsStrTmp);
        }
        else
        {
            resultTmp.prepend("00");
        }

        if (not minutesStrTmp.isEmpty())
        {
            resultTmp.prepend(minutesStrTmp + ":");
        }
        else
        {
            resultTmp.prepend("00:");
        }

        if (not hoursStrTmp.isEmpty())
        {
            resultTmp.prepend(hoursStrTmp + ":");
        }

        if (not daysStrTmp.isEmpty())
        {
            resultTmp.prepend(daysStrTmp + "d ");
        }

        break;
    }
    return resultTmp;
}

mainWindow_c::mainWindow_c()
{
    this->setObjectName("mainWindow_");

    QShortcut* quitShortCut(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this));
    //Qt::WidgetWithChildrenShortcut works when the main window is focused (with other modals windows open)
    //20191103 except Qt::WidgetWithChildrenShortcut
    //all the other Qt::ShortcutContext fail to work with other modal window opened
    //focusing or not focusing the main window
    quitShortCut->setContext(Qt::WidgetWithChildrenShortcut);
    QObject::connect(quitShortCut, &QShortcut::activated, this, &mainWindow_c::close);

    //bottom row tables: base, changes and result
    QHBoxLayout* bottomRowLayoutTmp = new QHBoxLayout;

    QVBoxLayout* timeBaseColumnLayoutTmp(new QVBoxLayout);

    timeValuesTable_pri = new QTableWidget(0, 6);
    timeValuesTable_pri->setObjectName("timeBaseTable_QTableWidget_");
    timeValuesTable_pri->setSelectionBehavior(QAbstractItemView::SelectRows);

    {
        //0 From in milliseconds
        //1 From in hh:mm:ss.zzz
        //2 To range in milliseconds
        //3 To in hh:mm:ss.zzz
        //4 elapsed in milliseconds
        //5 elapsed in hh:mm:ss.zzz
        QStringList labelsTmp;
        labelsTmp
                << appConfig_ptr_ext->translate_f("From Ms")
                << appConfig_ptr_ext->translate_f("From")
                << appConfig_ptr_ext->translate_f("To Ms")
                << appConfig_ptr_ext->translate_f("To")
                << appConfig_ptr_ext->translate_f("Elapsed Ms")
                << appConfig_ptr_ext->translate_f("Elapsed");
        timeValuesTable_pri->setHorizontalHeaderLabels(labelsTmp);
    }
    timeValuesTable_pri->horizontalHeader()->setObjectName(timeValuesTable_pri->objectName() + "QHeaderView_");
    timeValuesTable_pri->horizontalHeader()->setSortIndicatorShown(false);
    timeValuesTable_pri->setShowGrid(true);
    timeValuesTable_pri->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    timeValuesTable_pri->setSortingEnabled(false);

    timeBaseColumnLayoutTmp->addWidget(new QLabel(appConfig_ptr_ext->translate_f("Values")));
    timeBaseColumnLayoutTmp->addWidget(timeValuesTable_pri);

    bottomRowLayoutTmp->addLayout(timeBaseColumnLayoutTmp);

    timeChangesTable_pri = new QTableWidget(0, 7);
    timeChangesTable_pri->setObjectName("timeChangeTable_QTableWidget_");
    timeChangesTable_pri->setSelectionBehavior(QAbstractItemView::SelectRows);

    {
        //0 sign +/-
        //1 From in milliseconds
        //2 From in hh:mm:ss.zzz
        //3 To range in milliseconds
        //4 To in hh:mm:ss.zzz
        //5 elapsed in milliseconds
        //6 elapsed in hh:mm:ss.zzz
        QStringList labelsTmp;
        labelsTmp
                << appConfig_ptr_ext->translate_f("Sign")
                << appConfig_ptr_ext->translate_f("From Ms")
                << appConfig_ptr_ext->translate_f("From")
                << appConfig_ptr_ext->translate_f("To Ms")
                << appConfig_ptr_ext->translate_f("To")
                << appConfig_ptr_ext->translate_f("Elapsed Ms")
                << appConfig_ptr_ext->translate_f("Elapsed");
        timeChangesTable_pri->setHorizontalHeaderLabels(labelsTmp);
    }
    timeChangesTable_pri->horizontalHeader()->setObjectName(timeChangesTable_pri->objectName() + "QHeaderView_");
    timeChangesTable_pri->horizontalHeader()->setSortIndicatorShown(false);
    timeChangesTable_pri->setShowGrid(true);
    timeChangesTable_pri->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    timeChangesTable_pri->setSortingEnabled(false);

    QVBoxLayout* timeChangeColumnLayoutTmp(new QVBoxLayout);

    timeChangeColumnLayoutTmp->addWidget(new QLabel(appConfig_ptr_ext->translate_f("Changes")));
    timeChangeColumnLayoutTmp->addWidget(timeChangesTable_pri);

    bottomRowLayoutTmp->addLayout(timeChangeColumnLayoutTmp);

    timeResultsTable_pri = new QTableWidget(0, 4);
    timeResultsTable_pri->setObjectName("timeResultTable_QTableWidget_");
    timeResultsTable_pri->setSelectionBehavior(QAbstractItemView::SelectRows);

    {
        //0 From in milliseconds
        //1 From in hh:mm:ss.zzz
        //2 To range in milliseconds
        //3 To in hh:mm:ss.zzz
        QStringList labelsTmp;
        labelsTmp
                << appConfig_ptr_ext->translate_f("From Ms")
                << appConfig_ptr_ext->translate_f("From")
                << appConfig_ptr_ext->translate_f("To Ms")
                << appConfig_ptr_ext->translate_f("To");
        timeResultsTable_pri->setHorizontalHeaderLabels(labelsTmp);
    }
    timeResultsTable_pri->horizontalHeader()->setObjectName(timeResultsTable_pri->objectName() + "QHeaderView_");
    timeResultsTable_pri->horizontalHeader()->setSortIndicatorShown(false);
    timeResultsTable_pri->setShowGrid(true);
    timeResultsTable_pri->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    timeResultsTable_pri->setSortingEnabled(false);

    QVBoxLayout* timeResultColumnLayoutTmp(new QVBoxLayout);

    timeResultColumnLayoutTmp->addWidget(new QLabel(appConfig_ptr_ext->translate_f("Results")));
    timeResultColumnLayoutTmp->addWidget(timeResultsTable_pri);

    bottomRowLayoutTmp->addLayout(timeResultColumnLayoutTmp);

    bottomRowLayoutTmp->insertStretch(-1, 1);


    QVBoxLayout* tablesEditorLayoutTmp(new QVBoxLayout);

    createTableEditControlsWidgets_f(tablesEditorLayoutTmp);

    QVBoxLayout* tablesExportOptionsLayoutTmp(new QVBoxLayout);

    createTableExportOptionsWidgets_f(tablesExportOptionsLayoutTmp);

    QHBoxLayout* optionsLayoutTmp(new QHBoxLayout);
    optionsLayoutTmp->addLayout(tablesEditorLayoutTmp);
    optionsLayoutTmp->addLayout(tablesExportOptionsLayoutTmp);
    optionsLayoutTmp->insertStretch(-1, 1);

    QVBoxLayout* mainLayoutTmp(new QVBoxLayout);
    mainLayoutTmp->addLayout(optionsLayoutTmp);
    mainLayoutTmp->addLayout(bottomRowLayoutTmp);

#ifdef __ANDROID__
    baseWidget_pri->setLayout(mainLayoutTmp);

#else
    this->setLayout(mainLayoutTmp);
#endif

    setWindowTitle(appConfig_ptr_ext->translate_f("Time range calculator"));

    if (appConfig_ptr_ext->configLoaded_f())
    {
        //qInfo() << "configFile_f().second" << endl;

        restoreGeometry(appConfig_ptr_ext->widgetGeometry_f(this->objectName()));

        loadTableGeometry_f(timeValuesTable_pri);
        loadTableGeometry_f(timeChangesTable_pri);
        loadTableGeometry_f(timeResultsTable_pri);
    }
}

void mainWindow_c::clearAllTables_f()
{
    timeValuesTable_pri->setRowCount(0);
    timeChangesTable_pri->setRowCount(0);
    timeResultsTable_pri->setRowCount(0);
    timeBaseValues_pri.clear();
    timeChangeValues_pri.clear();
}

void mainWindow_c::addTimeFrom_f()
{
    while (true)
    {
        if (editValuesTableRadio_pri->isChecked())
        {
            addTimeFromBase_f();
            break;
        }
        if (editChangesTableRadio_pri->isChecked())
        {
            addTimeFromChange_f();
            break;
        }
        break;
    }
}

void mainWindow_c::addTimeFromBase_f()
{
    int_fast64_t parsedInputTmp(parseInput_f());
    if (parsedInputTmp > 0 and isValidTimeValueToInsert_f(parsedInputTmp, false))
    {

        {
            QTableWidgetItem* millisecondsCellTmp(new QTableWidgetItem);
            millisecondsCellTmp->setData(Qt::DisplayRole, parsedInputTmp);
            millisecondsCellTmp->setFlags(millisecondsCellTmp->flags() bitand compl Qt::ItemIsEditable);

            QTableWidgetItem* formatedTimeCellTmp(new QTableWidgetItem);
            formatedTimeCellTmp->setData(Qt::DisplayRole, formatedTime_f(parsedInputTmp));
            formatedTimeCellTmp->setFlags(formatedTimeCellTmp->flags() bitand compl Qt::ItemIsEditable);

            auto newIndexTmp(timeValuesTable_pri->rowCount());
            timeValuesTable_pri->insertRow(newIndexTmp);
            timeValuesTable_pri->setItem(newIndexTmp, 0, millisecondsCellTmp);
            timeValuesTable_pri->setItem(newIndexTmp, 1, formatedTimeCellTmp);

            timeValuesTable_pri->setItem(newIndexTmp, 2, new QTableWidgetItem);
            timeValuesTable_pri->setItem(newIndexTmp, 3, new QTableWidgetItem);
            timeValuesTable_pri->setItem(newIndexTmp, 4, new QTableWidgetItem);
            timeValuesTable_pri->setItem(newIndexTmp, 5, new QTableWidgetItem);

            timeValuesTable_pri->selectRow(newIndexTmp);
            timeValuesTable_pri->sortItems(0, Qt::AscendingOrder);
        }

        timeBaseValues_pri.emplace(parsedInputTmp, 0);

        //add the result row
        {
            QTableWidgetItem* millisecondsCellTmp(new QTableWidgetItem);
            millisecondsCellTmp->setData(Qt::DisplayRole, parsedInputTmp);
            millisecondsCellTmp->setFlags(millisecondsCellTmp->flags() bitand compl Qt::ItemIsEditable);

            QTableWidgetItem* formatedTimeCellTmp(new QTableWidgetItem);
            formatedTimeCellTmp->setData(Qt::DisplayRole, formatedTime_f(parsedInputTmp));
            formatedTimeCellTmp->setFlags(formatedTimeCellTmp->flags() bitand compl Qt::ItemIsEditable);

            auto newIndexTmp(timeResultsTable_pri->rowCount());
            timeResultsTable_pri->insertRow(newIndexTmp);
            timeResultsTable_pri->setItem(newIndexTmp, 0, millisecondsCellTmp);
            timeResultsTable_pri->setItem(newIndexTmp, 1, formatedTimeCellTmp);
            timeResultsTable_pri->setItem(newIndexTmp, 2, new QTableWidgetItem);
            timeResultsTable_pri->setItem(newIndexTmp, 3, new QTableWidgetItem);

            timeResultsTable_pri->sortItems(0, Qt::AscendingOrder);
        }

        if (timeChangesTable_pri->rowCount() > 0)
        {
            updateResults_f();
        }
    }
}

void mainWindow_c::addTimeFromChange_f()
{
    int_fast64_t parsedInputTmp(parseInput_f());
    if (parsedInputTmp >= 0 and isValidTimeValueToInsert_f(parsedInputTmp, false))
    {
        int rowTmp(timeChangesTable_pri->rowCount());

        QTableWidgetItem* signCellTmp(new QTableWidgetItem);
        signCellTmp->setData(Qt::DisplayRole, changeValuePositiveRadio_pri->isChecked() ? "+" : "-");
        signCellTmp->setFlags(signCellTmp->flags() bitand compl Qt::ItemIsEditable);

        QTableWidgetItem* millisecondsCellTmp(new QTableWidgetItem);
        millisecondsCellTmp->setData(Qt::DisplayRole, parsedInputTmp);
        millisecondsCellTmp->setFlags(millisecondsCellTmp->flags() bitand compl Qt::ItemIsEditable);

        QTableWidgetItem* formatedTimeCellTmp(new QTableWidgetItem);
        formatedTimeCellTmp->setData(Qt::DisplayRole, formatedTime_f(parsedInputTmp));
        formatedTimeCellTmp->setFlags(formatedTimeCellTmp->flags() bitand compl Qt::ItemIsEditable);

        timeChangesTable_pri->insertRow(rowTmp);
        //0 sign +/-
        //1 From in milliseconds
        //2 From in hh:mm:ss.zzz
        //3 To range in milliseconds
        //4 To in hh:mm:ss.zzz
        //5 elapsed in milliseconds
        //6 elapsed in hh:mm:ss.zzz
        timeChangesTable_pri->setItem(rowTmp, 0, signCellTmp);
        timeChangesTable_pri->setItem(rowTmp, 1, millisecondsCellTmp);
        timeChangesTable_pri->setItem(rowTmp, 2, formatedTimeCellTmp);

        timeChangesTable_pri->setItem(rowTmp, 3, new QTableWidgetItem);
        timeChangesTable_pri->setItem(rowTmp, 4, new QTableWidgetItem);
        timeChangesTable_pri->setItem(rowTmp, 5, new QTableWidgetItem);
        timeChangesTable_pri->setItem(rowTmp, 6, new QTableWidgetItem);

        timeChangesTable_pri->selectRow(rowTmp);

        timeChangesTable_pri->sortItems(1, Qt::AscendingOrder);

        timeChangeValues_pri.emplace(parsedInputTmp, 0);

//        if (timeBaseTable_pri->rowCount() > 0)
//        {
//            updateResults_f();
//        }
    }
}

void mainWindow_c::addTimeToBase_f()
{
    while (true)
    {
        int rowTmp(-1);
        //check if a row is selected
        QList<QTableWidgetItem *> selectionTmp = timeValuesTable_pri->selectedItems();
        if (selectionTmp.isEmpty())
        {

        }
        else
        {
            std::set<int> rowIndexSetTmp;
            //get the selected row (indexes)
            for (const QTableWidgetItem* item_ite_con : selectionTmp)
            {
                rowIndexSetTmp.emplace(item_ite_con->row());
            }

            rowTmp = *rowIndexSetTmp.begin();
        }
#ifdef DEBUGJOUVEN
        qDebug() << "addRangeChange_f() rowTmp " << rowTmp;
#endif
        //if selected
        if (rowTmp not_eq -1)
        {
            //0 From in milliseconds
            //1 From in hh:mm:ss.zzz
            //2 To range in milliseconds
            //3 To in hh:mm:ss.zzz
            //4 elapsed in milliseconds
            //5 elapsed in hh:mm:ss.zzz
            int_fast64_t timeBaseValueTmp(timeValuesTable_pri->item(rowTmp, 0)->data(Qt::DisplayRole).toLongLong());
            int_fast64_t parsedInputTmp(parseInput_f());

            auto oldToValueTmp(timeBaseValues_pri[timeBaseValueTmp]);
            timeBaseValues_pri[timeBaseValueTmp] = 0;
            if (timeBaseValueTmp > parsedInputTmp
                or not isValidTimeValueToInsert_f(parsedInputTmp, true)
                or not isValidRangeToInsert_f(timeBaseValueTmp, parsedInputTmp)
                )
            {
                timeBaseValues_pri[timeBaseValueTmp] = oldToValueTmp;
                break;
            }


            auto elapsedTmp(parsedInputTmp - timeBaseValueTmp);
            timeValuesTable_pri->item(rowTmp, 2)->setData(Qt::DisplayRole, parsedInputTmp);
            timeValuesTable_pri->item(rowTmp, 3)->setData(Qt::DisplayRole, formatedTime_f(parsedInputTmp));
            timeValuesTable_pri->item(rowTmp, 4)->setData(Qt::DisplayRole, elapsedTmp);
            timeValuesTable_pri->item(rowTmp, 5)->setData(Qt::DisplayRole, formatedTime_f(elapsedTmp));

            timeValuesTable_pri->sortItems(0, Qt::AscendingOrder);

            timeResultsTable_pri->item(rowTmp, 2)->setData(Qt::DisplayRole, parsedInputTmp);
            timeResultsTable_pri->item(rowTmp, 3)->setData(Qt::DisplayRole, formatedTime_f(parsedInputTmp));

            timeResultsTable_pri->sortItems(0, Qt::AscendingOrder);

            timeBaseValues_pri[timeBaseValueTmp] = elapsedTmp;

            if (timeChangesTable_pri->rowCount() > 0)
            {
                updateResults_f();
            }
        }

        break;
    }
}


void mainWindow_c::addTimeToChange_f()
{
    while (true)
    {
        int rowTmp(-1);
        //check if a row is selected
        QList<QTableWidgetItem *> selectionTmp = timeChangesTable_pri->selectedItems();
        if (selectionTmp.isEmpty())
        {

        }
        else
        {
            std::set<int> rowIndexSetTmp;
            //get the selected row (indexes)
            for (const QTableWidgetItem* item_ite_con : selectionTmp)
            {
                rowIndexSetTmp.emplace(item_ite_con->row());
            }

            rowTmp = *rowIndexSetTmp.begin();
        }
#ifdef DEBUGJOUVEN
        qDebug() << "addRangeChange_f() rowTmp " << rowTmp;
#endif
        //if selected
        if (rowTmp not_eq -1)
        {
            //0 sign +/-
            //1 From in milliseconds
            //2 From in hh:mm:ss.zzz
            //3 To range in milliseconds
            //4 To in hh:mm:ss.zzz
            //5 elapsed in milliseconds
            //6 elapsed in hh:mm:ss.zzz
            int_fast64_t timeChangeValueTmp(timeChangesTable_pri->item(rowTmp, 1)->data(Qt::DisplayRole).toLongLong());
            int_fast64_t parsedInputTmp(parseInput_f());

            auto oldToValueTmp(timeChangeValues_pri[timeChangeValueTmp]);
            timeChangeValues_pri[timeChangeValueTmp] = 0;
            if (timeChangeValueTmp > parsedInputTmp
                or not isValidTimeValueToInsert_f(parsedInputTmp, true)
                or not isValidRangeToInsert_f(timeChangeValueTmp, parsedInputTmp)
                )
            {
                timeChangeValues_pri[timeChangeValueTmp] = oldToValueTmp;
                break;
            }

            auto elapsedTmp(parsedInputTmp - timeChangeValueTmp);
            timeChangesTable_pri->item(rowTmp, 3)->setData(Qt::DisplayRole, parsedInputTmp);
            timeChangesTable_pri->item(rowTmp, 4)->setData(Qt::DisplayRole, formatedTime_f(parsedInputTmp));
            timeChangesTable_pri->item(rowTmp, 5)->setData(Qt::DisplayRole, elapsedTmp);
            timeChangesTable_pri->item(rowTmp, 6)->setData(Qt::DisplayRole, formatedTime_f(elapsedTmp));

            timeChangesTable_pri->sortItems(0, Qt::AscendingOrder);


            bool negativeTmp(timeChangesTable_pri->item(rowTmp, 0)->data(Qt::DisplayRole).toString() == "-");
            if (negativeTmp)
            {
                elapsedTmp = elapsedTmp * -1;
            }
            timeChangeValues_pri[timeChangeValueTmp] = elapsedTmp;

            if (timeValuesTable_pri->rowCount() > 0)
            {
                updateResults_f();
            }
        }

        break;
    }
}

void mainWindow_c::addTimeTo_f()
{
    while (true)
    {
        if (editValuesTableRadio_pri->isChecked())
        {
            addTimeToBase_f();
            break;
        }
        if (editChangesTableRadio_pri->isChecked())
        {
            addTimeToChange_f();
            break;
        }
        break;
    }
}


//this function checks if a time value intersects with any from-to range
bool mainWindow_c::isValidTimeValueToInsert_f(const int_fast64_t time_par_con, const bool isToTime_par_con) const
{
    bool resultTmp(false);
    while (true)
    {
        //to times can overlap with from
        if (isToTime_par_con)
        {
        }
        else
        {
            //if it's already in
            if (timeBaseValues_pri.count(time_par_con) > 0)
            {
                break;
            }
            if (timeChangeValues_pri.count(time_par_con) > 0)
            {
                break;
            }
        }

        //check timeValue
        bool intersectsTmp(false);
        for (const auto timeValue_ite_con : timeBaseValues_pri)
        {
            if (timeValue_ite_con.second not_eq 0
                and (timeValue_ite_con.first < time_par_con and time_par_con < (timeValue_ite_con.first + timeValue_ite_con.second))
                )
            {
                intersectsTmp = true;
                break;
            }
        }
        if (intersectsTmp)
        {
            break;
        }
        //check timeChange
        for (const auto timeValue_ite_con : timeChangeValues_pri)
        {
            if (timeValue_ite_con.second not_eq 0
                and (timeValue_ite_con.first < time_par_con and time_par_con < (timeValue_ite_con.first + std::abs(timeValue_ite_con.second)))
                )
            {
                intersectsTmp = true;
                break;
            }
        }
        if (intersectsTmp)
        {
            break;
        }

        resultTmp = true;
        break;
    }
    return resultTmp;
}

//this function checks if a from-to range intersects time value
bool mainWindow_c::isValidRangeToInsert_f(
        const int_fast64_t timeFromInsert_par_con
        , const int_fast64_t timeToInsert_par_con) const
{
    bool resultTmp(false);
    while (true)
    {
        //check if intersects
        bool intersectsTmp(false);
        //timeBaseValues_pri
        for (const auto timeValue_ite_con : timeBaseValues_pri)
        {
            //check against timeBase values
            if (timeFromInsert_par_con < timeValue_ite_con.first and timeValue_ite_con.first < timeToInsert_par_con)
            {
#ifdef DEBUGJOUVEN
                qDebug() << "intersects 1"
                         << "insert from " << timeFromInsert_par_con
                         << "insert to " << timeToInsert_par_con
                         << "value " << timeValue_ite_con.first;
#endif
                intersectsTmp = true;
                break;
            }
            if (timeValue_ite_con.second > 0)
            {
                const auto toRangeTmp(timeValue_ite_con.first + timeValue_ite_con.second);
                if (timeFromInsert_par_con < toRangeTmp and toRangeTmp < timeToInsert_par_con)
                {
#ifdef DEBUGJOUVEN
                    qDebug() << "intersects 2"
                             << "insert from " << timeFromInsert_par_con
                             << "insert to " << timeToInsert_par_con
                             << "value " << timeValue_ite_con.first;
#endif
                    intersectsTmp = true;
                    break;
                }
            }
        }

        //timeChangeValues_pri
        for (const auto timeValue_ite_con : timeChangeValues_pri)
        {
            //check against timeBase values
            if (timeFromInsert_par_con < timeValue_ite_con.first and timeValue_ite_con.first < timeToInsert_par_con)
            {
#ifdef DEBUGJOUVEN
                qDebug() << "intersects 3"
                         << "insert from " << timeFromInsert_par_con
                         << "insert to " << timeToInsert_par_con
                         << "value " << timeValue_ite_con.first;
#endif
                intersectsTmp = true;
                break;
            }
            const auto toRangeTmp(timeValue_ite_con.first + std::abs(timeValue_ite_con.second));
            if (timeFromInsert_par_con < toRangeTmp and toRangeTmp < timeToInsert_par_con)
            {
#ifdef DEBUGJOUVEN
                qDebug() << "intersects 4"
                         << "insert from " << timeFromInsert_par_con
                         << "insert to " << timeToInsert_par_con
                         << "value " << timeValue_ite_con.first;
#endif
                intersectsTmp = true;
                break;
            }
        }

        if (intersectsTmp)
        {
            break;
        }

        resultTmp = true;
        break;
    }
    return resultTmp;
}

void mainWindow_c::removeSelected_f()
{
    while (true)
    {
        if (editValuesTableRadio_pri->isChecked())
        {
            removeSelectedTimeBaseTable_f();
            break;
        }
        if (editChangesTableRadio_pri->isChecked())
        {
            removeSelectedTimeChangeTable_f();
            break;
        }
        break;
    }
}

void mainWindow_c::exportToClipboardButtonPushed_f()
{
    exportToClipboard_f(nullptr);
}

QString& mainWindow_c::addSpaceOnce_f(QString& str_par)
{
    if (str_par.isEmpty()
        or str_par.back() == ' '
        or str_par.back() == '\n'
        or str_par.back() == '+')
    {

    }
    else
    {
        str_par.append(' ');
    }
    return str_par;
}

void mainWindow_c::exportToClipboard_f(QTableWidget* focusedTable_par)
{
    bool exportAllTmp(exportPresetCombo_pri->currentData().toString() == "All columns");
    bool exportMillisecondsTmp(exportPresetCombo_pri->currentData().toString() == "Milliseconds");
    bool exportTimeRangesTmp(exportPresetCombo_pri->currentData().toString() == "Time ranges");

    bool exportTableValuesTmp(tableToExportCombo_pri->currentData().toString() == "Values");
    bool exportTableChangesTmp(tableToExportCombo_pri->currentData().toString() == "Changes");
    bool exportTableResultsTmp(tableToExportCombo_pri->currentData().toString() == "Results");

    bool includeElapsedTmp(resultExportIncludeElapsed_pri->isChecked());
    bool includeSignTmp(resultExportIncludeSign_pri->isChecked());

    QString timeRangeSeparatorTmp;
    if (timeRangeSeparatorStringRadio_pri->isChecked())
    {
        timeRangeSeparatorTmp = exportTimeRangeSeparatorStringLineEdit_pri->text();
    }
    if (timeRangeSeparatorNewlineRadio_pri->isChecked())
    {
        timeRangeSeparatorTmp = "\n";
    }

    QString timeRowSeparatorTmp;
    if (timeRowSeparatorStringRadio_pri->isChecked())
    {
        timeRowSeparatorTmp = exportTimeRowSeparatorStringLineEdit_pri->text();
    }
    if (timeRowSeparatorNewlineRadio_pri->isChecked())
    {
        timeRowSeparatorTmp = "\n";
    }

    QTableWidget* tableToExportTmp;
    if (exportTableValuesTmp)
    {
        tableToExportTmp = timeValuesTable_pri;
    }
    if (exportTableChangesTmp)
    {
        tableToExportTmp = timeChangesTable_pri;
    }
    if (exportTableResultsTmp)
    {
        tableToExportTmp = timeResultsTable_pri;
    }
    if (focusedTable_par not_eq nullptr)
    {
        tableToExportTmp = focusedTable_par;
        if (focusedTable_par == timeValuesTable_pri)
        {
            exportTableValuesTmp = true;
            exportTableChangesTmp = false;
            exportTableResultsTmp = false;
        }
        if (focusedTable_par == timeChangesTable_pri)
        {
            exportTableValuesTmp = false;
            exportTableChangesTmp = true;
            exportTableResultsTmp = false;
        }
        if (focusedTable_par == timeResultsTable_pri)
        {
            exportTableValuesTmp = false;
            exportTableChangesTmp = false;
            exportTableResultsTmp = true;
        }
    }

    QList<QTableWidgetItem*> selectedItemsTmp(tableToExportTmp->selectedItems());
    if (selectedItemsTmp.size() > 0)
    {
        std::set<int> selectedRowsTmp;
        for (const QTableWidgetItem* cell_ite_con : selectedItemsTmp)
        {
            selectedRowsTmp.emplace(cell_ite_con->row());
        }
        QString textToClipBoardTmp;
        int columnOffset(0);
        if (exportTableChangesTmp)
        {
            columnOffset = 1;
        }
        for (const int row_ite_con : selectedRowsTmp)
        {
            QString rowStringTmp;
            //sign
            if (exportTableChangesTmp and includeSignTmp)
            {
                rowStringTmp
                        .append(tableToExportTmp->item(row_ite_con, 0)->text());
            }
            //from milliseconds
            if (exportMillisecondsTmp or exportAllTmp)
            {
                rowStringTmp
                        .append(tableToExportTmp->item(row_ite_con, columnOffset)->text())
                        .append(timeRangeSeparatorTmp);
            }
            //to milliseconds
            if (exportMillisecondsTmp or exportAllTmp)
            {
                rowStringTmp
                        .append(tableToExportTmp->item(row_ite_con, columnOffset + 2)->text());
            }
            //elapsed milliseconds
            if (includeElapsedTmp)
            {
                if (exportTableValuesTmp or exportTableChangesTmp)
                {
                    if (exportMillisecondsTmp or exportAllTmp)
                    {
                        addSpaceOnce_f(rowStringTmp);
                        rowStringTmp.append(tableToExportTmp->item(row_ite_con, columnOffset + 4)->text());
                    }
                }
                //elapsed values don't change, that's why there aren't any columns, because it would be the same as the timeValuesTable_pri
                if (exportTableResultsTmp)
                {
                    if (exportMillisecondsTmp or exportAllTmp)
                    {
                        addSpaceOnce_f(rowStringTmp);
                        rowStringTmp.append(timeValuesTable_pri->item(row_ite_con, columnOffset + 4)->text());
                    }
                }
            }
            //from timestamp
            if (exportTimeRangesTmp or exportAllTmp)
            {
                addSpaceOnce_f(rowStringTmp);
                rowStringTmp
                        .append(tableToExportTmp->item(row_ite_con, columnOffset + 1)->text())
                        .append(timeRangeSeparatorTmp);
            }
            //to timestamp
            if (exportTimeRangesTmp or exportAllTmp)
            {
                rowStringTmp.append(tableToExportTmp->item(row_ite_con, columnOffset + 3)->text());
            }
            if (includeElapsedTmp)
            {
                if (exportTableValuesTmp or exportTableChangesTmp)
                {
                    if (exportTimeRangesTmp or exportAllTmp)
                    {
                        addSpaceOnce_f(rowStringTmp);
                        rowStringTmp.append(tableToExportTmp->item(row_ite_con, columnOffset + 5)->text());
                    }
                }
                //elapsed values don't change, that's why there aren't any columns, because it would be the same as the timeValuesTable_pri
                if (exportTableResultsTmp)
                {
                    if (exportTimeRangesTmp or exportAllTmp)
                    {
                        addSpaceOnce_f(rowStringTmp);
                        rowStringTmp.append(timeValuesTable_pri->item(row_ite_con, columnOffset + 5)->text());
                    }
                }
            }
            if (*selectedRowsTmp.rbegin() not_eq row_ite_con)
            {
                rowStringTmp.append(timeRowSeparatorTmp);
            }
            textToClipBoardTmp.append(rowStringTmp);
        }
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText(textToClipBoardTmp);
    }
}

void timeInput_c::keyPressEvent(QKeyEvent* event_par)
{
    bool controlKeysTmp(false);
#ifdef DEBUGJOUVEN
     qDebug() << "0 event_par->key() " << event_par->key();
#endif
    if (event_par->key() == Qt::Key_A)
    {
#ifdef DEBUGJOUVEN
        qDebug() << "1 event_par->key() " << event_par->key();
#endif
        controlKeysTmp = true;
    }
    if (event_par->key() == Qt::Key_S)
    {
#ifdef DEBUGJOUVEN
        qDebug() << "3 event_par->key() " << event_par->key();
#endif
        controlKeysTmp = true;
    }
    if (event_par->key() == Qt::Key_D)
    {
#ifdef DEBUGJOUVEN
        qDebug() << "4 event_par->key() " << event_par->key();
#endif
        controlKeysTmp = true;
    }
    if (event_par->key() == Qt::Key_F)
    {
#ifdef DEBUGJOUVEN
        qDebug() << "5 event_par->key() " << event_par->key();
#endif
        controlKeysTmp = true;
    }
    if (controlKeysTmp)
    {
        event_par->ignore();
    }
    else
    {
        this->QLineEdit::keyPressEvent(event_par);
    }
}

void mainWindow_c::showAboutMessage_f()
{
    plainQMessageBox_f
    (
                appConfig_ptr_ext->translate_f(
                    "<p>Basic time arithmetic between single timestamp/time range and other time range/s, "
                    "plus some export options</p>"
                    "<p>Creaded by: Jouven<br>"
                    R"(Source: <a href="https://github.com/jouven/timeRangeCalculator">github.com/jouven/timeRangeCalculator</a><br>)"
                    R"(Homepage: <a href="https://avidcalm.com">avidcalm.com</a></p>)"
                    )
                , appConfig_ptr_ext->translate_f("About TimeRangeCalculator")
                , this
    );
}
