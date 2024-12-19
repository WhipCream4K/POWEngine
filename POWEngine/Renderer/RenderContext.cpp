#include "pch.h"
#include "RenderContext.h"

#include "Platform/common/GL/OpenGLRenderContext.h"

using namespace powe;

SharedPtr<RenderContext> RenderContext::CreateDefault() noexcept
{
    AllocatorContext context{};
    auto* upStream{context.GetResource()};
    return AllocateShared<OpenGLRenderContext>(upStream);
}
