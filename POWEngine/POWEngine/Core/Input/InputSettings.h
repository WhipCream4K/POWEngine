#pragma once

#include "POWEngine/Window/WindowContext.h"
#include "Key.h"
#include "InputParams.h"
#include <unordered_set>
#include <unordered_map>

namespace powe
{
	struct ActionMap
	{
		using KeyPoolType = std::unordered_set<Key, KeyHasher>;

		ActionMap(std::vector<ActionKey> keyVec)
			: keys(std::move(keyVec))
		{
		}

		std::vector<ActionKey> keys;
		KeyPoolType keyPool;
	};

	struct AxisMap
	{
		using KeyPoolType = std::unordered_map<Key, float, KeyHasher>;

		AxisMap(std::vector<AxisKey> keyVec)
			: keys(std::move(keyVec))
		{
		}

		std::vector<AxisKey> keys;
		KeyPoolType keyPool;
	};

	class InputSettings final
	{
	public:

		using KeyPool = std::unordered_map<Key, InputState, KeyHasher>;

		InputSettings() = default;
		InputSettings(const InputSettings&) = delete;
		InputSettings& operator=(const InputSettings&) = delete;
		InputSettings(InputSettings&&) = default;
		InputSettings& operator=(InputSettings&&) = delete;
		~InputSettings() = default;

	public:

		void ParseWndMessages(
			const WindowMessages& winMessages, 
			const std::array<WndMessageHWIdx, MinimumWindowEventCnt>& inputHardWareIdx);

		static bool IsKeyBoardPressed(KeyType key);

	private:

		void ParseKeyBoardKey(uint8_t eventId,const Key& key);
		void ParseMouseKey(uint8_t eventId, const Key& key);

		std::unordered_map<std::string, ActionMap> m_ActionKeyMappings;
		std::unordered_map<std::string, AxisMap> m_AxisKeyMappings;

		// Data of every hardware mapping
		KeyPool m_MainKeyPool;
	};
}



