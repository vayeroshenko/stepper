#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <settings.h>
#include <QWidget>
#include <QGroupBox>
#include <QValidator>
#include <QtSerialPort/QSerialPort>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    settings *appSettings;
    QVector<QGroupBox*> axesGroup;

    void setSettingsTab();

    void windowSetup();



private slots:

    void on_connectButton_clicked();

    void on_xStartButton_clicked();

    void on_yStartButton_clicked();

    void on_zStartButton_clicked();

    void on_aStartButton_clicked();

    void on_xStopButton_clicked();

    void on_yStopButton_clicked();

    void on_zStopButton_clicked();

    void on_aStopButton_clicked();


    void on_enableButton_clicked();



    void on_optionBox_currentIndexChanged(const QString &arg1);

    void on_axisBox_currentIndexChanged(const QString &arg1);

    void on_setParameterButton_clicked();

    void updatePos();

private:
    Ui::MainWindow *ui;

    QIntValidator *nAxesValidator;
    QIntValidator *sprValidator;
    QDoubleValidator *cmprValidator;
    QDoubleValidator *maxvValidator;
    QDoubleValidator *accelValidator;

    QString curSetting = "";
    QString curAxis = "";
};

#endif // MAINWINDOW_H
