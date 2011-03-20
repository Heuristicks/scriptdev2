#include "precompiled.h"
#include "../base/pet_ai.h"

struct MANGOS_DLL_DECL npc_buffmasterAI : public ScriptedPetAI
{
	npc_buffmasterAI(Creature* pCreature) : ScriptedPetAI(pCreature)
    {
	}
	
	void Reset()
	{
		if(Unit* m_owner = m_creature->GetOwner())
			m_owner->CastSpell(m_creature,60352,false);
	}

	void JustRespawned()
	{
		Reset();
	}
};

bool GossipHello_npc_buffmaster(Player* pPlayer, Creature* pCreature)
{
	if(pPlayer->GetGUID() != pCreature->GetOwnerGuid().GetCounter())
		return false;

	std::list<uint32> spell_ids;

	spell_ids.push_back(58452); //Armor
	spell_ids.push_back(48104); //Spirit
	spell_ids.push_back(48102); //Stamina
	spell_ids.push_back(58449); //Strength
	spell_ids.push_back(48100); //Intellect
	spell_ids.push_back(58451); //Agility
	spell_ids.push_back(72586); //Blessing of Forgotten Kings
	spell_ids.push_back(26035);	//Celebrate Good Times!

	std::list<uint32>::const_iterator itr;

	uint32 honorable_kills = pPlayer->GetUInt32Value(PLAYER_FIELD_KILLS);

	uint8 count = 0;
	for(itr = spell_ids.begin(); itr != spell_ids.end(); ++itr)
	{
		if((2^count)*10 > honorable_kills)
			break;
		pCreature->CastSpell(pPlayer, *itr, true);
		++count;
	}

	return true;
}

CreatureAI* GetAI_npc_buffmaster(Creature* pCreature)
{
    return new npc_buffmasterAI(pCreature);
}


void AddSC_npc_buffmaster()
{
    Script *newscript;

	newscript                   = new Script;
    newscript->Name             = "npc_buffmaster";
    newscript->pGossipHello     = &GossipHello_npc_buffmaster;
	newscript->GetAI			= &GetAI_npc_buffmaster;
    newscript->RegisterSelf();
}