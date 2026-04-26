#pragma once

#include <Windows.h>
#include <dxgi.h>
#include <d3d11.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#include "ui.h"
#include "globals.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

struct IDXGISwapChain_Present {
	static HRESULT WINAPI thunk(IDXGISwapChain* classPtr, UINT SyncInterval, UINT flags)
	{	
		ImGuiIO& io = ImGui::GetIO();
		static bool isPressed = false;
		

		if (GetAsyncKeyState(VK_F3) & 0x8000) {
			if (!isPressed) {
				isPressed = true;
				SKSE::GetTaskInterface()->AddUITask([]() {
					UI::SpellcraftingMenu::Toggle();
					});
			}
		}
		else {
			isPressed = false;
		}

		bool isLeftClicked = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
		bool isRightClicked = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;

		
		io.AddMouseButtonEvent(0, isLeftClicked);
		io.AddMouseButtonEvent(1, isRightClicked);

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

	
		bool isMyMenuOpen = false;
		auto ui = RE::UI::GetSingleton();
		if (ui) {
			isMyMenuOpen = ui->IsMenuOpen(UI::SpellcraftingMenu::MENU_NAME);
		}

		if (isMyMenuOpen) {

			io.MouseDrawCursor = true;

			UI::Render_Menu();

			if (!isMyMenuOpen) {
				UI::SpellcraftingMenu::Toggle();
			}
		}
		else {
			io.MouseDrawCursor = false;
		}

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		return func(classPtr, SyncInterval, flags);
	}
	static inline REL::Relocation<decltype(thunk)> func;
};

struct InputHook {
	static void thunk(RE::BSTEventSource<RE::InputEvent*>* a_source, RE::InputEvent* const* a_event) {
		static RE::InputEvent* const dummy[]{ nullptr };
		UI::ProcessInput(a_event);
		func(a_source, UI::isOpen ? dummy : a_event);
	}
	static inline REL::Relocation<decltype(thunk)> func;
};

void InstallHooks();