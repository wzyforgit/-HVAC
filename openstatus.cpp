#include "openstatus.h"
#include <QPainter>

OpenStatus::OpenStatus(const QString &labelName, QWidget *parent, deviceType type): QWidget(parent)
{
    setFixedSize(widgetSize,widgetSize);
    lightName=labelName;
    thisType=type;
}

void OpenStatus::totalTime()
{
    switch(gearNow)//费用统计
    {
    default:break;
    case low:Runtime.lowTime+=(QDateTime::currentDateTime().toTime_t()-startTime);break;
    case median:Runtime.medianTime+=(QDateTime::currentDateTime().toTime_t()-startTime);break;
    case high:Runtime.highTime+=(QDateTime::currentDateTime().toTime_t()-startTime);break;
    }
}

void OpenStatus::totalNow()
{
    if(status==true)
    {
        totalTime();
    }
    startTime=QDateTime::currentDateTime().toTime_t();
}

void OpenStatus::setStatus(bool now, qint8 t, qint8 w, gear gearSet)
{
    if((status==now&&gearNow==gearSet)||(status==false&&now==false))//状态相同
    {
        return;
    }
    else if(status==false&&now==true)//设备开启
    {
        startTime=QDateTime::currentDateTime().toTime_t();
        gearNow=gearSet;
        wind=w;
        temperature=t;
        status=now;
    }
    else if(status==true&&now==true&&gearNow!=gearSet)//状态切换
    {
        totalTime();
        /*设置新状态*/
        startTime=QDateTime::currentDateTime().toTime_t();
        gearNow=gearSet;
        wind=w;
        temperature=t;
    }
    else if(status==true&&now==false)//设备关闭
    {
        totalTime();
        status=false;
    }
    else
    {
        ;
    }
    update();
}

void OpenStatus::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    if(status)
    {
        painter.setBrush(QBrush(Qt::green,Qt::SolidPattern));
    }
    else
    {
        painter.setBrush(QBrush(QColor(246,246,246),Qt::SolidPattern));
    }
    painter.drawEllipse(QPointF(widgetSize/2,cycleSize),cycleSize,cycleSize);
    painter.end();

    painter.begin(this);
    painter.setFont(QFont(tr(" "),labelSize,labelSize));
    painter.drawText(QPointF(widgetSize/2-lightName.size()*labelSize/2,cycleSize*2+labelSize),lightName);
    painter.end();
}
