#include "Application.h"
#include "Physics/Constants.h"
#include <iostream>

bool Application::IsRunning() {
    return running;
}

void Application::Setup() {
    running = Graphics::OpenWindow();

    particles.emplace_back(Vec2{ 100.0f, 100.0f }, 1.0f, 4);
    particles.emplace_back(Vec2{ 100.0f, 200.0f }, 3.0f, 12);
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
    
    for (auto& particle : particles)
    {
        particle.AddForce(Vec2(2.0f * pixels_per_meter, 0.0f));
        particle.AddForce(Vec2(0.0f, 9.8f / particle.inverse_mass * pixels_per_meter));

        particle.Integrate(dt_seconds);

        if (particle.position.x - particle.radius < 0)
        {
            particle.velocity.x *= -1.0f;
            particle.position.x = particle.radius;
        }
        else if (particle.position.x + particle.radius >= Graphics::Width())
        {
            particle.velocity.x *= -1.0f;
            particle.position.x = Graphics::Width() - particle.radius - 1;
        }

        if (particle.position.y - particle.radius < 0)
        {
            particle.velocity.y *= -1.0f;
            particle.position.y = particle.radius;
        }
        else if (particle.position.y + particle.radius >= Graphics::Height())
        {
            particle.velocity.y *= -1.0f;
            particle.position.y = Graphics::Height() - particle.radius - 1;
        }
    }
}

void Application::Render() {
    Graphics::ClearScreen(0xFF056263);
    for (const auto& particle : particles)
    {
        Graphics::DrawFillCircle((int)particle.position.x, (int)particle.position.y, particle.radius, 0xFFFFFFFF);
    }
    Graphics::RenderFrame();
}

void Application::Destroy() {
    Graphics::CloseWindow();
}
