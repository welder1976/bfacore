/*
 * Copyright (C) 2020 BfaCore
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "AreaTrigger.h"
#include "AreaTriggerAI.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "the_motherlode.h"
enum NPC
{
    NPC_DATA_MOGUL_RAZDUNK = 132713,

};

 // 132713
class npc_mogul_razdunk : public CreatureScript
{
public:
    npc_mogul_razdunk() : CreatureScript("npc_mogul_razdunk") {}

    struct npc_mogul_razdunkAI : public ScriptedAI
    {
        npc_mogul_razdunkAI(Creature* creature) : ScriptedAI(creature)
        {
            
        }

        void Reset() override
        {
          instance->SetBossState(DATA_MOGUL_RAZDUNK, FAIL);
        }

        void JustDied(Unit* killer) override
        {
            instance->SetBossState(DATA_MOGUL_RAZDUNK, DONE);
        }
       

        void EnterCombat(Unit* /*who*/) override
        {
            instance->SetBossState(DATA_MOGUL_RAZDUNK, IN_PROGRESS);
        }

        
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_mogul_razdunkAI(creature);
    }
};

void AddSC_the_motherlode()
{
    new npc_mogul_razdunk();
}
