#include "FelSpawn.h"
#include "Area.h"
#include "ContainerItem.h"
#include <iostream>
#include <string>


namespace Game
{

  FelSpawn::FelSpawn () {}

  FelSpawn::FelSpawn (const std::string & name, const std::string & race, int hp, int armor, 
	int damage, int stam, std::shared_ptr <Area> area, int fire_res, int frost_res, int shadow_res)
  : Character (name, race, hp, armor, damage, stam, area, fire_res, frost_res, shadow_res)
  {
    this->initialize ();
  }

  void FelSpawn::initialize ()
  {
    FelAttack FelSlice;
    FelAttack Burn;
    FelAttack ManaBurn;

    FelSlice.damage = _damage;
    FelSlice.reduce_attack = 5;
    FelSlice.name = "Fel Slice";
	FelSlice.element = element_t::PHYSICAL;

    Burn.damage = 2*_damage;
    Burn.armor_reduce = 5;
    Burn.name = "Burn";
	Burn.element = element_t::FIRE;

    ManaBurn.damage = _damage*0.5;
    ManaBurn.mana_reduce = _damage*2;
    ManaBurn.name = "Mana Burn";
    ManaBurn.element = element_t::SHADOW;

    fel_attacks.push_back (FelSlice);
    fel_attacks.push_back (Burn);
    fel_attacks.push_back (ManaBurn);

  }

  void FelSpawn::fight (Character & character)
  {
    std::cout << _name << " is now attacking " << character.name () << "!" << std::endl;
		std::cout << "" << std::endl;

    if (this->state != state_t::FIGHTING)
      this->state = state_t::FIGHTING;

    //auto dice = std::bind (distribution, generator);
	std::string element;
    srand (time (NULL));
    int roll = rand () % 4;
	std::cout << "Roll this turn = " << roll << std::endl;

	if (roll < 3)
	{
		if (fel_attacks.at (roll).element == element_t::FIRE)
			element = "Fire";
		else if (fel_attacks.at (roll).element == element_t::FROST)
			element = "Frost";
		else if (fel_attacks.at (roll).element == element_t::SHADOW)
			element = "Shadow";
		else
			element = "Physical";
	}
	
    try
    {
      switch (roll)
      {
        case 0:
          std::cout << _name << " cast " << fel_attacks.at (roll).name << " on " << character.name ()
          << " for " << fel_attacks.at (roll).damage << " " << element << " damage!" << std::endl;
          character.spell_dmg_taken (fel_attacks.at (roll).damage, fel_attacks.at (roll).element);
          break;

        case 1:
          std::cout << _name << " cast " << fel_attacks.at (roll).name << " on " << character.name ()
          << " for " << fel_attacks.at (roll).damage << " " << element << " damage!" << std::endl;
          character.spell_dmg_taken (fel_attacks.at (roll).damage, fel_attacks.at (roll).element);
          break;

        case 2:
          std::cout << _name << " cast " << fel_attacks.at (roll).name << " on " << character.name ()
          << " for " << fel_attacks.at (roll).damage << " " << element << " damage";

          if (character.has_mana)
          {
            std::cout << " and " << fel_attacks.at (roll).mana_reduce << " mana!" << std::endl;
            character.change_mana (fel_attacks.at (roll).mana_reduce);
          }
          else
            std::cout << "!" << std::endl;

          character.spell_dmg_taken (fel_attacks.at (roll).damage, fel_attacks.at (roll).element);

          break;

        case 3:
          if (_stamina > cost_of_attack)
          {
            std::cout << _name << " melee attacks " << character.name () << " with their weapon!"
			<< std::endl;
            character.damage_taken (_damage);
            _stamina -= cost_of_attack;
          }
          else
            std::cout << _name << " didn't have the stamina to attack this round!" << std::endl;

          break;
      }

    }
    catch (std::out_of_range e)
    {
      std::cout << "The " << _race << " seems confused.." << std::endl;
      return;
    }


  }

}
