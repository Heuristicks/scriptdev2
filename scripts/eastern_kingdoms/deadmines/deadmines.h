/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_DEADMINES_H
#define DEF_DEADMINES_H

enum
{
    MAX_ENCOUNTER           = 1,

    TYPE_DEFIAS_ENDDOOR     = 1,
	TYPE_PHASE_TWO  		= 2,
	TYPE_MOB_ONE			= 3,
	TYPE_MOB_TWO			= 4,
	TYPE_MOB_THREE			= 5,
	TYPE_MOB_FOUR			= 6,
	TYPE_PHASE_TWO_DONE		= 7,
    TYPE_END                = 8,
    TYPE_ARREST_STEP        = 9,
    TYPE_GILNID_FIRE        = 10,
    DATA_DEFIAS_DOOR        = 2,
	DATA_GILNID_FIRE		= 3,

    INST_SAY_ALARM1         = -1036000,
    INST_SAY_ALARM2         = -1036001,

	GO_GILNID_FIRE			= 193203,
    GO_DOOR_LEVER           = 101833,
    GO_IRON_CLAD            = 16397,
    GO_DEFIAS_CANNON        = 16398,
	GO_BUNKER_PORTAL		= 400000,
	GO_VANCLEEF_PORTAL		= 400005,
	GO_BUNKER_PORTAL11		= 400001,
	GO_BUNKER_PORTAL12		= 400006,
	GO_BUNKER_PORTAL13		= 400007,

	GO_BUNKER_PORTAL21		= 400002,
	GO_BUNKER_PORTAL22		= 400008,
	GO_BUNKER_PORTAL23		= 400009,

	GO_BUNKER_PORTAL31		= 400003,
	GO_BUNKER_PORTAL32		= 400010,
	GO_BUNKER_PORTAL33		= 400011,

	GO_BUNKER_PORTAL41		= 400004,
	GO_BUNKER_PORTAL42		= 400012,
	GO_BUNKER_PORTAL43		= 400013,

    GO_VANCLEEF_CHEST       = 410000,
	
    NPC_MR_SMITE            = 646,
    NPC_PIRATE              = 657,

    NPC_VANCLEEF            = 639
};

#endif
