#include "Player.h"
#include "Area.h"
#include "Keepable.h"
#include <iostream>
#include <vector>
#include <string>
#include <cctype>

namespace Game
{

	Player::Player () { }

//	Player::~Player () { }

	Player::Player (const std::string &name, const std::string &race, int hp, int mana, int armor,
			int damage, int stam, std::shared_ptr <Area> area, int fire_res, int frost_res, int shadow_res) :
	Character (name, race, hp, armor, damage, stam, area, fire_res, frost_res, shadow_res)
	{
		initialize ();
		has_mana = true;
		_mana = mana;
	}

	void Player::create_spells ()
	{
		Spell Fireball;
		Spell Frostbolt;

		Frostbolt.mana_cost = 10; Frostbolt.damage = 100;
		Frostbolt.element = element_t::FROST;

		Fireball.mana_cost = 15; Fireball.damage = 120;
		Fireball.element = element_t::FIRE;

		_spells["Frostbolt"] = Frostbolt;
		_spells["Fireball"] = Fireball;

	}

	void Player::initialize ()
	{
		//Populate the maps
		spell_match ["frostbolt"] = frost_bolt;
		spell_match ["fireball"] = fire_ball;

		verb_match ["cast"] = cast_t;
		verb_match ["attack"] = attack_t;
		verb_match ["flee"] = flee_t;

		create_spells ();

	}
	void Player::fight (Character & other)
	{
		std::cout << "You are in combat with " << other.name() << " which is a " << other.race()
		<< std::endl;
		std::cout << "" << std::endl;

		std::cout << "Use your arsenal of spells to defeat the " << other.race() << std::endl;
		std::cout << "Be cautious which spells you use. Different races are affected differently"
		<< " to your spells elements!" << std::endl;

		std::cout << "" << std::endl;

		std::string input;
		std::string action;
		std::string spell;

		this->state = state_t::FIGHTING;

		while (this->state == state_t::FIGHTING)
		{
			std::cout << "It is your turn to attack! You have " << this->_mana << " mana and "
			<< this->_stamina <<" stamina your disposal." << std::endl;

			std::cout << "" << std::endl;
			std::cout << "Input action to continue: ";
			input.clear ();
			getline (std::cin, input);
			action.clear ();
			spell.clear ();

			section_input (input, action, spell);

			try
			{
				switch (verb_match.at (action))
				{
					case cast_t:
						switch (spell_match.at (spell))
						{
							case frost_bolt:
								if ( _mana < _spells["Frostbolt"].mana_cost)
								{
									std::cout << "Not enough mana!" << std::endl;
									continue;
								}

								other.spell_dmg_taken(_spells["Frostbolt"].damage + _spell_power,
														element_t::FROST);
								this->_mana -= _spells["Frostbolt"].mana_cost;
								break;

							case fire_ball:
								if ( _mana < _spells["Fireball"].mana_cost)
								{
									std::cout << "Not enough mana!" << std::endl;
									continue;
								}

								other.spell_dmg_taken (_spells["Fireball"].damage + _spell_power,
														element_t::FIRE);
								this->_mana -= _spells["Fireball"].mana_cost;
								break;

							default:
								std::cout << "I do not recognize that spell!" << std::endl;
								break;

						}

					break;

					case attack_t:
						if (this->_stamina < 10)
						{
							std::cout << "You don't have enough stamina to attack " << other.name ()
							<< " with your staff! Hope you at least have some mana left... "
							<< std::endl;

							continue;
						}

						other.damage_taken (this->_damage + _attack_power);
						this->_stamina -= 10;
						std::cout << "Nice swing! You hit " << other.name () << "!" << std::endl;
						break;

					case flee_t:
						if (this->_stamina < 20)
						{
							std::cout << "You don't have enough stamina to flee! Consider using"
							<< " your spells to defeat " << other.name () << "... " << std::endl;
							continue;
						}

						this->_stamina -= 20;
						std::cout << "You flee in fear of " << other.name () << "!"
						<< " Maybe you should gather some strength and wisdom before you fight"
						<< " again... " << std::endl;
						this->state = state_t::IDLE;
						break;

					default:
						std::cout << "I did not catch that. Quick now, we are in combat!"
						 << " Should we cast spells, attack with wand or flee?" << std::endl;
						continue;

				}
			}

			catch (std::out_of_range e)
			{
				std::cout << "Invalid input! Try again.." << std::endl;
				continue;
			}

			if (other.health () <= 0)
			{
				//other.state = state_t::DEAD;
				other.die ();
				std::cout << "You have defeated " << other.name () << " in combat!" << std::endl;
				this->state = state_t::IDLE;
				break;
			}

			// Enemys' turn to attack!
			other.fight (*this);

			if (this->_health <= 0)
			{
				std::cout << "You died from the fury of " << other.name () << "!" << std::endl;
				this->state = state_t::DEAD;
				break;
			}

			display_stats ();
			other.display_stats ();

		}

	}

	bool Player::pick_up (Keepable & item)
	{
		if (_bag.add (item))
		{
			std::cout << "You picked up " << item.name () << "!" << std::endl;
			_spell_power += item.spell_power ();
			_attack_power += item.attack_power ();
			hitpoints += item.health_points ();
			_mana += item.mana_points ();
			_area->drop_item (item);
			
		}
	}

	bool Player::drop (Keepable & item)
	{
		if (_bag.remove (item))
		{
			std::cout << "You dropped " << item.name () << "!" << std::endl;
			_spell_power -= item.spell_power ();
			_attack_power -= item.attack_power ();
			hitpoints -= item.health_points ();
			_mana -= item.mana_points ();
			_area->add_item (item);
		}
	}



	// Function inspired from:
	// http://cplussplussatplay.blogspot.se/2012/11/text-adventure-games-c-part-1.html
	void Player::section_input (std::string Cmd, std::string &wd1, std::string &wd2)
	{
		std::string sub_str;
		std::vector <std::string> words;
		char search = ' ';
		size_t i;

		// Split Command into vector
		for(i = 0; i < Cmd.size(); i++)
		{
			if(Cmd.at(i) != search)
				sub_str.insert(sub_str.end(), Cmd.at(i));

			if(i == Cmd.size() - 1)
			{
				words.push_back(sub_str);
				sub_str.clear();
			}

			if(Cmd.at(i) == search)
			{
				words.push_back(sub_str);
				sub_str.clear();
			}

		}

		// Clear out any blanks
		// I work backwords through the vectors here as a cheat not to invalidate the iterator
		for(i = words.size() - 1; i > 0; i--)
		{
			if(words.at(i) == "")
				words.erase(words.begin() + i);
		 }

		// Make words upper case
		/* for(i = 0; i < words.size(); i++)
		{
				for(j = 0; j < words.at(i).size(); j++)
				{
					if(islower(words.at(i).at(j)))
						words.at(i).at(j) = toupper(words.at(i).at(j));
				}
		}*/

		//I only want the first two words at most (verb / noun).
		if(words.size() == 0)
			std::cout << "No command given" << std::endl;

		if(words.size() == 1)
		{
			wd1 = words.at(0);
		}

		if(words.size() == 2)
		{
			wd1 = words.at(0);
			wd2 = words.at(1);
		}

		if(words.size() > 2)
			std::cout << "Command too long. Only type one or two words (verb and noun)"
			 << std::endl;
	}


}
