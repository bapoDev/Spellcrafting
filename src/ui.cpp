#include "ui.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <d3d11.h>

#include "utils.h"

static const RE::MagicSystem::CastingType castingTypeList[] = { 
	RE::MagicSystem::CastingType::kConcentration, 
	RE::MagicSystem::CastingType::kConstantEffect, 
	RE::MagicSystem::CastingType::kFireAndForget, 
	RE::MagicSystem::CastingType::kScroll
};

static const RE::MagicSystem::Delivery deliveryNames[] = { 
	RE::MagicSystem::Delivery::kAimed,
	RE::MagicSystem::Delivery::kNone,
	RE::MagicSystem::Delivery::kSelf,
	RE::MagicSystem::Delivery::kTargetActor,
	RE::MagicSystem::Delivery::kTargetLocation,
	RE::MagicSystem::Delivery::kTouch
};


void UI::Init_ImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	UI::ioShared = &io;

	auto* renderer = RE::BSGraphics::Renderer::GetSingleton();
	auto* device = reinterpret_cast<ID3D11Device*>(renderer->data.forwarder);
	auto* context = reinterpret_cast<ID3D11DeviceContext*>(renderer->data.context);
	auto* window = renderer->data.renderWindows->hWnd;

	ImGui_ImplDX11_Init(device, context);
	ImGui_ImplWin32_Init(window);
}

