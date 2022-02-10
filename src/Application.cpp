#include "Application.h"
#include "Physics/Constants.h"
#include <iostream>
#include "imgui_impl_sdl.h"

bool Application::IsRunning() {
    return running;
}

void Application::Setup() {
    running = Graphics::OpenWindow();

    particles.emplace_back(Vec2{ 100.0f, 100.0f }, 1.0f, 4);
    particles.emplace_back(Vec2{ 100.0f, 200.0f }, 3.0f, 12);

    fluid_area = {
        .x = 0,
        .y = Graphics::Height() / 2,
        .w = Graphics::Width(),
        .h = Graphics::Height() / 2
    };
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

    static Vec2 wind_force{ 2.0f * pixels_per_meter, 0.0f };
    static float gravity = 9.8f;

    Graphics::BeginImGuiFrame();

    {
        static int counter = 0;

        ImGui::Begin("Hello, world!");

        ImGui::Text("This is some useful text.");

        ImGui::SliderFloat("Gravity", &gravity, -100.0f, 100.0f);

        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
    
    for (auto& particle : particles)
    {
        particle.AddForce(wind_force);
        particle.AddForce(Vec2(0.0f, (gravity / particle.inverse_mass) * pixels_per_meter));

        SDL_Rect bounding_box = {
            .x = int(particle.position.x - particle.radius),
            .y = int(particle.position.y - particle.radius),
            .w = int(particle.radius * 2),
            .h = int(particle.radius * 2)
        };

        if (SDL_HasIntersection(&fluid_area, &bounding_box))
        {
            Vec2 opposite_velocity_dir = -particle.velocity.UnitVector();
            float drag_force_magnitude = particle.velocity.MagnitudeSquared();
            const float k = 0.01f;
            Vec2 drag_force = opposite_velocity_dir * k * drag_force_magnitude;
            particle.AddForce(drag_force);
        }

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

    Graphics::DrawFillRect(fluid_area, 0xFF0000FF);

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
