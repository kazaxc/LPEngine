#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <stdexcept>
#include <array>
#include <vector>
#include <memory>

#include "TestingApp.h"

TestingApp::TestingApp()
{
	LoadGameObjects();
}

TestingApp::~TestingApp()
{

}

void TestingApp::Run()
{
	LPEngine::RenderSystem renderSystem(m_Device, m_Renderer.GetSwapChainRenderPass());

	while (!m_Window.IsClosed())
	{
		SDL_Event event;
		SDL_PollEvent(&event);

		if (event.type == SDL_WINDOWEVENT_RESIZED)
		{
			m_Window.SetWindowResized();
			m_Window.SetWidth(event.window.data1);
			m_Window.SetHeight(event.window.data2);
		}

		if (event.type == SDL_QUIT)
		{
			m_Window.Close();
		}

		if (auto commandBuffer = m_Renderer.BeginFrame())
		{
			m_Renderer.BeginSwapChainRenderPass(commandBuffer);
			renderSystem.RenderGameObjects(commandBuffer, m_GameObjects);
			m_Renderer.EndSwapChainRenderPass(commandBuffer);
			m_Renderer.EndFrame();
		}
	}

	vkDeviceWaitIdle(m_Device.device());
}

std::unique_ptr<LPEngine::Model> createCubeModel(LPEngine::Device& device, glm::vec3 offset)
{
	std::vector<LPEngine::Model::Vertex> vertices{

		// left face (white)
		{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
		{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
		{{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
		{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
		{{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
		{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

		// right face (yellow)
		{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
		{{.5f, .5f, .5f}, {.8f, .8f, .1f}},
		{{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
		{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
		{{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
		{{.5f, .5f, .5f}, {.8f, .8f, .1f}},

		// top face (orange, remember y axis points down)
		{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
		{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
		{{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
		{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
		{{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
		{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

		// bottom face (red)
		{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
		{{.5f, .5f, .5f}, {.8f, .1f, .1f}},
		{{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
		{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
		{{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
		{{.5f, .5f, .5f}, {.8f, .1f, .1f}},

		// nose face (blue)
		{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
		{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
		{{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
		{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
		{{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
		{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

		// tail face (green)
		{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
		{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
		{{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
		{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
		{{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
		{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

	};
	for (auto& v : vertices) {
		v.position += offset;
	}
	return std::make_unique<LPEngine::Model>(device, vertices);
}

void TestingApp::LoadGameObjects()
{
	std::shared_ptr<LPEngine::Model> cubeModel = createCubeModel(m_Device, { 0.f, 0.f, 0.f });

	auto cubeObject = LPEngine::GameObject::CreateGameObject();
	cubeObject.model = cubeModel;
	cubeObject.transform.translation = { 0.f, 0.f, .5f };
	cubeObject.transform.scale = { .5f, .5f, .5f };
	m_GameObjects.push_back(std::move(cubeObject));
}