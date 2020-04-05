//
// Created by local on 04.04.2020.
//

#ifndef LOCALGAMEPAD_CLIENT_CONTROLLER_MAPPER_H
#define LOCALGAMEPAD_CLIENT_CONTROLLER_MAPPER_H
#include <controller/controller_offsets.h>
#include <cstdint>
#include <map>
#include <

class controller_mapper {
public:
    static char map_button(uint8_t button);
private:

};

#endif //LOCALGAMEPAD_CLIENT_CONTROLLER_MAPPER_H
