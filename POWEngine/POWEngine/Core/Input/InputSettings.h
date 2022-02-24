#pragma once

#include "POWEngine/Window/WindowContext.h"
#include "Key.h"
#include "InputTypes.h"
#include <unordered_set>
#include <unordered_map>

namespace powe
{
	struct ActionMap
	{
		using KeyPoolType = std::unordered_set<Key, KeyHasher>;

		ActionMap(std::vector<ActionKey> keyVec)
			: keys(std::move(keyVec))
			, currentInputEvent(InputEvent::IE_None)
		{
		}

		std::vector<ActionKey> keys;
		KeyPoolType keyPool;
		InputEvent currentInputEvent; // the state of this action map
	};

	struct AxisMap
	{
		using KeyPoolType = std::unordered_map<Key, float, KeyHasher>;

		AxisMap(std::vector<AxisKey> keyVec)
			: keys(std::move(keyVec))
			, currentInputAxis()
		{
		}

		std::vector<AxisKey> keys;
		KeyPoolType keyPool;
		float currentInputAxis; // the state of this axis map
	};

	class InputSettings final
	{
	public:

		using KeyPool = std::unordered_map<Key, InputState, KeyHasher>;

		InputSettings() = default;
		InputSettings(const InputSettings&) = delete;
		InputSettings& operator=(const InputSettings&) = delete;
		InputSettings(InputSettings&&) noexcept = default;
		InputSettings& operator=(InputSettings&&) noexcept = default;
		~InputSettings() = default;

	public:

		void ParseHWMessages(const HardwareMessages& hwMessages);

		[[nodiscard]] float GetInputAxis(const std::string& axisName, uint8_t playerIndex = 0) const;
		[[nodiscard]] InputEvent GetInputEvent(const std::string& actionName, uint8_t playerIndex = 0) const;

		static bool IsKeyBoardPressed(KeyType key);

	private:

		/**
		 * \brief Validate the current input in the MainKeyPool
		 * \param key treat every input key as an AxisKey in case there's a float value passing from the window event
		 * \param isKeyPressed if this key is pressed this frame
		 */
		void ValidateKeyState(const AxisKey& key, bool isKeyPressed);

		static InputEvent InterpretInputState(bool isKeyPressed, const InputEvent& savedInputState);

		void ValidateMouseDelta(const MousePos& mousePos);

		std::unordered_map<std::string, ActionMap> m_ActionKeyMappings;
		std::unordered_map<std::string, AxisMap> m_AxisKeyMappings;

		// Data of every hardware mapping
		KeyPool m_MainKeyPool;

		bool m_ShouldRevalidateMouseValue{};
	};
}



