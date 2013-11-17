#include "joystick.h"

Joystick::Joystick(VehicleConnection *connection) {
    stick = NULL;
    attached = false;
    vehicleConnection = connection;

    /* Initialize SDL */
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        qDebug() << "Failed to initialize joystick";
        return;
    }
    qDebug() << "Initialized SDL";

    SDL_JoystickEventState(SDL_ENABLE);

    if(SDL_NumJoysticks() < 1) {
        qDebug() << "No joystick connected";
        return;
    }
}

Joystick::~Joystick() {

}

void Joystick::detachJoystick() {
    SDL_JoystickClose(stick);
    stick = NULL;
    attached = false;
    qDebug() << "Detached Joystick";

    QString thrust_string;
    thrust_string.sprintf("/set_thrust %d,%d,%d,%d", 0.0, 0.0, 0.0, 0.0);
    vehicleConnection->writeToVehicle(thrust_string); /* set thrust to 0 when joystick disconnects */

    emit joystickStateChanged(false);
}

void Joystick::reattachJoystick() {
    detachJoystick();

    attachJoystick();
}


void Joystick::attachJoystick() {
    /* Attach the joystick */
    stick = SDL_JoystickOpen(0);
    SDL_Event event;
    qDebug() << "Attached joystick ";
    if(stick == NULL) {
        puts("Error attaching joystick!");
        return;
    }

    attached = true;
    emit joystickStateChanged(true);

    int x1_move;
    int y1_move;
    int x2_move;
    int y2_move;
    int heave_up;
    int heave_down;
    int surge_starboard;
    int surge_port;
    int heave_a;
    int heave_b;
    int avg_surge;

    //signal (SIGPIPE, SIG_IGN);

    while(attached) {
        while(SDL_PollEvent(&event)) {
            x1_move = SDL_JoystickGetAxis(stick, 0);
            y1_move = SDL_JoystickGetAxis(stick, 1);
            x2_move = SDL_JoystickGetAxis(stick, 2);
            y2_move = SDL_JoystickGetAxis(stick, 3);
            heave_up = SDL_JoystickGetButton(stick, 4) * 60;
            heave_down = SDL_JoystickGetButton(stick, 5) * 60 * -1;

            surge_starboard = -1 * (y2_move * (75.0/32768.0));
            surge_port = -1 * (y1_move * (75.0/32768.0));
            avg_surge = (surge_starboard+surge_port)/2;


            heave_b = heave_up + heave_down;

            heave_a = avg_surge; /* we're using a 3rd surge instead of 2nd heave */
            //heave_a = 0;

            QString thrust_string;
            thrust_string.sprintf("/set_thrust %d,%d,%d,%d", surge_port, surge_starboard, heave_a, heave_b);


            vehicleConnection->writeToVehicle(thrust_string);
            emit(thrustVectorChanged(surge_port,
                                     surge_starboard,
                                     avg_surge,
                                     heave_a));
            qDebug() << surge_port << " | " << surge_starboard << " | " << heave_a << " | " << heave_b;

            usleep(100000);
        }
        usleep(50000);
    }
    return;
}


