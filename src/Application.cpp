#include "Application.h"
#include "Physics/Constants.h"
#include <iostream>
#include "imgui_impl_sdl.h"
#include "Physics/Forces.h"

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
        ImGui_ImplSDL2_ProcessEvent(&event);

        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (ImGui::GetIO().WantCaptureKeyboard) break;

                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (ImGui::GetIO().WantCaptureMouse) break;

                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    Vec2 position{ (float)event.button.x, (float)event.button.y };
                    float mass = 10.0f;
                    int radius = 40;
                    particles.emplace_back(position, mass, radius);
                }
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

    static Vec2 wind_force{ 2.0f * pixels_per_meter, 0.0f };
    static float gravity = 9.8f;
    static float drag_strength = 0.01f;

    Graphics::BeginImGuiFrame();
    {
        ImGui::Begin("Hello, world!");

        ImGui::Text("This is some useful text.");

        ImGui::SliderFloat("Gravity", &gravity, -100.0f, 100.0f);
        ImGui::InputFloat("Drag strength", &drag_strength);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
    
    for (auto& particle : particles)
    {
        SDL_Rect bounding_box = {
            .x = int(particle.position.x - particle.radius),
            .y = int(particle.position.y - particle.radius),
            .w = int(particle.radius * 2),
            .h = int(particle.radius * 2)
        };

        Vec2 random_force = { 3.0f * pixels_per_meter, 5.0f * pixels_per_meter };

        particle.AddForce(random_force);
        phyz::AddFrictionForce(particle, 5.0f * pixels_per_meter);

        particle.Integrate(dt_seconds);

        if (particle.position.x - particle.radius < 0)
        {
            particle.velocity.x *= -1.0f;
            particle.position.x = particle.radius;
        }
        else if (particle.position.x + particle.radius >= Graphics::Width())
        {
            particle.velocity.x *= -1.0f;
            particle.position.x = Graphics::Width() - particle.radius;
        }

        if (particle.position.y - particle.radius < 0)
        {
            particle.velocity.y *= -1.0f;
            particle.position.y = particle.radius;
        }
        else if (particle.position.y + particle.radius >= Graphics::Height())
        {
            particle.velocity.y *= -1.0f;
            particle.position.y = Graphics::Height() - particle.radius;
        }
    }
}

void Application::Render() {
    Graphics::ClearScreen(0xFF056263);

    for (const auto& particle : particles)
    {
        Graphics::DrawFillCircle((int)particle.position.x, (int)particle.position.y, particle.radius, 0xFFFFFFFF);
    }


    ImGui::ShowDemoWindow();

    Graphics::RenderFrame();
}

void Application::Destroy() {
    Graphics::CloseWindow();
}
