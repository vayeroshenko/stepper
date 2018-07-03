#include "settings.h"
#include <QFile>
#include <QTextStream>
#include <iostream>
#include <QDebug>


settings::settings():
    QObject()
{
    port = new MySerialPort(serialStatus);
//    if (!port->getErrorMessage().isEmpty())
//        port = NULL;

    timer.setInterval(1000);
    timer.start();
    QObject::connect(&timer, &QTimer::timeout, this, &settings::handleTimeout);


    if (serialStatus == false){
        connectionMessage = port->getErrorMessage();
    } else {
        connectionMessage = "Connected";
    }
    readConfFile();
}

settings::~settings()
{
    delete port;
}

void settings::readConfFile() {
    readConfFile(filename);
}

void settings::readConfFile(QString name)
{
    QFile file(name);
    if (!file.exists())
        generateDefaultFile();
    if (!file.open(QIODevice::ReadOnly)) {
        ioErrorMessage = "Cannot open configuration file";
    }
    QTextStream str(&file);
    while (!str.atEnd()){
        QString line = str.readLine();
        setValues(line.split(QRegExp("\\s")));
    }
    file.close();
}

void settings::saveConfFile() {
    saveConfFile(filename);
}

void settings::saveConfFile(QString name)
{
    QFile file(name);
    if (!file.open(QIODevice::WriteOnly))
        ioErrorMessage = "Cannot create configuration file";
    QTextStream str(&file);
    str << "naxes" << naxes << endl;
    str << "spr";
    for (int i = 0; i < 4; ++i) str << spr[i] << endl;
    str << "cmpr";
    for (int i = 0; i < 4; ++i) str << cmpr[i] << endl;
    str << "maxv";
    for (int i = 0; i < 4; ++i) str << maxv[i] << endl;
    str << "accel";
    for (int i = 0; i < 4; ++i) str << accel[i] << endl;
    file.close();

}

void settings::setValues(QStringList buffer)
{
    if (buffer[0] == "#") return;
    else if (buffer[0] == "naxes") naxes = buffer[1].toInt();
    else if (buffer[0] == "spr") {
        for (int i = 0; i < naxes; ++i) spr[i] = buffer[i+1].toDouble();
    } else if (buffer[0] == "cmpr") {
        for (int i = 0; i < naxes; ++i) cmpr[i] = buffer[i+1].toDouble();
    } else if (buffer[0] == "maxv") {
        for (int i = 0; i < naxes; ++i) maxv[i] = buffer[i+1].toDouble();
    } else if (buffer[0] == "accel") {
        for (int i = 0; i < naxes; ++i) accel[i] = buffer[i+1].toDouble();
    }
    fillNotUsed();
}

void settings::fillNotUsed()
{
    for (int i = naxes; i < 4; ++i){
        spr[i] = 1600;
        cmpr[i] = 1;
        maxv[i] = 1;
        accel[i] = 0.4;
    }
}

void settings::generateDefaultFile()
{
    QFile file("conf");
    if (!file.open(QIODevice::WriteOnly))
        ioErrorMessage = "Cannot create configuration file";
    QTextStream str(&file);
    str << "naxes " << naxes;
    str << endl << "spr ";
    for (int i = 0; i < 4; ++i) str << 1600 << " ";
    str << endl << "cmpr ";
    for (int i = 0; i < 4; ++i) str << 1 << " ";
    str << endl << "maxv ";
    for (int i = 0; i < 4; ++i) str << 1 << " ";
    str << endl << "accel ";
    for (int i = 0; i < 4; ++i) str << 0.4 << " ";
    file.close();
}

void settings::connect()
{
    if (!port->checkPort()) {
        port = new MySerialPort(serialStatus);
        if (serialStatus == false){
            connectionMessage = port->getErrorMessage();
        } else {
            connectionMessage = "Connected";
        }
    }
    else connectionMessage = "Connected";
}

int settings::getNAxes()
{
    return naxes;
}

