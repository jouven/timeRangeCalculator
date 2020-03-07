#ifndef TIMERANGECALCULATOR_MAINWINDOW_HPP
#define TIMERANGECALCULATOR_MAINWINDOW_HPP

#include <QWidget>
#include <QLineEdit>

#include <map>

//TODO-FUTURE allow to export to mpv EDL forcing only one row in timeValuesTable_pri
//if I ever need it, not right now

class QTableWidget;
class QPushButton;
class QLabel;
class QMessageBox;
class QDateTimeEdit;
class QRadioButton;
class QVBoxLayout;
class QComboBox;
class QCheckBox;
class QLineEdit;

class timeInput_c : public QLineEdit
{
    void keyPressEvent(QKeyEvent* event_par) override;
};

class mainWindow_c : public QWidget
{
    Q_OBJECT

    //possible formats by priority:
    //1 a number (will be considered as milliseconds, i.e. 500)
    //2 hh:mm:ss.zzz (time formats, i.e. 23:59:59.999, which is also the maximum value using this format, see parseInput_f)
    //3 value+letter separated by one or more spaces, i.e. 50s 50d 50m = 50 years + 50 days + 50 minutes
    //order doesn't matter, there can be duplicates, values without a letter are ignored,
    // ":", "/", "-" are replaced by spaces, in the end everything is converted to milliseconds
    //possible letters:
    //z = millisecond
    //s = second
    //m = minute
    //h = hour
    //d = day
    //months and years are ignored because variable values
    //time adjusments like leap seconds are not factored
    timeInput_c* timeInputLineEdit_pri;

    QRadioButton* editValuesTableRadio_pri;
    QRadioButton* editChangesTableRadio_pri;
    QRadioButton* changeValuePositiveRadio_pri;
    QRadioButton* changeValueNegativeRadio_pri;

    QComboBox* tableToExportCombo_pri;
    QComboBox* exportPresetCombo_pri;

    QRadioButton* timeRangeSeparatorStringRadio_pri;
    QRadioButton* timeRangeSeparatorNewlineRadio_pri;
    QRadioButton* timeRowSeparatorStringRadio_pri;
    QRadioButton* timeRowSeparatorNewlineRadio_pri;

    QCheckBox* resultExportIncludeElapsed_pri;
    QCheckBox* resultExportIncludeSign_pri;

    QLineEdit* exportTimeRangeSeparatorStringLineEdit_pri;
    QLineEdit* exportTimeRowSeparatorStringLineEdit_pri;

    QTableWidget* timeValuesTable_pri;
    QTableWidget* timeChangesTable_pri;
    QTableWidget* timeResultsTable_pri;

    //for both maps: key is time "From", value is time elapsed and From+elapsed=To
    std::map<int_fast64_t, int_fast64_t> timeBaseValues_pri;
    std::map<int_fast64_t, int_fast64_t> timeChangeValues_pri;

    void createTableEditControlsWidgets_f(QVBoxLayout* layout_par);
    void createTableExportOptionsWidgets_f(QVBoxLayout* layout_par);

    void closeEvent(QCloseEvent* event) override;

    void saveTableGeometry_f(QTableWidget* table_par) const;
    void loadTableGeometry_f(QTableWidget* table_par);

    void updateResults_f();
    int_fast64_t parseInput_f() const;
    QString formatedTime_f(const int_fast64_t milliseconds_par_con) const;

    //this function checks if a time value intersects with any from-to range
    //isToTime = false, time_par_con is "from", isToTime = true,s time_par_con is "from"
    bool isValidTimeValueToInsert_f(const int_fast64_t time_par_con, const bool isToTime_par_con) const;
    //this function checks if a from-to range intersects time value
    bool isValidRangeToInsert_f(const int_fast64_t timeFromInsert_par_con, const int_fast64_t timeToInsert_par_con) const;
    int getRowFromTimeBase_f(const int_fast64_t time_par_con) const;

    void addTimeFromBase_f();
    void addTimeFromChange_f();

    void addTimeToBase_f();
    void addTimeToChange_f();

    void removeSelectedTimeBaseTable_f();
    void removeSelectedTimeChangeTable_f();

    void keyPressEvent(QKeyEvent* event_par) override;

    void exportToClipboard_f(QTableWidget* focusedTable_par = nullptr);
    QString& addSpaceOnce_f(QString& str_par);
public:
    explicit mainWindow_c();
private Q_SLOTS:
    void clearAllTables_f();
    void addTimeFrom_f();
    void addTimeTo_f();
    void removeSelected_f();
    void exportToClipboardButtonPushed_f();
    void showAboutMessage_f();
};
#endif // TIMERANGECALCULATOR_MAINWINDOW_HPP
