#pragma once
#include <map>
#include <stack>
#include <string>
#include "block.h"

namespace Real2D {
    struct PlayerPacketC2S : public PacketC2S {
        block_t choosingBlock;
        void sendPacket();
    };
    class PacketC2S : public Packet {
    protected:
        std::stack<PacketS2C> stack;
    };
    class PacketS2C : public Packet {
    protected:
        std::stack<PacketC2S> stack;
    };
    class Packet {};
}
