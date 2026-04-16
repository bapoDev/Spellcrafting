#include "ui.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <d3d11.h>

void UI::Init_ImGui() 
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	auto* renderer = RE::BSGraphics::Renderer::GetSingleton();
	auto* device = reinterpret_cast<ID3D11Device*>(renderer->data.forwarder);
	auto* context = reinterpret_cast<ID3D11DeviceContext*>(renderer->data.context);
	auto* window = renderer->data.renderWindows->hWnd;

	ImGui_ImplDX11_Init(device, context);
	ImGui_ImplWin32_Init(window);
	
}

	static int counter = 0;

void UI::Render_ImGui()
{
	
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame(); // or your manual input update
	ImGui::NewFrame();

	if (GetAsyncKeyState(VK_HOME))
	{
		UI:isOpen = !UI::isOpen;
	}

	if (UI::isOpen)
	{
		RE::PlayerControls::GetSingleton()->blockPlayerInput = true;
		ImGui::ShowDemoWindow();
	}
	else {
		RE::PlayerControls::GetSingleton()->blockPlayerInput = false;
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	
}