#pragma once

namespace powe
{
	class FontImpl
	{
	public:

		FontImpl() = default;
		FontImpl(const std::string& path);

		virtual ~FontImpl() = default;
	};
}



