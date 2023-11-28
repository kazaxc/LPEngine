#include <iostream>
#include <string>
#include <cerrno>

#include "../../include/rendering/Window.h"
#include "../../include/core/Logger.h"

namespace LPEngine
{
    Window::Window(const std::string& title, int width, int height)
        : m_Title(title), m_Width(width), m_Height(height)
    {
        if (!Init())
        {
            Logger::Log(LogLevel::FATAL, "Failed to initialize window: ", SDL_GetError());
            m_IsClosed = true;
        }
        else
        {
            Logger::Log(LogLevel::INFO, "Window initialized");
            m_IsClosed = false;
        }

        Update();
    }

    Window::~Window()
    {
        SDL_DestroyWindow(m_Window);
        SDL_Quit();
    }

    bool Window::Init()
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            Logger::Log(LogLevel::ERROR, "Failed to initialize SDL: ", SDL_GetError());
            return false;
        }

        m_Window = SDL_CreateWindow(m_Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Width, m_Height, SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN);
        if (m_Window == nullptr)
        {
            Logger::Log(LogLevel::ERROR, "Failed to create window: ", SDL_GetError());
            return false;
        }

        m_Surface = SDL_GetWindowSurface(m_Window);
        if (m_Surface == nullptr)
        {
            Logger::Log(LogLevel::ERROR, "Failed to get window surface: ", SDL_GetError());
            return false;
        }

        // Load window icon and set it
        SDL_Surface* icon = IMG_Load("assets/icon.png");
        if (icon == nullptr)
        {
            Logger::Log(LogLevel::ERROR, "Failed to load window icon: ", SDL_GetError());
            return false;
        }

        SDL_SetWindowIcon(m_Window, icon);

        return true;
    }

    void Window::Update()
    {
        SDL_Event e;
        while (!m_IsClosed)
        {   
            while (SDL_PollEvent(&e))
            {
                if (e.type == SDL_QUIT)
                {
                    m_IsClosed = true;
                }
            }
        }

        if (m_IsClosed)
        {
            Logger::Log(LogLevel::INFO, "Window closed!");
        }
    }
}