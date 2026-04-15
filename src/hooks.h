#pragma once

#include <Windows.h>
#include <dxgi.h>

#include "ui.h"

struct IDXGISwapChain_Present {
	static HRESULT WINAPI thunk(IDXGISwapChain* classPtr, UINT SyncInterval, UINT flags)
	{
		Render_ImGui();
		return func(classPtr, SyncInterval, flags);
	}
	static inline REL::Relocation<decltype(thunk)> func;
};

void InstallHooks();