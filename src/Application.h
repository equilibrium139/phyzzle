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

        Particle particle = {
            .position = {100.0f, 100.0f},
            .velocity = Vec2{-1.0f, 0.0f} * pixels_per_meter,
            .acceleration = Vec2{-1.0f, 0.0f} * pixels_per_meter,
            .mass = 1.0f,
            .radius = 4
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
