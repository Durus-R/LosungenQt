#include <QRegularExpression>
#include <QScrollBar>
#include <QScrollEvent>
#include <QDesktopServices>
#include "mainwindow.h"
#include "csv_parser.h"
#include "ui_mainwindow.h"

void pageChanged(Ui::MainWindow *ui, int newYear, int newMonth)
{
    int month = newMonth + 1;
    int year = newYear;
    if (month > 12) {
        month = 1;
        year++;
    }
    ui->calNextMonth->setCurrentPage(year, month);
}

bool bothChecked(QAction *action1, QAction *action2)
{
    return action1->isChecked() && action2->isChecked();
}

bool bothUnchecked(QAction *action1, QAction *action2)
{
    return !action1->isChecked() && !action2->isChecked();
}
QDate daysUntilSunday(QDate date)
{
    int daysToSunday = (7 - date.dayOfWeek()) % 7;
    return date.addDays(daysToSunday);
}
QRegularExpression& i_regex(){

    static QRegularExpression regex(R"(/([^/]+)/)");
    return regex;
}
QRegularExpression& b_regex(){
    static QRegularExpression regex(R"(\*([^*]+)\*)");
    return regex;
}
QString enrich(QString input) {
    // Replace all /text/ with <i>text</i> and *text* with <b>text</b>
    qDebug() << input;
    input.replace(i_regex(), "<i>\\1</i>");
    input.replace(b_regex(), "<b>\\1</b>");
    qDebug() << input;
    return input;
}

void updateTexts(Ui::MainWindow *ui, const csv_parser &parser) {
    if (bothUnchecked(ui->actionVers, ui->actionText)) {
        ui->actionVers->setChecked(true);
        ui->actionText->setChecked(true);
    }
     QDate selectedDate = ui->calThisMonth->selectedDate();
        const QStringList dateData = parser.get_date(selectedDate);
        QString losungsvers = "";
        if (ui->actionVers->isChecked()) {
            losungsvers = dateData[3];
        }
        if (bothChecked(ui->actionVers, ui->actionText)) {
            losungsvers += ": ";
        }
        if (ui->actionText->isChecked()) {
            losungsvers += dateData[4];
        }
        QString lehrvers = "";
        if (ui->actionVers->isChecked()) {
            lehrvers = dateData[5];
        }
        if (bothChecked(ui->actionVers, ui->actionText)) {
            lehrvers += ": ";
        }
        if (ui->actionText->isChecked()) {
            lehrvers += dateData[6];
        }
        if (dateData.size() > 0) {
            ui->txtLehrvers->setText(enrich(lehrvers));
            ui->txtLosungsvers->setText(enrich(losungsvers));
        } else {
            ui->txtLehrvers->clear();
            ui->txtLosungsvers->clear();
        }
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
    if (obj == this->ui->centralwidget && event->type() == QEvent::Wheel){
        return true;
    }
    return false;
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->centralwidget->installEventFilter(this);

    setWindowIcon(QIcon(":/icon.png"));

    // Implement calendars
    QDate currentDate = QDate::currentDate();
    ui->calThisMonth->setCurrentPage(currentDate.year(), currentDate.month());
    pageChanged(ui, currentDate.year(), currentDate.month());
    connect(ui->calThisMonth, &QCalendarWidget::currentPageChanged, this, [this](int newYear, int newMonth) {
        pageChanged(ui, newYear, newMonth);
    });

    connect(ui->calThisMonth, &QCalendarWidget::selectionChanged, this, [this]() {
        ui->calNextMonth->setSelectedDate(ui->calThisMonth->selectedDate());
        pageChanged(ui,
                    ui->calThisMonth->selectedDate().year(),
                    ui->calThisMonth->selectedDate().month());
    });
    connect(ui->calNextMonth, &QCalendarWidget::selectionChanged, this, [this]() {
        ui->calThisMonth->setSelectedDate(ui->calNextMonth->selectedDate());
        pageChanged(ui,
                    ui->calNextMonth->selectedDate().year(),
                    ui->calNextMonth->selectedDate().month());
    });

    // Implement actionButtons for date
    connect(ui->actionMorgen, &QAction::triggered, this, [this]() {
        ui->calThisMonth->setSelectedDate(QDate::currentDate().addDays(1));
    });
    connect(ui->actionSonntag, &QAction::triggered, this, [this]() {
        ui->calThisMonth->setSelectedDate(daysUntilSunday(ui->calThisMonth->selectedDate()));
    });

    // Implement parser
    const csv_parser parser(QFile(":/Losungen_2025.csv"));
    updateTexts(ui, parser);
    connect(ui->calThisMonth, &QCalendarWidget::selectionChanged, this, [this, parser]() {
       updateTexts(ui, parser);
    });
    connect(ui->actionText, &QAction::toggled, this, [this, parser]() {
       updateTexts(ui, parser);
    });
    connect(ui->actionVers, &QAction::toggled, this, [this, parser]() {
       updateTexts(ui, parser);
    });

    // Implement copy buttons
    connect(ui->btnCopyLehrvers, &QPushButton::clicked, this, [this]() {
        ui->txtLehrvers->selectAll();
        ui->txtLehrvers->copy();
    });
    connect(ui->btnCopyLosungsvers, &QPushButton::clicked, this, [this]() {
        ui->txtLosungsvers->selectAll();
        ui->txtLosungsvers->copy();
    });

    // Implement other action buttons
    connect(ui->actionNutzungsbedingungen, &QAction::triggered, this, [this]() {
        QDesktopServices::openUrl(QUrl("https://www.losungen.de/digital/nutzungsbedingungen/"));
    });
    connect(ui->actionInfo, &QAction::triggered, this, [this]() {
        QDesktopServices::openUrl(QUrl("https://www.losungen.de"));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
