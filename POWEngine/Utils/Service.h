#pragma once

namespace powe
{

	// CRTP for defining a service
	template<typename T>
	class IService
	{
	public:
		
		static std::string ServiceType() { return typeid(T).name(); }
	};
	
}



