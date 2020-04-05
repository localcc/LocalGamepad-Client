//
// Created by local on 02.04.2020.
//

#ifndef LOCALGAMEPAD_CLIENT_SDL_GAMEPAD_H
#define LOCALGAMEPAD_CLIENT_SDL_GAMEPAD_H
#include <memory>
#include <functional>
#include <cstdint>
#include <map>
#include <SDL_gamecontroller.h>
#include <udp_client/udp_client.h>
#include <bitset>
#include <SDL.h>
// Because sdl
#undef main


class sdl_gamepad {
public:
    explicit sdl_gamepad(std::unique_ptr<udp_client> client);
    void handle();
    void stop();
    ~sdl_gamepad();
private:
    static void controller_button(sdl_gamepad* gamepad_instance, SDL_Event event);
    static void controller_axis(sdl_gamepad* gamepad_instance, SDL_Event event);
    void send_data_to_server();
    std::bitset<15> digital_data;
    short* analog_data;
    std::unique_ptr<udp_client> client;
    char client_id;
    SDL_GameController* gameController;
    bool running{};
    const std::map<uint32_t, std::function<void(sdl_gamepad*, SDL_Event)>> event_handlers = {
            {SDL_CONTROLLERBUTTONUP, &sdl_gamepad::controller_button},
            {SDL_CONTROLLERBUTTONDOWN, &sdl_gamepad::controller_button},
            {SDL_CONTROLLERAXISMOTION, &sdl_gamepad::controller_axis}};

};

#endif //LOCALGAMEPAD_CLIENT_SDL_GAMEPAD_H
