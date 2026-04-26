#pragma once
#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

//bool CreateSpell(const char* name, RE::SpellItem* baseSpell, RE::MagicSystem::Delivery delivery, float magnitude, int area, int duration);
std::vector<RE::EffectSetting*> GetPlayerKnownEffects();
void CreateSpell(const char* name, RE::EffectSetting* effect, RE::MagicSystem::CastingType castingType, RE::MagicSystem::Delivery delivery, float magnitude, int area, int duration);

const char* GetDeliveryName(RE::MagicSystem::Delivery delivery);
const char* GetCastTypeName(RE::MagicSystem::CastingType ct);