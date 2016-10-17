#ifndef KEEPABLE_H
#define KEEPABLE_H
#include "Item.h"

namespace Game
{
	class Keepable : public Item
	{
		protected:
			unsigned int _attack_power;
			unsigned int _spell_power;
			unsigned int _health_points;
			unsigned int _mana_points;
	
		public:
			Keepable ();
			Keepable (const Keepable &);
			Keepable  (const std::string &, unsigned int, unsigned int, unsigned int, unsigned int,
						unsigned int, unsigned int, unsigned int);
			
			virtual unsigned int attack_power () const { return _attack_power; }
			virtual unsigned int spell_power () const { return _spell_power; }
			virtual unsigned int health_points () const { return _health_points; }
			virtual unsigned int mana_points () const { return _mana_points; }
 
	};
}
#endif
