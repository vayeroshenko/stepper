#include <myserialport.h>


#include <QObject>
#include <QDebug>


using namespace std;

//MySerialPort::MySerialPort(bool &status);
//MySerialPort::~MySerialPort();

MySerialPort::MySerialPort(bool &status)
{
    auto serialDatalist = QSerialPortInfo::availablePorts();
    if (serialDatalist.isEmpty()){
        errorMessage = "No devices connected";
        status = false;
        return;
    }
    for (auto i: serialDatalist){
        if (this->openSerialPort(&i))
        {
            status = true;
            return;
        } else {
            status = false;
        };
    }
    if (status == false) {
        errorMessage = "Translation stage is not found";
        return;
    }
    return;
}

MySerialPort::~MySerialPort()
{
    serialPort->close();
    delete serialPort;
    return;
}

bool MySerialPort::openSerialPort(QSerialPortInfo *serialData)
{
    QSerialPort *serial = new QSerialPort(*serialData);
    setPortParameters(serial);
    const QByteArray data = "*connect_";
    this->writeData(serial, data);
    auto response = this->getMessage(serial);
    if (!response.isEmpty()) {
    if (response[0] == "#connected") {

//        cout << "have found nice port" << endl;
//        serial->close();
        serialPort = serial;
        serialPortInfo = serialData;
//        serialPort->setPortName("COM");
//        setPortParameters(serialPort);
//        cout << "have constructed the port we will work with" << endl;
//        this->closeSerialPort(serial);

        return true;
    } else {
        this->closeSerialPort(serial);
        return false;
    }
    }

    this->closeSerialPort(serial);
    return false;

}

void MySerialPort::closeSerialPort(QSerialPort *serial)
{
    if (serial->isOpen())
        serial->close();
}

void MySerialPort::writeData(QSerialPort *serial, const QByteArray &data)
{
    serial->write(data);
}

void MySerialPort::writeData(const QByteArray &data)
{
    writeData(this->serialPort, data);
}

void MySerialPort::readData(QSerialPort *serial)
{
   serial->waitForReadyRead();
   QByteArray serialData = serial->readAll();
//   messageBuffer.append(serialData);
   QStringList buffer;
   if (!serialData.isEmpty()){
       QString msgBuffer = serialData;
       buffer = msgBuffer.split(QRegExp("_"));
   }
   msgQueue.append(buffer);
   msgQueue.removeAll("\r\n");
}

QStringList MySerialPort::getMessage(QSerialPort *serial)
{
    readData(serial);
    QStringList outMessage;
    if (!msgQueue.isEmpty()){
        int slice = 0;
        bool fullmsg = false;
        for (int i = 0; i< msgQueue.length(); ++i) {
            if (msgQueue[i] == "/") {
                fullmsg = true;
                slice = i;
            }
        }

        qDebug() << "!!!!!! msgQueue from getMessage() !!!!!!!!";
        for (auto i:msgQueue)
            qDebug() << i;
        qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";

        if (!fullmsg){
            qDebug() << "MESSAGE IS NOT FULL";
            getMessage(serial);
        }
        if (slice != 0) {
            for (int i = 0; i <= slice; ++i) {
                outMessage.append(msgQueue[0]);
                msgQueue.pop_front();
            }
        }
    }
    qDebug() << "!!!!!! outMessage from getMessage() !!!!!!!!";
    for (auto i:outMessage)
        qDebug() << i;
    qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";

    return outMessage;
}

QString MySerialPort::getErrorMessage()
{
    return errorMessage;
}

void MySerialPort::setPortParameters(QSerialPort *serial)
{

//    while (!serial->isOpen()) { }
    serial->open(QIODevice::ReadWrite);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    QThread::msleep(3000);
    return;
}

bool MySerialPort::checkPort()
{
    if (serialPort == NULL)
        return false;
    if (!serialPort->isOpen())
        return false;

    QByteArray data = "*connect_/_";
    writeData(data);

    QStringList response = getMessage();
//    cout << response.toStdString() << endl;
    if (response[0] == "#connected") {
        return true;
    } else {
        return false;
    }
}

QStringList MySerialPort::getMessage()
{
    QStringList response = this->getMessage(serialPort);
    return response;
}

void MySerialPort::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        closeSerialPort(serialPort);
        serialPort = NULL;

    }
}

//#include "myserialport.moc"
