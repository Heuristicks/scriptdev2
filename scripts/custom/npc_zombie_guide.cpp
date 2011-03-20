#include "precompiled.h"
#include "nazi_zombies.h"

bool GossipHello_npc_zombie_guide(Player* pPlayer, Creature* pCreature)
{
    pPlayer->ADD_GOSSIP_ITEM(0, "Would you like to start the Defias Zombie invasion?", GOSSIP_SENDER_MAIN, 1000);
    pPlayer->ADD_GOSSIP_ITEM(0, "What level is my group on?", GOSSIP_SENDER_MAIN, 2000);

    pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE ,pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_zombie_guide(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
    switch(uiAction)
    {
        case 1000:
			pCreature->MonsterWhisper("Good luck!", pPlayer,false);
			if(pInstance)
				pInstance->SetData(TYPE_START_GAME,0);
			pPlayer->CLOSE_GOSSIP_MENU();
        case 2000:
			/*if(pInstance)
				pCreature->MonsterWhisper("Your current level is %u.",pInstance->GetData(TYPE_LEVEL),pPlayer, false);*/
            pPlayer->CLOSE_GOSSIP_MENU();
			break;
    }
    return true;
}

void AddSC_npc_zombie_guide()
{
    Script *newscript;

	newscript                   = new Script;
    newscript->Name             = "npc_zombie_guide";
    newscript->pGossipHello     = &GossipHello_npc_zombie_guide;
    newscript->pGossipSelect    = &GossipSelect_npc_zombie_guide;
    newscript->RegisterSelf();
}