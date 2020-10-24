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


enum Mother
{
    ENERGIZE = 1,
    CLEANSING_PURGE,
    SANITIZING_STRIKE,
    PURIFYING_FLAME,
    WIND_TUNNEL,

    //Rooms
    SPELL_FIRST_ROOM_OCCUPANT = 267814,
    SPELL_SECOND_ROOM_OCCUPANT = 267817,
    SPELL_THIRD_ROOM_OCCUPANT = 267819,

    //Depleted Energy
    SPELL_DEPLETED_ENERGY = 274205,

    //Cleansing Purge
    SPELL_CLEANSING_PURGE_AT = 268089,    // on the sniffer
    SPELL_CLEANSING_PURGE_AURA = 268277, // on the sniffer

    //Purifying Flame
    SPELL_PURIFYING_FLAME_SUMMON = 267802, // on the sniffer
    SPELL_PURIFYING_FLAME_DAMAGE = 267803, // on the sniffer
    SPELL_PURIFYING_FLAME_AT = 274192,	 // on the sniffer Caster Entry: 136325

    //Sanitizing Strike
    SPELL_SANITIZING_STRIKE = 267787,

    //Wind Tunnel
    SPELL_WIND_TUNNEL_AT_R_TO_L = 267885,			// on the sniffer
    SPELL_WIND_TUNNEL_AT_L_TO_R = 267878,			// on the sniffer

    //275919 Wind Tunnel

    //Defense Grid
    SPELL_DEFENSE_GRID_DAMAGE = 267821,
    /* 267810 Defense Grid
   
    */
   
    // Mythic ?
    //Uldir Defensive Beams
    SPELL_ULDIR_DEFENSE_BEAM_H_AT_M = 275228,
    SPELL_ULDIR_DEFENSE_BEAM_V_AT_M = 277961,
    SPELL_ULDIR_DEFENSE_BEAM_V_AT_R = 281642,
    SPELL_ULDIR_DEFENSE_BEAM_V_AT_L = 281644,
    SPELL_ULDIR_DEFENSE_BEAM_AURA = 275432,
};

///< Uldir Boss MOTHER - 135452
struct uldir_boss_mother : public BossAI
{
    uldir_boss_mother(Creature* creature) : BossAI(creature, DATA_BOSS_MOTHER)
    {
        m_Instance = creature->GetInstanceScript();
    }

    void Reset() override
    {
        events.Reset();
        summons.DespawnAll();
        me->RemoveAllAreaTriggers();
        me->SetReactState(ReactStates::REACT_AGGRESSIVE);

        me->SetPowerType(POWER_ENERGY);
        me->SetPower(POWER_ENERGY, 0);
        me->SetMaxPower(POWER_ENERGY, 100);

        if (m_Instance)
        {
            m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            m_Instance->SetBossState(DATA_BOSS_MOTHER, NOT_STARTED);
        }
    }

    void EnterEvadeMode(EvadeReason why) override
    {
        m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FIRST_ROOM_OCCUPANT);
        m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SECOND_ROOM_OCCUPANT);
        m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_THIRD_ROOM_OCCUPANT);

        me->GetMotionMaster()->Clear();
        me->GetMotionMaster()->MoveTargetedHome();
        _EnterEvadeMode();
    }

    void EnterCombat(Unit* /*who*/) override
    {
        m_Instance->DoAddAuraOnPlayers(SPELL_FIRST_ROOM_OCCUPANT);

        events.ScheduleEvent(Mother::ENERGIZE, 5s);
        events.ScheduleEvent(Mother::PURIFYING_FLAME, 6s);

        if (m_Instance)
        {
            m_Instance->SetBossState(DATA_BOSS_MOTHER, IN_PROGRESS);
            m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me, 1);
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
            case Mother::ENERGIZE:
            {
                me->ModifyPower(POWER_ENERGY, 5);

                if (me->GetPower(POWER_ENERGY) == 100)
                {
                    events.ScheduleEvent(SANITIZING_STRIKE, 1s);
                    me->SetPower(POWER_ENERGY, 0);
                    break;
                }
                events.ScheduleEvent(Mother::ENERGIZE, 2s);

            } break;

            case Mother::SANITIZING_STRIKE:
            {
                if (Unit* victim = me->GetVictim())
                    me->CastSpell(victim, SPELL_SANITIZING_STRIKE);

                events.ScheduleEvent(Mother::ENERGIZE, 2s);

            } break;

            case Mother::WIND_TUNNEL:
            {
                if (Unit* victim = me->GetVictim())
                {
                    Position wind_tunnel_pos = wind_tunnel_positions[2];

                    if (victim->HasAura(SPELL_FIRST_ROOM_OCCUPANT))
                        wind_tunnel_pos = wind_tunnel_positions[0];

                    else if (victim->HasAura(SPELL_SECOND_ROOM_OCCUPANT))
                        wind_tunnel_pos = wind_tunnel_positions[1];

                    if (Creature* wind_tunnel = me->FindNearestCreature(UldirCreatures::NPC_WIND_TUNNEL, 75.0f))
                    {
                        if (urand(0, 1))
                            wind_tunnel->CastSpell(wind_tunnel, SPELL_WIND_TUNNEL_AT_L_TO_R);
                        else
                            wind_tunnel->CastSpell(wind_tunnel, SPELL_WIND_TUNNEL_AT_R_TO_L);
                    }
                }

                events.Repeat(12s);
            }

            case Mother::PURIFYING_FLAME:
            {
                if (Unit* victim = me->GetVictim())
                {

                    std::list<Player*> playerList;
                    GetPlayerListInGrid(playerList, me, 50.0f);
                }

            } break;

            default:
                break;
            }
        }

        DoMeleeAttackIfReady();
    }

