//Please ask before using my scripts, no matter how bad they may be

#include "precompiled.h"
#include "deadmines.h"

enum 
{
	SPELL_THRASH = 3391,
	SPELL_HEMOTOXIN = 58782,
	SPELL_ASCEND = 64487,
	SPELL_BERSERK = 47008,
    SPELL_SHADOWSTEP = 63793,
    SPELL_VANISH = 26888,
    SPELL_AMBUSH = 41390,
    SPELL_CHEAPSHOT = 30986,
    SPELL_ACCELERATE = 58820,

	NPC_MINION	 = 100053
};

#define BUNKER_ONE_X	-155.729202f
#define BUNKER_ONE_Y	-1162.078613f
#define BUNKER_ONE_Z	302.165283f
#define BUNKER_ONE_O	4.152422f

#define BUNKER_TWO_X	-280.639893f
#define BUNKER_TWO_Y	-1128.343506f
#define BUNKER_TWO_Z	302.165192f
#define BUNKER_TWO_O	4.255481f

#define BUNKER_THREE_X	-403.648438f
#define BUNKER_THREE_Y	-1067.263062f
#define BUNKER_THREE_Z	302.165161f
#define BUNKER_THREE_O	4.010479f

#define BUNKER_FOUR_X	-466.752686f
#define BUNKER_FOUR_Y	-962.396606f
#define BUNKER_FOUR_Z	302.165314f
#define BUNKER_FOUR_O	0.542290f

#define PHASE_THREE_X   -504.034821f
#define PHASE_THREE_Y   -1289.834229f
#define PHASE_THREE_Z   357.806641f
#define PHASE_THREE_O   2.112040f

#define VANCLEEF_SHIP_X	-64.4683f
#define VANCLEEF_SHIP_Y	-819.627014f
#define VANCLEEF_SHIP_Z 41.132f
#define VANCLEEF_SHIP_O 4.73688f

