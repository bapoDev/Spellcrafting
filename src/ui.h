#pragma once

#include <imgui_impl_win32.h>
#include <Windows.h>

namespace UI {
	inline ID3D11ShaderResourceView* g_SliderHandleSRV = nullptr;

	inline bool isOpen = false;
	inline WNDPROC oWndProc = nullptr;
	inline ImGuiIO* ioShared = nullptr;
	inline int g_HandleWidth = 0;
	inline int g_HandleHeight = 0;

	void Init_ImGui();
	void Render_Menu();
	ImGuiKey ScanCodeToImGuiKey(std::uint32_t scanCode);
	void ProcessInput(RE::InputEvent* const* a_event);

	class SpellcraftingMenu : public RE::IMenu {
	public:
		constexpr static std::string_view MENU_NAME = "SpellcraftingMenu";

		SpellcraftingMenu();

		static void Register();
		static void Toggle();
	};
};