#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QLineEdit>
#include "tcpserver.h"
#include "openstatus.h"
#include "searchDialog.h"

class QErrorMessage;
class QStackedLayout;
class QSplitter;
class QPushButton;
class QComboBox;
class QTextEdit;
class QGridLayout;
class QVBoxLayout;
class QTimer;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    static const int studentDeviceNum=120;

    static const int villaNum=1;
    static const int villaHotWaterNum=5*villaNum;
    static const int villaWarmNum=8*villaNum;
    static const int villaAirConditionerNum=8*villaNum;
    static const int villaDeviceNum=villaHotWaterNum+
                                    villaWarmNum+
                                    villaAirConditionerNum;

    static const int communityFamilyNum=14;
    static const int communityHotWaterNum=1*communityFamilyNum;
    static const int communityWarmNum=2*communityFamilyNum;
    static const int communityAirConditionerNum=2*communityFamilyNum;
    static const int communityDeviceNum=communityHotWaterNum+
                                        communityWarmNum+
                                        communityAirConditionerNum;

    const int deviceNumGroup[8]={
        studentDeviceNum,studentDeviceNum,
        villaHotWaterNum,villaWarmNum,villaAirConditionerNum,
        communityHotWaterNum,communityWarmNum,communityAirConditionerNum
    };

    /*档位费用倍率（x元/秒）*/
    const double Plow=0.1;
    const double Pmedian=0.1;
    const double Phigh=0.1;

    static const int saveCycle=1*60*1000;//30分钟存档一次

    const char *dbLocal="lastHour.log";

    enum deviceStatus
    {
        close,
        open
    };

    enum deviceLocal
    {
        studentHotWaterSys=0,
        studentAirConditionerSys,
        villaHotWaterSys,
        villaWarmSys,
        villaAirConditionerSys,
        communityHotWaterSys,
        communityWarmSys,
        communityAirConditionerSys,
        deviceLocalNum
    };

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    deviceStatusChange(int,deviceStatus);
public slots:
    void setDeviceStatus(deviceLocal local, int deviceNum, deviceStatus status);
    void ReceiveData(const std::string &src);
    void addTextToInfoDisplayBox(const QString &text);
    void dealSearch();
    void saveData();
    void readData();
private:
    QSplitter *All;
    void createAll();

    void createStatusLight();
    QStackedLayout *statusLightLayout;

    QGridLayout *studentHotLayout;
    OpenStatus *studentHots[studentDeviceNum];

    QGridLayout *studentAirConditionerLayout;
    OpenStatus *studentAirConditioner[studentDeviceNum];

    QGridLayout *villaLayout;
    OpenStatus *villaHotWater[villaHotWaterNum];
    OpenStatus *villaWarm[villaWarmNum];
    OpenStatus *villaAirConditioner[villaAirConditionerNum];

    QGridLayout *communityLayout;
    OpenStatus *communityHotWater[communityHotWaterNum];
    OpenStatus *communityWarm[communityWarmNum];
    OpenStatus *communityAirConditioner[communityAirConditionerNum];

    OpenStatus **deviceGroup[deviceLocalNum];

    void dealBitmap(deviceLocal deviceLocalName, unsigned char *message);
    void dealOpenRequest(unsigned int deviceLocalName, unsigned char *message);
    void saveT(FILE* fp,OpenStatus *devices[],int num);
    void readT(FILE* fp,OpenStatus *devices[],int num);

    void createConsole();
    QLabel *ConsoleLabel;
    QLineEdit *searchBox;
    QPushButton *searchStart;
    QLabel *lightLayoutControlLabel;
    QComboBox *lightLayoutControl;
    QPushButton *closeWindow;

    QTimer *saveTimer;

    QLabel *InfoDisplayLabel;
    QTextEdit *InfoDisplayBox;
    QPushButton *cleanInfoDisplayBox;
    QVBoxLayout *UartSetLayout;

    tcpServer *server;
    searchDialog *searchResult;
};

#endif // MAINWINDOW_H