struct MANGOS_DLL_DECL boss_vancleefAI : public ScriptedAI
{
    boss_vancleefAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Reset();
	}

	bool m_bPhase2;
    bool m_bPhase3;
    bool m_bPhaseThree;
    bool m_bEnd;
    bool m_bTeleported;
    bool m_bAscend;
	bool minionsAlive[4];
	ScriptedInstance* m_pInstance;

	uint32 m_uiMinionTimer;

	uint32 m_uiAscendTimer;
	uint32 m_uiBerserkTimer;
	bool m_bBerserk;

	uint32 m_uiThrashTimer;
	uint32 m_uiToxinTimer;
    uint32 m_uiTargetSwitchTimer;
    uint64 m_uiMinionsGUID[4];

	bool minions;

	void SpawnMinions()
	{
		Creature* pMinion[4];
		if(pMinion[0] = m_creature->SummonCreature(NPC_MINION, BUNKER_ONE_X, BUNKER_ONE_Y, BUNKER_ONE_Z, BUNKER_ONE_O,TEMPSUMMON_CORPSE_DESPAWN,3600000))
        {
		    minionsAlive[0] = true;
            m_uiMinionsGUID[0] = pMinion[0]->GetGUID();
        }
		if(pMinion[1] = m_creature->SummonCreature(NPC_MINION, BUNKER_TWO_X, BUNKER_TWO_Y, BUNKER_TWO_Z, BUNKER_TWO_O,TEMPSUMMON_CORPSE_DESPAWN,3600000))
        {
            minionsAlive[1] = true;
            m_uiMinionsGUID[1] = pMinion[1]->GetGUID();
        }
		if(pMinion[2] = m_creature->SummonCreature(NPC_MINION, BUNKER_THREE_X, BUNKER_THREE_Y, BUNKER_THREE_Z, BUNKER_THREE_O,TEMPSUMMON_CORPSE_DESPAWN,3600000))
        {
            minionsAlive[2] = true;
            m_uiMinionsGUID[2] = pMinion[2]->GetGUID();
        }
		if(pMinion[3] = m_creature->SummonCreature(NPC_MINION, BUNKER_FOUR_X, BUNKER_FOUR_Y, BUNKER_FOUR_Z, BUNKER_FOUR_O,TEMPSUMMON_CORPSE_DESPAWN,3600000))
        {
            minionsAlive[3] = true;
            m_uiMinionsGUID[3] = pMinion[3]->GetGUID();
        }
		minions = true;
	}

	void Reset()
    {
		m_bPhase2 = false;
        m_bPhase3 = false;
        m_bTeleported = false;
        m_bPhaseThree = true;
        m_bEnd = false;
        m_bAscend = false;
		m_uiMinionTimer = 0;
		m_uiAscendTimer = 8000;
		m_uiToxinTimer = 15000;
		m_uiThrashTimer = urand(2500,5000);
        m_uiTargetSwitchTimer = 20000;

		m_uiBerserkTimer = 7.5*MINUTE*IN_MILLISECONDS;
		m_bBerserk = false;

		minions = false;

        if(m_pInstance)
        {
            m_pInstance->SetData(TYPE_PHASE_TWO,NOT_STARTED);
            m_pInstance->SetData(TYPE_PHASE_TWO_DONE,NOT_STARTED);
            m_pInstance->SetData(TYPE_END,NOT_STARTED);
            m_pInstance->SetData(TYPE_MOB_ONE,NOT_STARTED);
            m_pInstance->SetData(TYPE_MOB_TWO,NOT_STARTED);
            m_pInstance->SetData(TYPE_MOB_THREE,NOT_STARTED);
            m_pInstance->SetData(TYPE_MOB_FOUR,NOT_STARTED);
        }

		for(int i=0; i<4; ++i)
		{
            if(m_uiMinionsGUID[i])
			{
				if (Creature* pMinion = (Creature*)(m_creature->GetMap()->GetUnit(m_uiMinionsGUID[i])))
                {
                    if (pMinion->isAlive())
                        pMinion->ForcedDespawn();
                }
			    m_uiMinionsGUID[i] = 0;
			}
            if(minionsAlive[i])
				minionsAlive[i] = false;
		}
	}

	void JustDied (Unit* pKiller)
	{
		for(int i=0; i<4; ++i)
		{
			if(m_uiMinionsGUID[i])
			{
				if (Creature* pMinion = (Creature*)(m_creature->GetMap()->GetUnit(m_uiMinionsGUID[i])))
                {
                    if (pMinion->isAlive())
                        pMinion->ForcedDespawn();
                }
			    m_uiMinionsGUID[i] = 0;
			}
		}
	}

	void SummonedCreatureJustDied(Creature* pSummoned)
	{
            if(pSummoned->GetGUID() == m_uiMinionsGUID[0])
		    {
			    m_pInstance->SetData(TYPE_MOB_ONE, DONE);
			    if(minionsAlive[0] = false)
                {
                    if(minions && !minionsAlive[0] && !minionsAlive[1] && !minionsAlive[2] && !minionsAlive[3])
		            {
                        if(m_pInstance)
                        {
                            m_pInstance->SetData(TYPE_PHASE_TWO_DONE, DONE);
                        }
                    }
                }
		    }
            if(pSummoned->GetGUID() == m_uiMinionsGUID[1])
		    {
			    m_pInstance->SetData(TYPE_MOB_TWO, DONE);
			    if(minionsAlive[1] = false)
                {
                    if(minions && !minionsAlive[0] && !minionsAlive[1] && !minionsAlive[2] && !minionsAlive[3])
		            {
                        if(m_pInstance)
                        {
                            m_pInstance->SetData(TYPE_PHASE_TWO_DONE, DONE);
                        }
                    }
                }
		    }
            if(pSummoned->GetGUID() == m_uiMinionsGUID[2])
		    {
			    m_pInstance->SetData(TYPE_MOB_THREE, DONE);
			    if(minionsAlive[2] = false)
                {
                    if(minions && !minionsAlive[0] && !minionsAlive[1] && !minionsAlive[2] && !minionsAlive[3])
		            {
                        if(m_pInstance)
                        {
                            m_pInstance->SetData(TYPE_PHASE_TWO_DONE, DONE);
                        }
                    }
                }
		    }
            if(pSummoned->GetGUID() == m_uiMinionsGUID[3])
		    {
			    m_pInstance->SetData(TYPE_MOB_FOUR, DONE);
			    if(minionsAlive[3] = false)
                {
                    if(minions && !minionsAlive[0] && !minionsAlive[1] && !minionsAlive[2] && !minionsAlive[3])
		            {
                        if(m_pInstance)
                        {
                            m_pInstance->SetData(TYPE_PHASE_TWO_DONE, DONE);
                        }
                    }
                }
		    }
        if(minions && !minionsAlive[0] && !minionsAlive[1] && !minionsAlive[2] && !minionsAlive[3])
		{
            if(m_pInstance)
            {
                m_pInstance->SetData(TYPE_PHASE_TWO_DONE, DONE);
            }
			minions = false;
        }
	}

    void GetTarget()
    {
        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        for (ThreatList::const_iterator iter = tList.begin();iter != tList.end(); ++iter)
        {
            if(Unit* pPlayer = m_creature->GetMap()->GetUnit((*iter)->getUnitGuid()))
            {
                if(pPlayer->getClass() == CLASS_MAGE || pPlayer->getClass() == CLASS_PRIEST || pPlayer->getClass() == CLASS_WARLOCK)
                {
                    m_creature->AddThreat(pPlayer,5000);
                    return;
                }
                else
                    DoCastSpellIfCan(m_creature,SPELL_ACCELERATE);
            }    
        }
    }

    void TeleportTo()
    {
        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        for (ThreatList::const_iterator iter = tList.begin();iter != tList.end(); ++iter)
        {
            if(Unit* pPlayer = m_creature->GetMap()->GetUnit((*iter)->getUnitGuid()))
            {
                pPlayer->Relocate(PHASE_THREE_X + urand(-10,10), PHASE_THREE_Y + urand(-10,10), PHASE_THREE_Z, PHASE_THREE_O);
            }
        }
        m_creature->Relocate(PHASE_THREE_X, PHASE_THREE_Y, PHASE_THREE_Z, PHASE_THREE_O);    
    }

    void TeleportToShip()
    {
        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        for (ThreatList::const_iterator iter = tList.begin();iter != tList.end(); ++iter)
        {
            if(Unit* pPlayer = m_creature->GetMap()->GetUnit((*iter)->getUnitGuid()))
            {
                pPlayer->Relocate(VANCLEEF_SHIP_X + (urand(-3,3)),VANCLEEF_SHIP_Y + (urand(-3,3)),VANCLEEF_SHIP_Z,VANCLEEF_SHIP_O);
            }
        }
        m_creature->Relocate(VANCLEEF_SHIP_X,VANCLEEF_SHIP_Y,VANCLEEF_SHIP_Z,VANCLEEF_SHIP_O);    
    }

	void UpdateAI(const uint32 diff)
	{
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
		if (!m_bPhase2 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 66)
		{
			if (m_pInstance)
			{
				m_pInstance->SetData(TYPE_PHASE_TWO, IN_PROGRESS);
			}
			m_bPhase2 = true;
		}
        if (!m_bPhase3 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 66)
        {
            TeleportTo();
            m_bPhase3 = true;
        }
        if(!m_bEnd && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 3)
        {
            if (m_pInstance)
            {
                m_pInstance->SetData(TYPE_END, IN_PROGRESS);
            }
            TeleportToShip();
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_bPhaseThree = false;
            m_bEnd = true;
        }
		if (m_bPhase2)
		{
			if(!minions)
			{
				SpawnMinions();
                m_creature->MonsterYell("Get out of my sight!",LANG_UNIVERSAL,m_creature->getVictim());
			}
            if(m_uiAscendTimer < diff && !m_bAscend)
			{
				DoCastSpellIfCan(m_creature,SPELL_ASCEND);
                m_bAscend = true;
			}
			else
				m_uiAscendTimer -= diff;
		}
        if(m_bPhase3 && m_bPhaseThree)
        {
            if(m_uiTargetSwitchTimer < diff)
                GetTarget();
            else
                m_uiTargetSwitchTimer -= diff;
        }
		if(m_uiThrashTimer < diff)
		{
			Unit* pTarget = NULL;
			if(pTarget = m_creature->getVictim())
			{
				DoCast(pTarget,SPELL_THRASH);
			}
			m_uiThrashTimer = urand(2500,5000);
		}
		else
		{
			m_uiThrashTimer -= diff;
		}
		if(m_uiToxinTimer < diff)
		{
			Unit* pTarget = NULL;
			if(pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
			{
				DoCastSpellIfCan(pTarget,SPELL_HEMOTOXIN);
				m_uiToxinTimer = 15000;
			}
		}
		else
		{
			m_uiToxinTimer -= diff;
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
        if(minions && !minionsAlive[0] && !minionsAlive[1] && !minionsAlive[2] && !minionsAlive[3])
		{
            if(m_pInstance)
            {
                m_pInstance->SetData(TYPE_PHASE_TWO, DONE);
            }
			minions = false;
        }
		DoMeleeAttackIfReady();
	}
};

bool GOHello_go_bunker_portal(Player* pPlayer, GameObject* pGo)
{
	switch(urand(0,3))
	{
	case 0:
		pPlayer->TeleportTo(36,BUNKER_ONE_X,BUNKER_ONE_Y,BUNKER_ONE_Z,BUNKER_ONE_O);
		break;
	case 1:
		pPlayer->TeleportTo(36,BUNKER_TWO_X,BUNKER_TWO_Y,BUNKER_TWO_Z,BUNKER_TWO_O);
		break;
	case 2:
		pPlayer->TeleportTo(36,BUNKER_THREE_X,BUNKER_THREE_Y,BUNKER_THREE_Z,BUNKER_THREE_O);
		break;
	case 3:
		pPlayer->TeleportTo(36,BUNKER_FOUR_X,BUNKER_FOUR_Y,BUNKER_FOUR_Z,BUNKER_FOUR_O);
		break;
	}
	return true;
}

bool GOHello_go_bunker_portal1(Player* pPlayer, GameObject* pGo)
{
	pPlayer->TeleportTo(36,BUNKER_ONE_X,BUNKER_ONE_Y,BUNKER_ONE_Z,BUNKER_ONE_O);
	return true;
}

bool GOHello_go_bunker_portal2(Player* pPlayer, GameObject* pGo)
{
	pPlayer->TeleportTo(36,BUNKER_TWO_X,BUNKER_TWO_Y,BUNKER_TWO_Z,BUNKER_TWO_O);
	return true;
}

bool GOHello_go_bunker_portal3(Player* pPlayer, GameObject* pGo)
{
	pPlayer->TeleportTo(36,BUNKER_THREE_X,BUNKER_THREE_Y,BUNKER_THREE_Z,BUNKER_THREE_O);
	return true;
}

bool GOHello_go_bunker_portal4(Player* pPlayer, GameObject* pGo)
{
	pPlayer->TeleportTo(36,BUNKER_FOUR_X,BUNKER_FOUR_Y,BUNKER_FOUR_Z,BUNKER_FOUR_O);
	return true;
}

bool GOHello_go_vancleef_portal(Player* pPlayer, GameObject* pGo)
{
	pPlayer->TeleportTo(36,VANCLEEF_SHIP_X,VANCLEEF_SHIP_Y,VANCLEEF_SHIP_Z,VANCLEEF_SHIP_O);
	return true;
}

void AddSC_dm_go_scripts()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "go_bunker_portal";
    newscript->pGossipHelloGO =           &GOHello_go_bunker_portal;
    newscript->RegisterSelf();
	
	newscript = new Script;
    newscript->Name = "go_bunker_portal1";
    newscript->pGossipHelloGO =           &GOHello_go_bunker_portal1;
    newscript->RegisterSelf();
	
	newscript = new Script;
    newscript->Name = "go_bunker_portal2";
    newscript->pGossipHelloGO =           &GOHello_go_bunker_portal2;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "go_bunker_portal3";
    newscript->pGossipHelloGO =           &GOHello_go_bunker_portal3;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "go_bunker_portal4";
    newscript->pGossipHelloGO =           &GOHello_go_bunker_portal4;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "go_vancleef_portal";
    newscript->pGossipHelloGO =           &GOHello_go_vancleef_portal;
    newscript->RegisterSelf();
}

CreatureAI* GetAI_boss_vancleef(Creature* pCreature)
{
    return new boss_vancleefAI(pCreature);
}

void AddSC_boss_vancleef()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_vancleef";
    NewScript->GetAI = &GetAI_boss_vancleef;
    NewScript->RegisterSelf();
}