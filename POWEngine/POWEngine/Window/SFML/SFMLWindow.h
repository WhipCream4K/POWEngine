#pragma once

#include "POWEngine/Core/Input/InputStruct.h"
#include "POWEngine/Window/WindowImpl.h"

#if USE_SFML_WINDOW

#include <SFML/Graphics.hpp>

namespace powe
{

	class SFMLWindow : public WindowImpl
	{

	public:

		SFMLWindow(uint32_t width, uint32_t height, const std::string& title, OtherWindowParams others);
		SFMLWindow(uint32_t width, uint32_t height, const std::string& title);
		const HardwareMessages& PollHardwareMessages(bool& shouldEarlyExit, bool& shouldIgnoreInputs) override;
		void PollHardwareMessages(HardwareMessages& hardwareMessages,bool& shouldEarlyExit,bool& shouldIgnoreInputs) override;
		void Resize(uint32_t width, uint32_t height) override;
		void SetTitle(const std::string& title) override;
		const glm::uvec2& GetRelativeMousePos() const override;
		void ClearWindow() override;
		void SetClearColor(const glm::uvec4&) override;
		void Display() override;
		void SetVSync(bool VSync) override;
		const glm::uvec4& GetClearColor() const override;

		sf::RenderWindow& GetRenderWindow() { return m_WndHandle; }

		~SFMLWindow() override;

	private:

		//WindowMessages m_WndMessages;
		HardwareMessages m_HWMessages;
		sf::RenderWindow m_WndHandle;
		glm::uvec2 m_MousePosLastPoll;
		glm::uvec4 m_ClearColor{};
		glm::fvec2 m_DeltaMousePos;
	};

}

#endif



