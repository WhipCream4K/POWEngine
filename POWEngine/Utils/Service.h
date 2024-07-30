#pragma once

namespace powe
{
	/**
	 * \brief Just a base class for all Service types
	 */

	class IService
	{
	public:

		IService() = default;
		IService(const IService&) = delete;
		IService& operator=(const IService&) = delete;
		IService(IService&&) = default;
		IService& operator=(IService&&) = default;
		virtual ~IService() = default;

	};


}



