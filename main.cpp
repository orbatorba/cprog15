#include "Character.h"
#include "Player.h"
#include "Dragon.h"
#include "FelSpawn.h"
#include "GulDan.h"
#include "Area.h"
#include "Usable.h"
#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>

using namespace Game;

enum class Actions { Go, Fight, CheckBag, PickUp, Drop, Open, Quit, Use, Nothing };

enum class Directions
{ NORTH, SOUTH, EAST, WEST, SOUTHEAST, SOUTHWEST, NORTHEAST, NORTHWEST };

enum class Types { Dragon, Troll, Item, Civilian, Area };


static std::map <std::string, Actions> _actions;
static std::map <std::string, Directions> _directions;
static std::map <std::string, Types> _types;
std::shared_ptr <Player> _player;
std::shared_ptr <GulDan> _gul_dan;

std::vector <std::shared_ptr<Area>> _areas;
std::vector <std::shared_ptr<Character>> _characters;
std::vector <std::shared_ptr <Item>> _items;

static void initialize ();
static void display_help ();
static void cleanup_characters ();
static void cleanup_areas ();
static void delete_characters ();
static void cleanup_items ();

int main (int argc, char* argv[])
{
	std::cout << std::endl;
	std::cout << "You are now playing Quest for Magic! You are a wizard with deep knowledge in elements of Shadow, Fire and Frost. You have returned to the lands of Azeroth, which once was in harmony. Now these lands has been corrupted by the Burning Legion, led by the powerful Warlock Gul'Dan. It is your task to cleanse the lands of Fel by defeating Gul'Dan. Be cautious though, as you will need both experience in combat and powerful items in order to defeat Gul'Dan..." << std::endl;

	std::cout << "" << std::endl;

	display_help ();
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
		if (first == "pick" || first == "talk" || first == "check")
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

				case Actions::Open:
					_player->area()->open_chest (what_action);
					break;

				case Actions::CheckBag:
					if (what_action != "")
						break;

					_player->check_bag ();
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

				case Actions::Use:

					try
					{
						_player->use (_player->get_item (what_action));
					}
					catch (std::out_of_range e)
					{
						std::cout << "Can't use that item! " << std::endl;
					}

				case Actions::Quit:
					if (what_action != "")
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
		if (_player->area()->name() == "Karazhan" && _player->hasKey()) 
		{
			_player->area()->neighbor (direction_t::SOUTH)->setLocked(true);
			_player->drop(_player->get_item("Fel Key"));
			_player->area()->drop_item (_player->area()->get_item("Fel Key"));
		}

		if (_player->state == state_t::DEAD)
		{
			delete_characters ();
			cleanup_areas ();
			cleanup_items ();
			running = false;
		}
		if (_gul_dan->state == state_t::DEAD)
		{
			std::cout << "Gul'Dan falls to the ground, and the surrounding Fel Spawn suddenly becomes silent. After a little while they close in on you, and there is no way to go...  "<< std::endl;
			std::cout << "From the ashes of Gul'Dan's body you can hear a dark mumble" 
			<< "\n - 'No, leave him.. '" << std::endl;
			std::cout << " A green spirit lifts from Gul'Dan's corpse.. "<< std::endl;
			std::cout << " - 'This wizards fate is not yet sealed..', says the spirit as a dark portal opens.." << std::endl;
			std::cout << "The spirit fades into the portal, followed by the Fel Spawns... \n Karazhan brightens up with magic and spawning around you is the visions of the Great Mages of Kirin'Tor..." << std::endl;
			std::cout << " - 'You have proven your legacy, young Merlin! The blurred path we once foresaw for you has changed. You shall shall recieve the Mark of the Kirin'Tor, the greatest of accomodations, and join us on the path against the Burning Legion!'"<< std::endl;
			std::cout << " - 'I accept the Mark, Guardian. But when the time comes, I will face Gul'Dan again, alone. There is a reason I could cripple Gul'Dan today, where the power of Kirin'Tor has continously failed, but I don't know what it is..  You have been lying to me, Mages. All these years, you have refused to take me in amongst the Mages, my legacy. Now I start to see the true meaning of a wizard..'" << std::endl;
			std::cout << " - 'Merlin, you shall not interfere with the forces you do not understand!'"<< std::endl;

			std::cout << " - 'I will not take part of your fear towards the unknown, Guardian. We can not afford that anymore, and you know it aswell! I shall travel to Azshara, and explore the true story of my bloodline.'" << std::endl;
			std::cout << " - 'So be it, Wizard...'" << std::endl;
			
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
	std::string desc2 = "The neighboring woods to Val'Sharath, haunted by Dark Legion Dragons. You can hear terrifying sounds and lights emerging from the north. What is that? At the same time feel warm winds from the east with the smell of death, it must be coming from the Eastern Plaugelands! But why warm winds? Since the Scourge invaded the eastern lands there has been nothing but a cold wasteland....";

	std::string stratholme_desc = "A once peaceful and powerful Human city, was overtaken by the Scourge and Prince Arthas during the Third War. For several years, the city of Stratholme has been under the reign of Baron Rivendare, the Lich King's coordinator of the Plaugelands. During the invasion, Gul'Dan gained influence over the Scourge, by poisoning one of the city's Major Guardians with Fel, Dreadlord Balnazzar. It is rumored that Balnazzar now is a Fel Lord, and holds the key to the magical ruins of Karazhan, where Gul'Dan is believed to rule his army.";
	
	std::string deadwind_desc = "West of the Dark River and north of Darkwood stretches the rocky pathway of Deadwind Pass. No living being wanders this place, as the pass leads north to the ghostly citadel Karazhan. However, rumors in Darkshire says that red and green light have been seen on the rocky tops of Deadwind Pass. Also, terrifying roars have been heard. Some say it might be dragons, but green light? Sounds like the work of Gul'Dan... ";

	std::string karazhan_desc = "The magic citadel, Karazhan! You feel strong resemblance of this place, have you been here before? Everything around you starts shaking, and green goey hot liquid erupts from the ground. It must be as the Mages of Kirin'Tor has feared, the magical city Karazhan - once home to the Kirin'Tor - has been corrupted by fel! \n\n Someone notices your presence, and the ground you stand on gets lifted in the air and starts moving towards a great hall. Fel Spawns appear from the dark corners and surrounds you, while you hear a dark laughter so intense your body is vibrating.\n - 'This can't be..', you mumble. 'Who possesses such power?' \n - 'You are not prepared, wizard!' \n - 'Gul'Dan!' \n The doors locks behind you, there is no option but to fight Gul'Dan...";

	std::shared_ptr <Area> _start (new Area (area1, desc1));
	std::shared_ptr <Area> _darkwood (new Area (area2, desc2));
	std::shared_ptr <Area> _epl (new Area (static_cast<std::string> ("Eastern Plaugelands"),
	static_cast <std::string> ("The eastern lands of Lordearon, Eastern Kingdoms. These lands were once the Humans' military headquarters, with the military officers held up in the city of Stratholme. After the Third War and Scourge invasion, Eastern Plaugelands was lost and has since been held at bay by the Humans. With the Dark River coming down from the Northern Mountains, Eastern Plaugelands and Stratholme is seperated from the western parts of The Eastern Kingdoms. When Gul'Dan invaded, the Scourge has moved from these lands, and Fel Spawns now roams the plains. The question is, why would Gul'Dan overtake these lands? And why would the Scourge let him?")));
	std::shared_ptr <Area> _stratholme (new Area (static_cast<std::string>("Stratholme"), stratholme_desc));

	std::shared_ptr <Area> _deadwind_pass (new Area (static_cast<std::string>("Deadwind Pass"), deadwind_desc));
	
	std::shared_ptr <Area> _karazhan (new Area (static_cast<std::string>("Karazhan"), karazhan_desc));


	/*--- STORE AREAS --- */
	_areas.push_back (_start);
	_areas.push_back (_darkwood);
	_areas.push_back (_epl);
	_areas.push_back (_stratholme);
	_areas.push_back (_deadwind_pass);
	_areas.push_back (_karazhan);
	


	/* -----CREATE CHARACTERS ----*/
	std::string name = "Merlin";
	std::string race = "Human";
	std::string name2 = "Thelryssa";
	std::string race2 = "Dragon";
	std::string dragon_name = "Neltharion";

	_player = std::shared_ptr <Player> (new Player (name, race, 500, 500, 5, 50, 100, _start, 10, 10));

	std::shared_ptr <Dragon> _dragon (new Dragon (name2, race2, 250, 150, 10, 50, 100, _start, 10, 0));

	std::shared_ptr <Dragon> _dragon2 (new Dragon (dragon_name, race2, 250, 250, 20, 70, 100, _darkwood, 20, 0));

	std::shared_ptr <FelSpawn> _felimp (new FelSpawn (static_cast<std::string> ("Imp"),
		static_cast<std::string> ("Fel Spawn"), 250, 5, 50, 100, _epl, 10, 0, 10));

	std::shared_ptr <FelSpawn> _felhound (new FelSpawn (static_cast<std::string> ("Fel Hound"),
		static_cast<std::string> ("Fel Spawn"), 250, 5, 50, 100, _epl, 10, 0, 10));

	std::shared_ptr <FelSpawn> _fellord (new FelSpawn (static_cast<std::string> ("Lord Balnazzar"),
		static_cast<std::string> ("Fel Spawn"), 500, 10, 75, 200, _stratholme, 15, 10, 15));

	_gul_dan = std::shared_ptr <GulDan> (new GulDan (static_cast<std::string> ("Gul'Dan"),
		static_cast<std::string> ("Ancient Warlock"), 1000, 10, 100, 200, _stratholme, 15, 15, 15));

	std::shared_ptr <Dragon> _feldragon (new Dragon (static_cast<std::string>("Fel Dragon"), race2,
			350, 300, 20, 100, 100, _deadwind_pass, 30, 0));	


	/* --- STORE CHARACTERS ---*/
	_characters.push_back (_player);
	_characters.push_back (_dragon);
	_characters.push_back (_dragon2);
	_characters.push_back (_felimp);
	_characters.push_back (_felhound);
	_characters.push_back (_fellord);
	_characters.push_back (_feldragon);
	_characters.push_back (_gul_dan);

	/* ---- CREATE ITEMS ---- */
	
	std::shared_ptr <Keepable> _frostfire_blade (new Keepable (static_cast<std::string>("Frostfire Blade"), 5, 1, 10, 5, 10, 50, 50));
	std::shared_ptr <Keepable> _tome_of_secrets (new Keepable (static_cast<std::string>("Tome of Secrets"), 1, 1, 10, 0, 20, 20, 20));

	std::shared_ptr <Keepable> _dragon_tear (new Keepable (static_cast<std::string>("Dragon's Tear"), 1, 1, 10, 0, 10, 40, 50));

	std::shared_ptr <Keepable> _cloak_of_mystery (new Keepable (static_cast<std::string>("Mystery Cloak"), 1, 1, 50, 10, 50, 50, 50));
	std::shared_ptr <Keepable> _robe_of_wizards (new Keepable (static_cast<std::string>("Wizards Robe"), 1, 1, 50, 10, 100, 100, 50)); 

	std::shared_ptr <Usable> _health_potion (new Usable (static_cast <std::string> ("Health Potion"), 0, 200, 0));
	
	std::shared_ptr <Usable> _major_potion (new Usable (static_cast <std::string> ("Major Potion"), 0, 400, 0));
	
	std::shared_ptr <Usable> _mana_potion (new Usable (static_cast <std::string> ("Mana Potion"), 200, 0, 0));
	
	std::shared_ptr <Usable> _spell_potion (new Usable (static_cast <std::string> ("Spell Potion"), 0, 0, 10));

	std::shared_ptr <Keepable> _fel_key (new Keepable (static_cast <std::string> ("Fel Key"), 0, 0, 1000, 0, 0, 0, 0));

	std::shared_ptr <ContainerItem> _big_chest (new ContainerItem (static_cast <std::string> ("Big Chest"), 10, 1, 100));

	/* ---- STORE ITEMS --- */	
	_items.push_back (_frostfire_blade);
	_items.push_back (_tome_of_secrets);
	_items.push_back (_dragon_tear);
	_items.push_back (_cloak_of_mystery);
	_items.push_back (_robe_of_wizards);
	_items.push_back (_health_potion);
	_items.push_back (_major_potion);
	_items.push_back (_mana_potion);
	_items.push_back (_spell_potion);
	_items.push_back (_fel_key);
	_items.push_back (_big_chest);
	

	/* --- ASSIGN ITEMS TO CHARACTERS --- */	
	_player->pick_up (*_frostfire_blade);
	_player->pick_up (*_tome_of_secrets); 

	_dragon->pick_up (*_dragon_tear);
	_dragon->pick_up (*_health_potion);

	_dragon2->pick_up (*_mana_potion);
	_felhound->pick_up (*_cloak_of_mystery);
	_felimp->pick_up (*_spell_potion);
	_fellord->pick_up (*_fel_key);
	_feldragon->pick_up (*_robe_of_wizards);
	//_feldragon->pick_up (*_major_potion);

	/* --- ASSIGN ITEMS TO CHESTS, AND CHESTS TO AREAS --- */
	_big_chest->add (*_major_potion);
	_stratholme->add_chest (*_big_chest);


	/*	----	ASSIGN CHARACTERS TO AREAS, AND AREAS TO AREAS (NEIGHBOURS) ----*/

	_start->add_neighbor (_darkwood, direction_t::NORTH);
	_darkwood->add_neighbor (_start, direction_t::SOUTH);
	_darkwood->add_neighbor (_epl, direction_t::EAST);
	_darkwood->add_neighbor (_deadwind_pass, direction_t::NORTH);
	_epl->add_neighbor (_darkwood, direction_t::WEST);
	_epl->add_neighbor (_stratholme, direction_t::NORTH);
	_stratholme->add_neighbor (_epl, direction_t::SOUTH);
	_deadwind_pass->add_neighbor (_darkwood, direction_t::SOUTH);
	_deadwind_pass->add_neighbor (_karazhan, direction_t::NORTH);
	_karazhan->add_neighbor (_deadwind_pass, direction_t::SOUTH);
	_karazhan->setLocked (true);
	

	_start->add_character (*_dragon);
	_darkwood->add_character (*_dragon2);
	_epl->add_character (*_felimp);
	_epl->add_character (*_felhound);
	_stratholme->add_character (*_fellord);
	_deadwind_pass->add_character (*_feldragon);
	_karazhan->add_character (*_gul_dan);
	_start->enter (*_player);

	std::cout << std::endl;


	/*	---- ASSIGN ENUMS TO MAPS FOR INPUT PARSING ---- */

	_actions ["go"] = Actions::Go;
	_actions ["fight"] = Actions::Fight;
	_actions ["check bag"] = Actions::CheckBag;
	_actions ["pick up"] = Actions::PickUp;
	_actions ["drop"] = Actions::Drop;
	_actions ["open"] = Actions::Open;
	_actions ["quit"] = Actions::Quit;
	_actions ["use"] = Actions::Use;
	_actions [""] = Actions::Nothing;

	_directions ["north"] = Directions::NORTH;
	_directions ["south"] = Directions::SOUTH;
	_directions ["east"] = Directions::EAST;
	_directions ["west"] = Directions::WEST;
//	_directions ["southeast"] = Directions::SOUTHEAST;
//	_directions ["southwest"] = Directions::SOUTHWEST;
//	_directions ["northeast"] = Directions::NORTHEAST;
//	_directions ["northwest"] = Directions::NORTHWEST;

	_types ["dragon"] = Types::Dragon;
	_types ["troll"] = Types::Troll;
	_types ["civilian"] = Types::Civilian;
	_types ["item"] = Types::Item;
	_types ["area"] = Types::Area;

}

void display_help ()
{
	std::cout << " You will make use of the cmd-line to command your wizard"
				<< " through the lands, and through battle!" << std::endl;

	std::cout << std::endl;

	std::cout << "You can command your Wizard by the following commands: " << std::endl;
	std::cout << "-------------------------------------------------------" << std::endl;
	std::cout << "$ go 'direction' [north, south, east, west] - to move your character to a different "	 << "Area." << std::endl;
	std::cout << "$ fight 'character name' [Eg. 'fight Thelryssa'] - to fight a Character." << std::endl;
	std::cout << "$ pick up 'item name' [Eg. 'pick up Sword Of Truth'] - to pick up an Item." << std::endl;
	std::cout << "$ drop 'item name' - to drop an Item." << std::endl;
	std::cout << "$ use 'item name' - to use an Item. Remember, only some items are usable!" << std::endl;
	std::cout << "$ quit - to quit the game." << std::endl;
	std::cout << "-------------------------------------------------------" << std::endl;
	std::cout << std::endl;

	std::cout << "When in combat, you use the following commands: " << std::endl;	
	std::cout << "-------------------------------------------------------" << std::endl;
	std::cout << "$ cast 'spell name' [E.g 'cast frostbolt'] - to cast a spell. Spells cost mana"
	 << " but deal more damage!" << std::endl;
	
	//TODO: List possible spells
	
	std::cout << "$ attack - to attack with your weapon. Attacks spends your stamina!" << std::endl;
	std::cout << "$ flee - to flee from the battle. Fleeing will cost stamina!" << std::endl;
	std::cout << "--------------------------------------------------------" << std::endl;
	
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
