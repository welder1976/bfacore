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

#include "ScriptMgr.h"
#include "Player.h"

enum SceneSpells
{
    SPELL_DEATHWING_SIMULATOR = 201184
};

class scene_deathwing_simulator : public SceneScript
{
    public:
        scene_deathwing_simulator() : SceneScript("scene_deathwing_simulator") { }

    // Called when a player receive trigger from scene
    void OnSceneTriggerEvent(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/, std::string const& triggerName) override
    {
        if (triggerName == "BURN PLAYER")
            player->CastSpell(player, SPELL_DEATHWING_SIMULATOR, true); // Deathwing Simulator Burn player
    }
};

class scene_the_story_of_huln : public SceneScript
{
public:
    scene_the_story_of_huln() : SceneScript("scene_the_story_of_huln") { }

    // Called when a scene is either canceled or completed
    void OnSceneEnd(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) override
    {
        player->KilledMonsterCredit(98825);
        player->SummonCreature(98825, Position(4057.2099f, 4320.37f, 675.603f, 1.2158f), TEMPSUMMON_MANUAL_DESPAWN, 0, 0, true);
    }
};

void AddSC_scene_scripts()
{
    new scene_deathwing_simulator();
    new scene_the_story_of_huln();
}
