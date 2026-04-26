#include "utils.h"

static const char* castingNames[] = { "Self", "Touch", "Aimed", "TargetActor", "TargetLocation", "Total"};
static const char* deliveryNames[] = { "ConstantEffect", "FireAndForget", "Concentration", "Scroll" };
static const char* spellTypeNames[] = { "kSpell", "kDisease", "kPower", "kLesserPower", "kAbility", "kPoison", "kEnchantment", "kPotion" };

static const auto eitherHand = RE::TESForm::LookupByID<RE::BGSEquipSlot>(0x13F44);

void CreateSpell(const char* name, RE::EffectSetting* effect, RE::MagicSystem::CastingType castingType, RE::MagicSystem::Delivery delivery, float magnitude, int area, int duration)
{
	RE::BGSPerk* perk = RE::TESForm::LookupByID<RE::BGSPerk>(0xC44BB); // Auto-calculate later
	//RE::BGSEquipSlot* slot = RE::TESForm::LookupByID<RE::BGSEquipSlot>(0x13F44);

	auto spellFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::SpellItem>();
	RE::SpellItem* newSpell = spellFactory->Create();
	auto newEffect = new RE::Effect();
	
	//effect->data.castingType = castingType;
	//effect->data.delivery = delivery;
	
	newEffect->baseEffect = effect; // check
	newEffect->SetDuration(duration); // to test
	newEffect->SetMagnitude(magnitude); // to test
	newEffect->baseEffect->data.spellmakingArea = area;

	newSpell->SetFullName(name); // check
	newSpell->data.spellType = RE::MagicSystem::SpellType::kSpell; // check

	newSpell->effects.push_front(newEffect); 

	newSpell->SetCastingType(castingType);
	newSpell->SetDelivery(delivery);

	//newSpell->equipSlot = slot;
	newSpell->data.chargeTime = 0.50000F;
	newSpell->data.castDuration = 0.0F;
	newSpell->data.range = 0.0F;
	newSpell->data.castingPerk = perk;

	RE::PlayerCharacter::GetSingleton()->AddSpell(newSpell);
}


/**
* Gets all of the known effects of the player, *should* only contain legit effects AND should include modded effects
*/
std::vector<RE::EffectSetting*> GetPlayerKnownEffects()
{
	auto* player = RE::PlayerCharacter::GetSingleton();
	if (!player) return {};
	
	std::unordered_set<const char*> effectsSet;
	effectsSet.reserve(player->addedSpells.size());

	std::vector<RE::EffectSetting*> effectsVector;
	effectsVector.reserve(player->addedSpells.size());

	for (auto spell : player->addedSpells)
	{
		if (!spell) continue;
		if (spell->GetSpellType() != RE::MagicSystem::SpellType::kSpell) continue;

		for (auto effect : spell->effects)
		{
			if (!effect || !effect->baseEffect || strlen(effect->baseEffect->GetFullName()) == 0) continue;
			if (effectsSet.find(effect->baseEffect->GetFullName()) != effectsSet.end()) continue;

			effectsSet.insert(effect->baseEffect->GetFullName());
			effectsVector.push_back(effect->baseEffect);
		}
	}
	return effectsVector;
}

const char* GetDeliveryName(RE::MagicSystem::Delivery delivery) 
{
	switch (delivery) {
	case RE::MagicSystem::Delivery::kAimed:
		return "Aimed";
	case RE::MagicSystem::Delivery::kSelf:
		return "Self";
	case RE::MagicSystem::Delivery::kTargetActor:
		return "Target Actor";
	case RE::MagicSystem::Delivery::kTouch:
		return "Touch";
	case RE::MagicSystem::Delivery::kTargetLocation:
		return "Target Location";
	case RE::MagicSystem::Delivery::kNone:
		return "None";
	}
	return "Error/Unknown";
}

const char* GetCastTypeName(RE::MagicSystem::CastingType ct) 
{
	switch (ct) {
	case RE::MagicSystem::CastingType::kConcentration:
		return "Concentration";
	case RE::MagicSystem::CastingType::kConstantEffect:
		return "Constant Effect";
	case RE::MagicSystem::CastingType::kFireAndForget:
		return "Fire and Forget";
	case RE::MagicSystem::CastingType::kScroll:
		return "Scroll";
	}
	return "Error/Unknown";
}