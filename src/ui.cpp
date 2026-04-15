#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <d3d11.h>

void Init_ImGui() 
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

void Render_ImGui()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Temporary
	ImGui::ShowDemoWindow();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}