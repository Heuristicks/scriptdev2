#include "precompiled.h"
#include "World.h"

extern DatabaseType SD2Database;

bool QuestComplete_npc_ioc_resource(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
     uint32 quest_id = pQuest->GetQuestId();
	 if(quest_id != 110005 && quest_id != 110006 && quest_id != 110015 && quest_id != 110016)
		 return true;
     bool enoughOil = false;
     bool enoughOre = false;
     uint32 oreReq;
     uint32 oilReq;
     uint32 oreAmount;
     uint32 oilAmount;
	 QueryResult* pQuery;
     switch(quest_id)
     {
        case 110005:
        {
            /* For Horde ORE Quest 110005 */
            QueryResult* pQuery = SD2Database.PQuery("UPDATE `wareffort` SET collected = collected + 50 WHERE `resource` = \"horde_ore\";");
            break;
        }
        case 110006:
        {
            /* For Horde Oil Quest 110006*/
            pQuery = SD2Database.PQuery("UPDATE `wareffort` SET collected = collected + 50 WHERE `resource` = \"horde_oil\";");
            break;
        }
        case 110015:
        {
            /* For alliance ore Quest 110015 */
            pQuery = SD2Database.PQuery("UPDATE `wareffort` SET collected = collected + 50 WHERE `resource` = \"alli_ore\";");
            break;
        }
        case 110016:
        {
            /* For alliance Oil Quest 110016 */
            pQuery = SD2Database.PQuery("UPDATE `wareffort` SET collected = collected + 50 WHERE `resource` = \"alli_oil\";");
            break;
        }
        default:
            return true;
     }
     switch(quest_id)
     {
        case 110005:
        case 110006:
        {
            QueryResult* q_oreReq = SD2Database.PQuery("SELECT collected FROM `wareffort` WHERE resource = \"ore_required\";");
            QueryResult* q_oreAmount = SD2Database.PQuery("SELECT collected FROM `wareffort` WHERE resource = \"horde_ore\";");
            if(q_oreReq && q_oreAmount)
            {
                Field* pFields = q_oreReq->Fetch();
                Field* pFields2 = q_oreAmount->Fetch();
                if(pFields && pFields2)
                {
                    oreReq = pFields[0].GetUInt32();
                    oreAmount = pFields2[0].GetUInt32();
                    if(oreAmount >= oreReq)
                    {
                        enoughOre = true;
                    }
                }      
            }
            delete q_oreReq;
            delete q_oreAmount;
            QueryResult* q_oilReq = SD2Database.PQuery("SELECT collected FROM `wareffort` WHERE resource = \"oil_required\";");
            QueryResult* q_oilAmount = SD2Database.PQuery("SELECT collected FROM `wareffort` WHERE resource = \"horde_oil\";");
            if(q_oilReq && q_oilAmount)
            {
                Field* pFields = q_oilReq->Fetch();
                Field* pFields2 = q_oilAmount->Fetch();
                if(pFields && pFields2)
                {
                    oilReq = pFields[0].GetUInt32();
                    oilAmount = pFields2[0].GetUInt32();
                    if(oilAmount >= oilReq)
                    {
                        enoughOil = true;
                    }
                }      
            }
            delete q_oilReq;
            delete q_oilAmount;
            break;
        }
        case 110015:
        case 110016:
        {
            QueryResult* q_oreReq = SD2Database.PQuery("SELECT collected FROM `wareffort` WHERE resource = \"ore_required\";");
            QueryResult* q_oreAmount = SD2Database.PQuery("SELECT collected FROM `wareffort` WHERE resource = \"alli_ore\";");
            if(q_oreReq && q_oreAmount)
            {
                Field* pFields = q_oreReq->Fetch();
                Field* pFields2 = q_oreAmount->Fetch();
                if(pFields && pFields2)
                {
                    oreReq = pFields[0].GetUInt32();
                    oreAmount = pFields2[0].GetUInt32();
                    if(oreAmount >= oreReq)
                    {
                       enoughOre = true;
                    }
                }      
            }
            delete q_oreReq;
            delete q_oreAmount;
            QueryResult* q_oilReq = SD2Database.PQuery("SELECT collected FROM `wareffort` WHERE resource = \"oil_required\";");
            QueryResult* q_oilAmount = SD2Database.PQuery("SELECT collected FROM `wareffort` WHERE resource = \"alli_oil\";");
            if(q_oilReq && q_oilAmount)
            {
                Field* pFields = q_oilReq->Fetch();
                Field* pFields2 = q_oilAmount->Fetch();
                if(pFields && pFields2)
                {
                    oilReq = pFields[0].GetUInt32();
                    oilAmount = pFields2[0].GetUInt32();
                    if(oilAmount >= oilReq)
                    {
                        enoughOil = true;
                    }
                }      
            }
            delete q_oilReq;
            delete q_oilAmount;
            break;
        }
     }
     if(enoughOil && enoughOre)
     {
         if(quest_id == 110005 || quest_id == 110006)
         {
             QueryResult* hordeWin = SD2Database.PQuery("UPDATE `wareffort` SET collected = collected + 1 WHERE resource = \"horde_win\";");
             QueryResult* hordeOilReset = SD2Database.PQuery("UPDATE `wareffort` SET collected = 0 WHERE resource = \"horde_oil\";");
             QueryResult* hordeOreReset = SD2Database.PQuery("UPDATE `wareffort` SET collected = 0 WHERE resource = \"horde_ore\";");
             pCreature->SummonCreature(111098,pCreature->GetPositionX(),pCreature->GetPositionY(),pCreature->GetPositionZ(),pCreature->GetOrientation(),TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,1*MINUTE*IN_MILLISECONDS);
         }
         else
         {
             QueryResult* alliWin = SD2Database.PQuery("UPDATE `wareffort` SET collected = collected + 1 WHERE resource = \"alli_win\";");
             QueryResult* alliOilReset = SD2Database.PQuery("UPDATE `wareffort` SET collected = 0 WHERE resource = \"alli_oil\";");
             QueryResult* alliOreReset = SD2Database.PQuery("UPDATE `wareffort` SET collected = 0 WHERE resource = \"alli_ore\";");
             pCreature->SummonCreature(111099,pCreature->GetPositionX(),pCreature->GetPositionY(),pCreature->GetPositionZ(),pCreature->GetOrientation(),TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,1*MINUTE*IN_MILLISECONDS);
         }
     }
     return true;
}

void AddSC_npc_ioc_resource()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "npc_ioc_resource";
    NewScript->pQuestRewardedNPC = &QuestComplete_npc_ioc_resource;
    NewScript->RegisterSelf();
}