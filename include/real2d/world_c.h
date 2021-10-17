#pragma once
#include "world.h"

namespace Real2D {
    class WorldRenderer : public WorldListener {
    private:
        bool is_dirty;
        World* world;
    public:
        WorldRenderer(World* _world);
        ~WorldRenderer();
        void blockChanged(int x, int y, int z) override;
        void lightColChanged(int x, int z) override;
        void markDirty();
        bool isDirty();
        void render(double delta);
        void pick();
        void renderPick(double delta);
    };
}
