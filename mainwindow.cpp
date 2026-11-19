#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , backend{}
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    ui->pushButton->setText(backend.receive("*IDN?\n").value_or("Error"));
    // backend.send(  "*RST\n"                      // Reset urządzenia
    //              "VOLT 0\n"                    // Bezpieczny poziom bazowy 0V
    //              "CURR 0\n"                  // Zabezpieczenie prądowe 0.1A (dostosuj do obciążenia)
    //              "VOLT:MODE LIST\n"            // Tryb sekwencyjny (LIST) dla napięcia
    //              "LIST:VOLT 5.0, 0.0\n"        // Parametry: impuls 5V, przerwa 0V
    //              "LIST:DWEL 0.5, 0.5\n"        // Czas trwania: impuls 0.5s, przerwa 0.5s
    //              "LIST:STEP AUTO\n"            // Przechodzenie do kolejnego kroku automatycznie
    //              "LIST:COUN 10\n"              // Całość wykonaj 10 razy
    //              "TRIG:TRAN:SOUR IMM\n"        // Natychmiastowe wyzwalanie układu Transient
    //              "OUTP ON\n"                   // Aktywacja wyjścia
    //              "INIT:TRAN\n"
    //              );
}


void MainWindow::on_checkBox_wlaczone_wymuszenie_checkStateChanged(const Qt::CheckState &arg1)
{
    if(arg1 == Qt::CheckState::Checked)
    {


    backend.send(QString(
                         "*RST\n"                      // Reset urządzenia
                         "DISP:WIND:TEXT:DATA \"Dzieje sie!\"\n"
                 "VOLT %1\n"
                 "CURR 0.1\n"
                 "INIT:CONT ON\n"
            "TRIG:SOUR IMM\n"
                 "OUTP ON\n"
                )
                     .arg(static_cast<double>(ui->spinBox_3amplitudanapiecia->value()) / 1000.0)
                     .arg(static_cast<double>(ui->spinBox_dlugosc_imp->value()) / 1000.0)
                     .arg(static_cast<double>(ui->spinBox_interwal->value()) / 1000.0)
                     .arg(1000)
                 );
        qDebug() << "PARAMS:";
        qDebug() << static_cast<double>(ui->spinBox_3amplitudanapiecia->value()) / 1000.0;
        qDebug() << static_cast<double>(ui->spinBox_dlugosc_imp->value()) / 1000.0;
        qDebug() << static_cast<double>(ui->spinBox_interwal->value()) / 1000.0;
    } else {

    backend.send("*RST\n"           // Natychmiastowe wyzwalanie układu Transient
                     "ABOR\n"
                     "DISP:WIND:TEXT:DATA \"OFF :(\"\n"
                 "OUTP OFF\n"
                 );
    }
}

