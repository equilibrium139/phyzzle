#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "./Physics/Particle.h"
#include <cstdint>

class Application {
    private:
        bool running = false;

        std::uint32_t last_frame_time_ticks = 0;
        float dt = 0.0f;

        Particle particle = {
            .position = {50.0f, 100.0f},
            .velocity = {0.0f, 0.0f},
            .acceleration = {0.0f, 0.1f},
            .mass = 1.0f
        };

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
