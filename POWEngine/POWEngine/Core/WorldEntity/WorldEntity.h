#pragma once

#include "POWEngine/Core/Input/InputSettings.h"
#include "PipelineLayer.h"

namespace powe
{
	class SystemBase;
	using SystemPipeline = std::array<std::vector<SharedPtr<SystemBase>>, size_t(PipelineLayer::Count)>;

	class WorldEntity final
	{
	public:

		WorldEntity();
		WorldEntity(const WorldEntity&) = delete;
		WorldEntity& operator=(const WorldEntity&) = delete;
		WorldEntity(WorldEntity&&) noexcept = default;
		WorldEntity& operator=(WorldEntity&&) noexcept = default;
		~WorldEntity() = default;

	public:

		InputSettings& GetInputSettings() { return  m_InputSettings; }

	private:

		InputSettings m_InputSettings;
		SystemPipeline m_SystemPipeline;
	};
}


