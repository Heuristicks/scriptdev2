#include "precompiled.h"

/*########
# mob_mirror_image AI
#########*/

enum MirrorImage
{
    SPELL_FROSTBOLT = 59638,
    SPELL_FIREBLAST = 59637
};

struct MANGOS_DLL_DECL mob_mirror_imageAI : public ScriptedAI
{
    mob_mirror_imageAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        bLocked = false;
        Reset();
    }
    ObjectGuid m_uiCreatorGUID;
    uint32 m_uiFrostboltTimer;
    uint32 m_uiFireBlastTimer;
    float fDist;
    float fAngle;
    bool bLocked;

    void Reset()
    {
        m_uiFrostboltTimer = urand(500, 1500);
        m_uiFireBlastTimer = urand(4500, 6000);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!bLocked)
        {
			m_uiCreatorGUID = m_creature->GetCreatorGuid();
            if (Player* pOwner = (Player*)(m_creature->GetMap()->GetUnit(m_uiCreatorGUID)))
            {
                fDist = m_creature->GetDistance(pOwner);
                fAngle = m_creature->GetAngle(pOwner);
            }
            bLocked = true;
        }

        Player* pOwner = (Player*)(m_creature->GetMap()->GetUnit(m_uiCreatorGUID));
        if (!pOwner || !pOwner->IsInWorld())
        {
            m_creature->ForcedDespawn();
            return;
        }
        
        ObjectGuid targetGUID;

        if (pOwner->getVictim())
            targetGUID = pOwner->getVictim()->GetObjectGuid();

        Unit* pTarget = m_creature->GetMap()->GetUnit(targetGUID);

        if (!pTarget || !m_creature->CanInitiateAttack() || !pTarget->isTargetableForAttack() ||
			!m_creature->IsHostileTo(pTarget) || !pTarget->isInAccessablePlaceFor(m_creature))
        {
            if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() != FOLLOW_MOTION_TYPE)
            {
                m_creature->InterruptNonMeleeSpells(false);
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveFollow(pOwner, fDist, fAngle);
            }
            return;
        }

        if (m_uiFrostboltTimer <= uiDiff)
        {
            m_creature->CastSpell(pTarget, SPELL_FROSTBOLT, false, NULL, NULL, pOwner->GetGUID());
            m_uiFrostboltTimer = urand(3000, 4500);
        } else m_uiFrostboltTimer -= uiDiff;

        if (m_uiFireBlastTimer <= uiDiff)
        {
            m_creature->CastSpell(pTarget, SPELL_FIREBLAST, false, NULL, NULL, pOwner->GetGUID());
            m_uiFireBlastTimer = urand(9000, 12000);
        } else m_uiFireBlastTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_mirror_image(Creature* pCreature)
{
    return new mob_mirror_imageAI(pCreature);
}

void AddSC_mob_mirror_image()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "mob_mirror_image";
    NewScript->GetAI = &GetAI_mob_mirror_image;
    NewScript->RegisterSelf();
}