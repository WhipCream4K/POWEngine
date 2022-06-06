
#include <poweCustomtype.h>

#include "PrefabDesc.h"

namespace powe
{
	class WorldEntity;
	class GameObject;
}

class Plate
{

public:

	static SharedPtr<powe::GameObject> Create(powe::WorldEntity&,const PlateDesc& desc);
	
};