void UI::Render_Menu()
{
	ImGui::Begin("My Custom Menu");

	static auto effects = GetPlayerKnownEffects();
	static int effect_selected_idx = 0;
	static float magnitude = 0.0f;
	static int area = 0;
	static int duration = 0;
	static char buf[100] = "";
	static bool isHostile = false;

	if (ImGui::IsWindowAppearing())
	{
		effects = GetPlayerKnownEffects();
		effect_selected_idx = 0;
		magnitude = 0;
		area = 0;
		duration = 0;
		isHostile = false;
	}

	ImGui::Text("Holy shit, it actually works!");
	
	ImGui::InputText("Spell Name", buf, 100);

	if (ImGui::BeginListBox("Effect List"))
	{
		static ImGuiTextFilter filter;

		filter.Draw("Search...");
		for (int n = 0; n < effects.size(); n++)
		{
			const bool is_selected = (effect_selected_idx == n);
			if (filter.PassFilter(effects[n]->GetFullName()))
				if (ImGui::Selectable(effects[n]->GetFullName(), is_selected))
					effect_selected_idx = n;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}

	
	static int castingType_selected_idx = 0;

	const auto defaultCT = castingTypeList[castingType_selected_idx];
	if (ImGui::BeginCombo("Casting Type", GetCastTypeName(defaultCT)))
	{
		for (int n = 0; n < IM_COUNTOF(castingTypeList); n++)
		{
			const bool is_selected = (castingType_selected_idx == n);
			if (ImGui::Selectable(GetCastTypeName(castingTypeList[n]), is_selected))
				castingType_selected_idx = n;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	static int delivery_selected_idx = 0;

	const auto defaultDelivery = deliveryNames[delivery_selected_idx];
	if (ImGui::BeginCombo("Delivery", GetDeliveryName(defaultDelivery)))
	{
		for (int n = 0; n < IM_COUNTOF(deliveryNames); n++)
		{
			const bool is_selected = (delivery_selected_idx == n);
			if (ImGui::Selectable(GetDeliveryName(deliveryNames[n]), is_selected))
				delivery_selected_idx = n;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	
	ImGui::SliderFloat("Magnitude", &magnitude, 0, 1000);
	ImGui::SliderInt("Area", &area, 0, 1000);
	ImGui::SliderInt("Duration", &duration, 0, 1000);


	if (ImGui::Button("Create Spell")) {
		CreateSpell(buf,
			effects[effect_selected_idx],
			castingTypeList[castingType_selected_idx],
			deliveryNames[delivery_selected_idx],
			magnitude,
			area,
			duration
		);
		UI::SpellcraftingMenu::Toggle();
		RE::SendHUDMessage::ShowHUDMessage("%s has been added.", buf);
	}

	if (ImGui::Button("Cancel"))
		UI::SpellcraftingMenu::Toggle();

	ImGui::End();
}



UI::SpellcraftingMenu::SpellcraftingMenu()
{
	menuFlags.set(
		RE::UI_MENU_FLAGS::kUpdateUsesCursor,
		RE::UI_MENU_FLAGS::kRequiresUpdate,
		RE::UI_MENU_FLAGS::kCustomRendering,
		RE::UI_MENU_FLAGS::kDisablePauseMenu,
		RE::UI_MENU_FLAGS::kPausesGame,
		RE::UI_MENU_FLAGS::kUsesBlurredBackground
	);

	depthPriority = 10;

	inputContext = RE::IMenu::Context::kMenuMode;
}

void UI::SpellcraftingMenu::Register()
{
	RE::UI::GetSingleton()->Register(MENU_NAME, []() -> RE::IMenu* {
		return new SpellcraftingMenu();
		});
	REX::INFO("Menu registered");
}

void UI::SpellcraftingMenu::Toggle()
{
	auto ui = RE::UI::GetSingleton();
	auto msgQueue = RE::UIMessageQueue::GetSingleton();

	if (!ui || !msgQueue) return;

	if (!ui->IsMenuOpen(MENU_NAME))
	{
		msgQueue->AddMessage(MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, nullptr);
		isOpen = true;
	}
	else {
		msgQueue->AddMessage(MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
		isOpen = false;
	}
}

// All the code following this line was taken from Meridiano : https://github.com/Meridiano

ImGuiKey UI::ScanCodeToImGuiKey(std::uint32_t scanCode) {
	switch (scanCode) {
	case 0x01: return ImGuiKey_Escape;
	case 0x02: return ImGuiKey_1;
	case 0x03: return ImGuiKey_2;
	case 0x04: return ImGuiKey_3;
	case 0x05: return ImGuiKey_4;
	case 0x06: return ImGuiKey_5;
	case 0x07: return ImGuiKey_6;
	case 0x08: return ImGuiKey_7;
	case 0x09: return ImGuiKey_8;
	case 0x0A: return ImGuiKey_9;
	case 0x0B: return ImGuiKey_0;
	case 0x0C: return ImGuiKey_Minus;
	case 0x0D: return ImGuiKey_Equal;
	case 0x0E: return ImGuiKey_Backspace;
	case 0x0F: return ImGuiKey_Tab;
	case 0x10: return ImGuiKey_Q;
	case 0x11: return ImGuiKey_W;
	case 0x12: return ImGuiKey_E;
	case 0x13: return ImGuiKey_R;
	case 0x14: return ImGuiKey_T;
	case 0x15: return ImGuiKey_Y;
	case 0x16: return ImGuiKey_U;
	case 0x17: return ImGuiKey_I;
	case 0x18: return ImGuiKey_O;
	case 0x19: return ImGuiKey_P;
	case 0x1A: return ImGuiKey_LeftBracket;
	case 0x1B: return ImGuiKey_RightBracket;
	case 0x1C: return ImGuiKey_Enter;
	case 0x1D: return ImGuiKey_LeftCtrl;
	case 0x1E: return ImGuiKey_A;
	case 0x1F: return ImGuiKey_S;
	case 0x20: return ImGuiKey_D;
	case 0x21: return ImGuiKey_F;
	case 0x22: return ImGuiKey_G;
	case 0x23: return ImGuiKey_H;
	case 0x24: return ImGuiKey_J;
	case 0x25: return ImGuiKey_K;
	case 0x26: return ImGuiKey_L;
	case 0x27: return ImGuiKey_Semicolon;
	case 0x28: return ImGuiKey_Apostrophe;
	case 0x29: return ImGuiKey_GraveAccent;
	case 0x2A: return ImGuiKey_LeftShift;
	case 0x2B: return ImGuiKey_Backslash;
	case 0x2C: return ImGuiKey_Z;
	case 0x2D: return ImGuiKey_X;
	case 0x2E: return ImGuiKey_C;
	case 0x2F: return ImGuiKey_V;
	case 0x30: return ImGuiKey_B;
	case 0x31: return ImGuiKey_N;
	case 0x32: return ImGuiKey_M;
	case 0x33: return ImGuiKey_Comma;
	case 0x34: return ImGuiKey_Period;
	case 0x35: return ImGuiKey_Slash;
	case 0x36: return ImGuiKey_RightShift;
	case 0x37: return ImGuiKey_KeypadMultiply;
	case 0x38: return ImGuiKey_LeftAlt;
	case 0x39: return ImGuiKey_Space;
	case 0x3A: return ImGuiKey_CapsLock;
	case 0x3B: return ImGuiKey_F1;
	case 0x3C: return ImGuiKey_F2;
	case 0x3D: return ImGuiKey_F3;
	case 0x3E: return ImGuiKey_F4;
	case 0x3F: return ImGuiKey_F5;
	case 0x40: return ImGuiKey_F6;
	case 0x41: return ImGuiKey_F7;
	case 0x42: return ImGuiKey_F8;
	case 0x43: return ImGuiKey_F9;
	case 0x44: return ImGuiKey_F10;
	case 0x45: return ImGuiKey_NumLock;
	case 0x46: return ImGuiKey_ScrollLock;
	case 0x47: return ImGuiKey_Keypad7;
	case 0x48: return ImGuiKey_Keypad8;
	case 0x49: return ImGuiKey_Keypad9;
	case 0x4A: return ImGuiKey_KeypadSubtract;
	case 0x4B: return ImGuiKey_Keypad4;
	case 0x4C: return ImGuiKey_Keypad5;
	case 0x4D: return ImGuiKey_Keypad6;
	case 0x4E: return ImGuiKey_KeypadAdd;
	case 0x4F: return ImGuiKey_Keypad1;
	case 0x50: return ImGuiKey_Keypad2;
	case 0x51: return ImGuiKey_Keypad3;
	case 0x52: return ImGuiKey_Keypad0;
	case 0x53: return ImGuiKey_KeypadDecimal;
	case 0x57: return ImGuiKey_F11;
	case 0x58: return ImGuiKey_F12;
	case 0x9C: return ImGuiKey_KeypadEnter;
	case 0x9D: return ImGuiKey_RightCtrl;
	case 0xB5: return ImGuiKey_KeypadDivide;
	case 0xB7: return ImGuiKey_PrintScreen;
	case 0xB8: return ImGuiKey_RightAlt;
	case 0xC5: return ImGuiKey_Pause;
	case 0xC7: return ImGuiKey_Home;
	case 0xC8: return ImGuiKey_UpArrow;
	case 0xC9: return ImGuiKey_PageUp;
	case 0xCB: return ImGuiKey_LeftArrow;
	case 0xCD: return ImGuiKey_RightArrow;
	case 0xCF: return ImGuiKey_End;
	case 0xD0: return ImGuiKey_DownArrow;
	case 0xD1: return ImGuiKey_PageDown;
	case 0xD2: return ImGuiKey_Insert;
	case 0xD3: return ImGuiKey_Delete;
	}
	return ImGuiKey_None;
}

template <int mode, typename T1 = int, typename T2 = int>
void AddImGuiInput(T1 v1 = NULL, T2 v2 = NULL) {
	if (UI::isOpen) {
		if constexpr (mode == 0) UI::ioShared->AddMouseWheelEvent(0.0F, v1);
		if constexpr (mode == 1) UI::ioShared->AddMouseButtonEvent(v1, v2);
		if constexpr (mode == 2) UI::ioShared->AddKeyEvent(v1, v2);
		if constexpr (mode == 3) UI::ioShared->AddInputCharacter(v1);
	}
}

void UI::ProcessInput(RE::InputEvent* const* a_event)
{
	for (auto event = *a_event; event; event = event->next) {
		switch (event->GetEventType()) {
		case RE::INPUT_EVENT_TYPE::kButton: {
			if (auto buttonEvent = event->AsButtonEvent(); buttonEvent) {
				auto scanCode = buttonEvent->GetIDCode();
				switch (event->GetDevice()) {
				case RE::INPUT_DEVICE::kMouse: {
					if (scanCode == 8 || scanCode == 9) {
						auto mult = 17.0F - float(scanCode) * 2.0F;
						AddImGuiInput<0>(buttonEvent->Value() * mult);
					}
					else {
						if (scanCode > 4) scanCode = 4;
						AddImGuiInput<1>(scanCode, buttonEvent->IsPressed());
					}
				} break;
				case RE::INPUT_DEVICE::kKeyboard: {
					auto imguiKey = UI::ScanCodeToImGuiKey(scanCode);
					bool keyDown = buttonEvent->IsPressed();

					if (imguiKey == ImGuiKey_Home && keyDown) {
						UI::SpellcraftingMenu::Toggle();
					}
					AddImGuiInput<2>(imguiKey, keyDown);
					switch (imguiKey) {
					case ImGuiKey_LeftShift:
					case ImGuiKey_RightShift:
						AddImGuiInput<2>(ImGuiMod_Shift, keyDown);
						break;
					case ImGuiKey_LeftCtrl:
					case ImGuiKey_RightCtrl:
						AddImGuiInput<2>(ImGuiMod_Ctrl, keyDown);
						break;
					case ImGuiKey_LeftAlt:
					case ImGuiKey_RightAlt:
						AddImGuiInput<2>(ImGuiMod_Alt, keyDown);
						break;
					}
				} break;
				}
			}
		} break;
		case RE::INPUT_EVENT_TYPE::kChar: {
			auto charEvent = event->AsCharEvent();
			if (charEvent) AddImGuiInput<3>(charEvent->keyCode);
		} break;
		}
	}
}