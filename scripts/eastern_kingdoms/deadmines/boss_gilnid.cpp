//Please ask before using my scripts, no matter how bad they may be

#include "precompiled.h"
#include "deadmines.h"

enum
{
	//Gildnid
	SPELL_MOLTEN = 62373,
	SPELL_BERSERK = 47008,
	SPELL_CUTMETAL = 65104,
	SPELL_BURNINGOIL = 40239,
	SPELL_STUN = 39796,
	SPELL_SLOW = 24415
};

struct MANGOS_DLL_DECL boss_gilnidAI : public ScriptedAI
{
	boss_gilnidAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Reset();
	}

	uint32 m_uiMoltenTimer;
	uint32 m_uiCutMetalTimer;
	uint32 m_uiAoETimer;
	uint32 m_uiBerserkTimer;
	float fX;
	float fY;
	float fZ;
	bool m_bBerserk;
	bool m_bDoingAoE;

    ScriptedInstance* m_pInstance;

	uint64 m_uiTargetGUID;
	
	void Reset()
    {
        m_uiMoltenTimer = urand(20000, 30000);   
        m_uiCutMetalTimer = urand(5000,10000);   
        m_uiAoETimer = 45000;
		m_uiBerserkTimer = MINUTE*5*IN_MILLISECONDS;
		m_bBerserk = false;
		m_bDoingAoE = false;
		fX = 0;
		fY = 0;
		fZ = 0;
		m_uiTargetGUID = 0;
	}

	void UpdateAI(const uint32 diff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
		{
            return;
		}
		if (m_uiCutMetalTimer < diff)
		{
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
			{
				DoCastSpellIfCan(pTarget,SPELL_CUTMETAL);
				m_uiCutMetalTimer = urand(5000,10000);
			}
		}
		else 
			m_uiCutMetalTimer -= diff;
		if (!m_bDoingAoE && m_uiAoETimer < diff)
		{
			if (Unit* pSqTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
			{
				fX = pSqTarget->GetPositionX();
				fY = pSqTarget->GetPositionY();
				fZ = pSqTarget->GetPositionZ();
				m_uiTargetGUID = pSqTarget->GetGUID();
                if(m_pInstance)
                {
                    if(GameObject* pGo = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(DATA_GILNID_FIRE)))
                    {
                        m_pInstance->SetData(TYPE_GILNID_FIRE,IN_PROGRESS);
                        pGo->Relocate(fX,fY,fZ);
                    }
                }
			}
			m_bDoingAoE = true;
		}
		else if (!m_bDoingAoE)
			m_uiAoETimer -= diff;
		if (m_bDoingAoE)
		{
			Unit* pTarget = m_creature->GetMap()->GetUnit(m_uiTargetGUID);
			if(pTarget && pTarget->IsWithinDist3d(fX, fY, fZ, 5))
			{
				if (!pTarget->HasAura(SPELL_BURNINGOIL))
				{
					DoCastSpellIfCan(pTarget, SPELL_BURNINGOIL);
					DoCastSpellIfCan(pTarget,SPELL_SLOW);
				}
			}
			else
			{
				m_bDoingAoE = false;
				fX = 0;
				fY = 0;
				fZ = 0;
				m_uiTargetGUID = 0;
				m_uiAoETimer = 45000;
			}
		}
		if (m_uiMoltenTimer < diff)
		{
			DoCastSpellIfCan(m_creature, SPELL_MOLTEN);
			m_uiMoltenTimer = urand(20000,30000);
		}
		else
			m_uiMoltenTimer -= diff;
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

CreatureAI* GetAI_boss_gilnid(Creature* pCreature)
{
    return new boss_gilnidAI(pCreature);
}

void AddSC_boss_gilnid()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_gilnid";
    NewScript->GetAI = &GetAI_boss_gilnid;
    NewScript->RegisterSelf();
}






