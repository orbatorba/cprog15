#include "Item.h"

namespace Game
{

	Item::Item () { }
	Item::Item (const Item & other)
	{
		_name = other._name;
		_weight = other._weight;
		_volume = other._volume;
		_price = other._price;
	}

	Item::Item (const std::string & name, unsigned int weight, unsigned int volume, unsigned int price) :
		_name (name),
		_weight (weight),
		_volume (volume),
		_price (price)
	{ }

	Item::~Item () { }
	
}
