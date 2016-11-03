#include "mainwindow.h"
#include <QtWidgets>

#include "common.h"
#include <iostream>

OpenStatus::runTime operator+(OpenStatus::runTime lhs,OpenStatus::runTime rhs)
{
    lhs.lowTime+=rhs.lowTime;
    lhs.medianTime+=rhs.medianTime;
    lhs.highTime+=rhs.highTime;
    return lhs;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    createStatusLight();
    createConsole();
    createAll();

    setCentralWidget(All);
    setWindowTitle(tr("Console"));
    //showFullScreen();

    addTextToInfoDisplayBox(tr("主机启动成功\n"));
    readData();
    saveTimer->start(saveCycle);
}

void MainWindow::saveT(FILE* fp,OpenStatus *devices[],int num)
{
    for(int i=0;i!=num;++i)
    {
        std::fprintf(fp,"%d %d %d\n",
                    devices[i]->getRunTimeInfo().lowTime,
                    devices[i]->getRunTimeInfo().medianTime,
                    devices[i]->getRunTimeInfo().highTime
                    );
    }
}

void MainWindow::saveData()
{
    for(int i=0;i!=deviceLocalNum;++i)
    {
        for(int j=0;j!=deviceNumGroup[i];++j)
        {
            deviceGroup[i][j]->totalNow();
        }
    }

    auto fp=std::fopen(dbLocal,"w");
    if(fp==nullptr)
    {
        addTextToInfoDisplayBox(tr("日志文件写入失败\n"));
        return;
    }

    //宿舍区
    saveT(fp,studentHots,studentDeviceNum);
    saveT(fp,studentAirConditioner,studentDeviceNum);
    //别墅
    saveT(fp,villaHotWater,villaHotWaterNum);
    saveT(fp,villaWarm,villaWarmNum);
    saveT(fp,villaAirConditioner,villaAirConditionerNum);
    //小区
    saveT(fp,communityHotWater,communityHotWaterNum);
    saveT(fp,communityWarm,communityWarmNum);
    saveT(fp,communityAirConditioner,communityAirConditionerNum);

    std::fclose(fp);
}

void MainWindow::readT(FILE* fp,OpenStatus *devices[],int num)
{
    for(int i=0;i!=num;++i)
    {
        OpenStatus::runTime info;
        std::fscanf(fp,"%d %d %d\n",
                    &info.lowTime,
                    &info.medianTime,
                    &info.highTime
                    );
        devices[i]->setRunTimeInfo(info);
    }
}

void MainWindow::readData()
{
    auto fp=std::fopen(dbLocal,"r");
    if(fp==nullptr)
    {
        addTextToInfoDisplayBox(tr("日志文件读取失败\n"));
        return;
    }

    //宿舍区
    readT(fp,studentHots,studentDeviceNum);
    readT(fp,studentAirConditioner,studentDeviceNum);
    //别墅
    readT(fp,villaHotWater,villaHotWaterNum);
    readT(fp,villaWarm,villaWarmNum);
    readT(fp,villaAirConditioner,villaAirConditionerNum);
    //小区
    readT(fp,communityHotWater,communityHotWaterNum);
    readT(fp,communityWarm,communityWarmNum);
    readT(fp,communityAirConditioner,communityAirConditionerNum);

    std::fclose(fp);
}

void MainWindow::createAll()
{
    /*控制面板*/
    auto leftTemp=new QWidget;
    leftTemp->setLayout(UartSetLayout);
    leftTemp->setMaximumWidth(140);

    /*右侧监控窗口*/
    auto rightTemp=new QWidget;
    rightTemp->setLayout(statusLightLayout);

    /*左右分页*/
    All=new QSplitter;

    All->setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap("source//background.png");
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    All->setPalette(palette);

    All->addWidget(leftTemp);
    All->addWidget(rightTemp);
}

