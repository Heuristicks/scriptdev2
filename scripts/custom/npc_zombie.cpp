#include "precompiled.h"
#include "nazi_zombies.h"

struct MANGOS_DLL_DECL npc_zombieAI : public ScriptedAI
{
	npc_zombieAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_Map = pCreature->GetMap();
		Reset();
    }

    ScriptedInstance* m_pInstance;
    Map* m_Map;
    Player* pTarget;

	void Reset()
    {
    }

	void JustDied(Unit* pKiller)
	{
		if(((Player*)pKiller)->StoreNewItemInBestSlots(100506,10))
			return;
	}

	void KilledUnit(Unit* pVictim)
	{
		pTarget = NULL;
	}

	void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            if(!pTarget)
            {
                pTarget = roll_chance_f(50) ? m_Map->GetPlayers().getFirst()->getSource() : m_Map->GetPlayers().getLast()->getSource();
            }
            /*else
            {
                if(m_pInstance)
                {
                    m_pInstance->PassZombieTarget(m_creature->GetGUID(),pTarget->GetGUID());
                }
            }*/
        }
        else
		{
            DoMeleeAttackIfReady();
			pTarget = NULL;
		}
    }
};

CreatureAI* GetAI_npc_zombie(Creature* pCreature)
{
    return new npc_zombieAI(pCreature);
}

void AddSC_npc_zombie()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "npc_zombie";
    NewScript->GetAI = &GetAI_npc_zombie;
    NewScript->RegisterSelf();
}