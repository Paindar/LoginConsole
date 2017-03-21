#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkReply>
#include <QSystemTrayIcon>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void loginClick();
    void replyFinish(QNetworkReply*);
    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void changeEvent(QEvent * event) override;
private:
    Ui::MainWindow *ui;
    QNetworkAccessManager*manager;
    QSystemTrayIcon* mSysTrayIcon;
    bool status=false;
    QString username,password;

};

#endif // MAINWINDOW_H
