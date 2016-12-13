#ifndef GULDAN_H
#define GULDAN_H
#include "FelSpawn.h"

namespace Game
{
	class GulDan : public FelSpawn
	{
		protected:
			void initialize () override;
		
		public:
			GulDan ();
			GulDan (const std::string & name, const std::string & race, int hp, int armor,
			int damage, int stam, std::shared_ptr <Area> area, int fire_res = 0,
			int frost_res = 0, int shadow_res = 0);
	};

}
#endif
