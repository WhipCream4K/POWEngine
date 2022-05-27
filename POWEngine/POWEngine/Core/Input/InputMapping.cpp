#include "pch.h"
#include "InputMapping.h"

#include "InputSettings.h"

powe::ActionMap::ActionMap(const std::vector<ActionKeyPack>& keys)
{
	for (const auto& key : keys)
	{
		AddKey(key);
	}
}

void powe::ActionMap::AddKey(const ActionKeyPack& key)
{
	if (!keyPool.contains(key.key))
	{
		keyPool.insert(key.key);

		systemKeyHandle.try_emplace(key.key, key.sysKey);
	}
}

void powe::ActionMap::ModifiedKey(const Key& oldKey, const ActionKeyPack& newKey)
{
	const auto findItr{ keyPool.find(oldKey) };
	if(findItr != keyPool.end())
	{
		keyPool.erase(findItr);
		keyPool.insert(newKey.key);

		const auto sysKeyFind{ systemKeyHandle.find(oldKey) };
		systemKeyHandle.erase(sysKeyFind);

		systemKeyHandle.try_emplace(newKey.key, newKey.sysKey);
	}
}

void powe::ActionMap::RemoveKey(const Key& key)
{
	const auto findItr{ keyPool.find(key) };
	if (findItr != keyPool.end())
	{
		keyPool.erase(findItr);

		const auto sysKeyFind{ systemKeyHandle.find(key) };
		systemKeyHandle.erase(sysKeyFind);
	}
}

powe::AxisMap::AxisMap(const std::vector<AxisKeyPack>& axisKeyPacks)
{
	for (const auto& keyPack : axisKeyPacks)
	{
		AddKey(keyPack);
	}
}

void powe::AxisMap::AddKey(const AxisKeyPack& key)
{
	keyPool.try_emplace(key.key, key.scale);
}

void powe::AxisMap::ModifiedKey(const Key& oldKey, const AxisKeyPack& newKey)
{
	const auto findItr{ keyPool.find(oldKey) };
	if (findItr != keyPool.end())
	{
		keyPool.erase(findItr);
		keyPool.try_emplace(newKey.key, newKey.scale);
	}
}

void powe::AxisMap::RemoveKey(const Key& key)
{
	const auto findItr{ keyPool.find(key) };
	if (findItr != keyPool.end())
	{
		keyPool.erase(findItr);
	}
}
