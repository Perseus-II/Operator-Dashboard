#ifndef VEHICLECONNECTION_H
#define VEHICLECONNECTION_H

#include <QDebug>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <strings.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>

class VehicleConnection : public QObject
{
    Q_OBJECT

public:
    VehicleConnection();
    ~VehicleConnection();  
    int doConnect(QString, int, int*);
    int writeToVehicle(QString);
    char *writeAndRead(QString);
    bool connected;
    int *fd;

public slots:
    void disconnectFromVehicle();
    void connectToVehicle(QString, QString);


private:

signals:
    void connectionStatusChanged(int);

};

#endif // VEHICLECONNECTION_H
