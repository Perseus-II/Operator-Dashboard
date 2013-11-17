#include "diagnostics.h"

Diagnostics::Diagnostics(VehicleConnection *con)
{
    this->connection = con;
}

Diagnostics::~Diagnostics() {
    this->connection = NULL;
}

void Diagnostics::process() {
    char *vehicle_info;
    float s1,s2,s3,h1;
    char *pch;
    float pitch, roll, yaw;
    float lat, lon, prev_lat, prev_lon;
    float kp, ki, kd;
    float depth_ft, depth_m, temperature;
    float gps_siv, gps_siu, gps_dop;

    while(1) {
        if(!this->connection->connected) continue;
        vehicle_info = this->connection->writeAndRead("/info");
        qDebug() << "Finished writing /info";
        usleep(250000);
        if(vehicle_info != NULL) {
            //qDebug() << vehicle_info;
            if(pch == NULL) return;
            pch = strtok(vehicle_info, ",");
            lat = atof(pch);
            //qDebug() << "position lat = " << pch;
            if(pch == NULL) return;
            pch = strtok(NULL, ",");
            lon = atof(pch);
            //qDebug() << "position lon = " << pch;
            if(pch == NULL) return;
            pch = strtok(NULL, ",");
            s1 = atof(pch);
            //qDebug() << "thrust 1 = " << pch;
            if(pch == NULL) return;
            pch = strtok(NULL, ",");
            s2 = atof(pch);
            //qDebug() << "thrust 2 = " << pch;
            if(pch == NULL) return;
            pch = strtok(NULL, ",");
            s3 = atof(pch);
            //qDebug() << "thrust 3 = " << pch;
            if(pch == NULL) return;
            pch = strtok(NULL, ",");
            h1 = atof(pch);
            //qDebug() << "thrust 4 = " << pch;
            if(pch == NULL) return;
            pch = strtok(NULL, ",");
            //qDebug() << "pitch = " << pch;
            if(pch == NULL) return;
            pitch = atof(pch);
            pch = strtok(NULL, ",");
            //qDebug() << "roll = " << pch;
            roll = atof(pch);
            if(pch == NULL) return;
            pch = strtok(NULL, ",");
            //qDebug() << "yaw = " << pch;
            yaw = atof(pch);
            if(pch == NULL) return;
            pch = strtok(NULL, ",");
            //qDebug() << "pitch_d = " << pch;
            if(pch == NULL) return;
            pch = strtok(NULL, ",");
            //qDebug() << "roll_d = " << pch;
            if(pch == NULL) return;
            pch = strtok(NULL, ",");
            //qDebug() << "yaw_d = " << pch;
            if(pch == NULL) return;
            pch = strtok(NULL, ",");
            //qDebug() << "mode = " << pch;
            if(pch == NULL) return;
            pch = strtok(NULL, ",");
            kp = atof(pch);
            //qDebug() << "kp = " << kp;
            if(pch == NULL) return;
            pch = strtok(NULL, ",");
            ki = atof(pch);
            //qDebug() << "ki = " << ki;
            if(pch == NULL) return;
            pch = strtok(NULL, ",");
            kd = atof(pch);
            //qDebug() << "kd = " << kd;
            if(pch == NULL) return;
            pch = strtok(NULL, ",");
            depth_ft = atof(pch);
            qDebug() << "depth_ft = " << depth_ft;
            if(pch == NULL) return;
            pch = strtok(NULL, ",");
            depth_m = atof(pch);
            //qDebug() << "depth_m = " << depth_m;
            if(pch == NULL) return;
            pch = strtok(NULL, ",");
            temperature = atof(pch);
            //qDebug() << "temperature = " << temperature;
            if(pch == NULL) return;
            pch = strtok(NULL, ",");
            gps_siv = atof(pch);
            //qDebug() << "gps siv = " << gps_siv;
            if(pch == NULL) return;
            pch = strtok(NULL, ",");
            gps_siu = atof(pch);
            //qDebug() << "gps siu = " << gps_siu;
            if(pch == NULL) return;
            pch = strtok(NULL, ",");
            gps_dop = atof(pch);
            //qDebug() << "gps dop = " << gps_dop;

            emit(infoUpdated(pitch, roll, yaw));
            emit(pidValuesUpdated(kp, ki, kd));
            qDebug() << "Lat = " << lat << " | Lon = " << lon;

            /* only add point to map if our position has changed */
            if((lat != prev_lat || lon != prev_lon) && gps_dop == 1.0) {
                qDebug() << "Updating map";
                emit newMapPointAvailable(QPointF(lat, lon));
            }

            qDebug() << "New thrust vector: " << s1 << " " << s2 << " " << s3 << " " << h1;
            emit(thrustVectorChanged(s1,s2,s3,h1));

            /* store previous lon lat pair */
            prev_lat = lat;
            prev_lon = lon;

        }

        free(vehicle_info);

    }
}
