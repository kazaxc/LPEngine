#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <string>

#include "../core/Defines.h"

namespace LPEngine
{
    class LPENGINE_API Window
    {
        public:
            Window(const std::string& title, int width, int height);
            ~Window();

            void Update();
            void Clear();

            inline bool IsClosed() const { return m_IsClosed; }

        private:
            bool Init();
            bool m_IsClosed;

            std::string m_Title;
            int m_Width;
            int m_Height;

            SDL_Window* m_Window;
            SDL_Surface* m_Surface;
    };
}