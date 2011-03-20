//Please ask before using my scripts, no matter how bad they may be

#include "precompiled.h"

enum {
    SPELL_CLEAVE        = 40505,
    SPELL_POISON        = 50255,
    SPELL_BERSERK       = 47008,

    SPELL_STUN          = 39568,
    SPELL_SILENCE       = 38913,
    SPELL_ROOT          = 75215,
};

struct MANGOS_DLL_DECL boss_greenskinAI : public ScriptedAI
{
    boss_greenskinAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiCleaveTimer;
    uint32 m_uiPoisonTimer;
    uint32 m_uiRouletteTimer;
    uint32 m_uiBerserkTimer;

    bool m_bBerserk;

    void Reset()
    {
        m_uiCleaveTimer = 5000;
        m_uiPoisonTimer = 15000;
        m_uiRouletteTimer = 45000;
        m_uiBerserkTimer = 5*MINUTE*IN_MILLISECONDS;

        m_bBerserk = false;
    }

    void DoRoulette()
    {
        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        for (ThreatList::const_iterator iter = tList.begin();iter != tList.end(); ++iter)
        {
            if (Unit* pTarget = m_creature->GetMap()->GetUnit((*iter)->getUnitGuid()))
            {
                switch(urand(1,3))
                {
					case 1:
						pTarget->CastSpell(pTarget,SPELL_STUN,false);
						break;
					case 2:
						pTarget->CastSpell(pTarget,SPELL_ROOT,false);
						break;
					case 3:
						DoCastSpellIfCan(pTarget,SPELL_SILENCE);
						break;
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
        if(m_uiCleaveTimer < diff)
        {
            if(Unit* pTarget = m_creature->getVictim())
            {
                DoCastSpellIfCan(pTarget,SPELL_CLEAVE);
                m_uiCleaveTimer = 5000;
            }
        }
        else
        {
            m_uiCleaveTimer -= diff;
        }
        if(m_uiPoisonTimer < diff)
        {
            if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
            {
                DoCastSpellIfCan(pTarget,SPELL_POISON);
                m_uiPoisonTimer = 15000;
            }
        }
        else
        {
            m_uiPoisonTimer -= diff;
        }
        if(m_uiRouletteTimer < diff)
        {
            DoRoulette();
            m_uiRouletteTimer = 45000;
        }
        else 
        {
            m_uiRouletteTimer -= diff;
        }
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

CreatureAI* GetAI_boss_greenskin(Creature* pCreature)
{
    return new boss_greenskinAI(pCreature);
}

void AddSC_boss_greenskin()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_greenskin";
    NewScript->GetAI = &GetAI_boss_greenskin;
    NewScript->RegisterSelf();
}
