#pragma once

#include <vector>
#include <memory>
#include <cassert>

#include "../core/Defines.h"
#include "Window.h"
#include "Device.h"
#include "SwapChain.h"


namespace LPEngine
{
	class LPENGINE_API Renderer
	{
	public:
		Renderer(Window& window, Device& device);
		~Renderer();

		Renderer(const Renderer&) = delete;
		void operator=(const Renderer&) = delete;

		VkRenderPass GetSwapChainRenderPass() const { return m_SwapChain->getRenderPass(); }
		bool IsFrameInProgress() const { return m_FrameStarted; }

		VkCommandBuffer GetCurrentCommandBuffer() const
		{
			assert(m_FrameStarted && "Cannot get command buffer when frame not started");
			return m_CommandBuffers[m_CurrentFrameIndex];
		}

		int GetFrameIndex() const
		{
			assert(m_FrameStarted && "Cannot get frame index when frame not started");
			return m_CurrentFrameIndex;
		}

		VkCommandBuffer BeginFrame();
		void EndFrame();
		void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		void CreateCommandBuffers();
		void FreeCommandBuffers();
		void RecreateSwapChain();

		Window& m_Window;
		Device& m_Device;
		std::unique_ptr<SwapChain> m_SwapChain;
		std::vector<VkCommandBuffer> m_CommandBuffers;

		uint32_t m_CurrentImageIndex;
		int m_CurrentFrameIndex{ 0 };
		bool m_FrameStarted{ false };
	};
}