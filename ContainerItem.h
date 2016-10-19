#ifndef CONTAINER_H
#define CONTAINER_H
#include "Keepable.h"
#include <map>

namespace Game 
{
	//class Keepable;
	class ContainerItem : public Item
	{
		
		protected:
			std::map <std::string, Keepable &> _container;
			unsigned int _hold_weight;
			unsigned int _hold_volume;
			unsigned int _current_volume = 0;
			unsigned int _current_hold_volume = 0;
			
			
		public:
			ContainerItem ();
			ContainerItem (std::string & name, unsigned int weight, unsigned int volume,
				unsigned int price, unsigned int hold_weight = 100, unsigned int hold_volume = 15);
			~ContainerItem ();
			virtual bool add (Keepable &);
			virtual bool remove (Keepable &);
			virtual bool contains (std::string &);
			//virtual Keepable & get_item (std::string &);
			virtual void list_items ();

	};
}
#endif
