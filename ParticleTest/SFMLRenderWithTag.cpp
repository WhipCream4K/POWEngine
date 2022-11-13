#include "SFMLRenderWithTag.h"

#include "UserComponents.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Renderer/Components/SFML/SFMLDrawComponent.h"

SFMLRenderWithTag::SFMLRenderWithTag()
{
    DefineSystemKeys<powe::Transform2D,powe::SFMLDrawComponent,RenderTag>();
}
