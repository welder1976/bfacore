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

#include "Creature.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "World.h"
#include "InstanceScript.h"
#include "uldir.h"

struct instance_uldir : public InstanceScript
{
    instance_uldir(InstanceMap* map) : InstanceScript(map) { }

    void Initialize() override
    {
        SetBossNumber(UldirData::DATA_MAX_ENCOUNTERS);

        talocGUID = ObjectGuid::Empty;
        motherGUID = ObjectGuid::Empty;
        vectisGUID = ObjectGuid::Empty;
        devourerGUID = ObjectGuid::Empty;
        zekvozGUID = ObjectGuid::Empty;
        zulGUID = ObjectGuid::Empty;
        mythraxGUID = ObjectGuid::Empty;
        ghuunGUID = ObjectGuid::Empty;
    }

    void OnCreatureCreate(Creature* creature) override
    {
        switch (creature->GetEntry())
        {
        case UldirCreatures::ULDIR_BOSS_TALOC:
            talocGUID = creature->GetGUID();
            break;

        case UldirCreatures::ULDIR_BOSS_MOTHER:
            motherGUID = creature->GetGUID();
            break;

        case UldirCreatures::ULDIR_BOSS_VECTIS:
            vectisGUID = creature->GetGUID();
            break;

        case UldirCreatures::ULDIR_BOSS_ZEKVOZ:
            zekvozGUID = creature->GetGUID();
            break;

        case UldirCreatures::ULDIR_BOSS_FETID_DEVOURER:
            devourerGUID = creature->GetGUID();
            break;
        case UldirCreatures::ULDIR_BOSS_ZUL:
            zulGUID = creature->GetGUID();
            break;

        case UldirCreatures::ULDIR_BOSS_MYTHRAX:
            mythraxGUID = creature->GetGUID();
            break;

        case UldirCreatures::ULDIR_BOSS_GHUUN:
            ghuunGUID = creature->GetGUID();
            break;

        default:
            break;
        }
    }

    void OnUnitDeath(Unit* unit) override
    {
        switch (unit->GetEntry())
        {
        case UldirCreatures::ULDIR_BOSS_TALOC:
            break;

        case UldirCreatures::ULDIR_BOSS_MOTHER:
            break;

        case UldirCreatures::ULDIR_BOSS_VECTIS:
        case UldirCreatures::ULDIR_BOSS_ZEKVOZ:
        case UldirCreatures::ULDIR_BOSS_FETID_DEVOURER:
            break;

        case UldirCreatures::ULDIR_BOSS_ZUL:
            break;

        case UldirCreatures::ULDIR_BOSS_MYTHRAX:
            break;

        case UldirCreatures::ULDIR_BOSS_GHUUN:
            break;

        default:
            break;
        }
    }

    ObjectGuid GetGuidData(uint32 identifier) const override
    {
        switch (identifier)
        {
        case UldirCreatures::ULDIR_BOSS_TALOC:
            return talocGUID;

        case UldirCreatures::ULDIR_BOSS_MOTHER:
            return motherGUID;

        case UldirCreatures::ULDIR_BOSS_VECTIS:
            return vectisGUID;

        case UldirCreatures::ULDIR_BOSS_ZEKVOZ:
            return zekvozGUID;

        case UldirCreatures::ULDIR_BOSS_FETID_DEVOURER:
            return devourerGUID;

        case UldirCreatures::ULDIR_BOSS_ZUL:
            return zulGUID;

        case UldirCreatures::ULDIR_BOSS_MYTHRAX:
            return mythraxGUID;

        case UldirCreatures::ULDIR_BOSS_GHUUN:
            return ghuunGUID;

        default:
            return ObjectGuid::Empty;
        }
    }

    ObjectGuid talocGUID;
    ObjectGuid motherGUID;
    ObjectGuid vectisGUID;
    ObjectGuid devourerGUID;
    ObjectGuid zekvozGUID;
    ObjectGuid zulGUID;
    ObjectGuid mythraxGUID;
    ObjectGuid ghuunGUID;
};

void AddSC_instance_uldir()
{
    RegisterInstanceScript(instance_uldir, 1861);
}