#include "precompiled.h"

enum {
	//Sneed's Shredder
	SPELL_REPAIR = 34619,
	SPELL_FEAR = 39176,
	SPELL_SHIELD = 29476,
	SPELL_BERSERK = 47008,
    SPELL_IGNOREARMOR = 54678,
	NPC_ROBOT = 100051,
	NPC_SNEED = 643
};

struct MANGOS_DLL_DECL boss_sneedsshredderAI : public ScriptedAI
{
    boss_sneedsshredderAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		Reset();
	}

	uint64 m_uiRobotGUID;
	uint32 m_uiFearTimer;
	uint32 m_uiRobotTimer;
	uint32 m_uiRepairTimer;
	uint32 m_uiBerserkTimer;
    uint32 m_uiIgnoreArmorTimer;
	bool m_bBerserk;
	bool fuelbot;

	void Reset()
    {
        m_uiFearTimer = urand(10000, 15000);    //Every 10-15 seconds
        m_uiRobotTimer = urand(30000,45000);     //Every 30-45 seconds
        m_uiRepairTimer = urand(30000, 45000);  //Cast no sooner than once every 30 seconds
		m_uiBerserkTimer = MINUTE*5*IN_MILLISECONDS;	//5 minutes
        m_uiIgnoreArmorTimer = 0;
		m_bBerserk = false;
		fuelbot = false;
        if(Creature* pRobot = (Creature*)(m_creature->GetMap()->GetUnit(m_uiRobotGUID)))
		{
			pRobot->ForcedDespawn();
		}
		m_uiRobotGUID = 0;
	}

	void JustSummoned(Creature* pSummoned) 
	{
		DoCastSpellIfCan(m_creature, SPELL_SHIELD);
		fuelbot = true;
	}

	void SummonedCreatureJustDied(Creature* pSummoned)
	{
        if(m_creature->HasAura(SPELL_SHIELD))
            m_creature->RemoveAurasDueToSpell(SPELL_SHIELD);
	    fuelbot = false;
	}

	void JustDied(Unit* pKiller) 
	{
		if(Unit* pRobot = m_creature->GetMap()->GetUnit(m_uiRobotGUID))
		{
			((Creature*)pRobot)->ForcedDespawn();
		}
		m_creature->SummonCreature(NPC_SNEED,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ(),m_creature->GetOrientation(),TEMPSUMMON_MANUAL_DESPAWN ,NULL);
	}

	void UpdateAI(const uint32 diff)
	{

		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
				return;
		if (m_uiFearTimer < diff)
		{
			if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
			{
				DoCastSpellIfCan(target,SPELL_FEAR);
				m_uiFearTimer = urand(10000, 15000); 
			}
		}
		else 
			m_uiFearTimer -= diff;
		if (m_uiRobotTimer < diff && !fuelbot)
		{
			if(Creature* pRobot = m_creature->SummonCreature(NPC_ROBOT,-308.481598f,-516.300110f,54.103420f,1,TEMPSUMMON_CORPSE_DESPAWN,3600000))
			{
				m_uiRobotGUID = pRobot->GetGUID();
			}
			m_uiRobotTimer = urand(45000,60000);
		}
		else 
			m_uiRobotTimer -= diff;
		if (m_uiRepairTimer < diff) 
		{
			DoCastSpellIfCan(m_creature,SPELL_REPAIR);
			m_uiRepairTimer = urand(30000, 60000);
		}
		else 
			m_uiRepairTimer -= diff;
        if(m_uiIgnoreArmorTimer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_IGNOREARMOR);
            m_uiIgnoreArmorTimer = urand(15000,25000);
        }
        else
            m_uiIgnoreArmorTimer -= diff;
		// Berserk after 5 minutes
		if(!m_bBerserk)
		{
			if (m_uiBerserkTimer < diff)
			{
				DoCastSpellIfCan(m_creature, SPELL_BERSERK);
				m_bBerserk = true;
			}
			else
			{
				m_uiBerserkTimer -= diff;
			}
		}
		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_boss_sneedsshredder(Creature* pCreature)
{
    return new boss_sneedsshredderAI(pCreature);
}

void AddSC_boss_sneedsshredder()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_sneedsshredder";
    NewScript->GetAI = &GetAI_boss_sneedsshredder;
    NewScript->RegisterSelf();
}