#pragma once

#include <string>
#include <memory>
#include <vector>

#include "LPEngine.h"

class TestingApp
{
public:
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;

	TestingApp();
	~TestingApp();

	TestingApp(const TestingApp&) = delete;
	void operator=(const TestingApp&) = delete;

	void Run();

private:
	void CreatePipelineLayout();
	void CreatePipeline();
	void CreateCommandBuffers();
	void DrawFrame();

	LPEngine::Window m_Window{ "LPEngine", WIDTH, HEIGHT };
	LPEngine::Device m_Device{ m_Window };
	LPEngine::SwapChain m_SwapChain{ m_Device, m_Window.GetExtent() };
	std::unique_ptr<LPEngine::Pipeline> m_Pipeline;
	VkPipelineLayout m_PipelineLayout;
	std::vector<VkCommandBuffer> m_CommandBuffers;
};