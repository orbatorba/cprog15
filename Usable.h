#ifndef USABLE_H
#define USABLE_H
#include "Keepable.h"

namespace Game
{
	class Usable : public Keepable
	{
		
		protected:
			unsigned int _mana_gain;
			unsigned int _health_gain;
			unsigned int _all_resistance;

		public:
		
			Usable ();
			Usable (const std::string &, unsigned int, unsigned int, unsigned int);	
			//virtual void use ();
			virtual unsigned int mana_gain () const override { return _mana_gain; }
			virtual unsigned int health_gain () const override { return _health_gain; }
			virtual unsigned int all_resistance () const override { return _all_resistance; }


					 
	};

}


#endif
