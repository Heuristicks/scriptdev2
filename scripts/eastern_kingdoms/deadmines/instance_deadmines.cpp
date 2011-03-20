/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Instance_Deadmines
SD%Complete: 0
SDComment: Placeholder
SDCategory: Deadmines
EndScriptData */

#include "precompiled.h"
#include "deadmines.h"

#define GUARD_X        -99.101883f
#define GUARD_Y        -804.701477f
#define GUARD_Z        30.679993f
#define GUARD_O        0.417167f

#define MOVEMENT_X1    -69.487518f
#define MOVEMENT_Y1    -787.486267f
#define MOVEMENT_Z1    39.725628f

#define MOVEMENT_X2    -39.101337f
#define MOVEMENT_Y2    -797.230530f
#define MOVEMENT_Z2     39.338821f

#define MOVEMENT_X3     -64.4683f
#define MOVEMENT_Y3     -819.627014f
#define MOVEMENT_Z3     41.132f

struct MANGOS_DLL_DECL instance_deadmines : public ScriptedInstance
{
    instance_deadmines(Map* pMap) : ScriptedInstance(pMap) 
	{
		Initialize();
	}

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    uint8 type_phase_two_done;

    uint64 m_uiIronCladGUID;
    uint64 m_uiCannonGUID;
    uint64 m_uiSmiteGUID;

	uint64 m_uiGilnidFireGUID;
	uint64 m_uiBunkerPortalGUID;
	uint64 m_uiVanCleefPortalGUID[4];

	uint64 m_uiBunkerPortal1GUID1;
	uint64 m_uiBunkerPortal1GUID2;
	uint64 m_uiBunkerPortal1GUID3;
	uint64 m_uiBunkerPortal2GUID1;
	uint64 m_uiBunkerPortal2GUID2;
	uint64 m_uiBunkerPortal2GUID3;
	uint64 m_uiBunkerPortal3GUID1;
	uint64 m_uiBunkerPortal3GUID2;
	uint64 m_uiBunkerPortal3GUID3;
	uint64 m_uiBunkerPortal4GUID1;
	uint64 m_uiBunkerPortal4GUID2;
	uint64 m_uiBunkerPortal4GUID3;

    uint32 m_uiIronDoor_Timer;
    uint32 m_uiDoor_Step;

    uint32 m_uiArrest_Step;
    uint32 m_uiArrest_Timer;
    uint32 m_uiArrestStep2_Timer;
    uint32 m_uiArrestStep3_Timer;
    uint32 m_uiArrestStep4_Timer;
    uint32 m_uiArrestStep5_Timer;
    uint32 m_uiArrestStep5_Timer2;
    uint64 m_uiGuardGUID[4];
    uint64 m_uiCaptainGUID;
    uint64 m_uiVanCleefGUID;
    uint64 m_uiVCChestGUID;


    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiIronCladGUID = 0;
        m_uiCannonGUID = 0;
        m_uiSmiteGUID = 0;
		m_uiGilnidFireGUID = 0;
		m_uiBunkerPortalGUID = 0;
		m_uiBunkerPortal1GUID1 = 0;
		m_uiBunkerPortal1GUID2 = 0;
		m_uiBunkerPortal1GUID3 = 0;
		m_uiBunkerPortal2GUID1 = 0;
		m_uiBunkerPortal2GUID2 = 0;
		m_uiBunkerPortal2GUID3 = 0;
		m_uiBunkerPortal3GUID1 = 0;
		m_uiBunkerPortal3GUID2 = 0;
		m_uiBunkerPortal3GUID3 = 0;
		m_uiBunkerPortal4GUID1 = 0;
		m_uiBunkerPortal4GUID2 = 0;
		m_uiBunkerPortal4GUID3 = 0;
        m_uiVCChestGUID = 0;
        type_phase_two_done = 0;

