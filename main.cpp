#include "Character.h"
#include "Player.h"
#include "Dragon.h"
#include "FelSpawn.h"
#include "Area.h"
#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>

using namespace Game;

enum class Actions { Go, Fight, Examine, PickUp, Drop, Talk, Quit, Nothing };

enum class Directions
{ NORTH, SOUTH, EAST, WEST, SOUTHEAST, SOUTHWEST, NORTHEAST, NORTHWEST };

enum class Types { Dragon, Troll, Item, Civilian, Area };

static std::map <std::string, Actions> _actions;
static std::map <std::string, Directions> _directions;
static std::map <std::string, Types> _types;
static std::shared_ptr <Player> _player;

static std::vector <std::shared_ptr<Area>> _areas;
static std::vector<std::shared_ptr<Character>> _characters;
static std::vector <std::shared_ptr <Item>> _items;

static void initialize ();
static void cleanup_characters ();
static void cleanup_areas ();
static void delete_characters ();
static void cleanup_items ();

int main (int argc, char* argv[])
{
	std::cout << "You are now playing Quest for Magic! Use the cmd-line to command your wizard"
				<< " through the lands, and through battle..." << std::endl;

	std::cout << "" << std::endl;

	initialize ();

	std::string input;
	std::string action;
	std::string what_action;

	std::string first, second, third, forth;

	bool running = true;
	while (running)
	{
		std::cout << std::endl;
		std::cout << "Please enter an action to proceed: ";
		input.clear ();
		getline (std::cin, input);
	
//		std::transform (input.begin(), input.end(), input.begin(), ::tolower);	
		action.clear ();
		what_action.clear ();

		first.clear(); second.clear(); third.clear(); forth.clear ();

		std::istringstream iss (input);
		iss >> first >> second >> third >> forth;

		// Handle cases where the first two input words combine into an Action.
		if (first == "pick" || first == "talk")
		{
			action = first + " " + second;
			what_action = third;
			if (forth != "")
				what_action += " " + forth;
		}
		else
		{
			action = first;
			what_action = second;
			if (third != "")
				what_action += " " + third;

			if (forth != "")
				what_action += " " + forth;
		}

		std::cout << "" << std::endl;
		try
		{
			switch (_actions.at(action))
			{
				case Actions::Fight:
						
					try
					{
						_player->fight (_player->area ()->characters ().at (what_action));
					}
					catch (std::out_of_range e)
					{
						std::cout << "That character is in a different area!" << std::endl;
						continue;
					}

					break;

				case Actions::Go:

					switch (_directions.at (what_action))
					{
						case Directions::NORTH:
							_player->go (direction_t::NORTH);
							break;

						case Directions::SOUTH:
							_player->go (direction_t::SOUTH);
							break;

						case Directions::WEST:
							_player->go (direction_t::WEST);
							break;

						case Directions::EAST:
							_player->go (direction_t::EAST);
							break;

						default:
							std::cout << "That is not a possible direction... " << std::endl;
					}

					break;

				case Actions::PickUp:
					
					try
					{
						_player->pick_up (_player->area()->get_item (what_action));	
					}
					catch (std::out_of_range e)
					{
						std::cout << "Can't pick up that!" << std::endl;
					}
					break;

				case Actions::Drop:
					try
					{
						_player->drop (_player->get_item (what_action));
					}
					catch (std::out_of_range e)
					{
						std::cout << "Can't drop that item! " << std::endl;
					}
					break;

				case Actions::Quit:
					if (second != "")
						break;

					std::cout << "You had a good run! Thank you for playing." << std::endl;
					delete_characters ();
					cleanup_areas ();
					cleanup_items ();
					running = false;
					break;

				default:
					std::cout << "Not a possible action!" << std::endl;
					continue;
			}
		}
		catch (std::out_of_range e)
		{
			std::cout << "Invalid input! Try again.. " << std::endl;
			continue;
		}

		if (_player->state == state_t::DEAD)
		{
			delete_characters ();
			cleanup_areas ();
			cleanup_items ();
			running = false;
		}
		cleanup_characters ();
	}

	return 0;

}

