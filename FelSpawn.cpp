#include "FelSpawn.h"
#include "Area.h"
#include "ContainerItem.h"
#include "Item.h"
#include <iostream>
#include <string>


namespace Game
{

  FelSpawn::FelSpawn () {}

  FelSpawn::FelSpawn (const std::string & name, const std::string & race, int hp, int armor, int damage,
                      int stam, std::shared_ptr <Area> area, int fire_res, int frost_res, int shadow_res)
  : Character (name, race, hp, armor, damage, stam, area, fire_res, frost_res, shadow_res)
  {
    initialize ();
    distribution = std::uniform_int_distribution<int>(0,3);

  }

  void FelSpawn::initialize ()
  {
    FelAttack FelSlice;
    FelAttack Burn;
    FelAttack ManaBurn;

    FelSlice.damage = 50;
    FelSlice.reduce_attack = 5;
    FelSlice.name = "Fel Slice";

    Burn.damage = 100;
    Burn.armor_reduce = 5;
    Burn.name = "Burn";

    ManaBurn.damage = 20;
    ManaBurn.mana_reduce = 40;
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

    auto dice = std::bind (distribution, generator);
    srand (time (NULL));
    int roll = rand () % 4;
    std::cout << "Fel Spawn rolls: " << roll << std::endl;
    try
    {
      switch (roll)
      {
        case 0:
          std::cout << _name << " " << fel_attacks.at (roll).name << "s " << character.name ()
          << " for " << fel_attacks.at (roll).damage << " Fire damage!" << std::endl;
          character.spell_dmg_taken (fel_attacks.at (roll).damage, fel_attacks.at (roll).element);
          break;

        case 1:
          std::cout << _name << " " << fel_attacks.at (roll).name << "s " << character.name ()
          << " for " << fel_attacks.at (roll).damage << " Fire damage!" << std::endl;
          character.spell_dmg_taken (fel_attacks.at (roll).damage, fel_attacks.at (roll).element);
          break;

        case 2:
          std::cout << _name << " " << fel_attacks.at (roll).name << "s " << character.name ()
          << " for " << fel_attacks.at (roll).damage << " Shadow damage";

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
            std::cout << _name << " attacks " << character.name () << " with it's claws!" << std::endl;
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
