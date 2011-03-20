#include "precompiled.h"

enum {
	//Spells
	SPELL_HAMMERSLAM = 37179,
	SPELL_WOUNDINGSTRIKE = 58830,
    SPELL_DEMOLISH = 34625,
    SPELL_BROKENBONES = 62354,
	SPELL_BERSERK = 47008
};

struct MANGOS_DLL_DECL boss_rhahkzorAI : public ScriptedAI
{
	boss_rhahkzorAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		Reset();
    }

	uint32 m_uiHammerSlamTimer;
    uint32 m_uiWoundingStrikeTimer;
    uint32 m_uiBerserkTimer;
    uint32 m_uiBrokenBonesTimer;
	bool m_bBerserk;

	void Reset()
    {
        m_uiBrokenBonesTimer = 0;
        m_uiWoundingStrikeTimer = urand(3000,6000);						//3 seconds
        m_uiBerserkTimer = MINUTE*5*IN_MILLISECONDS;				    //5 minutes
        m_uiHammerSlamTimer = 10000;									//10 seconds
		m_bBerserk = false;
    }

	void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
		{
            return;
		}

        // Wounding Strike
        if (m_uiWoundingStrikeTimer < uiDiff)
        {
           if(m_creature->getVictim()->HasAura(SPELL_WOUNDINGSTRIKE))
               DoCastSpellIfCan(m_creature->getVictim(),SPELL_DEMOLISH);
           else
               DoCastSpellIfCan(m_creature->getVictim(),SPELL_WOUNDINGSTRIKE);
            m_uiWoundingStrikeTimer = urand(3000,5000);
        }
        else
            m_uiWoundingStrikeTimer -= uiDiff;
		//Hammer Slam
		if (m_uiHammerSlamTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_HAMMERSLAM);
            m_uiHammerSlamTimer = 10000;
        }
        else
            m_uiHammerSlamTimer -= uiDiff;
        if(m_uiBrokenBonesTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_BROKENBONES);
            m_uiBrokenBonesTimer = 20000;
        }
        else
            m_uiBrokenBonesTimer -= uiDiff;
        // Berserk after 5 minutes
        if(!m_bBerserk)
		{
			if (m_uiBerserkTimer < uiDiff)
			{
				DoCastSpellIfCan(m_creature, SPELL_BERSERK);
				m_bBerserk = true;
			}
			else
				m_uiBerserkTimer -= uiDiff;
		}
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_rhahkzor(Creature* pCreature)
{
    return new boss_rhahkzorAI(pCreature);
}

void AddSC_boss_rhahkzor()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_rhahkzor";
    NewScript->GetAI = &GetAI_boss_rhahkzor;
    NewScript->RegisterSelf();
}