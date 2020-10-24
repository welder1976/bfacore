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
#include "ScriptedCreature.h"
#include "CellImpl.h"
#include "GridNotifiersImpl.h"
#include "MoveSplineInit.h"
#include "SpellAuraEffects.h"

enum UldirData
{
    DATA_BOSS_TALOC,
    DATA_BOSS_MOTHER,
    DATA_BOSS_FETID_DEVOURER,
    DATA_BOSS_ZEKVOZ,
    DATA_BOSS_VECTIS,
    DATA_BOSS_ZUL,
    DATA_BOSS_MYTHRAX,
    DATA_BOSS_GHUUN,
    DATA_MAX_ENCOUNTERS
};

enum UldirCreatures
{
    ULDIR_BOSS_TALOC = 137119,
    ULDIR_BOSS_MOTHER = 135452,
    ULDIR_BOSS_FETID_DEVOURER = 133298,
    ULDIR_BOSS_ZEKVOZ = 134445,
    ULDIR_BOSS_VECTIS = 134442,
    ULDIR_BOSS_ZUL = 138967,
    ULDIR_BOSS_MYTHRAX = 134546,
    ULDIR_BOSS_GHUUN = 132998,

    NPC_PURIFYING_FLAME = 136289,
    NPC_PURIFYING_FLAME_2 = 136325,
    NPC_WIND_TUNNEL = 136322,
    NPC_CHAMBER_NUMBER_1 = 136429,
    NPC_CHAMBER_NUMBER_2 = 137022,
    NPC_CHAMBER_NUMBER_3 = 137023,

    NPC_DEFENSE_GRID = 136298
};

enum Gameobjects
{
    GO_TALOC_ELEVATOR = 287464, //state 25
    GO_MOTHER_CACHE   = 291079,
};
