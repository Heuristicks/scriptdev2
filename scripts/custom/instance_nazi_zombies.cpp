#include "precompiled.h"
#include "nazi_zombies.h"

struct Locations {float x,y,z,o;};

static Locations SpawnLoc[] =
{
    {-34.660583f, 41.145603f, -32.634007f, 1.583540f},
    {-56.896942f, 41.145603f, -32.634007f, 1.583540f},
    {-56.896942f, 102.004730f, -32.634007f, 4.722335f},
    {-34.660583f, 102.004730f, -32.634007f, 4.722335f},
    {-90.666328f, 29.943455f, -31.717339f, 1.509181f},
    {-90.666328f, 112.775787f, -31.717339f, 4.701390f},
    {-131.542770f, 71.591400f, -31.717339f, 6.277859f},
    {73.395905f, 71.614571f, -31.717335f, 3.142035f},
    {72.889359f, 116.169914f, -31.717335f, 3.142035f},
    {-46.403950f, 120.350517f, -40.050659f, 1.573592f},
    {-46.403950f, 179.855637f, -40.050659f, 4.730675f}
};

struct MANGOS_DLL_DECL instance_nazi_zombies : public ScriptedInstance
{
    instance_nazi_zombies(Map* pMap) : ScriptedInstance(pMap) 
	{
		Initialize();
	}

    uint32 m_uiSpawnTimer;
    uint32 m_uiZombieCheckTimer;
    uint32 m_uiPlayerCheckTimer;
	uint32 m_uiLevel;
	bool started;
    uint64 m_uiSpawnCounter;
    uint64 m_uiZombieGuideGUID;
    std::vector<uint64> playerGUIDs;
    std::map<uint64,uint64> zombieTargets;

    void Initialize()
    {
        if(started)
        {
            QueryResult* pQuery = SD2Database.PQuery("INSERT INTO nazi_zombies (player_guid_1,player_guid_2,player_guid_3,player_guid_4,player_guid_5,level) VALUES (u%,u%,u%,u%,u%,u%);",playerGUIDs.front(),playerGUIDs.at(playerGUIDs.front()+1),playerGUIDs.at(playerGUIDs.front()+2),playerGUIDs.at(playerGUIDs.front()+3),playerGUIDs.at(playerGUIDs.front()+4),m_uiLevel);
            zombiesGone(true);
        }
        m_uiLevel = 0;
        m_uiSpawnTimer = 5050;
        m_uiZombieCheckTimer = 2500;
        m_uiPlayerCheckTimer = 5000;
        m_uiSpawnCounter = 0;
        started = true;
    }
    
    void OnCreatureCreate(Creature* pCreature)
    {
        if(pCreature->GetEntry() == 100612)     //Zombie Guide
            m_uiZombieGuideGUID = pCreature->GetGUID();               
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
		started = true;
    }

	uint32 GetData(uint32 uiType)
	{
		return m_uiLevel;
	}
	
    uint32 GetZombieTypeId()
    {
        if(m_uiLevel < 5)
            return 100613; //Zombie 1
        else if(m_uiLevel < 10)
            return 100614; //Zombie 2
        else if(m_uiLevel < 15)
            return 100615; //Zombie 3
        else if(m_uiLevel < 20)
            return 100616; //Zombie 4
        else
            return 100617; //Zombie 5
    }

    uint32 GetSpawnTimer()
    {
        return 5000 - ((m_uiLevel + 1)*50);
    }

    uint32 GetSpawnAmountForLevel()
    {
        return (m_uiLevel^(1+(1/2)))+5;
    }

    bool needsToSpawn()
    {
        return m_uiSpawnCounter < GetSpawnAmountForLevel();
    }
    
    bool zombiesGone(bool despawn)
    {
        if(Creature* pGuide = instance->GetCreature(m_uiZombieGuideGUID))
        {
            std::list<Creature*> zombieList;
            GetCreatureListWithEntryInGrid(zombieList,pGuide,GetZombieTypeId(),999999.0f);
            if(zombieList.empty())
            {
                if(despawn)
                {
                    std::list<Creature*>::iterator itr;
                    for(itr = zombieList.begin(); itr != zombieList.end(); ++itr)
                    {
                        if(Creature* pCreature = *itr)
                        {
                            pCreature->ForcedDespawn();
                        }
                    }
                }
                else
                    return true;
            }
            else if(despawn)
            {
                return true;
            }
        }
		return false;
    }

