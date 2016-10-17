#include "Keepable.h"

namespace Game
{
	Keepable::Keepable () { }

	Keepable::Keepable (const Keepable & other) :
		Item (other),
		_attack_power (other._attack_power),
		_spell_power (other._spell_power),
		_health_points (_health_points),
		_mana_points (_mana_points)
	{ }   

	Keepable::Keepable (const std::string & name, unsigned int weight, unsigned int volume, 
						unsigned int price, unsigned int attack_power, unsigned int spell_power,
						unsigned int health_points, unsigned int mana_points) :
		Item (name, weight, volume, price),
		_attack_power (attack_power),
		_spell_power (spell_power),
		_health_points (health_points),
		_mana_points (mana_points)
	{ }
	

}
