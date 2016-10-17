#include "ContainerItem.h"

namespace Game
{
	ContainerItem::ContainerItem () { }
	
	ContainerItem::ContainerItem (std::string & name, unsigned int weight, unsigned int volume, 
						unsigned int price, unsigned int hold_weight, unsigned int hold_volume)
		_name (name),
		_weight (weight,
		_volume (volume),
		_price (price),
		_hold_weight (hold_weight),
		_hold_volume (hold_volume)
	{}

	ContainerItem::~ContainerItem ()
	{
		_container.clear ();
	}

	bool ContainerItem::add (Item & item) 
	{
		if (item.weight () + _weight <= _hold_weight)
		{
			if (item.volume () + _current_hold_volume <= _hold_volume)
			{
				int inserted = _container.insert (
								std::pair<std::string, Item &> (item.name (), item);
				if (inserted == 1)
				{
					_weight += item.weight ();
					_current_hold_volume += item.volume ();
					return true;
				}
				else
				{
					std::cout << "Can only contain one Item of type " << item.name () << std:: endl;
					return false;
				}
			}
			else
				std::cout << "Not enough volume in container!" << std::endl;
				return false;
		}
		else
			std::cout << "Not enough hold weight in container!" << std::endl;
			return false;
	}

	bool ContainerItem::remove (Item & item)
	{
		int erased = _container.erase (item.name ());
		if (erased == 1)
		{
			_weight -= item.weight ();
			_current_hold_volume -= item.volume ();
			return true;
		}
		
		std::cerr << "Could not remove item! " << std::endl;
		return false;

	}

	bool ContainerItem::contains (std::string & name)
	{
		if (_container.find (name) != _container.end ())
			return true;

		return false;
	}

	void ContainerItem::list_items ()
	{
		std::cout << std::endl;
		std::cout << _name << " contains the following items: " << std::endl;
		for (auto it = _container.begin (); it != _container.end(); ++it)
		{
			size_t len = it->first.size ();
			for (size_t i = 0; i <= len; ++i)
				std::cout << "——";
			std::cout << std::endl; 
			std::cout << "| " << it->first << " |" << std::endl;
			if (it == _container.end () - 1)
			{
				for (size_t i = 0; i <= len; ++i)
					std::cout << "——";
				std::cout << std::endl; 
			}
		}
	}
}