    Player* GetZombieTarget(uint64 guid)
    {
        return (Player*)(instance->GetUnit((zombieTargets.find(guid))->second));
    }

    /*void PassZombieTarget(uint64 zombieGUID,uint64 targetGUID)
    {
        zombieTargets.insert(zombieGUID,targetGUID);
    }*/

    void SendZombieMove()
    {
        if(Creature* pGuide = instance->GetCreature(m_uiZombieGuideGUID))
        {
            std::list<Creature*> zombieList;
            GetCreatureListWithEntryInGrid(zombieList,pGuide,GetZombieTypeId(),999999.0f);
            if(zombieList.empty())
                return;
            else
            {
                for(std::list<Creature*>::iterator itr = zombieList.begin(); itr != zombieList.end(); ++itr)
                {
                    if(Creature* pCreature = *itr)
                    {
                        /*uint64 guid = pCreature->GetGUID();
                        Player* pTarget = GetZombieTarget(guid);*/
                        if(!pCreature->getVictim())
                        {
                            Player* pTarget;
                            Map::PlayerList player = instance->GetPlayers();
                            if(roll_chance_f(/*50*/100))
                            {
                                for(Map::PlayerList::iterator itr = player.begin(); itr != player.end(); ++itr)
                                {   
                                    int i = player.getSize();
                                    if(roll_chance_f(1/i))
                                    {
                                        if(Player* pPlayer = itr->getSource())
                                        {
                                            pTarget = pPlayer;
                                        }
                                    }
                                    --i;
                                }
                            }
                            /*else
                            {
                                for(Map::PlayerList::iterator itr = player.end(); itr != player.begin(); --itr)
                                {   
                                    int i = 5;
                                    if(roll_chance_f(1/i))
                                    {
                                        if(Player* pPlayer = itr->getSource())
                                        {
                                            pTarget = pPlayer;
                                        }
                                    }
                                    --i;
                                }
                            }*/        
                            pCreature->GetMotionMaster()->MovePoint(0,pTarget->GetPositionX(),pTarget->GetPositionY(),pTarget->GetPositionZ());
                        }
                    }
                }
            }
        }
    }

    void Update(uint32 diff)
    {
        if(!started)
            return;
        if(needsToSpawn())
        {
            if(m_uiSpawnTimer < diff)
            {
                if(Creature* pGuide = instance->GetCreature(m_uiZombieGuideGUID))
                {
                    uint32 rand = urand(1,11);
                    if(Creature* pZombie = pGuide->SummonCreature(GetZombieTypeId(),SpawnLoc[rand].x,SpawnLoc[rand].y,SpawnLoc[rand].z,SpawnLoc[rand].o,TEMPSUMMON_CORPSE_DESPAWN,NULL))
                    {
                        ++m_uiSpawnCounter;
                    }
                }
                m_uiSpawnTimer = GetSpawnTimer();
            }
            else
                m_uiSpawnTimer -= diff;
        }
        else if(m_uiZombieCheckTimer < diff)
        {
            if(zombiesGone(false))
            {
                m_uiSpawnCounter = 0;
                ++m_uiLevel;
            }
            else
            {
                SendZombieMove();
            }
            m_uiZombieCheckTimer = 2500;
        }
        else
            m_uiZombieCheckTimer -= diff;
        if(m_uiPlayerCheckTimer < diff)
        {
            Map::PlayerList player = instance->GetPlayers();
            if(!player.isEmpty())
            {
                playerGUIDs.clear();
                Map::PlayerList::iterator itr;
                for(itr = player.begin(); itr != player.end(); ++itr)
                {
                    if(Player* pPlayer = itr->getSource())
                    {
                        playerGUIDs.push_back(pPlayer->GetGUID());
                    }
                }
            }
            else
                instance_nazi_zombies::Initialize();
            m_uiPlayerCheckTimer = 5000;
        }
        else
            m_uiPlayerCheckTimer -= diff;
    }
};

InstanceData* GetInstanceData_instance_nazi_zombies(Map* pMap)
{
    return new instance_nazi_zombies(pMap);
}

void AddSC_instance_nazi_zombies()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_nazi_zombies";
    newscript->GetInstanceData = &GetInstanceData_instance_nazi_zombies;
    newscript->RegisterSelf();
}