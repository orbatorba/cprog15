#ifndef ITEM_H
#define ITEM_h
#include "Enums.h"
#include <string>
#include <iostream>

namespace Game
{
	
	class Item
	{
		protected:
			std::string _name;
			unsigned int _weight;
			unsigned int _volume;
			unsigned int _price;

		public:
			
			Item ();
			Item (const Item &);
			Item (const std::string &, unsigned int, unsigned int, unsigned int);
			virtual ~Item ();

			virtual unsigned int weight () const { return _weight; }
			virtual unsigned int volume () const { return _volume; }
			virtual unsigned int price () const { return _price; }
		
	};
}

#endif
