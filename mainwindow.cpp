#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <settings.h>

#include <iostream>
#include <QTimer>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    appSettings = new settings();

    ui->setupUi(this);
    ui->statusLine->setText(appSettings->connectionMessage);

    axesGroup.append(ui->xAxisGroup);
    axesGroup.append(ui->yAxisGroup);
    axesGroup.append(ui->zAxisGroup);
    axesGroup.append(ui->aAxisGroup);

    int nAxes = appSettings->getNAxes();

    if (nAxes > 2) this->setMinimumSize(860,585);
    else this->setMinimumSize(860,360);

    for (int i = 3; i >= nAxes; --i)
        axesGroup[i]->hide();

    if (appSettings->port->serialPort != NULL){
        connect(appSettings->port->serialPort, SIGNAL(error(QSerialPort::SerialPortError)), appSettings->port, SLOT(handleError(QSerialPort::SerialPortError)));
    }

//    // run in main loop
//    auto timer = new QTimer(parent);
//    connect(timer, &QTimer::timeout, appSettings, [this]{appSettings->monitorPort();});
//    timer->start(10000);
//    //

    if (appSettings->motorEnabled)
        ui->enableButton->setText("Disable motors");
     else
        ui->enableButton->setText("Enable motors");

    setSettingsTab();

    resize(0,0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setSettingsTab()
{
    ui->optionBox->addItem("Select option...","");
    ui->optionBox->addItem("Number of axes","nAxes");
    ui->optionBox->addItem("Steps per rotation","spr");
    ui->optionBox->addItem("Cm per rotation","cmpr");
    ui->optionBox->addItem("Maximum velocity","maxv");
    ui->optionBox->addItem("Acceleration","accel");

    ui->axisBox->addItem("Select axis...","");
    ui->axisBox->addItem("First axis","axis1");
    ui->axisBox->addItem("Second axis","axis2");
    ui->axisBox->addItem("Third axis","axis3");
    ui->axisBox->addItem("Fourth axis","axis4");

    ui->settingsUnitsnitsLabel->setVisible(false);
}


void MainWindow::on_connectButton_clicked()
{
    appSettings->connect();
    ui->statusLine->setText(appSettings->connectionMessage);
}

void MainWindow::on_xStartButton_clicked()
{
    if (ui->xDestinationLine->text().isEmpty())
        return;
    appSettings->goTo(0,ui->xDestinationLine->text().toDouble());
}

void MainWindow::on_yStartButton_clicked()
{
    if (ui->xDestinationLine->text().isEmpty())
        return;
    appSettings->goTo(1,ui->yDestinationLine->text().toDouble());
}

void MainWindow::on_zStartButton_clicked()
{
    if (ui->xDestinationLine->text().isEmpty())
        return;
    appSettings->goTo(2,ui->zDestinationLine->text().toDouble());
}

void MainWindow::on_aStartButton_clicked()
{
    if (ui->xDestinationLine->text().isEmpty())
        return;
    appSettings->goTo(3,ui->aDestinationLine->text().toDouble());
}


void MainWindow::on_xStopButton_clicked()
{
    appSettings->stop(0);
}

void MainWindow::on_yStopButton_clicked()
{
    appSettings->stop(1);
}

void MainWindow::on_zStopButton_clicked()
{
    appSettings->stop(2);
}

void MainWindow::on_aStopButton_clicked()
{
    appSettings->stop(3);
}



void MainWindow::on_enableButton_clicked()
{
    if (!appSettings->serialStatus)
        return;
    if (appSettings->motorEnabled){
        appSettings->disableMotors();
        ui->enableButton->setText("Enable motors");
    }
     else {
        appSettings->enableMotors();
        ui->enableButton->setText("Disable motors");
    }
}

void MainWindow::on_optionBox_currentIndexChanged(const QString &arg1)
{
    this->curSetting = arg1;
    if (curSetting == "Number of axes") {
        ui->axisBox->setVisible(false);
    } else if (curSetting == "Steps per rotation") {
        ui->axisBox->setVisible(true);
    }
}

void MainWindow::on_axisBox_currentIndexChanged(const QString &arg1)
{
    this->curAxis = arg1;


}

void MainWindow::on_setParameterButton_clicked()
{
    if (curSetting == "" || curSetting == "Select option...") return;
    else if (curSetting == "Number of axes") appSettings->
}
