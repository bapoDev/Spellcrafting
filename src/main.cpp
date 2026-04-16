#include <SKSE/SKSE.h>
#include <RE/Skyrim.h>
#include "utils.h"
#include "ui.h"
#include "hooks.h"


void WelcomeMessage()
{
	RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
	std::string msg;
	if (player)
	{
		msg = std::format("Player loaded {}", player->GetDisplayFullName());
	}
	else {
		msg = std::format("Player singleton was null after load... weird...");
	}
	RE::SendHUDMessage::ShowHUDMessage(msg.c_str());
}


void OnMessage(SKSE::MessagingInterface::Message* message)
{
	if (message->type == SKSE::MessagingInterface::kInputLoaded)
	{
		SKSE::AllocTrampoline(1 << 7);
		UI::Init_ImGui();
		InstallHooks();
	}
	if (message->type == SKSE::MessagingInterface::kPostLoadGame)
	{
		SKSE::GetTaskInterface()->AddTask([]()
			{
				WelcomeMessage();
			});
	}
}


SKSEPluginLoad(const SKSE::LoadInterface* skse) {
	SKSE::Init(skse);

	REX::INFO("Plugin loaded!");

	auto* messagingInterface = SKSE::GetMessagingInterface();
	messagingInterface->RegisterListener(OnMessage);

	return true;
}
