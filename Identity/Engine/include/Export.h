#pragma once

#pragma warning(disable:4251)

#ifdef ENGINE
#define API_ENGINE __declspec(dllexport)
#else
#define API_ENGINE __declspec(dllimport)
#endif