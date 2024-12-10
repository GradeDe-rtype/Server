//
// Created by dan13615 on 12/9/24.
//

#ifndef DATAPACKET_HPP
#define DATAPACKET_HPP

#define MAX_ARGS_SIZE 256
#define MAX_COMMAND_SIZE 16

namespace Server
{
    struct DataPacket
    {
        char command[MAX_COMMAND_SIZE];
        char args[MAX_ARGS_SIZE];
    };
} // namespace Server

#endif // DATAPACKET_HPP
