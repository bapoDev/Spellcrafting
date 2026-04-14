/**
* Creates a spell with custom values
* baseSpell : the spell it's based off of
* delivery : the 'range' of the spell (touch, self, target...)
*
* returns true if the spell was correctly created, false otherwise
*/
RE::SpellItem* CreateSpell(const char* name, RE::Effect* effect)
{
	RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
	return nullptr;
}

std::vector<RE::Effect*> GetPlayerKnownEffects()
{
	REX::INFO("GetPlayerKnownEffects() executed");
	auto* player = RE::PlayerCharacter::GetSingleton();
	if (!player) return {};
	
	std::vector<RE::Effect*> effects;

	for (auto spell : player->addedSpells)
	{
		if (!spell) continue;

		for (auto effect : spell->effects)
		{
			if (!effect || !effect->baseEffect || strlen(effect->baseEffect->GetFullName()) == 0) continue;
			std::string effectName = effect->baseEffect->GetFullName();

			effects.push_back(effect);
			REX::INFO("New effect added : {}", effectName);
		}
	}

	return effects;
}

