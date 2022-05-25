#pragma once

#include "POWEngine/Singleton/ThreadSafeSingleton.h"
#include <any>
#include <unordered_map>

class AssetManager : public powe::ThreadSafeSingleton<AssetManager>
{
	friend class powe::ThreadSafeSingleton<AssetManager>;

public:

	template<typename T>
	void RegisterAsset(const std::string& name,const SharedPtr<T>& item);

	template<typename T>
	SharedPtr<T> GetAsset(const std::string& name) const;

private:

	std::unordered_map<std::string, std::any> m_Assets;

};

template <typename T>
void AssetManager::RegisterAsset(const std::string& name, const SharedPtr<T>& item)
{
	std::any asset{ std::make_any<SharedPtr<T>>(item) };
	m_Assets.try_emplace(name, asset);
}

template <typename T>
SharedPtr<T> AssetManager::GetAsset(const std::string& name) const
{
	if(m_Assets.contains(name))
	{
		return std::any_cast<SharedPtr<T>>(m_Assets.at(name));
	}

	return nullptr;
}

