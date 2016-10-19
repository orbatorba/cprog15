#include "Dragon.h"
#include "Area.h"
#include "ContainerItem.h"
#include <iostream>
#include <string>

namespace Game
{
	Dragon::Dragon () { }

//	Dragon::~Dragon () { }

	Dragon::Dragon (const std::string &name, const std::string &race, int hp, int mana, int armor, int damage,
					int stam, std::shared_ptr <Area> area, int fire_res, int frost_res, int shadow_res)
	: Character (name, race, hp, armor, damage, stam, area, fire_res, frost_res, shadow_res)
	{
		initialize ();
		has_mana = true;
		_mana = mana;
	}

	void Dragon::initialize ()
	{
		Spell Flamebreath;

		Flamebreath.mana_cost = 100; Flamebreath.damage = 250;
		Flamebreath.element = element_t::FIRE;
		Flamebreath.armor_reduce = 5;

		_spells["Flamebreath"] = Flamebreath;
	}

	void Dragon::fight (Character & character)
	{
		std::cout << _name << " is now attacking " << character.name () << "!" << std::endl;
		std::cout << "" << std::endl;

		if (this->state != state_t::FIGHTING)
			this->state = state_t::FIGHTING;

		double health_perc = (double) this->_health/this->hitpoints;

		if (health_perc < 0.5 && this->_mana >= _spells["Flamebreath"].mana_cost
			&& character.armor () > 0)
		{
			std::cout << "The dragons' belly lights up with fire as it starts channeling "
			<< "a Flamebreath!!!" << std::endl;

			character.spell_dmg_taken (_spells["Flamebreath"].damage, element_t::FIRE);

			if (character.armor () <= _spells["Flamebreath"].armor_reduce)
				character.set_armor (0);
			else
				character.set_armor ( character.armor () - _spells["Flamebreath"].armor_reduce );

			this->_mana -= _spells["Flamebreath"].mana_cost;
		}

		else if (this->_stamina > this->cost_of_attack)
		{
			std::cout << "The dragon attacks " << character.name () << " with its' mighty tail!"
			<< std::endl;

			character.damage_taken (this->_damage);
			this->_stamina -= this->cost_of_attack;
		}

		else
		{
			std::cout << "The dragon's stamina is diminished and can not continue fighting. "
			<< "Now is the chance to kill " << this->_name << "!" << std::endl;
		}


	}

}
