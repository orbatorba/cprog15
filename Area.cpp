#include "Area.h"
#include "Character.h"
#include <iostream>
namespace Game
{
	Area::Area () { }

	Area::~Area () 
	{
		std::cout << "Area destructed!" << std::endl;
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

/*	std::shared_ptr <Area> Area::neighbor (direction_t direciton) const
	{
		try
		{
			return & _neighbors.at (direction);
		}
		catch (std::out_of_range e)
		{
			std::cout << "There is no neighboring area in that direction" << std::endl;
		}
	}*/

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
		_characters.insert (std::pair <std::string, Character &> (character.name (), character));

		if (_characters.at (character.name ()).name() == character.name ())
			std::cout << character.name () << " just entered " << this->_name << "!" << std::endl;

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
	
	void Area::clear_neighbors ()
	{
		_neighbors.clear();
	}

}
