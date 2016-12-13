#include "ContainerItem.h"
//#include "Keepable.h"

namespace Game
{
	ContainerItem::ContainerItem () : Item ()
	{
		_name = static_cast<std::string>("Bag");
		_weight = 1;
		_volume = 15;	
		_price = 0;
		_hold_weight = 100;
		_hold_volume = 15;
	}
	
	ContainerItem::ContainerItem (const std::string & name, unsigned int weight, unsigned int volume, unsigned int price, unsigned int hold_weight, unsigned int hold_volume)
		: Item (name, weight, volume, price)
		, _hold_weight (hold_weight)
		, _hold_volume (hold_volume)
	{}

	ContainerItem::~ContainerItem ()
	{
		_container.clear ();
	}

	bool ContainerItem::add (Keepable & item) 
	{
		if (item.weight () + _weight <= _hold_weight)
		{
			if (item.volume () + _current_hold_volume <= _hold_volume)
			{
				auto inserted = _container.insert (
								std::pair<std::string, Keepable &> (item.name (), item));
				if (inserted.second)
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

	bool ContainerItem::remove (Keepable & item)
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

	void ContainerItem::list_items () const
	{
		if (_container.empty ())
			return;
		std::cout << std::endl;
		auto last = _container.end()--;
		for (auto it = _container.begin (); it != _container.end(); ++it)
		{
			size_t len = it->first.size ();
			for (size_t i = 0; i <= len; ++i)
				std::cout << "—";
			std::cout << std::endl; 
			std::cout << "| " << it->first << " |" << std::endl;
			if (it == last)
			{
				for (size_t i = 0; i <= len; ++i)
					std::cout << "—";
				std::cout << std::endl; 
			}
		}
	}

	bool ContainerItem::empty () const
	{
		return _container.empty ();
	}
	
	void ContainerItem::clear ()
	{
		_container.clear ();
	}
}
