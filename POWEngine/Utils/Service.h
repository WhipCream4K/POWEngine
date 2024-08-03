#pragma once

namespace powe
{

	// CRTP for defining a service
	template<typename T>
	class IService
	{
	public:
		
		std::string ServiceType() const { return typeid(T).name(); }
	};
	
}



