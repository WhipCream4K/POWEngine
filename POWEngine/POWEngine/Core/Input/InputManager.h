#pragma once

// NOTE: For now I'm going to use window events rather than querying key state of the user
// because the window polling event is sufficient enough

namespace powe
{
	struct HardwareMessages;
	/**
	 * \brief Implementation of OS specific or framework specific input handling
	 * I don't know wether to use window sensitive events or key events
	 */
	class InputManager final
	{
	public:

		InputManager();

		void PollHardWareMessages(HardwareMessages& hwMessages,float deltaTime) const;

		InputManager(const InputManager&) = delete;
		InputManager& operator=(const InputManager&) = delete;
		InputManager(InputManager&&) = default;
		InputManager& operator=(InputManager&&) = default;
		~InputManager();

	private:

		class XInputImpl;
		OwnedPtr<XInputImpl> m_XInputImpl;
	};
	
}


