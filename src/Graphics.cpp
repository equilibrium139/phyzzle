#include "Graphics.h"
#include <iostream>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#if !SDL_VERSION_ATLEAST(2,0,17)
#error This ImGui backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

SDL_Window* Graphics::window = NULL;
SDL_Renderer* Graphics::renderer = NULL;
int Graphics::windowWidth = 0;
int Graphics::windowHeight = 0;

int Graphics::Width() {
    return windowWidth;
}

int Graphics::Height() {
    return windowHeight;
}

bool Graphics::OpenWindow() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL" << std::endl;
        return false;
    }

    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    windowWidth = display_mode.w;
    windowHeight = display_mode.h;
    window = SDL_CreateWindow(NULL, 0, 0, windowWidth, windowHeight, SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!window) {
        std::cerr << "Error creating SDL window" << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Error creating SDL renderer" << std::endl;
        return false;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window);
    ImGui_ImplSDLRenderer_Init(renderer);

    return true;
}

void Graphics::ClearScreen(Uint32 color) {
    SDL_SetRenderDrawColor(renderer, color >> 16, color >> 8, color, 255);
    SDL_RenderClear(renderer);
}

void Graphics::BeginFrame(Uint32 clear_color)
{
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    ClearScreen(clear_color);
}

void Graphics::RenderFrame() {
    ImGui::Render();
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(renderer);
}

void Graphics::DrawLine(int x0, int y0, int x1, int y1, Uint32 color) {
    lineColor(renderer, x0, y0, x1, y1, color);
}

void Graphics::DrawCircle(int x, int y, int radius, float angle, Uint32 color) {
    circleColor(renderer, x, y, radius, color);
    lineColor(renderer, x, y, x + cos(angle) * radius, y + sin(angle) * radius, color);
}

void Graphics::DrawFillCircle(int x, int y, int radius, Uint32 color) {
    filledCircleColor(renderer, x, y, radius, color);
}

void Graphics::DrawRect(int x, int y, int width, int height, Uint32 color) {
    lineColor(renderer, x - width / 2.0, y - height / 2.0, x + width / 2.0, y - height / 2.0, color);
    lineColor(renderer, x + width / 2.0, y - height / 2.0, x + width / 2.0, y + height / 2.0, color);
    lineColor(renderer, x + width / 2.0, y + height / 2.0, x - width / 2.0, y + height / 2.0, color);
    lineColor(renderer, x - width / 2.0, y + height / 2.0, x - width / 2.0, y - height / 2.0, color);
}

void Graphics::DrawFillRect(int x, int y, int width, int height, Uint32 color) {
    boxColor(renderer, x - width / 2.0, y - height / 2.0, x + width / 2.0, y + height / 2.0, color);
}

void Graphics::DrawPolygon(int x, int y, const std::vector<Vec2>& vertices, Uint32 color) {
    for (int i = 0; i < vertices.size(); i++) {
        int currIndex = i;
        int nextIndex = (i + 1) % vertices.size();
        lineColor(renderer, vertices[currIndex].x, vertices[currIndex].y, vertices[nextIndex].x, vertices[nextIndex].y, color);
    }
    filledCircleColor(renderer, x, y, 1, color);
}

void Graphics::DrawFillPolygon(int x, int y, const std::vector<Vec2>& vertices, Uint32 color) {
    std::vector<short> vx;
    std::vector<short> vy;
    for (int i = 0; i < vertices.size(); i++) {
        vx.push_back(static_cast<int>(vertices[i].x));
    }
    for (int i = 0; i < vertices.size(); i++) {
        vy.push_back(static_cast<int>(vertices[i].y));
    }
    filledPolygonColor(renderer, &vx[0], &vy[0], vertices.size(), color);
    filledCircleColor(renderer, x, y, 1, 0xFF000000);
}

void Graphics::DrawTexture(int x, int y, int width, int height, float rotation, SDL_Texture* texture) {
    SDL_Rect dstRect = {x - (width / 2), y - (height / 2), width, height};
    float rotationDeg = rotation * 57.2958;
    SDL_RenderCopyEx(renderer, texture, NULL, &dstRect, rotationDeg, NULL, SDL_FLIP_NONE);
}

void Graphics::CloseWindow(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}