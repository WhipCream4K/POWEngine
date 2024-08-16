#include "pch.h"
#include "Allocator.h"

powe::PMRResource* powe::DefaultAllocator::Application{new TrackableAllocator()};
powe::PMRResource* powe::DefaultAllocator::
Engine{new TrackableAllocator()};