#include "common.h"
#include "include.h"

#define FLAG_PROTECTION(time, flag)           \
	do                                        \
	{                                         \
		if (time > 0)                         \
			time--;                           \
		else                                  \
		{                                     \
			flag = 0;                         \
			GAIN_SET(L_GAIN, M_GAIN, R_GAIN); \
		}                                     \
	} while (0)

void Flag_protection(void)
{
	FLAG_PROTECTION(Ecc_Car->Protect.round, Ecc_Car->Round.state);
	FLAG_PROTECTION(Ecc_Car->Protect.obstacle, Ecc_Car->Flag.obstacle);
}