        m_uiArrestStep2_Timer = 2000;
        m_uiArrestStep3_Timer = 2000;
        m_uiArrestStep4_Timer = 2000;
        m_uiArrestStep5_Timer = 8000;
        m_uiArrestStep5_Timer2 = 15000;

        for(int i = 0; i < 4; ++i)
        {
            m_uiVanCleefPortalGUID[i] = 0;
        }

        for(int i = 0; i < 4; ++i)
        {
            m_uiGuardGUID[i] = 0;
        }
        m_uiCaptainGUID = 0;
        m_uiVanCleefGUID = 0;


        m_uiIronDoor_Timer = 0;
        m_uiDoor_Step = 0;
        m_uiArrest_Step = 0;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry()) 
        {
        case NPC_MR_SMITE:
            m_uiSmiteGUID = pCreature->GetGUID();
            break;
        case NPC_VANCLEEF:
            m_uiVanCleefGUID = pCreature->GetGUID();
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
		switch(pGo->GetEntry())
		{
		case GO_IRON_CLAD:
            m_uiIronCladGUID = pGo->GetGUID();
			break;
		case GO_DEFIAS_CANNON:
            m_uiCannonGUID = pGo->GetGUID();
			break;
		case GO_BUNKER_PORTAL:
			m_uiBunkerPortalGUID = pGo->GetGUID();
			break;
		case GO_BUNKER_PORTAL11:
			m_uiBunkerPortal1GUID1 = pGo->GetGUID();
			break;
		case GO_BUNKER_PORTAL12:
			m_uiBunkerPortal1GUID2 = pGo->GetGUID();
			break;
		case GO_BUNKER_PORTAL13:
			m_uiBunkerPortal1GUID3 = pGo->GetGUID();
			break;
		case GO_BUNKER_PORTAL21:
			m_uiBunkerPortal2GUID1 = pGo->GetGUID();
			break;
		case GO_BUNKER_PORTAL22:
			m_uiBunkerPortal2GUID2 = pGo->GetGUID();
			break;
		case GO_BUNKER_PORTAL23:
			m_uiBunkerPortal2GUID3 = pGo->GetGUID();
			break;
		case GO_BUNKER_PORTAL31:
			m_uiBunkerPortal3GUID1 = pGo->GetGUID();
			break;
		case GO_BUNKER_PORTAL32:
			m_uiBunkerPortal3GUID2 = pGo->GetGUID();
			break;
		case GO_BUNKER_PORTAL33:
			m_uiBunkerPortal3GUID3 = pGo->GetGUID();
			break;
		case GO_BUNKER_PORTAL41:
			m_uiBunkerPortal4GUID1 = pGo->GetGUID();
			break;
		case GO_BUNKER_PORTAL42:
			m_uiBunkerPortal4GUID2 = pGo->GetGUID();
			break;
		case GO_BUNKER_PORTAL43:
			m_uiBunkerPortal4GUID3 = pGo->GetGUID();
			break;
		case GO_VANCLEEF_PORTAL:
			for(int i = 0; i<4; ++i)
			{
				m_uiVanCleefPortalGUID[i] = pGo->GetGUID();
			}
			break;
        case GO_VANCLEEF_CHEST:
            m_uiVCChestGUID = pGo->GetGUID();
            break;
        case GO_GILNID_FIRE:
            m_uiGilnidFireGUID = pGo->GetGUID();
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
		switch(uiType)
        {
		    case TYPE_DEFIAS_ENDDOOR:
			    if (uiData == IN_PROGRESS)
			    {
				    if (GameObject* pGo = instance->GetGameObject(m_uiIronCladGUID))
				    {
					    pGo->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
					    m_uiIronDoor_Timer = 3000;
				    }
			    }
			    m_auiEncounter[0] = uiData;
			    break;
		    case TYPE_PHASE_TWO:
			    if (uiData == IN_PROGRESS)
			    {
				    DoRespawnGameObject(m_uiBunkerPortalGUID, 10000);
			    }
			    break;
            case TYPE_PHASE_TWO_DONE:
                if (uiData == DONE)
			    {
				    for(int i=0; i<4; ++i)
				    {
					    DoRespawnGameObject(m_uiVanCleefPortalGUID[i], 15*MINUTE*IN_MILLISECONDS);
				    }
				    DoRespawnGameObject(m_uiBunkerPortalGUID, 0);
				    DoRespawnGameObject(m_uiBunkerPortal2GUID1, 0);
				    DoRespawnGameObject(m_uiBunkerPortal3GUID1, 0);
				    DoRespawnGameObject(m_uiBunkerPortal4GUID1, 0);
				    DoRespawnGameObject(m_uiBunkerPortal1GUID1, 0);
				    DoRespawnGameObject(m_uiBunkerPortal3GUID2, 0);
				    DoRespawnGameObject(m_uiBunkerPortal4GUID2, 0);					
				    DoRespawnGameObject(m_uiBunkerPortal1GUID2, 0);
				    DoRespawnGameObject(m_uiBunkerPortal2GUID2, 0);
				    DoRespawnGameObject(m_uiBunkerPortal4GUID3, 0);
				    DoRespawnGameObject(m_uiBunkerPortal1GUID3, 0);
				    DoRespawnGameObject(m_uiBunkerPortal2GUID3, 0);
				    DoRespawnGameObject(m_uiBunkerPortal3GUID3, 0);
			    }
                type_phase_two_done = uiData;
                break;
            case TYPE_END:
                if(uiData == IN_PROGRESS)
                {
                    m_uiArrest_Timer = 0;
                }
                break;
		    case TYPE_MOB_ONE:
			    if (uiData == DONE)
			    {
				    DoRespawnGameObject(m_uiBunkerPortal2GUID1, 15*MINUTE*IN_MILLISECONDS);
				    DoRespawnGameObject(m_uiBunkerPortal3GUID1, 15*MINUTE*IN_MILLISECONDS);
				    DoRespawnGameObject(m_uiBunkerPortal4GUID1, 15*MINUTE*IN_MILLISECONDS);
			    }
			    break;
		    case TYPE_MOB_TWO:
			    if (uiData == DONE)
			    {
				    DoRespawnGameObject(m_uiBunkerPortal1GUID1, 15*MINUTE*IN_MILLISECONDS);
				    DoRespawnGameObject(m_uiBunkerPortal3GUID2, 15*MINUTE*IN_MILLISECONDS);
				    DoRespawnGameObject(m_uiBunkerPortal4GUID2, 15*MINUTE*IN_MILLISECONDS);
			    }
			    break;
		    case TYPE_MOB_THREE:
			    if (uiData == DONE)
			    {
				    DoRespawnGameObject(m_uiBunkerPortal1GUID2, 15*MINUTE*IN_MILLISECONDS);
				    DoRespawnGameObject(m_uiBunkerPortal2GUID2, 15*MINUTE*IN_MILLISECONDS);
				    DoRespawnGameObject(m_uiBunkerPortal4GUID3, 15*MINUTE*IN_MILLISECONDS);
			    }
			    break;
		    case TYPE_MOB_FOUR:
			    if (uiData == DONE)
			    {
				    DoRespawnGameObject(m_uiBunkerPortal1GUID3, 15*MINUTE*IN_MILLISECONDS);
				    DoRespawnGameObject(m_uiBunkerPortal2GUID3, 15*MINUTE*IN_MILLISECONDS);
				    DoRespawnGameObject(m_uiBunkerPortal3GUID3, 15*MINUTE*IN_MILLISECONDS);
			    }
			    break;
            case TYPE_GILNID_FIRE:
                if (uiData == IN_PROGRESS)
                {
                    DoRespawnGameObject(m_uiGilnidFireGUID,1*MINUTE*IN_MILLISECONDS);
                }
                else
                    DoRespawnGameObject(m_uiGilnidFireGUID,0);
                break;
		}
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
        case TYPE_DEFIAS_ENDDOOR:
            return m_auiEncounter[0];
            break;
        case TYPE_PHASE_TWO_DONE:
            return type_phase_two_done;
            break;
        case TYPE_ARREST_STEP:
            return m_uiArrest_Step;
            break;
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
		switch(uiData)
		{
			case DATA_DEFIAS_DOOR:
				return m_uiIronCladGUID;
				break;
			case DATA_GILNID_FIRE:
				return m_uiGilnidFireGUID;
				break;
		}
        return 0;
    }

    void Update(uint32 uiDiff)
    {
        if (m_uiIronDoor_Timer)
        {
            if (m_uiIronDoor_Timer <= uiDiff)
            {
                if (Creature* pMrSmite = instance->GetCreature(m_uiSmiteGUID))
                {
                    switch(m_uiDoor_Step)
                    {
                        case 0:
                            DoScriptText(INST_SAY_ALARM1,pMrSmite);
                            m_uiIronDoor_Timer = 2000;
                            ++m_uiDoor_Step;
                            break;
                        case 1:
                            if (Creature* pi1 = pMrSmite->SummonCreature(NPC_PIRATE, 93.68f,-678.63f,7.71f,2.09f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 1800000))
                                pi1->GetMotionMaster()->MovePoint(0,100.11f,-670.65f,7.42f);
                            if (Creature* pi2 = pMrSmite->SummonCreature(NPC_PIRATE,102.63f,-685.07f,7.42f,1.28f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 1800000))
                                pi2->GetMotionMaster()->MovePoint(0,100.11f,-670.65f,7.42f);
                            ++m_uiDoor_Step;
                            m_uiIronDoor_Timer = 10000;
                            break;
                        case 2:
                            DoScriptText(INST_SAY_ALARM2,pMrSmite);
                            m_uiDoor_Step = 0;
                            m_uiIronDoor_Timer = 0;
                            debug_log("SD2: Instance Deadmines: Iron door event reached end.");
                            break;
                    }
                }
                else
                    m_uiIronDoor_Timer = 0;
            }
            else
                m_uiIronDoor_Timer -= uiDiff;
        }
        if(m_uiArrest_Timer)
        {
            if(m_uiArrest_Timer <= uiDiff)
            {
                if (Creature* pVanCleef = instance->GetCreature(m_uiVanCleefGUID))
                {
                    switch(m_uiArrest_Step)
                    {
                        case 0:
                            for(int i = 0; i < 4; ++i)
                            {
                                if(Creature* pGuard = pVanCleef->SummonCreature(100502,GUARD_X+(urand(-.5,.5)),GUARD_Y+(urand(-.5,.5)),GUARD_Z+(urand(-.5,.5)),GUARD_O, TEMPSUMMON_MANUAL_DESPAWN,0))
                                    m_uiGuardGUID[i] = pGuard->GetGUID();
                            }
                            if(Creature* pCaptain = pVanCleef->SummonCreature(100503,GUARD_X,GUARD_Y,GUARD_Z,GUARD_O, TEMPSUMMON_MANUAL_DESPAWN,0))
                                m_uiCaptainGUID = pCaptain->GetGUID();
                            m_uiArrestStep2_Timer += 2000;
                            ++m_uiArrest_Step;
                            break;
                        case 1:
                            if(m_uiArrestStep2_Timer <= uiDiff)
                            {
                                for(int i = 0; i < 4; ++i)
                                {
                                    if(Creature* pGuard = instance->GetCreature(m_uiGuardGUID[i]))
                                    {
                                        pGuard->GetMotionMaster()->MovePoint(0,MOVEMENT_X1+(urand(-.25,.25)),MOVEMENT_Y1+(urand(-.25,.25)),MOVEMENT_Z1);
                                    }
                                }
                                if(Creature* pCaptain = instance->GetCreature(m_uiCaptainGUID))
                                {
                                    pCaptain->GetMotionMaster()->MovePoint(0,MOVEMENT_X1,MOVEMENT_Y1,MOVEMENT_Z1);
                                }
                            }
                            else
                            {
                                m_uiArrestStep2_Timer -= uiDiff;
                            }
                            m_uiArrestStep3_Timer += 2000;
                            ++m_uiArrest_Step;
                            break;
                        case 2:
                            if(m_uiArrestStep3_Timer <= uiDiff)
                            {
                                for(int i = 0; i < 4; ++i)
                                {
                                    if(Creature* pGuard = instance->GetCreature(m_uiGuardGUID[i]))
                                    {
                                        pGuard->GetMotionMaster()->MovePoint(0,MOVEMENT_X2+(urand(-.25,.25)),MOVEMENT_Y2+(urand(-.25,.25)),MOVEMENT_Z2);
                                    }
                                }
                                if(Creature* pCaptain = instance->GetCreature(m_uiCaptainGUID))
                                {
                                    pCaptain->GetMotionMaster()->MovePoint(0,MOVEMENT_X2,MOVEMENT_Y2,MOVEMENT_Z2);
                                }
                            }
                            else
                            {
                                m_uiArrestStep3_Timer -= uiDiff;
                            }
                            m_uiArrestStep4_Timer += 2000;
                            ++m_uiArrest_Step;
                            break;
                        case 3:
                            if(m_uiArrestStep4_Timer <= uiDiff)
                            {
                                for(int i = 0; i < 4; ++i)
                                {
                                    if(Creature* pGuard = instance->GetCreature(m_uiGuardGUID[i]))
                                    {
                                        pGuard->GetMotionMaster()->MovePoint(0,MOVEMENT_X3+(urand(-.25,.25)),MOVEMENT_Y3+(urand(-.25,.25)),MOVEMENT_Z3);
                                    }
                                }
                                if(Creature* pCaptain = instance->GetCreature(m_uiCaptainGUID))
                                {
                                    pCaptain->GetMotionMaster()->MovePoint(0,MOVEMENT_X3,MOVEMENT_Y3,MOVEMENT_Z3);
                                }
                            }
                            else
                            {
                                m_uiArrestStep4_Timer -= uiDiff;
                            }
                            m_uiArrestStep5_Timer += 2000;
                            ++m_uiArrest_Step;
                            break;
                        case 4:
                            if (m_uiArrestStep5_Timer <= uiDiff)
                            {
                                if(Creature* pCaptain = instance->GetCreature(m_uiCaptainGUID))
                                {
                                    pCaptain->MonsterYell("Edwin VanCleef, a warrant for your arrest has been issued by the King of Stormwind. You are to be charged with numerous counts of murder, grand larceny, and extortion. You have the right to remain silent; anything you say may be used against you in the court of law.",0,pCaptain);
                                }
                                if(m_uiArrestStep5_Timer2 <= uiDiff)
                                {
                                    for(int i = 0; i < 4; ++i)
                                    {
                                        if(Creature* pGuard = instance->GetCreature(m_uiGuardGUID[i]))
                                        {
                                            pGuard->ForcedDespawn();
                                        }
                                    }
                                    if(Creature* pCaptain = instance->GetCreature(m_uiCaptainGUID))
                                        pCaptain->ForcedDespawn();
                                    if(Creature* pVanCleef = instance->GetCreature(m_uiVanCleefGUID))
                                        pVanCleef->ForcedDespawn();
                                }
                            }
                            break;
                    }
                }
            }
            else
                m_uiArrest_Timer -= uiDiff;
        }
    }
};

InstanceData* GetInstanceData_instance_deadmines(Map* pMap)
{
    return new instance_deadmines(pMap);
}

void AddSC_instance_deadmines()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_deadmines";
    newscript->GetInstanceData = &GetInstanceData_instance_deadmines;
    newscript->RegisterSelf();
}
