#ifndef CHAR_H
#define CHAR_H
#include <vector>
#include <string>
#include <map>
#include <memory>

namespace Game
{

	class Area;
//	class Item;
	class ContainerItem;
	class Keepable;
	enum class state_t { DEAD, FIGHTING, IDLE };
	enum class element_t { FIRE, FROST, PHYSICAL, SHADOW };
	enum class direction_t { NORTH, SOUTH, EAST, WEST};

	struct Spell
		{
			int mana_cost;
			int damage;
			element_t element;
			int reduce_attack = 0;
			bool stuck = false;
			int armor_reduce = 0;
		};


	class Character
	{

		protected:

			std::string _name;
			std::string _race;

			int hitpoints;
			int _health;
			int _armor;

			int _damage;
			int _stamina;

			bool _stuck;
			std::shared_ptr <Area> _area;
			ContainerItem _bag;

			int _fire_resistance;
			int _frost_resistance;
			int _shadow_resistance;

			
//			std::vector<Item> _items;

		public:

			state_t state = state_t::IDLE;
			bool has_mana = false;

			/** --- CONSTRUCTORS --- **/

			Character ();
			Character (const Character &);
			Character (const std::string &name, const std::string &race, int hp, int armor, 
						int damage, int stam, std::shared_ptr <Area> area,
						int fire_res = 0, int frost_res = 0, int shadow_res = 0);

			virtual ~Character ();

					/** --- GETTERS --- **/

			std::string name () const { return _name; }
			std::string race () const { return _race; }
			int health () const { return _health; }
			int max_hp () const { return hitpoints; }
			int armor () const { return _armor; }
			int fire_resistance () const { return _fire_resistance; }
			int frost_resistance () const { return _frost_resistance; }
			int shadow_resistance () const { return _shadow_resistance; }


			int damage () const { return _damage; }
			int stamina () const { return _stamina; }
			virtual int mana () const { return 0; }

			bool stuck () const { return _stuck; }

			std::shared_ptr <Area> area () const { return _area; }
//			std::vector<Item> items () const;

			/** --- SETTERS --- **/

			void set_name (std::string name) { _name = name; }
			void set_race (std::string race) { _race = race; }
			virtual void set_health (int);
			virtual void change_mana (int mana) { }
			virtual void change_health (int);
			virtual void set_armor (int);
			virtual void set_damage (int);
			virtual void set_stamina (int);
			virtual void set_frost_res (int);
			virtual void set_fire_res (int);
			virtual void set_shadow_res (int);


			void set_stuck (bool stuck) { _stuck = stuck; }
			void set_area (std::shared_ptr <Area> area) { _area = area; }

			/** --- Other member functions --- **/

			virtual void damage_taken (int);
			virtual void spell_dmg_taken (int, element_t);
			virtual void display_stats () const;

//			virtual void action () = 0;
			virtual void go (direction_t);
			virtual void fight (Character &) = 0;
			virtual bool pick_up (Keepable &);
			virtual bool drop (Keepable &);
			virtual void check_bag ();
//			virtual void talk_to (Character &) = 0;



	};

}
#endif