void MainWindow::createStatusLight()
{
    QString baseId;

    /*宿舍热水*/
    studentHotLayout=new QGridLayout;
    for(int i=0,j=0,k=0;i!=studentDeviceNum;++i)
    {
        studentHots[i]=
                new OpenStatus(baseId.sprintf("宿舍%d",i+1),this,OpenStatus::HotWater);
        studentHotLayout->addWidget(studentHots[i],j,k++);
        if(k==8)
        {
            k=0;
            j++;
        }
    }

    /*宿舍空调*/
    studentAirConditionerLayout=new QGridLayout;
    for(int i=0,j=0,k=0;i!=studentDeviceNum;++i)
    {
        studentAirConditioner[i]=
                new OpenStatus(baseId.sprintf("宿舍%d",i+1),this,OpenStatus::AirConditioner);
        studentAirConditionerLayout->addWidget(studentAirConditioner[i],j,k++);
        if(k==8)
        {
            k=0;
            j++;
        }
    }

    /*别墅*/
    villaLayout=new QGridLayout;
    int j=0,k=0;

    /*别墅热水*/
    for(int i=0;i!=villaHotWaterNum;++i)
    {
        villaHotWater[i]=
                new OpenStatus(baseId.sprintf("热水%d",i+1),this,OpenStatus::HotWater);
        villaLayout->addWidget(villaHotWater[i],j,k++);
        if(k==8)
        {
            k=0;
            j++;
        }
    }
    /*别墅地热*/
    k=0;
    j++;
    for(int i=0;i!=villaWarmNum;++i)
    {
        villaWarm[i]=
                new OpenStatus(baseId.sprintf("地热%d",i+1),this,OpenStatus::Warm);
        villaLayout->addWidget(villaWarm[i],j,k++);
        if(k==8)
        {
            k=0;
            j++;
        }
    }
    /*别墅空调*/
    k=0;
    j++;
    for(int i=0;i!=villaAirConditionerNum;++i)
    {
        villaAirConditioner[i]=
                new OpenStatus(baseId.sprintf("空调%d",i+1),this,OpenStatus::AirConditioner);
        villaLayout->addWidget(villaAirConditioner[i],j,k++);
        if(k==8)
        {
            k=0;
            j++;
        }
    }

    /*住户*/
    communityLayout=new QGridLayout;
    j=0,k=0;

    /*住户热水*/
    for(int i=0;i!=communityHotWaterNum;++i)
    {
        communityHotWater[i]=
                new OpenStatus(baseId.sprintf("户%d水",i+1),this,OpenStatus::HotWater);
        communityLayout->addWidget(communityHotWater[i],j,k++);
        if(k==8)
        {
            k=0;
            j++;
        }
    }
    k=0;
    j++;
    /*住户地暖*/
    for(int i=0;i!=communityWarmNum;++i)
    {
        communityWarm[i]=
                new OpenStatus(baseId.sprintf("户%d地%d",i/2+1,(i%2)+1),this,OpenStatus::Warm);
        communityLayout->addWidget(communityWarm[i],j,k++);
        if(k==8)
        {
            k=0;
            j++;
        }
    }
    k=0;
    j++;
    /*住户空调*/
    for(int i=0;i!=communityAirConditionerNum;++i)
    {
        communityAirConditioner[i]=
                new OpenStatus(baseId.sprintf("户%d空%d",i/2+1,(i%2)+1),this,OpenStatus::AirConditioner);
        communityLayout->addWidget(communityAirConditioner[i],j,k++);
        if(k==8)
        {
            k=0;
            j++;
        }
    }

    /*版面切换*/
    QScrollArea *lights[4];
    for(int i=0;i!=4;++i)
    {
        lights[i]=new QScrollArea;
        lights[i]->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }
    lights[0]->setLayout(studentHotLayout);
    lights[1]->setLayout(studentAirConditionerLayout);
    lights[2]->setLayout(villaLayout);
    lights[3]->setLayout(communityLayout);

    statusLightLayout=new QStackedLayout;
    for(int i=0;i!=4;++i)
    {
        statusLightLayout->addWidget(lights[i]);
    }
    statusLightLayout->setCurrentIndex(0);

    deviceGroup[0]=studentHots;
    deviceGroup[1]=studentAirConditioner;
    deviceGroup[2]=villaHotWater;
    deviceGroup[3]=villaWarm;
    deviceGroup[4]=villaAirConditioner;
    deviceGroup[5]=communityHotWater;
    deviceGroup[6]=communityWarm;
    deviceGroup[7]=communityAirConditioner;
}

