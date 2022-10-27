#pragma once

namespace powe
{
	class TextureImpl
	{
	public:

		TextureImpl() = default;
		TextureImpl(const std::string&);
		virtual void SetRepeated(bool isRepeated) = 0;
		virtual ~TextureImpl() = default;
	};
}