private:
    InstanceScript* m_Instance;

    Position wind_tunnel_positions[3] =
    {
        { -135.3048f, -255.4786f, 695.2135f },
        { -81.3048f, -255.4786f, 695.2144f },
        { -27.3048f, -255.4786f, 695.2135f }
    };
};

///< Npc Chamber #1 - 136429
struct uldir_chamber_1 : public ScriptedAI
{
    uldir_chamber_1(Creature* creature) : ScriptedAI(creature) {}

    void Reset() override
    {
        me->SetReactState(REACT_PASSIVE);
    }
};

///< Npc Chamber #2 - 137022
struct uldir_chamber_2 : public ScriptedAI
{
    uldir_chamber_2(Creature* creature) : ScriptedAI(creature) {}

    void Reset() override
    {
        me->SetReactState(REACT_PASSIVE);
    }
};

///< Npc Chamber #3 - 137023
struct uldir_chamber_3 : public ScriptedAI
{
    uldir_chamber_3(Creature* creature) : ScriptedAI(creature) {}

    void Reset() override
    {
        me->SetReactState(REACT_PASSIVE);
    }
};

///< Npc Wind Tunnel - 136322
struct npc_mother_wind_tunnel : public ScriptedAI
{
    npc_mother_wind_tunnel(Creature* creature) : ScriptedAI(creature) {}

    void Reset() override
    {
        me->SetReactState(REACT_PASSIVE);
    }
};

///< Npc Purifying Flame (corners) - 136325
struct npc_corners_purifying_flame : public ScriptedAI
{
    npc_corners_purifying_flame(Creature* creature) : ScriptedAI(creature) {}

    void Reset() override
    {
        me->SetReactState(REACT_PASSIVE);
    }

    void DoAction(int32 action) override
    {

    }
};

///< Npc Purifying Flame (summon) - 136289
struct npc_summons_purifying_flame : public ScriptedAI
{
    npc_summons_purifying_flame(Creature* creature) : ScriptedAI(creature) {}

    void Reset() override
    {
        me->SetReactState(REACT_PASSIVE);
    }
};

///< Npc Defense Grid - 136298
struct npc_mother_defense_grid : public ScriptedAI
{
    npc_mother_defense_grid(Creature* creature) : ScriptedAI(creature)
    {
        timer = 200;
    }

    void Reset() override
    {
        me->SetReactState(REACT_PASSIVE);
    }

    void UpdateAI(uint32 diff) override
    {
        if (timer < diff)
        {
            timer = 200;
            std::list<Player*> playerList;
            GetPlayerListInGrid(playerList, me, 10.0f);

            for (Player* player : playerList)
            {
                if (player->HasAura(Mother::SPELL_FIRST_ROOM_OCCUPANT))
                {
                    // if player goes from chamber #1 to chamber #2
                    if (player->GetPosition().m_positionX > first_to_second)
                    {
                        player->RemoveAura(Mother::SPELL_FIRST_ROOM_OCCUPANT);
                        player->AddAura(Mother::SPELL_SECOND_ROOM_OCCUPANT);

                        instance->DoCastSpellOnPlayers(Mother::SPELL_DEFENSE_GRID_DAMAGE);
                        player->Yell(player->GetName(), LANG_UNIVERSAL);
                    }
                }
                else if (player->HasAura(Mother::SPELL_SECOND_ROOM_OCCUPANT))
                {
                    // if player goes from chamber #2 to chamber #3
                    if (player->GetPosition().m_positionX > second_to_third)
                    {
                        player->RemoveAura(Mother::SPELL_SECOND_ROOM_OCCUPANT);
                        player->AddAura(Mother::SPELL_THIRD_ROOM_OCCUPANT);

                        instance->DoCastSpellOnPlayers(Mother::SPELL_DEFENSE_GRID_DAMAGE);
                        player->Yell(player->GetName(), LANG_UNIVERSAL);
                    }
                    // if player goes from chamber #2 to chamber #1
                    else if (player->GetPosition().m_positionX < first_to_second)
                    {
                        player->RemoveAura(Mother::SPELL_SECOND_ROOM_OCCUPANT);
                        player->AddAura(Mother::SPELL_FIRST_ROOM_OCCUPANT);

                        instance->DoCastSpellOnPlayers(Mother::SPELL_DEFENSE_GRID_DAMAGE);
                        player->Yell(player->GetName(), LANG_UNIVERSAL);
                    }
                }

                else if (player->HasAura(Mother::SPELL_THIRD_ROOM_OCCUPANT))
                {
                    // if player goes from chamber #3 to chamber #2
                    if (player->GetPosition().m_positionX < second_to_third)
                    {
                        player->RemoveAura(Mother::SPELL_THIRD_ROOM_OCCUPANT);
                        player->AddAura(Mother::SPELL_SECOND_ROOM_OCCUPANT);

                        instance->DoCastSpellOnPlayers(Mother::SPELL_DEFENSE_GRID_DAMAGE);
                        player->Yell(player->GetName(), LANG_UNIVERSAL);
                    }
                }
            }
        }
        else
            timer -= diff;
    }

private:
    float timer;
    float first_to_second = -108.608f;
    float second_to_third = -54.6701f;
};

