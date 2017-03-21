#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "globals.h"
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <cstdlib>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QStandardItemModel>
#include <QFile>
#include <windows.h>

void MainWindow::loginClick()
{
    bool remUser=(ui->remUser->checkState()==Qt::Checked),remPswd=(ui->rempswd->checkState()==Qt::Checked);
    QFile file("data.json");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonDocument document;
        QJsonObject object;
        if(remUser)
        {
            object.insert("username",ui->usernameText->text());
            object.insert("rememberUser",QJsonValue(true));
        }
        else
            object.insert("rememberUser",QJsonValue(false));
        if(remPswd)
        {
            object.insert("password",ui->passwordText->text());
            object.insert("rememberPswd",QJsonValue(true));
        }
        else
            object.insert("rememberPswd",QJsonValue(false));
        document.setObject(object);
        file.write(document.toJson());
        file.close();
    }
    QNetworkRequest request;
    request.setUrl(QUrl(statusurl+QString(rand()*89999+10001)));
    status=true;
    manager->get(request);
}

void MainWindow::replyFinish(QNetworkReply *reply)
{
    QByteArray result=reply->readAll();
    QJsonDocument document=QJsonDocument::fromJson(result);
    QJsonObject object=document.object();
    QJsonValue value = object["status"];
    if(value.toInt()==1)
    {
        ui->textEdit->clear();
        for(auto iter=object.begin();iter!=object.end();iter++)
        {
            QString info=iter.key()+":\t";
            switch(iter.value().type())
            {
            case QJsonValue::Null:
                info+="None";
                break;
            case QJsonValue::String:
                info+=iter.value().toString();
                break;
            case  QJsonValue::Double:
                info+= QString::number(iter.value().toInt());
                break;
            default:
                qDebug()<<"Unknown type"+QString(iter.value().type());
            }
            ui->textEdit->append(info);
        }
    }
    else
    {
        if(!status)
        {
            ui->textEdit->clear();
            ui->textEdit->append("FAIL TO LOGIN!");
            ui->textEdit->append(QString::fromStdString(reply->readAll().toStdString()));
            for(auto iter=object.begin();iter!=object.end();iter++)
            {
                    QString info=iter.key()+":\t";
                    switch(iter.value().type())
                    {
                    case QJsonValue::Null:
                        info+="None";
                        break;
                    case QJsonValue::String:
                        info+=iter.value().toString();
                        break;
                    case  QJsonValue::Double:
                        info+= QString::number(iter.value().toInt());
                        break;
                    default:
                        qDebug()<<"Unknown type"+QString(iter.value().type());
                    }

                    ui->textEdit->append(info);
            }
        }
        else
        {
            status=false;
            QNetworkRequest request;
            request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.143 Safari/537.36");
            request.setRawHeader("Referer","http://a.suda.edu.cn/");
            request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
            request.setUrl(QUrl(posturl));
            QByteArray array;
            array.append("username="+ui->usernameText->text()+"&");
            array.append("password="+ui->passwordText->text().toLatin1().toBase64()+"&");
            array.append("enablemacauth=0");
            manager->post(request,array);
        }
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    manager=new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinish(QNetworkReply*)));
    ui->setupUi(this);

    mSysTrayIcon = new QSystemTrayIcon(this);
    QIcon icon = QIcon(":/ico/ico.ico");
    mSysTrayIcon->setIcon(icon);
    mSysTrayIcon->setToolTip(QObject::trUtf8("显示网关程序"));
    connect(mSysTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));
    mSysTrayIcon->show();


    QFile file("data.json");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"cannot open data.json";
        return;
    }
    QByteArray array=file.readAll();
    QJsonDocument document=QJsonDocument::fromJson(array);
    QJsonObject object=document.object();

    if(object.find("username")!=object.end())
        ui->usernameText->setText(object["username"].toString());
    if(object.find("password")!=object.end())
        ui->passwordText->setText(object["password"].toString());
    if(object.find("rememberUser")!=object.end())
        ui->remUser->setChecked(object["rememberUser"].toBool());
    if(object.find("rememberPswd")!=object.end())
        ui->rempswd->setChecked(object["rememberPswd"].toBool());
    file.close();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type()!=QEvent::WindowStateChange)
        return;
    if(this->windowState()==Qt::WindowMinimized)
    {
        this->hide();
    }
}

void MainWindow::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
        case QSystemTrayIcon::Trigger:
            //单击托盘图标
            break;
        case QSystemTrayIcon::DoubleClick:
            if (this->isMinimized())
            {
                this->showNormal();
            }
            //设置窗口置顶
            ::SetWindowPos(HWND(this->winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
            ::SetWindowPos(HWND(this->winId()), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

            this->show();
            this->activateWindow();
            break;
        default:
            break;
    }
}
