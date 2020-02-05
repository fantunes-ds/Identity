#pragma once
#include <Rendering/Renderer.h>

#define GFX_EXCEPT_NOINFO(hr) Engine::Rendering::Renderer::HrException(__LINE__, __FILE__, (hr))
#define GFX_THROW_NOINFO(hrcall) if (FAILED(hr = (hrcall))) throw Engine::Rendering::Renderer::HrException(__LINE__, __FILE__, hr)

#define GFX_EXCEPT(hr) Engine::Rendering::Renderer::HrException(__LINE__, __FILE__, (hr))
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Engine::Rendering::Renderer::DeviceException(__LINE__, __FILE__, (hr))
#define GFX_THROW_INFO_ONLY(call) (call)