#include "Application.h"
#include "Physics/Constants.h"
#include <iostream>

bool Application::IsRunning() {
    return running;
}

void Application::Setup() {
    running = Graphics::OpenWindow();
}

void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                break;
        }
    }
}

void Application::Update() {
    int current_time = SDL_GetTicks();
    int sleep_time = ms_per_frame - (previous_frame_time_ms - current_time);
    if (sleep_time > 0)
    {
        SDL_Delay(sleep_time);
    }

    current_time = SDL_GetTicks();
    int dt_ticks = current_time - previous_frame_time_ms;
    dt_seconds = (float)dt_ticks / 1000.0f;
    
    if (dt_seconds > ms_per_frame) dt_seconds = ms_per_frame;

    previous_frame_time_ms = current_time;
    
    particle.velocity += particle.acceleration * dt_seconds;
    particle.position += particle.velocity * dt_seconds;

    // Switch from meters to pixels
    if (particle.position.x - particle.radius < 0)
    {
        particle.velocity.x = std::abs(particle.velocity.x);
        particle.acceleration.x = std::abs(particle.acceleration.x);

        if (particle.velocity.x == 0.0f) particle.velocity.x = 10.0f;
    }
    else if (particle.position.x + particle.radius >= Graphics::Width())
    {
        particle.velocity.x = -std::abs(particle.velocity.x);
        particle.acceleration.x = -std::abs(particle.acceleration.x);
    }

    if (particle.position.y - particle.radius < 0)
    {
        particle.velocity.y = std::abs(particle.velocity.y);
        particle.acceleration.y = std::abs(particle.acceleration.y);
    }
    else if (particle.position.y + particle.radius >= Graphics::Height())
    {
        particle.velocity.y = -std::abs(particle.velocity.y);
        particle.acceleration.y = -std::abs(particle.acceleration.y);
    }
}

void Application::Render() {
    Graphics::ClearScreen(0xFF056263);
    Graphics::DrawFillCircle((int)particle.position.x, (int)particle.position.y, particle.radius, 0xFFFFFFFF);
    Graphics::RenderFrame();
}

void Application::Destroy() {
    Graphics::CloseWindow();
}
