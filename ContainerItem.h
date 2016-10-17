#ifndef CONTAINER_H
#define CONTAINER_H
#include "Item.h"
#include <map>

namespace Game 
{
	class ContainerItem : public Item
	{
		
		protected:
			std::map <std::string, Item &> _container;
			unsigned int _hold_weight = 100;
			unsigned int _hold_volume = 15;
			unsigned int _current_volume = 0;
			unsigned int _current_hold_volume = 0;
			
			
		public:
			ContainerItem ();
			ContainerItem (std::string & name, unsigned int weight, unsigned int volume,
							 unsigned int price, unsigned int hold_weight, unsigned int hold_volume);
			
			~ContainerItem ();

			virtual void add (Item &);
			virtual void remove (Item &);
			virtual bool contains (std::string &);
			virtual Item & get_item (std::string &);
			virtual void list_items ();

	};
}
#endif
