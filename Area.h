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
	class Keepable;
	class ContainerItem;
	class Area
	{

		protected:

			std::string _name;
			std::string _description;
			bool need_key = false;

			std::map <direction_t, std::shared_ptr <Area>> _neighbors;
			std::map <std::string, Keepable &> _items;
			std::map <std::string, Character &> _characters;
			std::map <std::string, ContainerItem &> _chests;

		public:

			Area ();
			Area (const std::string & name, const std::string & desc);
			Area (const std::shared_ptr <Area> other);
			~Area ();

			std::string description () const { return _description; }
			std::string name () const { return _name; }
			bool isLocked () const { return need_key; }
			void setLocked (bool key) { need_key = key; }

			std::map <direction_t, std::shared_ptr <Area>> neighbors () const 
			{
				 return _neighbors;
			 }
			void add_neighbor (std::shared_ptr <Area>, direction_t);
			std::shared_ptr <Area> neighbor (direction_t) const;
//			 std::shared_ptr <Area> neighbor (direction_t) const;
			bool has_neighbor (direction_t) const;

			std::map <std::string, Character &> characters () const { return _characters; }

			void enter (Character &);
			void leave (Character &);
			void clear_neighbors ();
			void add_character (Character &);
			void remove_character (Character &);
			
			void add_item (Keepable &);
			void drop_item (Keepable &);
			Keepable & get_item (std::string);
			void add_chest (ContainerItem &);
			void open_chest (std::string &);
			void add_to_chest (std::string &, Keepable &);

			void list_characters (std::string) const;
			void list_character_items (std::string) const;
			void list_items () const;
			
	};
}
#endif
