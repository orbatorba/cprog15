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
		
	void Usable::display_stats () const
	{
		std::cout << "------------------------" << std::endl;
        std::cout << "All Resistance Gain: " << _all_resistance << std::endl;
        std::cout << "Health Gain: " << _health_gain << std::endl;
        std::cout << "Mana Gain: " << _mana_gain << std::endl;
        std::cout << "------------------------" << std::endl;
	}

}
