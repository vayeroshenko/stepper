#ifndef MYSERIALPORT_H
#define MYSERIALPORT_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QObject>
#include <QThread>


#include <iostream>

class MySerialPort : public QObject
{

    Q_OBJECT

public:

    MySerialPort(bool &status);
    ~MySerialPort();


    void writeData(const QByteArray &data);
    QStringList getMessage();
    QString getErrorMessage();
    bool checkPort();


    QSerialPort *serialPort = NULL;
    bool *statusStatus;



private:
    QString errorMessage = "";
//    QByteArray messageBuffer = "";
    QStringList msgQueue;

    QSerialPortInfo *serialPortInfo = NULL;

    bool openSerialPort(QSerialPortInfo *serialData);
    void closeSerialPort(QSerialPort *serial);
    void writeData(QSerialPort *serial, const QByteArray &data);
    void readData(QSerialPort *serial);
    QStringList getMessage(QSerialPort *serial);
    void setPortParameters(QSerialPort *serial);



public slots:
    void handleError(QSerialPort::SerialPortError error);

};


#endif // MYSERIALPORT_H
