#include "joystick.h"

void Joystick::run() {
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

bool Joystick::attachJoystick(int s) {
    /* Attach the joystick */
    stick = SDL_JoystickOpen(s);
    qDebug() << "Attached joystick " << s;
    if(stick == NULL) {
        puts("Error attaching joystick!");
        return false;
    }

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

    signal (SIGPIPE, SIG_IGN);

    while(1) {
        while(SDL_PollEvent(&event)) {
            x1_move = SDL_JoystickGetAxis(stick, 0);
            y1_move = SDL_JoystickGetAxis(stick, 1);
            x2_move = SDL_JoystickGetAxis(stick, 2);
            y2_move = SDL_JoystickGetAxis(stick, 3);
            heave_up = SDL_JoystickGetButton(stick, 4) * 60;
            heave_down = SDL_JoystickGetButton(stick, 5) * 60 * -1;


            surge_starboard = -1 * (y2_move * (75.0/32768.0));
            surge_port = -1 * (y1_move * (75.0/32768.0));
            heave_a = heave_up + heave_down;
            heave_b = heave_up + heave_down;

            qDebug() << surge_port << " | " << surge_starboard << " | " << heave_a << " | " << heave_b;

            usleep(80000);

            switch(event.type) {
            case SDL_KEYDOWN:
                break;

            case SDL_QUIT:
                break;
            }
        }
    }
}


