#ifndef DRAGON_H
#define DRAGON_H

#include "Character.h"

namespace Game
{
	class Area;

	class Dragon : public Character
	{

		protected:

			std::map <std::string, Spell> _spells;
			int _mana;
			int cost_of_attack = 10;

			void initialize ();

		public:

			Dragon ();
//			~Dragon ();
			Dragon (const std::string & name, const std::string & race, int hp, int mana, int armor, int damage,
					int stam, std::shared_ptr <Area> area, int fire_res = 0, int frost_res = 0, int shadow_res = 0);

			int mana () const { return _mana; }
			void change_mana (int mana)
			{
				if (_mana >= mana)
					_mana -= mana;

				else
					_mana = 0;
			}

			void fight (Character &);
			//void action ();
	};
}
#endif
