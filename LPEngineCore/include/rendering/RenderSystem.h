#pragma once

#include <memory>
#include <vector>

#include "../core/Defines.h"
#include "../core/Logger.h"
#include "Device.h"
#include "../core/GameObject.h"
#include "Pipeline.h"
#include "Camera.h"

namespace LPEngine
{
	class LPENGINE_API RenderSystem
	{
	public:
		RenderSystem(Device& device, VkRenderPass renderPass);
		~RenderSystem();

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;

		void RenderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects, const Camera& camera);

	private:
		void CreatePipelineLayout();
		void CreatePipeline(VkRenderPass renderPass);

		Device& m_Device;
		std::unique_ptr<Pipeline> m_Pipeline;
		VkPipelineLayout m_PipelineLayout;
	};
}