void MainWindow::setDeviceStatus(deviceLocal local,int deviceNum,deviceStatus status)
{
    if(status==deviceStatus::close)
    {
        switch(local)
        {
            default:break;
            case deviceLocal::studentHotWaterSys:studentHots[deviceNum]->setStatus(false);break;
            case deviceLocal::studentAirConditionerSys:studentAirConditioner[deviceNum]->setStatus(false);break;
            case deviceLocal::villaHotWaterSys:villaHotWater[deviceNum]->setStatus(false);break;
            case deviceLocal::villaWarmSys:villaWarm[deviceNum]->setStatus(false);break;
            case deviceLocal::villaAirConditionerSys:villaAirConditioner[deviceNum]->setStatus(false);break;
            case deviceLocal::communityHotWaterSys:communityHotWater[deviceNum]->setStatus(false);break;
            case deviceLocal::communityWarmSys:communityWarm[deviceNum]->setStatus(false);break;
            case deviceLocal::communityAirConditionerSys:communityAirConditioner[deviceNum]->setStatus(false);break;
        }
    }
    else
    {
        switch(local)
        {
            default:break;
            case deviceLocal::studentHotWaterSys:studentHots[deviceNum]->setStatus(true);break;
            case deviceLocal::studentAirConditionerSys:studentAirConditioner[deviceNum]->setStatus(true);break;
            case deviceLocal::villaHotWaterSys:villaHotWater[deviceNum]->setStatus(true);break;
            case deviceLocal::villaWarmSys:villaWarm[deviceNum]->setStatus(true);break;
            case deviceLocal::villaAirConditionerSys:villaAirConditioner[deviceNum]->setStatus(true);break;
            case deviceLocal::communityHotWaterSys:communityHotWater[deviceNum]->setStatus(true);break;
            case deviceLocal::communityWarmSys:communityWarm[deviceNum]->setStatus(true);break;
            case deviceLocal::communityAirConditionerSys:communityAirConditioner[deviceNum]->setStatus(true);break;
        }
    }
}

