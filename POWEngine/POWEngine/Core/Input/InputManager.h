#pragma once

// NOTE: For now I'm going to use window events rather than querying key state of the user
// because the window polling event is sufficient enough

namespace powe
{
	/**
	 * \brief Implementation of OS specific or framework specific input handling
	 * I don't know wether to use window sensitive events or key events
	 */
	class InputManager final
	{
	public:

		InputManager() = default;

		void GetUserInput();

		InputManager(const InputManager&) = delete;
		InputManager& operator=(const InputManager&) = delete;
		InputManager(InputManager&&) = default;
		InputManager& operator=(InputManager&&) = default;
		~InputManager() = default;

	private:

		// keyboard lookup
		std::unordered_map<uint32_t, bool> m_KeyboardTable;
	};
	
}


