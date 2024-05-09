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
	void LoadGameObjects();
	void LoadEntites();

	LPEngine::Window m_Window{ "LPEngine", WIDTH, HEIGHT };
	LPEngine::Device m_Device{ m_Window };
	LPEngine::Renderer m_Renderer{ m_Window, m_Device };
	std::vector<LPEngine::GameObject> m_GameObjects;
	LPEngine::ECSManager m_EntityManager;
};