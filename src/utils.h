#pragma once
#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

//bool CreateSpell(const char* name, RE::SpellItem* baseSpell, RE::MagicSystem::Delivery delivery, float magnitude, int area, int duration);
std::vector<RE::Effect*> GetPlayerKnownEffects();
void CreateSpell(const char* name, RE::Effect* effect, int magnitude, int area, int duration);