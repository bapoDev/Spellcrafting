#pragma once

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

#ifdef SKYRIM_AE
#define OFFSET(se, ae) ae
#else
#define OFFSET(se, ae) se
#endif