#include "vehicleconnection.h"


VehicleConnection::VehicleConnection() {
    diagnosticsFD = (int*)malloc(sizeof(int));
    missionControlFD = (int*)malloc(sizeof(int));
    connected = false;
}

VehicleConnection::~VehicleConnection() {
    free(diagnosticsFD);
    free(missionControlFD);
}

void VehicleConnection::connectToVehicle(QString ipAddress, QString missionControlPort, QString diagnosticsPort) {
    bool res = true;

    res &= doConnect(ipAddress, missionControlPort.toInt(), missionControlFD);
    res &= doConnect(ipAddress, diagnosticsPort.toInt(), diagnosticsFD);

    connected = res;

    emit connectionStatusChanged(res);
}

int VehicleConnection::doConnect(QString ipAddress, int port, int *fd) {
    int res;
    long arg;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    struct timeval tv;
    fd_set myset;
    int sockfd;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0) {
        qDebug() << "ERROR opening control socket";
        return false;
    }

    server = gethostbyname(ipAddress.toLatin1().data());

    if(server == NULL) {
        qDebug() << "ERROR: no such host";
        return false;
    }

    /* put the socket in non-blocking mode */
    arg = fcntl(sockfd, F_GETFL, NULL);
    arg |= O_NONBLOCK;
    fcntl(sockfd, F_SETFL, arg);

    /* set up the serv_addr struct */
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    /* set up the timeout for the non-blocking connect */
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    FD_ZERO(&myset);
    FD_SET(sockfd, &myset);

    if((res = ::connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        if(errno != EINPROGRESS) {
            qDebug() << "Failed to connect to server";
            return false;
        }
    }
    res = select(sockfd+1, NULL, &myset, NULL, &tv);
    if(res <= 0) {
        qDebug() << "Connection timeout!";
        return false;
    }

    /* use getsockopt to check if the non-blocking socket connection was actually successful */
    int error;
    socklen_t len = sizeof(error);
    error = 0;
    if(getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0) {
        qDebug() << "getsockopt error";
        return false;
    }

    if(error != 0) {
        qDebug() << "getsockopt error: " << error;
        return false;
    }

    /* put the socket back into blocking mode */
    arg = fcntl(sockfd, F_GETFL, NULL);
    arg &= (~O_NONBLOCK);
    fcntl(sockfd, F_SETFL, arg);
    qDebug() << "Socket value: " << sockfd;
    /* set the connected flag to true */
    *fd = sockfd;

   /* pretend we're connected */
    return true;
}

void VehicleConnection::disconnectFromVehicle() {
    qDebug() << "Disconnecting from vehicle";
    ::close(*missionControlFD);
    ::close(*diagnosticsFD);
    connected = false;
    emit connectionStatusChanged(false);
}

int VehicleConnection::writeToVehicle(QString message, int fd) {
    qDebug() << "Writing " << message << " to " << fd;
    return 1;
}

