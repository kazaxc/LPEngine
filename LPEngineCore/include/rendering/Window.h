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

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		SDL_Window* GetWindow() { return m_Window; }
		VkExtent2D GetExtent() { return { static_cast<uint32_t>(m_Width), static_cast<uint32_t>(m_Height) }; }

		bool IsClosed() const { return m_IsClosed; }

		bool WindowResized() { return m_framebufferResized; }
		void ResetWindowResized() { m_framebufferResized = false; }
		void SetWindowResized() { m_framebufferResized = true; }

		void SetWidth(int width) { m_Width = width; }
		void SetHeight(int height) { m_Height = height; }


		void Close() { m_IsClosed = true; }

		void CreateSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		bool Init();
		bool m_IsClosed;

		std::string m_Title;
		int m_Width;
		int m_Height;

		bool m_framebufferResized = false;

		SDL_Window* m_Window;
	};
}