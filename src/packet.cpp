#include "real2d/packet.h"

using std::stack;
using Real2D::Packet;
using Real2D::PacketC2S;

std::stack<PacketC2S*> PacketC2S::stack;
void PacketC2S::sendPacket() {
    stack.push(this);
}
Packet* PacketC2S::getLastPacket() {
    if (stack.empty()) {
        return nullptr;
    }
    Packet* p = stack.top();
    stack.pop();
    return p;
}
