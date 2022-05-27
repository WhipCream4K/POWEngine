#pragma once
//#include "InputStruct.h"
#include "InputVars.h"
#include "Key.h"
#include "ListsOfKeys.h"

#include "POWEngine/Core/CustomTypes.h"


namespace powe
{
	struct KeyData;
	struct ActionMap
	{
		using KeyPoolType = std::unordered_set<Key, KeyHasher>;
		using SysKeyHandle = std::unordered_map<Key, SysKeyType, KeyHasher>;

		struct ActionKeyPack
		{
			ActionKeyPack(InputDevice device, KeyType keyCode,SysKeyType inSysKey = 0)
				: key(device,keyCode)
				, sysKey(inSysKey)
			{
			}

			Key key{};
			SysKeyType sysKey{};
		};

		explicit ActionMap(const std::vector<ActionKeyPack>& keys);

		void AddKey(const ActionKeyPack& key);
		void ModifiedKey(const Key& oldKey, const ActionKeyPack& newKey);
		void RemoveKey(const Key& key);

		KeyPoolType keyPool{};
		SysKeyHandle systemKeyHandle{};
		//std::array<InputEvent, MAXPLAYER>  currentInputEvent{}; // the state of this action map
	};

	struct AxisMap
	{
		using KeyPoolType = std::unordered_map<Key, float, KeyHasher>;

		AxisMap() = default;

		struct AxisKeyPack
		{
			AxisKeyPack(InputDevice device, KeyType keyCode, float inScale)
				: key(device, keyCode)
				, scale(inScale)
			{
			}

			Key key{};
			float scale{};
		};

		explicit AxisMap(const std::vector<AxisKeyPack>& axisKeyPacks);

		void AddKey(const AxisKeyPack& key);
		void ModifiedKey(const Key& oldKey, const AxisKeyPack& newKey);
		void RemoveKey(const Key& key);


		KeyPoolType keyPool{};
		std::array<float, MAXPLAYER> currentSumAxis{};
	};
}