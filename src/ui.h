#pragma once

#include <imgui_impl_win32.h>
#include <Windows.h>

namespace UI {
	inline bool isOpen = false;
	inline WNDPROC oWndProc = nullptr;

	void Init_ImGui();
	void Render_ImGui();
};