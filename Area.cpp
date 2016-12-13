#include "Area.h"
#include "Character.h"
#include <iostream>
namespace Game
{
	Area::Area () { }

	Area::~Area () 
	{
		auto it = _neighbors.begin();
		while (it != _neighbors.end())
		{
			it = _neighbors.erase(it);
		}
		 _neighbors.clear ();
	}

	Area::Area (const std::string & name, const std::string & desc) : 
		_name (name),
		_description (desc)
		{}

	Area::Area (const std::shared_ptr <Area> other) : 
		_name (other->_name),
		_description (other->_description) 
		{}


	void Area::add_neighbor (std::shared_ptr <Area> neighbor, direction_t direction)
	{
		_neighbors.insert (std::pair <direction_t, std::shared_ptr <Area>> (direction, neighbor));
	}

	std::shared_ptr <Area> Area::neighbor (direction_t direction) const
	{
		try
		{
			return _neighbors.at (direction);
		}
		catch (std::out_of_range e)
		{
			std::cout << "There is no neighboring area in that direction!" << std::endl;
		}

	}

	bool Area::has_neighbor (direction_t direction) const
	{
		try
		{
			_neighbors.at (direction);
			return true;
		}
		catch (std::out_of_range e)
		{
			return false;
		}
	}

	void Area::enter (Character & character)
	{
		auto entered =
		_characters.insert (std::pair <std::string, Character &> (character.name (), character));

		if (entered.second)
		{
			//Print info about the area, characters in the area and possible items.
			std::cout << character.name () << " just entered " << this->_name << "!" << std::endl;
			std::cout << std::endl;
			std::cout << _description << std::endl;
			if (! _characters.empty ())
			{
				list_characters (character.name ());
				list_character_items (character.name ());
			}
			list_items ();
			
		}
		else
			std::cout << character.name () << " couldn't enter " << this->_name << ". Seems like "
			<< "there is a duplicate character here... " << std::endl;
	}

	void Area::leave (Character & character)
	{
		int num = _characters.erase (character.name ());
		if (num < 1)
			std::cout << character.name () << " doesn't exist in " << this->_name << std::endl;

		else
			std::cout << character.name () << " leaves " << this->_name << std::endl;
	}

	void Area::add_character (Character & character)
	{
		_characters.insert (std::pair < std::string, Character &> (character.name (), character));
	}
	
	void Area::remove_character (Character & character)
	{
		_characters.erase (character.name ());
	}
	
	void Area::clear_neighbors ()
	{
		_neighbors.clear();
	}

	void Area::add_item (Keepable & item)
	{
	//	_items[item.name()] = item;
		auto inserted = _items.insert (std::pair < std::string, Keepable &> (item.name(), item));
		if (!inserted.second)
			std::cout << _name << " couldn't keep track of the item... " << std::endl;
	}

	void Area::drop_item (Keepable & item)
	{
		_items.erase (item.name ());
	}

	Keepable & Area::get_item (std::string name)
	{
		return _items.at (name);
	}
	
	void Area::add_chest (ContainerItem & item)
	{
		auto inserted = _chests.insert(std::pair <std::string, ContainerItem &> (item.name(), item));
		if (!inserted.second)
			std::cerr << "Could not add chest to " << _name << "with that name, " << item.name() 
			<< std::endl;
	}	
	
	void Area::open_chest (std::string & chest_name) 
	{
		try
		{
			auto container = _chests.at (chest_name).get_container ();
			if (container.empty())
			{
				std::cout << chest_name << " is empty!" << std::endl; 
				return;
			}

			auto it = container.begin ();
			while (it != container.end())
			{	
				std::cout << chest_name << " drops " << it->first << " on the ground!" << std::endl;
				this->add_item (it->second);
				it = container.erase (it);
			}
			
		}
		catch (std::out_of_range e)
		{
			std::cout << "There is no such chest in " << _name << "!" << std::endl;
		}
	}

	void Area::add_to_chest (std::string & name, Keepable & item)
	{
		try
		{
			_chests.at (name).add (item);
		}
		catch (std::out_of_range)
		{
			std::cerr << name << "does not exists in this Area!" << std::endl;
		}
	}

	void Area::list_characters (std::string name) const
	{
		std::cout << std::endl;
		std::cout << "The following characters are wandering in " << _name << ": " << std::endl;
		for (const auto& pair : _characters)
		{	
			if (pair.second.name () != name) 
				std::cout << pair.second.name () << " - " << pair.second.race () << std::endl;
		}
	}

	void Area::list_character_items (std::string name) const
	{
		std::cout << std::endl;
		std::cout << "The following items are carried by characters in " << _name << ": "
		<< std::endl;
		for (const auto& pair : _characters)
		{
			if (pair.second.name () != name)
				pair.second.check_bag ();
		}
	}

	void Area::list_items () const
	{
		if (!_items.empty ())
		{
			std::cout << std::endl;
			std::cout << _name << " contains the following loot: " << std::endl;
			for (const auto& pair : _items)
			{
				std::cout << pair.first << std::endl;
			}
		}

		if (!_chests.empty ())
		{
			std::cout << std::endl;
			std::cout << _name << " contains the following chests: " << std::endl;
			for (const auto& pair: _chests)
			{
				std::cout << pair.first << std::endl;
			}
		}	
		
	}

	

}
