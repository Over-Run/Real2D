#pragma once
#include <map>
#include <stack>
#include <string>
#include "block.h"

namespace Real2D {
    struct Packet {
        virtual void sendPacket() = 0;
    };
    struct PacketC2S : public Packet {
    protected:
        static std::stack<PacketC2S*> stack;
    public:
        void sendPacket();
        static Packet* getLastPacket();
    };
    struct PlayerPacketC2S : public PacketC2S {
        block_t choosingBlock;
    };
    struct KeyPacketC2S : public PacketC2S {
        int key;
        int action;
    };
}
