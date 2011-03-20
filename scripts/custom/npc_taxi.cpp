#include "precompiled.h"

bool GossipHello_npc_taxi(Player* pPlayer, Creature* pCreature)
{
	uint32 inscription_buffs[6] = { 58451, 58449, 48100, 48104, 48102, 58452 };
	if(pPlayer->IsMounted())
	{
		const Unit::AuraList mountAuras = pPlayer->GetAurasByType(SPELL_AURA_MOUNTED);
		for(Unit::AuraList::const_iterator itr = mountAuras.begin(); itr != mountAuras.end(); ++itr)
		{
			Aura* aur = *itr;
			if(aur)
			{
				pPlayer->RemoveAura(aur);
			}
		}
	}
	for(int i = 0; i < 6; ++i)
	{
		pPlayer->CastSpell(pPlayer,inscription_buffs[i],false);
	}
	return true;
}

void AddSC_npc_taxi()
{
	Script *newscript;

	newscript                   = new Script;
    newscript->Name             = "npc_taxi";
    newscript->pGossipHello     = &GossipHello_npc_taxi;
    newscript->RegisterSelf();

}