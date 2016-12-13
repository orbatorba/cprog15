#include "GulDan.h"

namespace Game
{
	GulDan::GulDan () {}
	
	GulDan::GulDan (const std::string & name, const std::string & race, int hp, int armor, 
	int damage, int stam, std::shared_ptr <Area> area, int fire_res, int frost_res, int shadow_res)
	: FelSpawn (name, race, hp, armor, damage, stam, area, fire_res, frost_res, shadow_res)
	{
		initialize ();
	} 

	void GulDan::initialize ()
	{
		fel_attacks.clear ();

		FelAttack Immolate;
		FelAttack ChaosBolt;
		FelAttack FelAgony;

		Immolate.damage = _damage*1.5;
		Immolate.armor_reduce = 5;
		Immolate.element = element_t::FIRE;
		Immolate.name = "Immolate";

		ChaosBolt.damage = _damage*2;
		ChaosBolt.element = element_t::SHADOW;
		ChaosBolt.name = "Chaos Bolt";

		FelAgony.damage = _damage;
		FelAgony.mana_reduce = _damage;
		FelAgony.reduce_attack = 10;
		FelAgony.element = element_t::SHADOW;
		FelAgony.name = "Fel Agony";
		
		fel_attacks.push_back (Immolate);
		fel_attacks.push_back (ChaosBolt);
		fel_attacks.push_back (FelAgony);
	}
}
