#ifndef AREA_H
#define AREA_H

//#include "Character.h"
#include <map>
#include <string>
#include <memory>

namespace Game
{
	enum class direction_t;
	class Character;
	class Area
	{

		protected:

			std::string _name;
			std::string _description;

			std::map <direction_t, std::shared_ptr <Area>> _neighbors;
			//std::map <std::string, Item &> _items;
			std::map <std::string, Character &> _characters;

		public:

			Area ();
			Area (const std::string & name, const std::string & desc);
			Area (const std::shared_ptr <Area> other);
			~Area ();

			std::string description () const { return _description; }
			std::string name () const { return _name; }

			virtual std::map <direction_t, std::shared_ptr <Area>> neighbors () const 
			{
				 return _neighbors;
			 }
			virtual void add_neighbor (std::shared_ptr <Area>, direction_t);
			virtual std::shared_ptr <Area> neighbor (direction_t) const;
//			virtual std::shared_ptr <Area> neighbor (direction_t) const;
			virtual bool has_neighbor (direction_t) const;

			virtual std::map <std::string, Character &> characters () const { return _characters; }

			virtual void enter (Character &);
			virtual void leave (Character &);

			virtual void clear_neighbors ();

			//virtual void pick_up (Item &);
			//virtual void drop (Item &);




	};
}
#endif
