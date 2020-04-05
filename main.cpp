#include <sdl_gamepad/sdl_gamepad.h>
#include <udp_client/udp_client.h>
#include <memory>
#include <thread>
#undef main
int main() {
    auto udp_client_instance = std::make_unique<udp_client>("127.0.0.1", 5781);
    auto* sdl_gamepad_instance = new sdl_gamepad(std::move(udp_client_instance));
    std::thread([&](){sdl_gamepad_instance->handle();}).detach();
    while(true){}
    return 0;
}