void initialize ()
{
	/* ---- CREATE AREAS ---*/
	std::string area1 = "Val'Sharath";
	std::string desc1 = "These woods once flourished, inhabited by mysterious creatures and guarded by the Night Elfs known as the Druids of the Moon. The once beautiful harmony between nature, beast and being was shattered by the Dark Legion during the invasion. Now all that is left is a dark forest with ruins of the once great citadel of the Night Elf, Darnassus.";
	std::string area2 = "Darkwood";
	std::string desc2 = "The neighboring woods to Val'Sharath, haunted by Dark Legion Dragons.";

	std::shared_ptr <Area> _start (new Area (area1, desc1));
	std::shared_ptr <Area> _darkwood (new Area (area2, desc2));
	std::shared_ptr <Area> _azsuna (new Area (static_cast<std::string> ("Azsuna"),
	static_cast <std::string> ("The Eastern shores of Broken Isles, occupated by hostile Nagas resisting the Dark Legion. The Nagas are known for their cunning in witchcraft and embraces the power of Frozen magic. ")));

	/* -----	CREATE CHARACTERS ----*/
	std::string name = "Merlin";
	std::string race = "Human";
	std::string name2 = "Thelryssa";
	std::string race2 = "Dragon";
	std::string dragon_name = "Neltharion";

	_player = std::shared_ptr <Player> (new Player (name, race, 500, 1000, 5, 50, 100, _start, 10, 10));
	std::shared_ptr <Dragon> _dragon  (new Dragon (name2, race2, 250, 150, 10, 50, 100, _start, 10, 0));
	std::shared_ptr <Dragon> _dragon2 (new Dragon (dragon_name, race2, 250, 250, 20, 70, 100, _darkwood, 20, 0));
	std::shared_ptr <FelSpawn> _felspawn (new FelSpawn (static_cast<std::string> ("Imp"), static_cast<std::string> ("Fel Spawn"),
	250, 5, 50, 100, _azsuna, 10, 0, 10));

	/*	----	ASSIGN CHARACTERS TO AREAS, AND AREAS TO AREAS (NEIGHBOURS) ----*/

	_start->add_neighbor (_darkwood, direction_t::NORTH);
	_darkwood->add_neighbor (_start, direction_t::SOUTH);
	_darkwood->add_neighbor (_azsuna, direction_t::EAST);
	_azsuna->add_neighbor (_darkwood, direction_t::WEST);

	_start->add_character (*_dragon);
	_darkwood->add_character (*_dragon2);
	_azsuna->add_character (*_felspawn);
	_start->enter (*_player);

	_characters.push_back (_player);
	_characters.push_back (_dragon);
	_characters.push_back (_dragon2);
	_characters.push_back (_felspawn);
	

	_areas.push_back (_start);
	_areas.push_back (_darkwood);
	_areas.push_back (_azsuna);
	
	std::cout << std::endl;

	/* ---- CREATE ITEMS ---- */
	
	std::shared_ptr <Keepable> _frostfire_blade (new Keepable (static_cast<std::string>(
	"Frostfire Blade"), 5, 1, 10, 5, 10, 50, 50));
	std::shared_ptr <Keepable> _tome_of_secrets (new Keepable (static_cast<std::string>(
	"Tome of Secrets"), 1, 1, 10, 0, 20, 20, 20));

	std::shared_ptr <Keepable> _dragon_tear (new Keepable (static_cast<std::string>("Dragon's Tear"),
	1, 1, 10, 0, 10, 5, 5));

	_player->pick_up (*_frostfire_blade);
	_player->pick_up (*_tome_of_secrets); 

	_dragon->pick_up (*_dragon_tear);

	_items.push_back (_frostfire_blade);
	_items.push_back (_tome_of_secrets);
	_items.push_back (_dragon_tear);
		
	/*	---- ASSIGN ENUMS TO MAPS FOR INPUT PARSING ---- */

	_actions ["go"] = Actions::Go;
	_actions ["fight"] = Actions::Fight;
	_actions ["examine"] = Actions::Examine;
	_actions ["pick up"] = Actions::PickUp;
	_actions ["drop"] = Actions::Drop;
	_actions ["talk to"] = Actions::Talk;
	_actions ["quit"] = Actions::Quit;
	_actions [""] = Actions::Nothing;

	_directions ["north"] = Directions::NORTH;
	_directions ["south"] = Directions::SOUTH;
	_directions ["east"] = Directions::EAST;
	_directions ["west"] = Directions::WEST;
	_directions ["southeast"] = Directions::SOUTHEAST;
	_directions ["southwest"] = Directions::SOUTHWEST;
	_directions ["northeast"] = Directions::NORTHEAST;
	_directions ["northwest"] = Directions::NORTHWEST;

	_types ["dragon"] = Types::Dragon;
	_types ["troll"] = Types::Troll;
	_types ["civilian"] = Types::Civilian;
	_types ["item"] = Types::Item;
	_types ["area"] = Types::Area;

}

void cleanup_characters ()
{
	auto it = _characters.begin ();
	while (it != _characters.end ())
	{
		if ((*it)->state == state_t::DEAD)
		{
			it = _characters.erase (it);
		}
		else
			++it;
	}
}

void cleanup_areas ()
{
	for (auto it = _areas.begin(); it != _areas.end(); ++it)
	{
		(*it)->clear_neighbors();
	}
	
	_areas.clear ();
}

void delete_characters ()
{
	auto it = _characters.begin ();
	while (it != _characters.end ())
	{
		it = _characters.erase (it);
	}

	_characters.clear ();
}

void cleanup_items ()
{
	_items.clear ();
}
