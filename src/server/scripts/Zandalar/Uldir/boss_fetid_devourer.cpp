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
#include "InstanceScript.h"
#include "Map.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellInfo.h"
#include "SpellScript.h"
#include "TemporarySummon.h"
#include "uldir.h"

enum FetidDevourerSpells
{
	SPELL_THRASHING_TERROR = 262255,
	SPELL_THRASHING_TERROR2 = 262256,
	SPELL_TERRIBLE_THRASH_TARGET_SELECTOR = 262274,
	SPELL_TERRIBLE_THRASH = 262277,

	SPELL_ROTTING_REGURGITATION_TARGET = 262291,
	SPELL_ROTTING_REGURGITATION = 262292,

	SPELL_SHOCKWAVE_STOMP = 262288,

	SPELL_MALODOROUS_MIASMA_AURA = 262313,
	SPELL_MALODOROUS_MIASMA_APPLY_ENEMY = 277408,
	SPELL_MALODOROUS_MIASMA_APPLY_ALLY = 277409,

	SPELL_FETID_FRENZY = 262378,

	SPELL_CONSUME_CORRUPTION = 262370,

	SPELL_ENTICING_ESSENCE = 262364,

	SPELL_TRASH_CHUTE_AT = 274470,
};

enum FetidDevourerEvents
{
	EVENT_ROTTING_REGURGITATION = 1,
	EVENT_SHOCKWAVE_STOMP,

};

enum FetidDevourerTexts
{
	SAY_ROTTING_REGURGITATION,
	SAY_WASTE_DISPOSAL_UNITS,
};

///< Uldir Boss Fetid Devourer - 133298
struct uldir_boss_fetid_devourer : public BossAI
{
	uldir_boss_fetid_devourer(Creature* creature) : BossAI(creature, DATA_BOSS_FETID_DEVOURER) {
	}
};

void AddSC_uldir_boss_fetid_devourer()
{

}