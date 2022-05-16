// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#define SFML_SUPPORT
#define USE_SFML_WINDOW 1
#define USE_SFML_RENDERER 1
#define RELEASE_WITH_LOGINFO 1

// add headers that you want to pre-compile here

#include <array>
#include <string>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <stdexcept>

#include "POWEngine/Core/CustomTypes.h"
#include "POWEngine/Math/Math.h"

#endif //PCH_H
