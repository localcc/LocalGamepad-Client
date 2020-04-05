//
// Created by local on 03.04.2020.
//

#include <sdl_gamepad/sdl_gamepad.h>
#include <cstdio>
#include <thread>

sdl_gamepad::sdl_gamepad(std::unique_ptr<udp_client> client): client(std::move(client)), digital_data(0), client_id(0), running(false){
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
        printf("Failed to init sdl: %s\n", SDL_GetError());
        return;
    }
    if(SDL_IsGameController(0)) {
        gameController = SDL_GameControllerOpen(0);
    }
    analog_data = new short[6];
    memset(analog_data, 0, 6);
}

void sdl_gamepad::handle() {
    this->client->connect();
    char* id_buf = new char[2];
    this->client->read_data(id_buf, 2);
    if(id_buf[0] != 2) {
        printf("client_id not received! aborting!\n");
        return;
    }
    this->client_id = id_buf[1];
    this->running = true;
    SDL_Event event;
    while(running) {
        while(SDL_PollEvent(&event)) {
            auto event_handler = event_handlers.find(event.type);
            if(event_handler != event_handlers.end()) {
                event_handler->second(this, event);
            }
        }
    }
    this->client->disconnect();

}

void sdl_gamepad::stop() {
    this->running = false;
}

sdl_gamepad::~sdl_gamepad() {
    auto* disconnect_buf = new unsigned char[2]{9, this->client_id};
    this->client->write_data(disconnect_buf, 2);
    delete[] disconnect_buf;
    this->client.reset();
    SDL_GameControllerClose(this->gameController);
    SDL_Quit();

}

void sdl_gamepad::controller_button(sdl_gamepad* gamepad_instance, SDL_Event event) {
    if(event.cbutton.which == 0) {
        gamepad_instance->digital_data.set(event.cbutton.button, event.cbutton.state);
        gamepad_instance->send_data_to_server();
    }
}

void sdl_gamepad::send_data_to_server() {
    auto* data = new unsigned char[16];
    data[0] = 1;
    data[1] = this->client_id;

    data[2] = this->digital_data.to_ulong() & 0xFFu;
    data[3] = (this->digital_data.to_ulong() >> 8u) & 0xFFu;

    for(short i = 1; i <= 6; i++) {
        // i - 2 to start with 1, so i *  2 will give the position to start writing at
        // then writing 2 bytes from short
        data[(i * 2) + 2] = this->analog_data[i - 1] & 0xFF;
        data[(i * 2) + 3] = (this->analog_data[i - 1] >> 8) & 0xFF;
    }
    client->write_data(data, 16);
    delete[] data;
}

void sdl_gamepad::controller_axis(sdl_gamepad* gamepad_instance, SDL_Event event) {
    if(event.caxis.which == 0) {
        gamepad_instance->analog_data[event.caxis.axis] = event.caxis.value;
        gamepad_instance->send_data_to_server();
    }
}


