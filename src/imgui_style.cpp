#include "imgui_style.h"

#include <imgui_internal.h>

#include "ui.h"

bool Style::CustomSlider(const char* label, float* value, float vmin, float vmax)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems) return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);

	ImVec2 pos = window->DC.CursorPos;
	float width = ImGui::CalcItemWidth();
	float height = ImGui::GetFrameHeight();
	ImRect bb(pos, ImVec2(pos.x + width, pos.y + height));

	ImGui::ItemSize(bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id)) return false;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

	bool value_changed = false;

	if (held) {
		float mouse_x = g.IO.MousePos.x;
		float t = ImClamp((mouse_x - bb.Min.x) / width, 0.0f, 1.0f);
		float new_value = vmin + t * (vmax - vmin);

		if (*value != new_value) {
			*value = new_value;
			value_changed = true;
		}
	}

	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	float track_y = bb.Min.y + height * 0.5f;

	draw_list->AddLine(ImVec2(bb.Min.x, track_y), ImVec2(bb.Max.x, track_y), IM_COL32(100, 100, 100, 255), 4.0f);

	float t = (*value - vmin) / (vmax - vmin);
	float handle_x = bb.Min.x + t * width;

	float half_w = (float)UI::g_HandleWidth * 0.5f;
	float half_h = (float)UI::g_HandleHeight * 0.5f;

	ImVec2 handle_top_left(handle_x - half_w, track_y - half_h);
	ImVec2 handle_bottom_right(handle_x + half_w, track_y + half_h);

	if (UI::g_SliderHandleSRV != nullptr) {
		draw_list->AddImage((ImTextureID)UI::g_SliderHandleSRV, handle_top_left, handle_bottom_right);
	}
	else {
		draw_list->AddCircleFilled(ImVec2(handle_x, track_y), 8.0f, IM_COL32(200, 200, 250, 255));
	}

	
	ImGui::RenderText(ImVec2(bb.Max.x + style.ItemInnerSpacing.x, bb.Min.y + style.FramePadding.y), label);

	return value_changed;
}