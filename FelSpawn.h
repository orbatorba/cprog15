#ifndef FEL_SPAWN_H
#define FEL_SPAWN_H
#include "Character.h"
#include <stdlib.h>
#include <time.h>
#include <random>

namespace Game
{
  class Area;

  struct FelAttack
  {
    std::string name;
    int damage;
    element_t element = element_t::FIRE;
    int reduce_attack = 0;
    bool stuck = false;
    int armor_reduce = 0;
    int mana_reduce = 0;
  };

  class FelSpawn : public Character
  {
    protected:

      int cost_of_attack = 10;
      std::vector <FelAttack> fel_attacks;
      std::default_random_engine generator;
      std::uniform_int_distribution<int> distribution;

      void initialize ();

    public:

      FelSpawn ();
      FelSpawn (const std::string & name, const std::string & race, int hp, int armor, int damage,
					int stam, std::shared_ptr <Area> area, int fire_res = 0, int frost_res = 0, int shadow_res = 0);

      void fight (Character &);


  };

}

#endif
