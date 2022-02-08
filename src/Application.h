#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "Physics/Constants.h"
#include "./Physics/Particle.h"
#include <cstdint>

class Application {
    private:
        bool running = false;

        int previous_frame_time_ms = 0;
        float dt_seconds = 0.0f;

        std::vector<Particle> particles;
    public:
        Application() = default;
        ~Application() = default;
        bool IsRunning();
        void Setup();
        void Input();
        void Update();
        void Render();
        void Destroy();
};

#endif
