#ifndef PLAYER_H
#define PLAYER_H
#include "Character.h"

namespace Game
{

	class Area;
	class Keepable;
	class Usable;
	//class ContainerItem;
	class Player : public Character
	{

		protected:

			//Enums to use in switch - case for user input while fighting
			enum class Spell_t { Frostbolt, Fireball, Meteor, Freeze};
			enum class Verbs_t { cast_t, attack_t, flee_t};


			//Maps to match user input with enums
			std::map <std::string, Spell_t> spell_match;
			std::map <std::string, Verbs_t> verb_match;

			//Map to store Player Spells
			std::map <std::string, Spell> _spells;
			int _mana;
			int _attack_power = 0;
			int _spell_power = 0;

			//Helper functions
			void create_spells ();
			void initialize ();

		public:

			Player ();
			Player (const std::string &name, const std::string &race, int hp, int mana, int armor,
					int damage, int stam, std::shared_ptr <Area> area, int fire_res = 0,
					 int frost_res = 0, int shadow_res = 0);

			int mana () const { return _mana; }
			void change_mana (int mana)
			{
				if (_mana >= mana)
					_mana -= mana;
				else
					_mana = 0;
			}
			void fight (Character &);
			/*void talk_to (Character &);
			void action ();*/
			bool pick_up (Keepable &) override;
			bool drop (Keepable &) override;
			void use (Keepable &);
			
			void list_spells () const;
			void section_input (std::string, std::string &, std::string &);

	};

}
#endif
