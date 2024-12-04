#include "pch.h"
#include "UsePMR.h"

using namespace powe;

UsePMR::UsePMR(const SharedPtr<PMRResource>& upStream)
    : m_MemResource(upStream)
{
}