#include <stdexcept>
#include <array>

#include "TestingApp.h"

TestingApp::TestingApp()
{
	CreatePipelineLayout();
	CreatePipeline();
	CreateCommandBuffers();
}

TestingApp::~TestingApp()
{
	vkDestroyPipelineLayout(m_Device.device(), m_PipelineLayout, nullptr);
}


void TestingApp::Run()
{
	while (!m_Window.IsClosed())
	{
		SDL_Event event;
		SDL_PollEvent(&event);

		if (event.type == SDL_QUIT)
		{
			m_Window.Close();
		}

		DrawFrame();
	}

	vkDeviceWaitIdle(m_Device.device());
}

void TestingApp::CreatePipelineLayout()
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;

	if (vkCreatePipelineLayout(m_Device.device(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
	{
		LPEngine::Logger::Log(LPEngine::LogLevel::ERROR, "Failed to create pipeline layout!");
		throw std::runtime_error("Failed to create pipeline layout!");
	}
}

void TestingApp::CreatePipeline()
{
	auto pipelineConfig = LPEngine::Pipeline::defaultPipelineConfigInfo(m_SwapChain.width(), m_SwapChain.height());
	pipelineConfig.renderPass = m_SwapChain.getRenderPass();
	pipelineConfig.pipelineLayout = m_PipelineLayout;
	m_Pipeline = std::make_unique<LPEngine::Pipeline>(m_Device, "shaders/basic.vert.spv", "shaders/basic.frag.spv", pipelineConfig);
}

void TestingApp::CreateCommandBuffers()
{
	m_CommandBuffers.resize(m_SwapChain.imageCount());

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

	for (int i = 0; i < m_CommandBuffers.size(); i++)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(m_CommandBuffers[i], &beginInfo) != VK_SUCCESS)
		{
			LPEngine::Logger::Log(LPEngine::LogLevel::ERROR, "Failed to begin recording command buffer!");
			throw std::runtime_error("Failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_SwapChain.getRenderPass();
		renderPassInfo.framebuffer = m_SwapChain.getFrameBuffer(i);
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_SwapChain.getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(m_CommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		m_Pipeline->Bind(m_CommandBuffers[i]);
		vkCmdDraw(m_CommandBuffers[i], 3, 1, 0, 0);

		vkCmdEndRenderPass(m_CommandBuffers[i]);
		if (vkEndCommandBuffer(m_CommandBuffers[i]) != VK_SUCCESS)
		{
			LPEngine::Logger::Log(LPEngine::LogLevel::ERROR, "Failed to record command buffer!");
			throw std::runtime_error("Failed to record command buffer!");
		}
	}
}

void TestingApp::DrawFrame()
{
	uint32_t imageIndex;
	auto result = m_SwapChain.acquireNextImage(&imageIndex);

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		LPEngine::Logger::Log(LPEngine::LogLevel::ERROR, "Failed to acquire next image!");
		throw std::runtime_error("Failed to acquire next image!");
	}

	result = m_SwapChain.submitCommandBuffers(&m_CommandBuffers[imageIndex], &imageIndex);

	if (result != VK_SUCCESS)
	{
		LPEngine::Logger::Log(LPEngine::LogLevel::ERROR, "Failed to submit command buffers!");
		throw std::runtime_error("Failed to submit command buffers!");
	}

	/*
	result = m_SwapChain.presentImage(imageIndex);

	if (result != VK_SUCCESS)
	{
		LPEngine::Logger::Log(LPEngine::LogLevel::ERROR, "Failed to present image!");
		throw std::runtime_error("Failed to present image!");
	}
	*/
}