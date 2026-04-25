#include "utils.h"

static const char* castingNames[] = { "Self", "Touch", "Aimed", "TargetActor", "TargetLocation", "Total"};
static const char* deliveryNames[] = { "ConstantEffect", "FireAndForget", "Concentration", "Scroll" };
static const char* spellTypeNames[] = { "kSpell", "kDisease", "kPower", "kLesserPower", "kAbility", "kPoison", "kEnchantment", "kPotion" };


void CreateSpell(const char* name, RE::EffectSetting* effect, RE::MagicSystem::CastingType castingType, RE::MagicSystem::Delivery delivery, int magnitude, int area, int duration)
{
	RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
	auto spellFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::SpellItem>();
	RE::SpellItem* newSpell = spellFactory->Create();
	auto newEffect = new RE::Effect();

	effect->data.castingType = castingType;
	effect->data.delivery = delivery;

	newEffect->baseEffect = effect;
	newEffect->SetDuration(duration);
	newEffect->SetMagnitude(magnitude);

	newSpell->SetFullName(name);
	newSpell->effects.push_front(newEffect);
	newSpell->SetCastingType(castingType);
	newSpell->SetDelivery(delivery);



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


/**
* Dev function - Logs every spell effect with info on them on Spellcrafting.log
* It includes : Spells, some Scrolls, Shouts, and probably shrine effects.
* Please note that only the player's effects are being logged because I try this with a 100% save with everything unlocked
* and I don't know how to enumerate all the effects in the game.
*/
void DumpEveryPlayerEffect()
{
	auto* player = RE::PlayerCharacter::GetSingleton();
	if (!player) return;

	for (auto spell : player->addedSpells)
	{
		if (!spell) continue;
		if (spell->GetSpellType() == RE::MagicSystem::SpellType::kSpell) continue;
		REX::INFO("--------------------------------------------------------------------------------------------");
		REX::INFO("Spell Name: {}", spell->GetFullName());

		auto castingName = castingNames[static_cast<int>(spell->GetCastingType())];
		if (!castingName) castingName = "Error/Unknown";

		auto deliveryName = deliveryNames[static_cast<int>(spell->GetDelivery())];
		if (!deliveryName) deliveryName = "Error/Unknown";

		auto spellType = spellTypeNames[static_cast<int>(spell->GetSpellType())];
		if (!spellType) spellType = "Error/Unknown";

		REX::INFO("Casting type: {}\nDelivery: {}", castingName, deliveryName);
		REX::INFO("Spell type: {}", spellType);
		
		for (auto effect : spell->effects)
		{
			if (!effect || !effect->baseEffect || strlen(effect->baseEffect->GetFullName()) == 0) continue;
			REX::INFO("Effect Name: {}", effect->baseEffect->GetFullName());
			REX::INFO("Effect info: \n\t Area: {}\n\t Duration: {}\n\t Magnitude: {}", effect->GetArea(), effect->GetDuration(), effect->GetMagnitude());
			auto effectCast = castingNames[static_cast<int>(effect->baseEffect->data.castingType)];
			if (!effectCast) effectCast = "Error/Unknown";
			auto effectDelivery = deliveryNames[static_cast<int>(effect->baseEffect->data.delivery)];
			if (!effectDelivery) effectDelivery = "Error/Unknown";
			REX::INFO("Effect Types: \n\t Casting Type: {}\n\t Delivery: {}", effectCast, effectDelivery);
		}
	}
}
