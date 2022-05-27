#pragma once

#include "POWEngine/Window/WindowContext.h"
#include "Key.h"
#include "ListsOfKeys.h"
#include <unordered_set>
#include <unordered_map>
#include <array>

#include "InputVars.h"
#include "InputMapping.h"

namespace powe
{
	struct KeyData
	{
		Key key{};
		uint8_t playerIndex{};
		float axisValue{};
		bool isDown{};
	};

	class InputSettings final
	{
	public:
		using KeyPool = std::unordered_map<Key, KeyState, KeyHasher>;

		InputSettings() = default;
		InputSettings(const InputSettings&) = delete;
		InputSettings& operator=(const InputSettings&) = delete;
		InputSettings(InputSettings&&) noexcept = default;
		InputSettings& operator=(InputSettings&&) noexcept = default;
		~InputSettings() = default;

	public:

		void ParseHWMessages(const HardwareMessages& hwMessages);

		[[nodiscard]] float GetInputAxis(const std::string& axisName, uint8_t playerIndex = 0) const;

		const std::unordered_map<std::string, ActionMap>& GetActionMap() const { return m_ActionKeyMappings; }
		const std::unordered_map<std::string, AxisMap>& GetAxisMap() const { return m_AxisKeyMappings; }

		void AddActionMapping(const std::string& name,const std::vector<ActionMap::ActionKeyPack>& keyPacks);
		void AddAxisMapping(const std::string& name, const std::vector<AxisMap::AxisKeyPack>& keyPacks);

		static bool IsKeyBoardPressed(KeyType key);

		const KeyPool& GetCurrentKeyState(uint8_t playerIndex) const { return m_MainKeyPool[playerIndex]; }
		const SysKeyType GetThisFrameSysKey() const { return m_CurrentFrameSystemKey; }

	private:

		static InputEvent InterpretInputState(bool isKeyPressed, const InputEvent& savedInputState);

		void AddKeyToMainKeyPool(const Key& key);
		void UpdateMainKeyPool();

		/**
		* \brief Evaluate the current input in the MainKeyPool
		* \param key treat every input key as an AxisKey in case there's a float value passing from the window event
		* \param isKeyPressed if this key is pressed this frame
		*/
		InputEvent EvaluateMainKeyPool(const KeyData& inKey, uint8_t sysKeys);
		void UpdateAxisMapping(const KeyData& inKey,InputEvent thisFrameEvent);

		void ValidateMouseDelta(const MousePos& mousePos);

		std::unordered_map<std::string, ActionMap> m_ActionKeyMappings;
		std::unordered_map<std::string, AxisMap> m_AxisKeyMappings;
		SysKeyType m_LastFrameSystemKey{};
		SysKeyType m_CurrentFrameSystemKey{};
			
		// Data of every hardware mapping
		//KeyPool m_MainKeyPool;
		std::array<KeyPool, MAXPLAYER> m_MainKeyPool;

		bool m_ShouldRevalidateMouseValue{};
	};
}



