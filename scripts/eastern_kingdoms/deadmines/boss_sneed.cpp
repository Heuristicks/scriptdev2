#include "precompiled.h"

enum
{
	//Sneed
	SPELL_DISARM = 6713,
	SPELL_ROCKET = 71786,
	SPELL_ENRAGE = 64217,
	SPELL_BERSERK = 47008,
    SPELL_BOMBEXPLODE = 71746,
	NPC_MINION = 100052,
    NPC_SNEEDMINE = 100054
};

struct MANGOS_DLL_DECL boss_sneedAI : public ScriptedAI
{
    boss_sneedAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		Reset();
	}

	uint64 m_uiAssistantGUID[5];
    uint32 m_uiMinionCount;
	uint32 m_uiDisarmTimer;
	uint32 m_uiRocketTimer;
	uint32 m_uiMinionTimer;
	uint32 m_uiBerserkTimer;
    uint32 m_uiBombTimer;
	bool m_bBerserk;
	bool minions;

	void Reset()
    {
        m_uiDisarmTimer = urand(10000, 15000);    //Every 10-15 seconds
        m_uiRocketTimer = urand(15000,30000);  
        m_uiMinionTimer = urand(30000, 45000);  //Cast no sooner than once every 45 seconds
        m_uiBombTimer = 20000;
		m_uiBerserkTimer = MINUTE*5*IN_MILLISECONDS;
		m_bBerserk = false;
		minions = false;
		for(uint32 i=0; i < 5; ++i)
        {
            if (m_uiAssistantGUID[i])
            {
                if (Creature* pAssistant = (Creature*)(m_creature->GetMap()->GetUnit(m_uiAssistantGUID[i])))
                {
                    if (pAssistant->isAlive())
                        pAssistant->ForcedDespawn();
                }
                m_uiAssistantGUID[i] = 0;
            }
        }
	}

	void JustDied(Unit* pKiller)
    {
		for(uint32 i=0; i < 5; ++i)
        {
            if (m_uiAssistantGUID[i])
            {
                if (Creature* pAssistant = (Creature*)(m_creature->GetMap()->GetUnit(m_uiAssistantGUID[i])))
                {
                    if (pAssistant->isAlive())
                        pAssistant->ForcedDespawn();
                }

                m_uiAssistantGUID[i] = 0;
            }
        }
	}

	void SummonedCreatureJustDied(Creature* pSummoned)
	{
		DoCastSpellIfCan(m_creature,SPELL_ENRAGE);
	}

	void UpdateAI(const uint32 diff)
	{
	    if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;
	    if (m_uiRocketTimer < diff)
	    {
		    if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
		    {
			    DoCastSpellIfCan(target,SPELL_ROCKET);
			    m_uiRocketTimer = urand(30000,45000); 
		    }
        }
	    else 
		    m_uiRocketTimer -= diff;
	    if (m_uiDisarmTimer < diff)
	    {
		    DoCastSpellIfCan(m_creature->getVictim(),SPELL_DISARM);
		    m_uiDisarmTimer = urand(10000,15000);
        }
        else 
            m_uiDisarmTimer -= diff;
        if (m_uiMinionTimer < diff && !minions) 
	    {
		    for(uint32 i = 0; i < 5; ++i) 
		    {
			    Creature* pAssistant = m_creature->SummonCreature(NPC_MINION, ((m_creature->GetPositionX())+(urand(2,3))), ((m_creature->GetPositionY())+(urand(2,3))),m_creature->GetPositionZ(), m_creature->GetOrientation(),TEMPSUMMON_CORPSE_DESPAWN, NULL);
			    if(pAssistant)
			    {
				    m_uiAssistantGUID[i] = pAssistant->GetGUID();
			    }
		    }
		    minions = true;
		    m_uiMinionTimer = urand(30000, 45000);
	    }
        else 
		    m_uiMinionTimer -= diff;
	    // Berserk after 5 minutes
        if(!m_bBerserk)
	    {
		    if (m_uiBerserkTimer < diff)
		    {
			    DoCastSpellIfCan(m_creature, SPELL_BERSERK);
			    m_bBerserk = true;
		    }
		    else
			    m_uiBerserkTimer -= diff;
	    }
	    DoMeleeAttackIfReady();
	}
};

struct MANGOS_DLL_DECL npc_sneed_mineAI : public ScriptedAI
{
    npc_sneed_mineAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		Reset();
	}

    uint32 m_uiDetonateTimer;

    void Reset() { m_uiDetonateTimer = 0; }

    void UpdateAI(const uint32 diff)
    {
        if(m_uiDetonateTimer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_BOMBEXPLODE);
        }
        else
            m_uiDetonateTimer -= diff;
    }
};

CreatureAI* GetAI_boss_sneed(Creature* pCreature)
{
    return new boss_sneedAI(pCreature);
}

CreatureAI* GetAI_npc_sneed_mine(Creature* pCreature)
{
    return new npc_sneed_mineAI(pCreature);
}

void AddSC_boss_sneed()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_sneed";
    NewScript->GetAI = &GetAI_boss_sneed;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "npc_sneed_mine";
    NewScript->GetAI = &GetAI_npc_sneed_mine;
    NewScript->RegisterSelf();
}