#include <stdexcept>
#include <array>
#include <vector>
#include <memory>

#include "../../include/rendering/Renderer.h"
#include "../../include/core/Logger.h"

namespace LPEngine
{
	Renderer::Renderer(Window& window, Device& device) : m_Window(window), m_Device(device)
	{
		RecreateSwapChain();
		CreateCommandBuffers();
	}

	Renderer::~Renderer()
	{
		FreeCommandBuffers();
	}


	void Renderer::RecreateSwapChain()
	{
		auto extent = m_Window.GetExtent();
		while (extent.width == 0 || extent.height == 0)
		{
			extent = m_Window.GetExtent();
			SDL_PollEvent(nullptr);
		}

		vkDeviceWaitIdle(m_Device.device());

		if (m_SwapChain == nullptr)
		{
			m_SwapChain = std::make_unique<LPEngine::SwapChain>(m_Device, extent);
		}
		else
		{
			std::shared_ptr<LPEngine::SwapChain> oldSwapChain = std::move(m_SwapChain);
			m_SwapChain = std::make_unique<LPEngine::SwapChain>(m_Device, extent, oldSwapChain);

			if (!oldSwapChain->compareSwapFormats(*m_SwapChain.get()))
			{
				LPEngine::Logger::Log(LPEngine::LogLevel::ERROR, "Swap chain image or depth has changed!");
				throw std::runtime_error("Swap chain image or depth has changed!");
			}
		}

		//CreatePipeline(); // No longer needed just here for reference
	}

	void Renderer::CreateCommandBuffers()
	{
		m_CommandBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_Device.getCommandPool();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());

		if (vkAllocateCommandBuffers(m_Device.device(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
		{
			LPEngine::Logger::Log(LPEngine::LogLevel::ERROR, "Failed to allocate command buffers!");
			throw std::runtime_error("Failed to allocate command buffers!");
		}
	}

	void Renderer::FreeCommandBuffers()
	{
		vkFreeCommandBuffers(m_Device.device(), m_Device.getCommandPool(), static_cast<uint32_t>(m_CommandBuffers.size()), m_CommandBuffers.data());
		m_CommandBuffers.clear();
	}

	VkCommandBuffer Renderer::BeginFrame()
	{
		assert(!m_FrameStarted && "Cannot call BeginFrame while frame is in progress");

		auto result = m_SwapChain->acquireNextImage(&m_CurrentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapChain();
			return nullptr;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			LPEngine::Logger::Log(LPEngine::LogLevel::ERROR, "Failed to acquire next image!");
			throw std::runtime_error("Failed to acquire next image!");
		}

		m_FrameStarted = true;

		auto commandBuffer = GetCurrentCommandBuffer();
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		{
			LPEngine::Logger::Log(LPEngine::LogLevel::ERROR, "Failed to begin recording command buffer!");
			throw std::runtime_error("Failed to begin recording command buffer!");
		}

		return commandBuffer;
	}

	void Renderer::EndFrame()
	{
		assert(m_FrameStarted && "Cannot call EndFrame while frame is not in progress");

		auto commandBuffer = GetCurrentCommandBuffer();

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		{
			LPEngine::Logger::Log(LPEngine::LogLevel::ERROR, "Failed to record command buffer!");
			throw std::runtime_error("Failed to record command buffer!");
		}

		auto result = m_SwapChain->submitCommandBuffers(&commandBuffer, &m_CurrentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_Window.WindowResized())
		{
			m_Window.ResetWindowResized();
			RecreateSwapChain();
		}
		else if (result != VK_SUCCESS)
		{
			LPEngine::Logger::Log(LPEngine::LogLevel::ERROR, "Failed to submit command buffers!");
			throw std::runtime_error("Failed to submit command buffers!");
		}

		m_FrameStarted = false;
		m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
	}

	void Renderer::BeginSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(m_FrameStarted && "Cannot begin render pass when frame is not in progress");
		assert(commandBuffer == GetCurrentCommandBuffer() && "Can only begin render pass for command buffer that is being recorded");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_SwapChain->getRenderPass();
		renderPassInfo.framebuffer = m_SwapChain->getFrameBuffer(m_CurrentImageIndex);
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_SwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_SwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(m_SwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, m_SwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void Renderer::EndSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(m_FrameStarted && "Cannot end render pass when frame is not in progress");
		assert(commandBuffer == GetCurrentCommandBuffer() && "Can only end render pass for command buffer that is being recorded");

		vkCmdEndRenderPass(commandBuffer);
	}
}