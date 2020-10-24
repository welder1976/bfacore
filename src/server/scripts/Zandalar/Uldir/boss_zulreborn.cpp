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
#include "ScriptMgr.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "ScriptedEscortAI.h"
#include "ScriptedGossip.h"
#include "ScriptedCreature.h"
#include "TemporarySummon.h"
#include "ScriptMgr.h"
#include "GameObject.h"
#include "MotionMaster.h"
#include "Creature.h"
#include "InstanceScript.h"
#include "Map.h"
#include "Object.h"
#include "GameObjectData.h"
#include "uldir.h"

enum ZulSpells
{

};

enum ZulTexts
{

};

///< Uldir Boss Zul - 138967
struct uldir_boss_zul : public BossAI
{
    uldir_boss_zul(Creature* creature) : BossAI(creature, DATA_BOSS_ZUL) {}
};

//Missing Script

void AddSC_boss_zul()
{
	//RegisterCreatureAI(uldir_boss_zul);
}