void MainWindow::createConsole()
{
    comLabel=new QLabel(tr("串口选择:"));
    ComBox=new QComboBox;
    UartSwitch=new QPushButton(tr("开始监控"));
    ConsoleLabel=new QLabel(tr("查询信息:"));
    lightLayoutControl=new QComboBox;
    lightLayoutControlLabel=new QLabel(tr("界面切换:"));
    closeWindow=new QPushButton(tr("退出程序"));

    searchBox=new QLineEdit;
    searchBox->setMaxLength(4);
    QRegExp regExp("[A-Ca-c][0-1][0-9][0-9]");
    searchBox->setValidator(new QRegExpValidator(regExp,this));

    searchStart=new QPushButton(tr("开始查询"));
    searchResult=new searchDialog;
    searchResult->setModal(true);

    InfoDisplayLabel=new QLabel(tr("反馈信息:"));
    InfoDisplayBox=new QTextEdit;
    InfoDisplayBox->setReadOnly(true);

    cleanInfoDisplayBox=new QPushButton(tr("清空消息区"));

    saveTimer=new QTimer(this);

    /*自动更新可用串口*/
    updateSerialInfo=new QTimer;
    updateSerialInfo->start(1000);
    connect(updateSerialInfo,&QTimer::timeout,[=](){
        static QList<QSerialPortInfo> proList;
        QList<QSerialPortInfo> nowList;
        for(auto &SerialInfo:QSerialPortInfo::availablePorts())
        {
            nowList+=SerialInfo;
        }
        if(nowList==proList)
        {
            return;
        }
        else
        {
            updateComBox(nowList);
            addTextToInfoDisplayBox(tr("检测到串口改变\n"));
        }
        proList=nowList;
    });

    /*界面切换*/
    lightLayoutControl->addItem(tr("宿舍热水"));
    lightLayoutControl->addItem(tr("宿舍空调"));
    lightLayoutControl->addItem(tr("别墅"));
    lightLayoutControl->addItem(tr("普通住户"));
    connect(lightLayoutControl,SIGNAL(currentIndexChanged(int)),statusLightLayout,SLOT(setCurrentIndex(int)));

    /*关闭窗口*/
    connect(closeWindow,SIGNAL(clicked()),this,SLOT(close()));

    /*清空消息区*/
    connect(cleanInfoDisplayBox,SIGNAL(clicked()),InfoDisplayBox,SLOT(clear()));

    /*处理查询*/
    connect(searchStart,SIGNAL(clicked()),this,SLOT(dealSearch()));

    /*串口开关*/
    connect(UartSwitch,SIGNAL(clicked()),this,SLOT(comSwitch()));

    /*接收数据*/
    ReceiveTimer=new QTimer;
    connect(ReceiveTimer,SIGNAL(timeout()),this,SLOT(ReceiveData()));

    /*数据存档*/
    connect(saveTimer,SIGNAL(timeout()),this,SLOT(saveData()));

    UartSetLayout=new QVBoxLayout;
    UartSetLayout->addWidget(comLabel);
    UartSetLayout->addWidget(ComBox);
    UartSetLayout->addWidget(UartSwitch);
    UartSetLayout->addWidget(ConsoleLabel);
    UartSetLayout->addWidget(searchBox);
    UartSetLayout->addWidget(searchStart);
    UartSetLayout->addWidget(lightLayoutControlLabel);
    UartSetLayout->addWidget(lightLayoutControl);
    UartSetLayout->addWidget(closeWindow);

    UartSetLayout->addStretch();

    UartSetLayout->addWidget(InfoDisplayLabel);
    UartSetLayout->addWidget(InfoDisplayBox);
    UartSetLayout->addWidget(cleanInfoDisplayBox);
}

void MainWindow::comSwitch()
{
    if(fComOpen==false)
    {
        /*打开串口*/
        ser.setPortName(QString("\\\\.\\")+getComName(ComBox->currentText()));
        ser.setBaudRate(115200);
        ser.setDataBits(QSerialPort::Data8);
        ser.setParity(QSerialPort::NoParity);
        ser.setStopBits(QSerialPort::OneStop);

        if(ser.open(QIODevice::ReadWrite)==false)
        {
            addTextToInfoDisplayBox(tr("串口正在被占用"));
            return;
        }
        fComOpen=true;
        ReceiveTimer->start(ReceiveCycle);
        UartSwitch->setText(tr("结束监控"));

        ComBox->setEnabled(false);
    }
    else
    {
        UartSwitch->setText(tr("开始监控"));
        fComOpen=false;
        /*关闭串口*/
        ser.close();
        ReceiveTimer->stop();
        ComBox->setEnabled(true);
    }
}

void MainWindow::updateComBox(const QList<QSerialPortInfo> &now)
{
    /*寻找减少*/
    for(int i=0;i<ComBox->count();i++)
    {
        auto inow=now.begin();
        for(;inow!=now.end();++inow)
        {
            if(ComBox->itemText(i)==inow->description()+" "+inow->portName())
            {
                break;
            }
        }
        if(inow==now.end())
        {
            ComBox->removeItem(i);
        }
    }

    /*寻找增加*/
    for(auto inow=now.begin();inow!=now.end();++inow)
    {
        int i=0;
        for(;i<ComBox->count();i++)
        {
            if(ComBox->itemText(i)==inow->description()+" "+inow->portName())
            {
                break;
            }
        }
        if(i==ComBox->count())
        {
            ComBox->addItem(inow->description()+" "+inow->portName());
        }
    }

    /*自适应调整下拉宽度*/
    int maxLen=0;
    for(auto &i:now)
    {
        if(maxLen<(i.description()+" "+i.portName()).size())
        {
            maxLen=(i.description()+" "+i.portName()).size();
        }
    }

    ComBox->view()->setMinimumWidth(maxLen*6+10);
}

