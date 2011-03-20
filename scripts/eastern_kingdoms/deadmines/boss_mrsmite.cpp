//Please ask before using my scripts, no matter how bad they may be

#include "precompiled.h"

#define CHARGE_EXPLAIN "Take no heed to my menacing auras' descriptions. Know that the lucky one should be followed and the unlucky should be feared."

enum {
	SPELL_THRASH = 3391,
	SPELL_SMITESTOMP = 6432,
	SPELL_BONESLICE = 70814,
	SPELL_POSITIVE = 28059,
	SPELL_NEGATIVE = 28084,
	SPELL_SPEED = 57060,
	SPELL_BERSERK = 47008,
	SPELL_BOMB = 64233,
};

struct MANGOS_DLL_DECL boss_mrsmiteAI : public ScriptedAI
{
    boss_mrsmiteAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		Reset();
	}

	bool Phase2;
	bool Phase3;
	bool chargedP;
	bool chargedN;
	bool m_bBerserk;
    bool explained;
	uint32 m_uiThrashTimer;
	uint32 m_uiSliceTimer;
	uint32 m_uiChargeTimer;
	uint32 m_uiBerserkTimer;
	uint32 m_uiPChargeTimer;
	uint32 m_uiNChargeTimer;

	void Reset()
    {
        m_uiThrashTimer = urand(5000, 7500);    
        m_uiSliceTimer = urand(10000,12500);    
        m_uiChargeTimer = 30000;
		m_uiPChargeTimer = 5000;
		m_uiNChargeTimer = 5000;
		m_uiBerserkTimer = MINUTE*5*IN_MILLISECONDS;
		Phase2 = false;
		Phase3 = false;
		chargedP = false;
		chargedN = false;
        explained = false;
		m_bBerserk = false;


        for (Map::PlayerList::const_iterator itr = m_creature->GetMap()->GetPlayers().begin(); itr != m_creature->GetMap()->GetPlayers().end(); ++itr)
		{
		    Player* pTemp = itr->getSource();
            if(pTemp)
            {
                if(pTemp->HasAura(SPELL_NEGATIVE))
                    pTemp->RemoveAurasDueToSpell(SPELL_NEGATIVE);
                if(pTemp->HasAura(SPELL_POSITIVE))
                    pTemp->RemoveAurasDueToSpell(SPELL_POSITIVE);
            }
        }
	}
	
	void JustDied (Unit* pKiller)
	{
		for (Map::PlayerList::const_iterator itr = m_creature->GetMap()->GetPlayers().begin(); itr != m_creature->GetMap()->GetPlayers().end(); ++itr)
		{
		    Player* pTemp = itr->getSource();
            if(pTemp)
            {
                if(pTemp->HasAura(SPELL_NEGATIVE))
                    pTemp->RemoveAurasDueToSpell(SPELL_NEGATIVE);
                if(pTemp->HasAura(SPELL_POSITIVE))
                    pTemp->RemoveAurasDueToSpell(SPELL_POSITIVE);
            }
        }
	}

	void UpdateAI(const uint32 diff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim()){
            return;
		}
		if (m_uiThrashTimer < diff)
		{
			DoCastSpellIfCan(m_creature->getVictim(),SPELL_THRASH);
			m_uiThrashTimer = urand(5000,75000); 
		}
		else
		{
			m_uiThrashTimer -= diff;
		}
		if (m_uiSliceTimer < diff)
		{
			DoCastSpellIfCan(m_creature->getVictim(),SPELL_BONESLICE);
			m_uiSliceTimer = urand(10000,12500); 
		}
		else
		{
			m_uiSliceTimer -= diff;
		}
		if (m_uiChargeTimer < diff)
		{
            if(!explained)
            {
                m_creature->MonsterSay(CHARGE_EXPLAIN,0,0);
                explained = true;
            }
			Unit* pTarget1 = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
			Unit* pTarget2 = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
			if (pTarget1)
			{
				pTarget1->CastSpell(pTarget1, SPELL_POSITIVE, false);
				chargedP = true;
			}
			if (pTarget2)
			{
				pTarget2->CastSpell(pTarget2, SPELL_NEGATIVE, false);
				chargedN = true;
			}
			m_uiChargeTimer = 30000;
		}
		else 
		{
			m_uiChargeTimer -= diff;
		}
		if(chargedP && m_uiPChargeTimer < diff)
		{
            for (Map::PlayerList::const_iterator itr = m_creature->GetMap()->GetPlayers().begin(); itr != m_creature->GetMap()->GetPlayers().end(); ++itr)
		    {
		        Player* pTemp = itr->getSource();
                if(pTemp)
                {
                    if(pTemp->HasAura(SPELL_POSITIVE))
                    {
                        for (Map::PlayerList::const_iterator itr = m_creature->GetMap()->GetPlayers().begin(); itr != m_creature->GetMap()->GetPlayers().end(); ++itr)
		                {
                            Player* pTempTarget = itr->getSource();
                            if(pTempTarget)
                            {
                                if(pTempTarget->IsWithinDist(pTemp,3.5f))
                                {
                                    pTemp->CastSpell(pTempTarget,SPELL_SPEED,true);
                                }
                            }
                        }
                        pTemp->RemoveAurasDueToSpell(SPELL_POSITIVE);
                        chargedP = false;
                    }
                }
			}
			m_uiPChargeTimer = 5000;
		}
		else if(chargedP)
			m_uiPChargeTimer -= diff;
		if(chargedN && m_uiNChargeTimer < diff)
		{
			for (Map::PlayerList::const_iterator itr = m_creature->GetMap()->GetPlayers().begin(); itr != m_creature->GetMap()->GetPlayers().end(); ++itr)
		    {
		        Player* pTemp = itr->getSource();
                if(pTemp)
                {
                    if(pTemp->HasAura(SPELL_NEGATIVE))
                    {
                        pTemp->CastSpell(pTemp,SPELL_BOMB,true);
                        pTemp->RemoveAurasDueToSpell(SPELL_NEGATIVE);
                        chargedN = false;
                    }
                }
			}
			m_uiNChargeTimer = 5000;
		}
		else if(chargedN)
			m_uiNChargeTimer -= diff;
		if (!Phase2 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 66)
		{
			Phase2 = true;
			DoCastSpellIfCan(m_creature->getVictim(),SPELL_SMITESTOMP);
		}
		if (!Phase3 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 33)
		{
			Phase3 = true;
			DoCastSpellIfCan(m_creature->getVictim(),SPELL_SMITESTOMP);
		}
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

CreatureAI* GetAI_boss_mrsmite(Creature* pCreature)
{
    return new boss_mrsmiteAI(pCreature);
}

void AddSC_boss_mrsmite()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_mrsmite";
    NewScript->GetAI = &GetAI_boss_mrsmite;
    NewScript->RegisterSelf();
}