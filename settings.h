#ifndef SETTINGS_H
#define SETTINGS_H

#include <myserialport.h>
#include <QTimer>
#include <QObject>
#include <QDebug>

class settings : public QObject
{

    Q_OBJECT

public:
    settings();
    ~settings();

    // port communication attributes
    MySerialPort *port = NULL;
    bool serialStatus = false;
    QString connectionMessage;
    QString ioErrorMessage;
    bool motorEnabled = false;

    // public methods
    void connect();
    int getNAxes();

    void monitorPort();
    void goTo(short axis, double position);
    void setPos(short axis, double position);
    void stop(short axis);
    void setup();
    void enableMotors();
    void disableMotors();
    void checkSerial();




    QTimer timer;
    QTimer timerSec;

    int naxes = 4;              // number of active axes


    // [X,Y,Z,A] axes
    double spr[4];               // steps per rotation
    double cmpr[4];              // step of moving screw (cm/rotation)
    double maxv[4];              // max velocity
    double accel[4];             // acceleration

    bool limit[8] = {false};     // if limit is fired
    bool ismoving[4];            // if stages is moving along axis
    double pos[4];               // current possition

public slots:
    void handleTimeout();

signals:
    void newData();

private:
    QString filename = "conf";  // name of configuration file






    // service methods
    void setValues(QStringList buffer);
    void readConfFile(QString name);
    void saveConfFile(QString name);
    void readConfFile();
    void saveConfFile();
    void fillNotUsed();
    void generateDefaultFile();





};

#endif // SETTINGS_H
