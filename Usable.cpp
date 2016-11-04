#include "Usable.h"

namespace Game
{
	Usable::Usable () { _usable = true; }

	Usable::Usable (const std::string & name, unsigned int mana_gain, unsigned int health_gain, unsigned int all_resistance) :
		Keepable (name, 1, 1, 10, 0, 0, 0, 0),
		_mana_gain (mana_gain),
		_health_gain (health_gain),
		_all_resistance (all_resistance)	
		{ 
			_usable = true;
		}

}
