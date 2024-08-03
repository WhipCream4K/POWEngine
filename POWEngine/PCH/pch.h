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

//#include "../Core/CustomTypes.h"
//#include "../Math/Math.h"
//#include "../Utils/MacroDef.h"

#include "../POWEngine/Core/CustomTypes.h"

#define USE_GLFW_WINDOW 1

// enum
// {
//     USE_GLFW_WINDOW = 1,
//     // USE_SFML_WINDOW = 1,
//     // USE_SFML_RENDERER = 1,
//     RELEASE_WITH_LOGINFO = 1,
//     USE_IMGUI = 1
// };

#endif //PCH_H
