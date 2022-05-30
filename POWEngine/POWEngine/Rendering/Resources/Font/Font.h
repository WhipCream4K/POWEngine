#pragma once

namespace powe
{
	class FontImpl;
	class Font
	{
	public:

		Font(const std::string& filePath);
		~Font();

		template<typename T>
		T* GetTypeInstance() const;

	private:

		OwnedPtr<FontImpl> m_FontImpl;
			
	};

	template <typename T>
	T* Font::GetTypeInstance() const
	{
		return static_cast<T*>(m_FontImpl.get());
	}
}



