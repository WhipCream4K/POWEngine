#pragma once

#include "Key.h"
#include "ListsOfKeys.h"
#include <unordered_map>
#include <array>

#include "InputVars.h"
#include "InputMapping.h"
#include "InputStruct.h"

namespace powe
{
	struct GamepadData;
	enum class EventType;
	struct MousePos;
	struct KeyState;
	struct HardwareMessages;
	struct HardwareData;
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
		bool GetInputAction(const std::string& actionName, InputEvent targetEvent, uint8_t playerIndex = 0) const;

		const std::unordered_map<std::string, ActionMap>& GetActionMap() const { return m_ActionKeyMappings; }
		const std::unordered_map<std::string, AxisMap>& GetAxisMap() const { return m_AxisKeyMappings; }

		void AddActionMapping(const std::string& name,const std::vector<ActionMap::ActionKeyPack>& keyPacks);
		void AddAxisMapping(const std::string& name, const std::vector<AxisMap::AxisKeyPack>& keyPacks);

		static bool IsKeyBoardPressed(KeyType key);

		const KeyPool& GetCurrentKeyState(uint8_t playerIndex) const { return m_MainKeyPool[playerIndex]; }
		const SysKeyType GetThisFrameSysKey() const { return m_CurrentFrameSystemKey; }
		
		glm::fvec2 GetMouseEnginePos() const;
		const glm::fvec2& GetFocusWindowDimension() const {return m_FocusWindowDimension;}
		const glm::fvec2& GetMouseAbsolutePos() const {return m_MouseAbsoluteAxisData;}

		void SetAssignFirstControllerToNextPlayer(bool state);

	private:

		// static InputEvent InterpretInputState(bool isKeyPressed, const InputEvent& savedInputState);
		void ProcessHWData(const HardwareMessages& hardwareMessages,int id);

		void AddKeyToMainKeyPool(const Key& key);
		void UpdateMainKeyPool();

		/**
		* \brief Evaluate the current input in the MainKeyPool
		* \param key treat every input key as an AxisKey in case there's a float value passing from the window event
		* \param isKeyPressed if this key is pressed this frame
		*/
		InputEvent EvaluateMainKeyPool(const KeyData& inKey);
		void UpdateAxisMapping(const KeyData& inKey,InputEvent thisFrameEvent);

		std::unordered_map<std::string, ActionMap> m_ActionKeyMappings;
		std::unordered_map<std::string, AxisMap> m_AxisKeyMappings;
		SysKeyType m_LastFrameSystemKey{};
		SysKeyType m_CurrentFrameSystemKey{};
			
		// Data of every hardware mapping
		//KeyPool m_MainKeyPool;
		std::array<KeyPool, MAXPLAYER> m_MainKeyPool;

		glm::fvec2 m_MouseAbsoluteAxisData{};
		glm::fvec2 m_FocusWindowDimension{};
		
		
		bool m_ShouldRevalidateMouseValue{};
		bool m_AssignFirstControllerToNextIndex{};
	};
}



