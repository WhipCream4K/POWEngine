#pragma once

#include "POWEngine/Core/Components/BaseComponent.h"

namespace powe
{
	class TextureImpl;
	class Texture
	{
	public:

		Texture() = default;
		explicit Texture(const std::string& filePath);
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;
		Texture(Texture&&) = delete;
		Texture& operator=(Texture&&) = delete;
		~Texture();

		template<typename T>
		T* GetTypeInstance() const;

	private:

		OwnedPtr<TextureImpl> m_TextureImpl;
	};

	template <typename T>
	T* Texture::GetTypeInstance() const
	{
		return dynamic_cast<T*>(m_TextureImpl.get());
	}
}


