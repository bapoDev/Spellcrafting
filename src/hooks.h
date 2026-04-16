#pragma once

#include <Windows.h>
#include <dxgi.h>
#include <imgui_impl_win32.h>

#include "ui.h"

struct IDXGISwapChain_Present {
	static HRESULT WINAPI thunk(IDXGISwapChain* classPtr, UINT SyncInterval, UINT flags)
	{	
		UI::Render_ImGui();

		return func(classPtr, SyncInterval, flags);
	}
	static inline REL::Relocation<decltype(thunk)> func;
};

struct BSInputDeviceManager_PollInputDevice {
	static void thunk(RE::BSInputDeviceManager* classPtr, float a_secsSinceLastFrame)
	{
		if (UI::isOpen)
		{
			return;
		}
		return func(classPtr, a_secsSinceLastFrame);
	}
	static inline REL::Relocation<decltype(thunk)> func;
};

void InstallHooks();