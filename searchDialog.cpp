#include "searchDialog.h"
#include "openstatus.h"
#include <QtWidgets>

searchDialog::searchDialog(QWidget *parent) : QDialog(parent)
{
    All=new QGridLayout(this);
    messageBox=new QTextEdit(this);
    messageBox->setReadOnly(true);
    OK=new QPushButton(tr("取消"),this);
    Clear=new QPushButton(tr("缴费"),this);

    connect(OK,SIGNAL(clicked()),this,SLOT(close()));
    connect(Clear,SIGNAL(clicked()),this,SLOT(clearMessages()));

    All->addWidget(messageBox,0,0,1,2);
    All->addWidget(Clear,1,0);
    All->addWidget(OK,1,1);

    setLayout(All);
    setWindowTitle(tr("查询结果"));
}

void searchDialog::clearMessages()
{
    OpenStatus::runTime cls={0,0,0};
    for(int i=0;i!=deviceNum;++i)
    {
        deviceGroup[i]->setRunTimeInfo(cls);
    }
    messageBox->setText(tr("缴费成功!"));
}

void searchDialog::message(OpenStatus *devices[],int num, const QString &text)
{
    deviceGroup=devices;
    deviceNum=num;
    messageBox->setText(text);
    show();
}

searchDialog::~searchDialog()
{
    ;
}
