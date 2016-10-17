#include "Character.h"
#include "Area.h"
#include "Item.h"
#include "Keepable.h"
#include "ContainerItem.h"
#include <iostream>
#include <cctype>

namespace Game
{

	Character::Character () { }

	Character::Character (const Character & other)
	{
		_name = other._name;
		_race = other._race;
		hitpoints = other.hitpoints;
		_health = other._health;
		_armor = other._armor;
		_damage = other._damage;
		_stamina = other._stamina;
		_area = other._area;
		_fire_resistance = other._fire_resistance;
		_frost_resistance = other._frost_resistance;
		_shadow_resistance = other._shadow_resistance;

//		_items = other._items;

	}

	Character::Character (const std::string &name, const std::string &race, int hp, int armor, 
							int damage, int stam,  std::shared_ptr <Area> area, int fire_res,
							 int frost_res, int shadow_res)
		: _name (name)
		, _race (race)
		, hitpoints (hp)
		, _health (hp)
		, _armor (armor)
		, _damage (damage)
		, _stamina (stam)
		, _area (area)
		, _fire_resistance (fire_res)
		, _frost_resistance (frost_res)
		, _shadow_resistance (shadow_res)

		{ }

	Character::~Character () 
	{
		std::cout << "Character detructed!" << std::endl;
		_area.reset();
	 }

	void Character::set_health (int health)
	{
		if (health > hitpoints)
			throw std::out_of_range ("Cannot set health above maximum hitpoints!");

		_health = health;
	}

	void Character::change_health (int health)
	{
		if (_health + health > hitpoints)
			_health = hitpoints;

		else if (_health + health <= 0)
		{
			_health = 0;
			state = state_t::DEAD;
		}

		else
			_health += health;
	}

	void Character::set_armor (int armor)
	{
		_armor = armor;
	}

	void Character::set_damage (int damage)
	{
		_damage = damage;
	}

	void Character::set_stamina (int stamina)
	{
		_stamina = stamina;
	}

	void Character::set_frost_res (int res)
	{
		_frost_resistance = res;
	}

	void Character::set_fire_res (int res)
	{
		_fire_resistance = res;
	}

	void Character::set_shadow_res (int res)
	{
		_shadow_resistance = res;
	}

	void Character::damage_taken (int damage)
	{
		if (damage <= 0)
			return;

		int damage_done = (damage - _armor);

		if (damage_done < 0)
		{
			std::cout << _name << " absorbed " << damage << " damage! " << _name << " has "
			<< _armor << " armor.. " << std::endl;
			return;
		}
		change_health (- damage_done);
		std::cout << _name << " took " << damage_done << " physical damage!" << std::endl;
	}

	void Character::spell_dmg_taken (int damage, element_t elem)
	{
		if (damage <= 0)
			return;

		switch (elem)
		{
			case element_t::FROST:
				change_health (- damage + _frost_resistance);
				std::cout << _name << " took " << damage - _frost_resistance << " frost damage!" <<
				std::endl;
				break;

			case element_t::FIRE:
				change_health (- damage + _fire_resistance);
				std::cout << _name << " took " << damage - _fire_resistance << " fire damage!" <<
				std::endl;
				break;

			case element_t::SHADOW:
				change_health (- damage + _shadow_resistance);
				std::cout << _name << " took " << damage - _shadow_resistance << " shadow damage!" <<
				std::endl;
				break;

			case element_t::PHYSICAL:
				change_health (- damage + _armor);
				std::cout << _name << " took " << damage - _armor << " physical damage!" <<
				std::endl;
				break;

			default:
				std::cout << "Unrecognized element! Seems like it could be from another world.." <<
				std:: endl;
				break;
		}
	}

	void Character::display_stats () const
	{
		std::cout << "" << std::endl;
		std::cout << " ----------- STATS FOR " << _name << " ----------" << std::endl;
		std::cout << "		HEALTH:		" << _health << std::endl;
		std::cout << "		STAMINA:	" << _stamina << std::endl;
		std::cout << "		ARMOR:		" << _armor << std::endl;
		std::cout << " ------------------------------------------" << std::endl;
		std::cout << "" << std::endl;

	}

	void Character::go (direction_t dir)
	{
		if (_area->has_neighbor (dir))
		{
			_area->leave (*this);
			_area = _area->neighbor (dir);
			_area->enter (*this);
			std::cout << _area->description () << std::endl;
		}
		else
		{
			std::cerr << _area->name() << " has no neighbour in that direction." << std::endl;
			return;
		}
	}

	bool Character::pick_up (Keepable & item)
	{
		if (_bag.add (item))
		{
			std::cout << _name << " picked up " << item.name () << "!" << std::endl;	
			return true;
		}
		return false;
	}

	bool Character::drop (Keepable & item)
	{
		if (_bag.remove (item))
		{
			std::cout << _name << " dropped " << item.name () << "!" << std::endl;
			return true;
		}

		std::cout << _name << " doesn't own that Item! " << std::endl;
		return false;
	}
	
	void Character::check_bag ()
	{
		_bag.list_items ();
	}	
}