///< Npc Red Queen Room - 136736
struct npc_red_queen_room : public ScriptedAI
{
    npc_red_queen_room(Creature* creature) : ScriptedAI(creature) {}

    void Reset() override
    {
        me->SetReactState(REACT_PASSIVE);
    }
};

Position wind_tunnel_push_positions[6] =
{
    { -135.568f, -285.398f,	695.298f, float(M_PI / 2.0) },
    { -135.576f, -225.922f,	695.298f, float(3 * M_PI / 2.0) },
    { -81.5712f, -285.398f,	695.298f, float(M_PI / 2.0) },
    { -81.6562f, -225.922f,	695.298f, float(3 * M_PI / 2.0) },
    { -27.5035f, -285.398f,	695.298f, float(M_PI / 2.0) },
    { -27.6267f, -225.922f,	695.298f, float(3 * M_PI / 2.0) }
};

///< Areatrigger Wind Tunnel - 267878
struct at_wind_tunnel_l_to_r : public AreaTriggerAI
{
    at_wind_tunnel_l_to_r(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) {}

    void OnInitialize() override
    {
        at->SetPeriodicProcTimer(500);
    }

    void OnPeriodicProc() override
    {
        std::list<Player*> playerList;
        GetPlayerListInGrid(playerList, at->GetCaster(), 100.0f);

        for (Player* player : playerList)
        {
            if (player->HasAura(SPELL_FIRST_ROOM_OCCUPANT))
                player->ApplyMovementForce(at->GetGUID(), wind_tunnel_push_positions[1],-5.f, 0);
            else if (player->HasAura(SPELL_SECOND_ROOM_OCCUPANT))
                player->ApplyMovementForce(at->GetGUID(), wind_tunnel_push_positions[3],-5.f, 0);
            else if (player->HasAura(SPELL_THIRD_ROOM_OCCUPANT))
                player->ApplyMovementForce(at->GetGUID(), wind_tunnel_push_positions[5],-5.f, 0);
        }
    }

    void OnRemove() override
    {
        if (InstanceScript* instance = at->GetInstanceScript())
            instance->DoRemoveForcedMovementsOnPlayers(at->GetGUID());
    }
};

///< Areatrigger Wind Tunnel - 267885
struct at_wind_tunnel_r_to_l : public AreaTriggerAI
{
    at_wind_tunnel_r_to_l(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) {}

    void OnInitialize() override
    {
        at->SetPeriodicProcTimer(500);
    }

    void OnPeriodicProc() override
    {
        std::list<Player*> playerList;
        GetPlayerListInGrid(playerList, at->GetCaster(), 75.0f);

        for (Player* player : playerList)
        {
            if (player->HasAura(SPELL_FIRST_ROOM_OCCUPANT))
                player->ApplyMovementForce(at->GetGUID(), wind_tunnel_push_positions[0], - 5.f, 0);
            else if (player->HasAura(SPELL_SECOND_ROOM_OCCUPANT))
                player->ApplyMovementForce(at->GetGUID(), wind_tunnel_push_positions[2], -5.f, 0);
            else if (player->HasAura(SPELL_THIRD_ROOM_OCCUPANT))
                player->ApplyMovementForce(at->GetGUID(), wind_tunnel_push_positions[4], - 5.f, 0);
        }
    }

    void OnRemove() override
    {
        if (InstanceScript* instance = at->GetInstanceScript())
            instance->DoRemoveForcedMovementsOnPlayers(at->GetGUID());
    }
};

void AddSC_boss_mother()
{
    RegisterCreatureAI(uldir_boss_mother);

    // Estos son los AT de cada Camara que hace Detectar a Mother
    RegisterCreatureAI(uldir_chamber_1);                 
    RegisterCreatureAI(uldir_chamber_2); 
    RegisterCreatureAI(uldir_chamber_3); 

    RegisterCreatureAI(npc_red_queen_room);				//136736
    RegisterCreatureAI(npc_mother_defense_grid);		//136298

    RegisterCreatureAI(npc_mother_wind_tunnel);			//136322
    RegisterCreatureAI(npc_corners_purifying_flame);	//136325
    RegisterCreatureAI(npc_summons_purifying_flame);	//136298

    RegisterAreaTriggerAI(at_wind_tunnel_l_to_r);  
    RegisterAreaTriggerAI(at_wind_tunnel_r_to_l);   
}

