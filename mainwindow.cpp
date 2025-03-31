#include "mainwindow.h"
#include "ui_mainwindow.h"

void pageChanged(Ui::MainWindow* ui, int newYear, int newMonth) {
    int month = newMonth + 1;
    int year = newYear;
    if (month > 12) {
        month = 1;
        year++;
    }
    ui->calNextMonth->setCurrentPage(year, month);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Set the calendar to the current month
    QDate currentDate = QDate::currentDate();
    ui->calThisMonth->setCurrentPage(currentDate.year(), currentDate.month());
    pageChanged(ui, currentDate.year(), currentDate.month());
    // Call pageChanged upon signal received
    connect(ui->calThisMonth, &QCalendarWidget::currentPageChanged, this, [this](int newYear, int newMonth) {
        pageChanged(ui, newYear, newMonth);
    });
    connect(ui->calThisMonth, &QCalendarWidget::selectionChanged, this, [this]() {
        ui->calNextMonth->setSelectedDate(ui->calThisMonth->selectedDate());
        pageChanged(ui, ui->calThisMonth->selectedDate().year(), ui->calThisMonth->selectedDate().month());
    });
    connect(ui->calNextMonth, &QCalendarWidget::selectionChanged, this, [this]() {
        ui->calThisMonth->setSelectedDate(ui->calNextMonth->selectedDate());
        pageChanged(ui, ui->calNextMonth->selectedDate().year(), ui->calNextMonth->selectedDate().month());
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}
