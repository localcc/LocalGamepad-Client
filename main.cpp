#include <sdl_gamepad/sdl_gamepad.h>
#include <udp_client/udp_client.h>
#include <memory>
#include <thread>
#include <csignal>

volatile bool running = false;

void signal_handler(int signal) {
    if(signal == SIGINT) {
        running = false;
    }
}

int main(int argc, char* argv[]) {
    if(argc < 3) {
        printf("Not enough arguments!\n Example: LocalGamepadClient [ip] [port]");
        return -1;
    }
    auto udp_client_instance = std::make_unique<udp_client>(argv[1], atoi(argv[2]));
    auto* sdl_gamepad_instance = new sdl_gamepad(std::move(udp_client_instance));
    std::thread t1([&](){
        sdl_gamepad_instance->handle();
    });
    t1.detach();
    signal(SIGINT, signal_handler);
    running = true;
    while(running){}
    sdl_gamepad_instance->stop();
    delete sdl_gamepad_instance;

    return 0;
}