QString MainWindow::getComName(const QString &ComBoxText)
{
    QString result;

    auto pText=ComBoxText.end();
    --pText;
    for(;*pText!=' ';--pText);
    pText++;

    for(;pText!=ComBoxText.end();++pText)
    {
        result+=*pText;
    }

    return result;
}

bool operator==(const QSerialPortInfo &lhs,const QSerialPortInfo &rhs)
{
    return lhs.portName()==rhs.portName() &&
           lhs.description()==rhs.description();
}

void MainWindow::dealSearch()
{
    bool fail=false;
    QString result;
    QString temp;
    int localNum;

    static OpenStatus *devices[villaDeviceNum];
    int deviceNum=0;

    auto src=searchBox->text().toStdString().c_str();
    std::sscanf(src+1,"%d",&localNum);
    if(src[0]=='A'||src[0]=='a')//宿舍区
    {
        if(localNum==0||localNum>studentDeviceNum)
        {
            fail=true;
            goto searchEnd;
        }
        result+=tr("学生宿舍%1\n").arg(localNum);

        localNum--;
        if(studentHots[localNum]->getStatus())
        {
            switch(studentHots[localNum]->getGear())
            {
            default:break;
            case OpenStatus::low:temp.sprintf("热水设备档位:低");break;
            case OpenStatus::median:temp.sprintf("热水设备档位:中");break;
            case OpenStatus::high:temp.sprintf("热水设备档位:高");break;
            }
            temp+=tr("\n温度:%1\n").arg(studentHots[localNum]->getTemperature());
        }
        else
        {
            temp.sprintf("热水设备已关闭\n");
        }
        devices[deviceNum++]=studentHots[localNum];
        result+=temp;

        if(studentAirConditioner[localNum]->getStatus())
        {
            switch(studentAirConditioner[localNum]->getGear())
            {
            default:break;
            case OpenStatus::low:temp.sprintf("空调设备档位:低");break;
            case OpenStatus::median:temp.sprintf("空调设备档位:中");break;
            case OpenStatus::high:temp.sprintf("空调设备档位:高");break;
            }
            temp+=tr("\n温度:%1,风速:%2\n")
                    .arg(studentAirConditioner[localNum]->getTemperature())
                    .arg(studentAirConditioner[localNum]->getWind());
        }
        else
        {
            temp.sprintf("空调设备已关闭\n");
        }
        devices[deviceNum++]=studentAirConditioner[localNum];
        result+=temp;

        double money=0;

        auto RunTime=studentHots[localNum]->getRunTimeInfo();
        result+=tr("热水设备各档位运行时间:\n低:%1s\n中:%2s\n高:%3s\n")
                .arg(RunTime.lowTime)
                .arg(RunTime.medianTime)
                .arg(RunTime.highTime);
        money=money+RunTime.lowTime*Plow+RunTime.medianTime*Pmedian+RunTime.highTime*Phigh;

        RunTime=studentAirConditioner[localNum]->getRunTimeInfo();
        result+=tr("空调设备各档位运行时间:\n低:%1s\n中:%2s\n高:%3s\n")
                .arg(RunTime.lowTime)
                .arg(RunTime.medianTime)
                .arg(RunTime.highTime);
        money=money+RunTime.lowTime*Plow+RunTime.medianTime*Pmedian+RunTime.highTime*Phigh;

        result+=tr("当前费用:%1元").arg(money);
    }
    else if(src[0]=='B'||src[0]=='b')//别墅区
    {
        if(localNum==0||localNum>villaNum)
        {
            fail=true;
            goto searchEnd;
        }
        result+=tr("别墅\n");

        temp.clear();
        for(int i=0;i!=villaHotWaterNum;++i)
        {
            if(villaHotWater[i]->getStatus())
            {
                switch(villaHotWater[i]->getGear())
                {
                default:break;
                case OpenStatus::low:temp+=tr("热水设备%1档位:低").arg(i+1);break;
                case OpenStatus::median:temp+=tr("热水设备%1档位:中").arg(i+1);break;
                case OpenStatus::high:temp+=tr("热水设备%1档位:高").arg(i+1);break;
                }
                temp+=tr("\n温度:%1\n").arg(villaHotWater[i]->getTemperature());
            }
            else
            {
                temp+=tr("热水设备%1已关闭\n").arg(i+1);
            }
            devices[deviceNum++]=villaHotWater[i];
        }
        result+=temp;

        temp.clear();
        for(int i=0;i!=villaWarmNum;++i)
        {
            if(villaWarm[i]->getStatus())
            {
                switch(villaWarm[i]->getGear())
                {
                default:break;
                case OpenStatus::low:temp+=tr("地暖设备%1档位:低").arg(i+1);break;
                case OpenStatus::median:temp+=tr("地暖设备%1档位:中").arg(i+1);break;
                case OpenStatus::high:temp+=tr("地暖设备%1档位:高").arg(i+1);break;
                }
                temp+=tr("\n温度:%1\n").arg(villaWarm[i]->getTemperature());
            }
            else
            {
                temp+=tr("地暖设备%1已关闭\n").arg(i+1);
            }
            devices[deviceNum++]=villaWarm[i];
        }
        result+=temp;

        temp.clear();
        for(int i=0;i!=villaAirConditionerNum;++i)
        {
            if(villaAirConditioner[i]->getStatus())
            {
                switch(villaAirConditioner[i]->getGear())
                {
                default:break;
                case OpenStatus::low:temp+=tr("空调设备%1档位:低").arg(i+1);break;
                case OpenStatus::median:temp+=tr("空调设备%1档位:中").arg(i+1);break;
                case OpenStatus::high:temp+=tr("空调设备%1档位:高").arg(i+1);break;
                }
                temp+=tr("\n温度:%1,风速:%2\n")
                        .arg(villaAirConditioner[i]->getTemperature())
                        .arg(villaAirConditioner[i]->getWind());
            }
            else
            {
                temp+=tr("空调设备%1已关闭\n").arg(i+1);
            }
            devices[deviceNum++]=villaAirConditioner[i];
        }
        result+=temp;

        OpenStatus::runTime RunTime={0,0,0};
        double money=0;
        for(int i=0;i!=villaHotWaterNum;++i)
        {
            RunTime=RunTime+villaHotWater[i]->getRunTimeInfo();
        }
        result+=tr("热水设备各档位运行时间:\n低:%1s\n中:%2s\n高:%3s\n")
                .arg(RunTime.lowTime)
                .arg(RunTime.medianTime)
                .arg(RunTime.highTime);
        money+=RunTime.lowTime*Plow+RunTime.medianTime*Pmedian+RunTime.highTime*Phigh;

        RunTime={0,0,0};
        for(int i=0;i!=villaWarmNum;++i)
        {
            RunTime=RunTime+villaWarm[i]->getRunTimeInfo();
        }
        result+=tr("地暖设备各档位运行时间:\n低:%1s\n中:%2s\n高:%3s\n")
                .arg(RunTime.lowTime)
                .arg(RunTime.medianTime)
                .arg(RunTime.highTime);
        money+=RunTime.lowTime*Plow+RunTime.medianTime*Pmedian+RunTime.highTime*Phigh;

        RunTime={0,0,0};
        for(int i=0;i!=villaAirConditionerNum;++i)
        {
            RunTime=RunTime+villaAirConditioner[i]->getRunTimeInfo();
        }
        result+=tr("空调设备各档位运行时间:\n低:%1s\n中:%2s\n高:%3s\n")
                .arg(RunTime.lowTime)
                .arg(RunTime.medianTime)
                .arg(RunTime.highTime);
        money+=RunTime.lowTime*Plow+RunTime.medianTime*Pmedian+RunTime.highTime*Phigh;

        result+=tr("当前费用:%1元").arg(money);
    }
    else//小区
    {
        if(localNum==0||localNum>communityFamilyNum)
        {
            fail=true;
            goto searchEnd;
        }

        result+=tr("普通住户%1\n").arg(localNum);

        localNum--;
        temp.clear();
        if(communityHotWater[localNum]->getStatus())
        {
            switch(communityHotWater[localNum]->getGear())
            {
            default:break;
            case OpenStatus::low:temp+=tr("热水设备档位:低");break;
            case OpenStatus::median:temp+=tr("热水设备档位:中");break;
            case OpenStatus::high:temp+=tr("热水设备档位:高");break;
            }
            temp+=tr("\n温度:%1\n").arg(communityHotWater[localNum]->getTemperature());
        }
        else
        {
            temp+=tr("热水设备已关闭\n");
        }
        devices[deviceNum++]=communityHotWater[localNum];
        result+=temp;

        temp.clear();
        for(int i=localNum*2;i!=localNum*2+2;++i)
        {
            if(communityWarm[i]->getStatus())
            {
                switch(communityWarm[i]->getGear())
                {
                default:break;
                case OpenStatus::low:temp+=tr("地暖设备%1档位:低").arg(i-localNum*2+1);break;
                case OpenStatus::median:temp+=tr("地暖设备%1档位:中").arg(i-localNum*2+1);break;
                case OpenStatus::high:temp+=tr("地暖设备%1档位:高").arg(i-localNum*2+1);break;
                }
                temp+=tr("\n温度:%1\n").arg(communityWarm[i]->getTemperature());
            }
            else
            {
                temp+=tr("地暖设备%1已关闭\n").arg(i-localNum*2+1);
            }
            devices[deviceNum++]=communityWarm[i];
        }
        result+=temp;

        temp.clear();
        for(int i=localNum*2;i!=localNum*2+2;++i)
        {
            if(communityAirConditioner[i]->getStatus())
            {
                switch(communityAirConditioner[i]->getGear())
                {
                default:break;
                case OpenStatus::low:temp+=tr("空调设备%1档位:低").arg(i-localNum*2+1);break;
                case OpenStatus::median:temp+=tr("空调设备%1档位:中").arg(i-localNum*2+1);break;
                case OpenStatus::high:temp+=tr("空调设备%1档位:高").arg(i-localNum*2+1);break;
                }
                temp+=tr("\n温度:%1,风速:%2\n")
                        .arg(communityAirConditioner[i]->getTemperature())
                        .arg(communityAirConditioner[i]->getWind());
            }
            else
            {
                temp+=tr("空调设备%1已关闭\n").arg(i-localNum*2+1);
            }
            devices[deviceNum++]=communityAirConditioner[i];
        }
        result+=temp;

        OpenStatus::runTime RunTime={0,0,0};
        double money=0;
        RunTime=communityHotWater[localNum]->getRunTimeInfo();
        result+=tr("热水设备各档位运行时间:\n低:%1s\n中:%2s\n高:%3s\n")
                .arg(RunTime.lowTime)
                .arg(RunTime.medianTime)
                .arg(RunTime.highTime);
        money+=RunTime.lowTime*Plow+RunTime.medianTime*Pmedian+RunTime.highTime*Phigh;

        RunTime={0,0,0};
        for(int i=localNum*2;i!=localNum*2+2;++i)
        {
            RunTime=RunTime+communityWarm[i]->getRunTimeInfo();
        }
        result+=tr("地暖设备各档位运行时间:\n低:%1s\n中:%2s\n高:%3s\n")
                .arg(RunTime.lowTime)
                .arg(RunTime.medianTime)
                .arg(RunTime.highTime);
        money+=RunTime.lowTime*Plow+RunTime.medianTime*Pmedian+RunTime.highTime*Phigh;

        RunTime={0,0,0};
        for(int i=localNum*2;i!=localNum*2+2;++i)
        {
            RunTime=RunTime+communityAirConditioner[i]->getRunTimeInfo();
        }
        result+=tr("空调设备各档位运行时间:\n低:%1s\n中:%2s\n高:%3s\n")
                .arg(RunTime.lowTime)
                .arg(RunTime.medianTime)
                .arg(RunTime.highTime);
        money+=RunTime.lowTime*Plow+RunTime.medianTime*Pmedian+RunTime.highTime*Phigh;

        result+=tr("当前费用:%1元").arg(money);
    }
searchEnd:
    if(fail)
    {
        addTextToInfoDisplayBox(tr("查询失败，请检查查询内容后重试\n"));
        return;
    }
    searchResult->message(devices,deviceNum,result);
}

