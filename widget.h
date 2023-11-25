#pragma once

#include <QObject>
#include <QWidget>
#include <QtNetwork>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QEventLoop>
#include <QDebug>
#include <QByteArray>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    /*
     * Constructor
     */
    Widget(QWidget *parent = nullptr);

    /*
     * Destructor
     */
    ~Widget();

private:
    /*
     * Pointer to the UI
     */
    Ui::Widget *ui;

    //--Variables--//
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QNetworkReply *reply;
    QJsonDocument jsonResponse;
    QJsonObject jsonObject;
    QJsonValue timezoneValue;
    QString ip{""};
    QString city{""};
    QString region{""};
    QString timeZone{""};

    /*
     * This function is called when the network manager has finished
     * processing the response. It reads the response and extracts the
     * timezone information.
     */
    void processTimezoneResponse(QNetworkReply *reply);

    /*
     * This function is called when the network manager has finished
     * processing the response. It reads the response and extracts the
     * IP address.
     */
    void requestTimezone();

    /*
     * This function is called when the network manager has finished
     * processing the response. It reads the response and extracts the
     * IP address.
     */
    void processIPResponse(QNetworkReply *reply);

    /*
     * This function is called when the user clicks the button.
     * It calls the requestIP() function.
     */
    void requestIP();

    /*
     * This function is called when the widget is created.
     * It initializes the network manager.
     */
    void initializeNetworkManager();

    /*
     * This function is called when the timezone information is available.
     * It updates the UI with the timezone information.
     */
    void updateUI();


private slots:
    /*
     * This function is called when the user clicks the button.
     * It calls the getTimezone() function.
     */
    void getTimezone();
};
