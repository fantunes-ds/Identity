#pragma once

#ifdef ENGINE
#define API_ENGINE __declspec(dllexport)
#else
#define API_ENGINE __declspec(dllimport)
#endif