void MainWindow::addTextToInfoDisplayBox(const QString &text)
{
    InfoDisplayBox->moveCursor(QTextCursor::EndOfBlock);
    InfoDisplayBox->insertPlainText(text);
}

void MainWindow::dealBitMapT(unsigned char info,int deviceNum,OpenStatus *devices[])
{
    unsigned char mask=0x03;
    for(int i=6;deviceNum!=0;--deviceNum,i-=2)
    {
        switch((info&(mask<<i))>>i)
        {
        default:break;
        case 0:devices[3-i/2]->setStatus(false);break;
        case 1:devices[3-i/2]->setStatus(true,OpenStatus::low);break;
        case 2:devices[3-i/2]->setStatus(true,OpenStatus::median);break;
        case 3:devices[3-i/2]->setStatus(true,OpenStatus::high);break;
        }
    }
}

void MainWindow::dealInfo(unsigned char *info)
{
    static unsigned char oldInfo[11];

    for(int i=0;i!=11;++i)
    {
        if(oldInfo[i]^info[i])
        {
            oldInfo[i]=info[i];
            switch(i)
            {
            default:break;
            case 0:dealBitMapT(info[i],4,deviceGroup[studentHotWaterSys]);break;
            case 1:dealBitMapT(info[i],4,deviceGroup[studentAirConditionerSys]);break;

            case 2:dealBitMapT(info[i],4,deviceGroup[villaHotWaterSys]);break;
            case 3:dealBitMapT(info[i],1,deviceGroup[villaHotWaterSys]+4);break;

            case 4:dealBitMapT(info[i],4,deviceGroup[villaWarmSys]);break;
            case 5:dealBitMapT(info[i],4,deviceGroup[villaWarmSys]+4);break;

            case 6:dealBitMapT(info[i],4,deviceGroup[villaAirConditionerSys]);break;
            case 7:dealBitMapT(info[i],4,deviceGroup[villaAirConditionerSys]+4);break;

            case 8:dealBitMapT(info[i],2,deviceGroup[communityHotWaterSys]);break;
            case 9:dealBitMapT(info[i],4,deviceGroup[communityWarmSys]);break;
            case 10:dealBitMapT(info[i],4,deviceGroup[communityAirConditionerSys]);break;
            }
        }
        else
        {
            continue;
        }
    }
}

void MainWindow::ReceiveData()
{
    QByteArray message;
    if(ser.bytesAvailable()<12)
    {
        return;
    }
    else
    {
        while(ser.bytesAvailable()>=12)
        {
            message=ser.read(12);
        }
    }
    if(message[0]=='H')//数据头
    {
        auto info=message.data();
        dealInfo((unsigned char*)(info+1));
    }
    else
    {
        addTextToInfoDisplayBox(tr("收到错误数据\n"));
    }
}

MainWindow::~MainWindow()
{
    delete statusLightLayout;
}
