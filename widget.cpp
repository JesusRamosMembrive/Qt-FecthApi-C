#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // Initialize the network manager
    connect(ui->pushButtonRequestTimezone, &QPushButton::clicked, this, &Widget::getTimezone);
}

Widget::~Widget()
{
    // Delete the UI
    delete ui;
}

void Widget::initializeNetworkManager()
{
    // Initialize the network manager
    manager = new QNetworkAccessManager(this);
}

void Widget::requestIP()
{
    // Disconnect the signal from the previous request
    connect(manager, &QNetworkAccessManager::finished, this, &Widget::processIPResponse);
    manager->get(QNetworkRequest(QUrl("https://api.ipify.org?format=json")));
}

void Widget::processIPResponse(QNetworkReply *reply)
{
    // Check for errors
    if (reply->error() == QNetworkReply::NoError) {
        // Read the response
        QByteArray response = reply->readAll();
        // Parse the response
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
        // Extract the IP address
        QJsonObject jsonObject = jsonResponse.object();
        // Store the IP address
        ip = jsonObject["ip"].toString();
        if (!ip.isEmpty()) {
            // Request the time zone
            requestTimezone();
        }
    }
    reply->deleteLater();
}

void Widget::requestTimezone()
{
    // Disconnect the signal from the previous request
    disconnect(manager, &QNetworkAccessManager::finished, nullptr, nullptr);
    // Connect the signal to the new request
    connect(manager, &QNetworkAccessManager::finished, this, &Widget::processTimezoneResponse);
    // Request the time zone
    manager->get(QNetworkRequest(QUrl("http://www.geoplugin.net/json.gp?ip=" + ip)));
}

void Widget::processTimezoneResponse(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        // Read the response
        QByteArray response = reply->readAll();
        // Parse the response
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
        // Extract the time zone
        QJsonObject jsonObject = jsonResponse.object();
        // Store the time zone
        timeZone = jsonObject["geoplugin_timezone"].toString();
        // Store the city and region
        region = jsonObject["geoplugin_city"].toString();
        city = jsonObject["geoplugin_region"].toString();
        // Update the UI
        updateUI();
    }
    else {
        qCritical() << "Error obtaining time zone:" << reply->errorString();
    }
    // Delete the reply
    reply->deleteLater();
}

void Widget::updateUI()
{
    // Display the IP address
    QString textToDisplay = "Your IP address is: " + ip + "\n";
    textToDisplay += "Your city is: " + city + "\n";
    textToDisplay += "Your region is: " + region + "\n";
    textToDisplay += "Your time zone is: " + timeZone + "\n";
    // Update the UI
    ui->labelTimezone->setText(textToDisplay);
}

void Widget::getTimezone()
{
    // Initialize the network manager
    initializeNetworkManager();
    // Request the IP address
    requestIP();
}
