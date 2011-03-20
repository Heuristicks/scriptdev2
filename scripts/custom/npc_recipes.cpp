#include "precompiled.h"
/*#include "SpellMgr.h"
#include "../../../../shared/Database/DBCStore.h"

bool GossipHello_npc_recipe(Player* pPlayer, Creature* pCreature)
{
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TEXT_ALCHEMY             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TEXT_BLACKSMITHING       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TEXT_COOKING             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TEXT_ENCHANTING          , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TEXT_ENGINEERING         , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TEXT_FIRSTAID            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TEXT_INSCRIPTION         , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TEXT_JEWELCRAFTING       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TEXT_LEATHERWORKING      , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 9);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TEXT_TAILORING           , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 10);
            pPlayer->SEND_GOSSIP_MENU(3541, pCreature->GetGUID());
            return true;
}

bool GossipSelect_npc_recipe(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if(pPlayer->GetMoney() < 1000000)
    {
        pPlayer->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, pCreature, NULL, 0);
        pPlayer->CLOSE_GOSSIP_MENU();
        return false;
    }
    else
    {
        pPlayer->ModifyMoney(-1000000);
        pPlayer->CLOSE_GOSSIP_MENU();    
    }
    uint32 skill_id;
    uint32 classmask = pPlayer->getClassMask();
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Alchemy
            skill_id = 171;
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Blacksmithing
            skill_id = 164;
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Cooking
            skill_id = 185;
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Enchanting
            skill_id = 333;
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Engineering
            skill_id = 202;
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //First Aid
            skill_id = 129;
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Inscription
            skill_id = 773;
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                   //Jewelcrafting
            skill_id = 755;
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:                   //Leatherworking
            skill_id = 165;
            break;
		case GOSSIP_ACTION_INFO_DEF + 10:                   //Tailoring
            skill_id = 197;
            break;
    }
    for (uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
    {
        SkillLineAbilityEntry const *skillLine = sSkillLineAbilityStore.LookupEntry(j);
        if (!skillLine)
            continue;

        // wrong skill
        if (skillLine->skillId != skill_id)
            continue;

        // not high rank
        if (skillLine->forward_spellid)
            continue;

        // skip racial skills
        if (skillLine->racemask != 0)
            continue;

        // skip wrong class skills
        if (skillLine->classmask && (skillLine->classmask & classmask) == 0)
            continue;

        SpellEntry const* spellInfo = sSpellStore.LookupEntry(skillLine->spellId);
        if (!spellInfo || !SpellMgr::IsSpellValid(spellInfo,pPlayer,false))
            continue;

        pPlayer->learnSpell(skillLine->spellId, false);
    }
    return true;
}

void AddSC_npc_recipe()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "npc_recipe";
    NewScript->pGossipHello = &GossipHello_npc_recipe;
    NewScript->pGossipSelect = &GossipSelect_npc_recipe;
    NewScript->RegisterSelf();
}*/