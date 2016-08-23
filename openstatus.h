#ifndef OPENSTATUS_H
#define OPENSTATUS_H

#include <QWidget>
#include <QDateTime>

class OpenStatus : public QWidget
{
    Q_OBJECT
    static const int cycleSize=15;
    static const int widgetSize=50;
    static const int labelSize=10;
public:
    enum deviceType
    {
        HotWater=0,
        AirConditioner,
        Warm
    };

    enum gear
    {
        low=0,
        median,
        high
    };

    struct runTime
    {
        int lowTime;
        int medianTime;
        int highTime;
    };

    OpenStatus(const QString &labelName, QWidget *parent = nullptr,deviceType type=HotWater);

    qint8 getTemperature() const {return status?temperature:-1;}
    qint8 getWind() const {return status?wind:-1;}
    gear getGear() const {return status?gearNow:low;}
    runTime getRunTimeInfo() const {return Runtime;}
    void setRunTimeInfo(runTime &rumTimeInfo) {Runtime=rumTimeInfo;}
    bool getStatus() const {return status;}

signals:

public slots:
    void setStatus(bool now,qint8 t=0,qint8 w=0,gear gearSet=gear::low);
    void totalNow();

private:
    void paintEvent(QPaintEvent *event);
    void totalTime();
    QString lightName="设备";
    bool status=false;

    /*设备相关*/
    deviceType thisType;
    gear gearNow=gear::low;
    qint8 temperature=0;
    qint8 wind=0;

    uint startTime;
    runTime Runtime={0,0,0};
};

#endif // OPENSTATUS_H
