// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here

#include <array>
#include <string>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <stdexcept>
#include <ranges>

#include "Core/CustomTypes.h"
#include "Core/Application/Application.h"
#include "Core/Window/WindowManager.h"
#include "Core/Window/Window.h"
#include "Core/ModulesManager.h"
#include "Core/Memory/MemoryManager.h"
#include "Core/Memory/Allocator.h"
#include "Core/Memory/AllocatorContext.h"
#include "Logger/Logger.h"
#include "Utils/Utils.h"

#endif //PCH_H
