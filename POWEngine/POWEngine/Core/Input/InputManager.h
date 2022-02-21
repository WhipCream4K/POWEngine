#pragma once

namespace powe
{
	/**
	 * \brief Implementation of OS specific or framework specific input handling
	 */
	class InputManager final
	{
	public:

		InputManager() = default;
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