void settings::monitorPort()
{
//    checkSerial();
    if (serialStatus == false)
        return;
    auto message = port->getMessage();
    if (message.isEmpty())
        return;
//    QStringList message = buffer.split(QRegExp("_"));
    QString msg = "";

//    for (auto i:message){
//        msg = i;
//        qDebug() << msg;
//    }
//    qDebug() << QString("..");

    int startI = 0;
    for (auto i:message){
        if (i == "#connected"){
            startI += 2;
            break;
        }
        startI ++;
    }

    while (startI != 0) {
        message.pop_front();
        startI --;
    }
    if (message.isEmpty()) return;

//    for (auto i:message){
//        msg = i;
//        qDebug() << msg;
//    }
//    qDebug() << QString(".......");



    if (message[0] == "#limiton") {
        if (message[1] == "left") {
            if (message[2] == "x") limit[0] = true;
            if (message[2] == "y") limit[2] = true;
            if (message[2] == "z") limit[4] = true;
            if (message[2] == "a") limit[6] = true;
        }
        if (message[1] == "right") {
            if (message[2] == "x") limit[1] = true;
            if (message[2] == "y") limit[3] = true;
            if (message[2] == "z") limit[5] = true;
            if (message[2] == "a") limit[7] = true;
        }
    }
    if (message[0] == "#limitoff") {
        if (message[1] == "left") {
            if (message[2] == "x") limit[0] = false;
            if (message[2] == "y") limit[2] = false;
            if (message[2] == "z") limit[4] = false;
            if (message[2] == "a") limit[6] = false;
        }
        if (message[1] == "right") {
            if (message[2] == "x") limit[1] = false;
            if (message[2] == "y") limit[3] = false;
            if (message[2] == "z") limit[5] = false;
            if (message[2] == "a") limit[7] = false;
        }
    }

    if (message[0] == "\r\n#currpos") {
        if (message[1] == "x") pos[0] = message[2].toDouble();
        if (message[1] == "y") pos[1] = message[2].toDouble();
        if (message[1] == "z") pos[2] = message[2].toDouble();
        if (message[1] == "a") pos[3] = message[2].toDouble();
    }

    if (message[0] == "#started") {
        if (message[1] == "x") ismoving[0] = true;
        if (message[1] == "y") ismoving[1] = true;
        if (message[1] == "z") ismoving[2] = true;
        if (message[1] == "a") ismoving[3] = true;
    }

    if (message[0] == "#stopped") {
        if (message[1] == "x") ismoving[0] = false;
        if (message[1] == "y") ismoving[1] = false;
        if (message[1] == "z") ismoving[2] = false;
        if (message[1] == "a") ismoving[3] = false;
    }
    emit newData();
}

void settings::goTo(short axis, double position)
{
    if (!serialStatus)
        return;
    double nsteps = position/cmpr[axis]*spr[axis];
//    QString msg = "*goto_";
    QString msg = "";
//    if (axis == 0) msg.append("x_");
//    if (axis == 1) msg.append("y_");
//    if (axis == 2) msg.append("z_");
//    if (axis == 3) msg.append("a_");
    if (axis == 0) msg.append("*xmove_");
    if (axis == 1) msg.append("*ymove_");
    if (axis == 2) msg.append("*zmove_");
    if (axis == 3) msg.append("*amove_");
    msg.append(QString::number(nsteps,'i',0));
    msg.append("_/_");
    timer.start();
    port->writeData(msg.toLocal8Bit());
    return;
}

void settings::setPos(short axis, double position)
{
    if (!serialStatus)
        return;
    double nsteps = position/cmpr[axis]*spr[axis];
    QString msg = "";
    if (axis == 0) msg.append("*xcurpos_");
    if (axis == 1) msg.append("*ycurpos_");
    if (axis == 2) msg.append("*zcurpos_");
    if (axis == 3) msg.append("*acurpos_");
    msg.append(QString::number(nsteps,'i',0));
    msg.append("_/_");
    timer.start();
    port->writeData(msg.toLocal8Bit());
    return;
}

void settings::setup()
{
    QString msg = "";
    msg += "*maxv_";
    for (auto i: maxv) msg += QString::number(i) + "_";
    msg += "*acc_";
    for (auto i: accel) msg += QString::number(i) + "_";
    port->writeData(msg.toLocal8Bit());
}

void settings::stop(short axis)
{
    if (!serialStatus)
        return;
    QString msg = "";
    if (axis == 0) msg.append("*xstop_/_");
    if (axis == 1) msg.append("*ystop_/_");
    if (axis == 2) msg.append("*zstop_/_");
    if (axis == 3) msg.append("*astop_/_");
    port->writeData(msg.toLocal8Bit());
    return;
}

void settings::enableMotors()
{
    if (!serialStatus)
        return;
    port->writeData(QString("*en_1_/_").toLocal8Bit());
    motorEnabled = true;
}

void settings::disableMotors()
{
    if (!serialStatus)
        return;
    port->writeData(QString("*en_0_/_").toLocal8Bit());
    motorEnabled = false;
}

void settings::checkSerial()
{
    serialStatus = port->checkPort();
}

void settings::handleTimeout()
{

    if (!port->checkPort()){
        timer.start();
        return;
    }
    monitorPort();
}


