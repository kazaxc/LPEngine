#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <stdexcept>
#include <array>
#include <vector>
#include <memory>
#include <chrono>

#include "TestingApp.h"

constexpr auto ITERATIONS = 10000;

TestingApp::TestingApp()
{
	auto startTime = std::chrono::high_resolution_clock::now();
	LoadGameObjects();
	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
	auto durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	LPEngine::Logger::Log(LPEngine::LogLevel::INFO, "Time to load game objects: " + std::to_string(duration.count()) + "us");
	LPEngine::Logger::Log(LPEngine::LogLevel::INFO, "Time to load game objects: " + std::to_string(durationMs.count()) + "ms");

	startTime = std::chrono::high_resolution_clock::now();
	LoadEntites();
	endTime = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
	durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	LPEngine::Logger::Log(LPEngine::LogLevel::INFO, "Time to load entities: " + std::to_string(duration.count()) + "us");
	LPEngine::Logger::Log(LPEngine::LogLevel::INFO, "Time to load entities: " + std::to_string(durationMs.count()) + "ms");
}

TestingApp::~TestingApp()
{

}

void TestingApp::Run()
{
	LPEngine::RenderSystem renderSystem(m_Device, m_Renderer.GetSwapChainRenderPass());
	LPEngine::Camera camera{};
	//camera.SetViewDirection(glm::vec3(0.f, 0.f, 0.f), glm::vec3(.5f, 0.f, 1.f));
	camera.SetViewTarget(glm::vec3(0.f, 2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

	auto currentTime = std::chrono::high_resolution_clock::now();

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

		auto newTime = std::chrono::high_resolution_clock::now();
		float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
		currentTime = newTime;

		float aspect = m_Renderer.GetAspectRatio();
		// Option to use orthographic projection
		//camera.SetOrthographicMode(-aspect, aspect, -1, 1, -1, 1);

		// Option to use perspective projection
		camera.SetPerspectiveMode(glm::radians(50.f), aspect, 0.1f, 10000.f);

		if (auto commandBuffer = m_Renderer.BeginFrame())
		{
			m_Renderer.BeginSwapChainRenderPass(commandBuffer);
			renderSystem.RenderGameObjects(commandBuffer, m_GameObjects, camera);
			//renderSystem.RenderEntities(commandBuffer, m_EntityManager, camera);
			m_Renderer.EndSwapChainRenderPass(commandBuffer);
			m_Renderer.EndFrame();
		}
	}

	vkDeviceWaitIdle(m_Device.device());
}

std::unique_ptr<LPEngine::Model> createCubeModel(LPEngine::Device& device, glm::vec3 offset)
{
	std::vector<LPEngine::Model::Vertex> vertices
	{

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
	for (auto& v : vertices)
	{
		v.position += offset;
	}
	return std::make_unique<LPEngine::Model>(device, vertices);
}

void TestingApp::LoadGameObjects()
{
	std::shared_ptr<LPEngine::Model> cubeModel = createCubeModel(m_Device, { 0.f, 0.f, 0.f });

	for (int i = 0; i < ITERATIONS; i++)
	{
		auto cubeObject = LPEngine::GameObject::CreateGameObject();
		cubeObject.model = cubeModel;
		cubeObject.transform.translation = { 0.f , 0.f, i };
		cubeObject.transform.scale = { .5f, .5f, .5f };
		m_GameObjects.push_back(std::move(cubeObject));
	}
}

void TestingApp::LoadEntites()
{
	std::shared_ptr<LPEngine::Model> cubeModel = createCubeModel(m_Device, { 0.f, 0.f, 0.f });

	for (int i = 0; i < ITERATIONS; i++)
	{
		// Create entity
		LPEngine::Entity entity;

		// Add components to entity
		LPEngine::ModelComponent model;
		model.model = cubeModel;

		LPEngine::TransformComponent transform;
		transform.translation = { 0.f, 0.f, 2.5f };
		transform.scale = { .5f, .5f, .5f };

		LPEngine::ColourComponent colour;
		colour.colour = { };

		entity.AddComponent(&model);
		entity.AddComponent(&transform);
		entity.AddComponent(&colour);

		// Add entity to entity manager
		m_EntityManager.AddEntity(entity);
	}
}