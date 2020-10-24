/*
 * Copyright (C) Latin Core Team
 * Copyright (C) Thordekk
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
#include "PhasingHandler.h"
#include "ScriptedGossip.h"
#include "ObjectMgr.h"
#include "Log.h"
    
    enum
{
    SPELL_PLAYER_CHOICE_WARLOCK = 199411,
    PLAYER_CHOICE_WARLOCK_AFFLICTION = 523,
    PLAYER_CHOICE_WARLOCK_DEMONOLOGY = 524,
    PLAYER_CHOICE_WARLOCK_DESTRUCTION = 525,
    PLAYER_CHOICE_WARLOCK_SELECTION = 245,
    
    ///WARLOCK Quest 
    NPC_RITSSYN_FLAMESCOWL_103506 = 103506,
    QUEST_THE_SIXTH = 40716,
    QUEST_THE_NEW_BLOOD = 40729,
    
    QUEST_THE_TOME_OF_BLIGHTED_IMPLEMENTS = 40684,
    QUEST_THE_TOME_OPENS_AGAIN = 43984,
    QUEST_A_GREATER_ARSENAL = 44089,
        ///AFFLICTION
    QUEST_TRACKING_DOWN_UTHALESH = 42939,
    QUEST_ULTHALESH_THE_DEADWIND_HARVESTER = 40495,
        ///DEMONOLOGY
    QUEST_RITUAL_REAGENTS = 42128,
        ///DESTRUCTION
    QUEST_FINDING_THE_SCEPTER = 43100,
    QUEST_AN_EYE_FOR_A_SCEPTER = 43153,
};

struct npc_ritssyn_flamescowl_103506 : public ScriptedAI
{
    npc_ritssyn_flamescowl_103506(Creature* creature) : ScriptedAI(creature) { SayHi = false; }

    void MoveInLineOfSight(Unit* who) override
    {
        if (!who || !who->IsInWorld())
            return;
        if (!me->IsWithinDist(who, 25.0f, false))
            return;

        Player* player = who->GetCharmerOrOwnerPlayerOrPlayerItself();

        if (!player)
            return;
        me->GetMotionMaster()->MoveFollow(player, PET_FOLLOW_DIST, me->GetFollowAngle());
        if (!SayHi)
        {
            SayHi = true;
            //summon one door
            Talk(0, player);
        }
    }

    void sQuestAccept(Player* player, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_THE_SIXTH)
        {
            me->Say(104179, player);
            me->DespawnOrUnsummon(5000);
        }
    }
private:
    bool SayHi;
};

class quest_phase_for_warlock : public PlayerScript {
public:
    quest_phase_for_warlock() : PlayerScript("quest_phase_for_warlock") {}
    
        void OnMapChanged(Player * player)
         {
        CheckPhase(player);
        }
     void OnUpdateArea(Player * player, uint32 newArea, uint32 /*oldArea*/)
         {
        CheckPhase(player);
        }
    
        void CheckPhase(Player * player)
         {
        if (player->getClass() != CLASS_WARLOCK)
             return;
        if (player->HasQuest(QUEST_THE_SIXTH) && player->GetAreaId() == 7594)
             PhasingHandler::AddPhase(player, 5646, true);
        
            if (player->HasQuest(QUEST_THE_NEW_BLOOD) && player->GetQuestStatus(QUEST_THE_NEW_BLOOD) == QUEST_STATUS_INCOMPLETE && player->GetAreaId() == 7594)
             PhasingHandler::AddPhase(player, 5646, true);
        
            if ((player->GetQuestStatus(QUEST_THE_NEW_BLOOD) == QUEST_STATUS_COMPLETE || player->GetQuestStatus(QUEST_THE_NEW_BLOOD) == QUEST_STATUS_REWARDED) && player->GetAreaId() == 7594)
             PhasingHandler::AddPhase(player, 6230, true);
        
            }
    
};

struct npc_ritssyn_flamescowl_101456 : public ScriptedAI
 {
    npc_ritssyn_flamescowl_101456(Creature * creature) : ScriptedAI(creature) { Initialize(); }
    
        void DoAction(int32 param)
         {
        switch (param)
            {
        case 1:
            break;
            }
         }
    
        void UpdateAI(uint32 diff) override
         {
        
            if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
             {
            me->GetScheduler().Schedule(Milliseconds(1000), [player](TaskContext context)
                 {
                GetContextCreature()->Say(105438, player);
                });
            }
         if (!UpdateVictim())
             return;
        
            DoMeleeAttackIfReady();
        }
    
        void sQuestAccept(Player * player, Quest const* quest) override
         {
        if (quest->GetQuestId() == QUEST_THE_NEW_BLOOD)
             {
                        //101456 FaceDirection: 3.508112
                me->Say(103743);
            me->GetMotionMaster()->MovePoint(1, Position(-851.656f, 4448.82f, 696.0333f), true);
            me->GetScheduler().Schedule(Milliseconds(3000), [](TaskContext context)
                 {
                GetContextCreature()->Say(103744);
                });
                        //101457 
                if (Creature* npc = me->FindNearestCreature(101457, 50.0f, true))
                 npc->GetMotionMaster()->MovePoint(1, Position(-867.946f, 4456.87f, 696.0369f), true);
                        //101454 
                if (Creature* npc = me->FindNearestCreature(101454, 20.0f, true))
                 npc->GetMotionMaster()->MovePoint(1, Position(-851.984f, 4443.61f, 696.0237f), true);
                        //98797 ->summon 111468 X: -834.566 Y: 4414.436 Z: 716.7865
                if (Creature* npc = me->FindNearestCreature(98797, 20.0f, true))
                 {
                npc->GetMotionMaster()->MovePoint(1, Position(-853.74f, 4452.87f, 696.0456f), true);
                npc->GetScheduler().Schedule(Milliseconds(5000), [](TaskContext context)
                     {
                    GetContextCreature()->Say(103745);
                    });
                npc->GetScheduler().Schedule(Milliseconds(7000), [](TaskContext context)
                     {
                    GetContextCreature()->Say(103746);
                    GetContextCreature()->CastSpell(GetContextCreature(), 199952, true);
                    
                        });
                npc->GetScheduler().Schedule(Milliseconds(9000), [player](TaskContext context)
                     {
                    if (Creature* door = player->GetSummonedCreatureByEntry(101433))
                         door->DespawnOrUnsummon();
                    player->SummonCreature(101433, Position(-868.9045f, 4438.607f, 696.7223f, 0.8927315f));
                    GetContextCreature()->Say(103747);
                    
                                            ///summon door //door 101433 delay 9000 cast 222203
                        if (Creature* door = player->GetSummonedCreatureByEntry(101433))
                         {
                                                ///door->CastSpell(door, 222203, true);
                            door->CastSpell(player, 187114, true);
                        if (!door->HasNpcFlag(UNIT_NPC_FLAG_SPELLCLICK))
                             door->AddNpcFlag(UNIT_NPC_FLAG_SPELLCLICK);
                        }
                    
                        });
                
                    }
            
                            //101455 ->summon 111466
                if (Creature* npc = me->FindNearestCreature(101455, 50.0f, true))
                 {
                npc->GetMotionMaster()->MovePoint(1, Position(-862.42f, 4457.42f, 696.0723f), true);
                npc->GetScheduler().Schedule(Milliseconds(11000), [](TaskContext context)
                     {
                    GetContextCreature()->Say(103748);
                    });
                npc->GetScheduler().Schedule(Milliseconds(13000), [player](TaskContext context)
                     {
                    GetContextCreature()->Say(103749);
                    if (Creature* door = player->GetSummonedCreatureByEntry(101433))
                         if (door->HasUnitFlag(UNIT_FLAG_NOT_SELECTABLE))
                         door->RemoveUnitFlag(UNIT_FLAG_NOT_SELECTABLE);
                    });
                                //underbelly-guard 111458 cast 217977
                    }
            
                            //101433 cast 187114 on player
                        //after player cast 101433 SPELL_CLICK 199967
                
                }
         }
    
        void sQuestReward(Player * player, Quest const* quest, uint32 /*opt*/)  override
         {
               // if (quest->GetQuestId() == QUEST_WITNESS_TO_THE_WOUND)
                   // {
            
                   // }
            }
     void Initialize()
         {
        
            }
    
        
        void MoveInLineOfSight(Unit * who) override
         {
        if (!who || !who->IsInWorld())
             return;
        if (!me->IsWithinDist(who, 15.0f, false))
             {
            
                return;
            }
         Player * player = who->GetCharmerOrOwnerPlayerOrPlayerItself();
        if (!player)
             return;
        
            }
 private:
    
        std::set<ObjectGuid> pList;
    ObjectGuid   m_playerGUID;
    };

struct npc_summoning_portal_101433 : public ScriptedAI
 {
    npc_summoning_portal_101433(Creature * creature) : ScriptedAI(creature) { }
    
        
        void DoAction(int32 param)
         {
        switch (param)
            {
        case 1:
            
                
                
                break;
            }
         }
    
        void OnSpellClick(Unit * clicker, bool& /*result*/)
         {
        if (Player* player = clicker->ToPlayer())
             {
                        //199966 199967
                            //199989->199967,199971
                            //212062->199967,204833    
                player->CastSpell(player, 199966, true);
            
                            //delay player->CastSpell(player, 199966, true);
                            //101455,101456,101457,98797 cast 199971
                            //101454  199971
                            /*
                            if (Creature* npc = player->FindNearestCreature(101455, 50.0f, true))
                                npc->CastSpell(me, 199971, true);
                            if (Creature* npc = player->FindNearestCreature(101456, 50.0f, true))
                                npc->CastSpell(me, 199971, true);
                            if (Creature* npc = player->FindNearestCreature(101457, 50.0f, true))
                                npc->CastSpell(npc, 199971, true);
                            if (Creature* npc = player->FindNearestCreature(98797, 50.0f, true))
                                npc->CastSpell(npc, 199971, true);
                            if (Creature* npc = player->FindNearestCreature(101454, 50.0f, true))
                                npc->CastSpell(npc, 199971, true);
                                */
                
                if (Creature* npc = player->FindNearestCreature(98797, 50.0f, true))
                 {
                npc->Say(103750);
                                //go 245958
                    npc->GetScheduler().Schedule(Milliseconds(2000), [](TaskContext context)
                         {
                    GetContextCreature()->Say(103764);
                    });
                npc->GetScheduler().Schedule(Milliseconds(6000), [](TaskContext context)
                     {
                    GetContextCreature()->Say(103752);
                    });
                npc->GetScheduler().Schedule(Milliseconds(10000), [](TaskContext context)
                     {
                    GetContextCreature()->Say(103754);
                    });
                npc->GetScheduler().Schedule(Milliseconds(14000), [player](TaskContext context)
                     {
                    GetContextCreature()->Say(103756);
                                        //fire 35845 cast 181569
                                            //fire 35845 cast aura 199994
                                            //99561 cast 199990 stopcast summon
                        player->CastSpell(player, 199990, true);
                    
                        });
                
                    npc->GetScheduler().Schedule(Milliseconds(20000), [](TaskContext context)
                         {
                                        //GetContextCreature()->Say(103754);
                                            /*98797 Points: X: -859.495 Y: 4449.82 Z: 696.0513
                                    98797 103759
                                    98797 cast 199952*/
                        GetContextCreature()->RemoveAura(204833);
                    GetContextCreature()->GetMotionMaster()->MovePoint(2, Position(-859.495f, 4449.82f, 696.0513f), true);
                    GetContextCreature()->Say(103759);
                    });
                npc->GetScheduler().Schedule(Milliseconds(20000), [](TaskContext context)
                     {
                    GetContextCreature()->CastSpell(GetContextCreature(), 199952, true);
                    });
                                /*
                            98831 cast 200016 catch 98797
                            98831 say 103760
                            98797  say 103761
                            98831 say 103762
                            98831 cast 200038 call trigger 200039->player
                            player cast 200032 to 101460*/
                    npc->GetScheduler().Schedule(Milliseconds(22000), [npc, player](TaskContext context)
                         {
                    if (Creature* boss = player->GetSummonedCreatureByEntry(98831))
                         {
                        boss->CastSpell(npc, 200016, true);
                        boss->Say(103760);
                        }
                     });
                npc->GetScheduler().Schedule(Milliseconds(24000), [](TaskContext context)
                     {
                    GetContextCreature()->Say(103761);
                    });
                npc->GetScheduler().Schedule(Milliseconds(26000), [npc, player](TaskContext context)
                     {
                    if (Creature* boss = player->GetSummonedCreatureByEntry(98831))
                         {
                        boss->Say(103762);
                        boss->CastSpell(player, 200038, true);
                        
                            }
                     });
                
                                    /*
                                    101460 cast 196229
                                    101460 cast 200033 to all
                    
                                    98831 say 103763
                                    player cast 199362
                                    player cast 194854
                                    (DstLocation) Location: X: 2964.78 Y: 1128.12 Z: 206.71
                                    (Cast) (Target) Orientation: 3.600265
                                    (Cast) (Target) MapID: 1522
                                    */
                    
                    npc->GetScheduler().Schedule(Milliseconds(30000), [npc, player](TaskContext context)
                         {
                    if (Creature* boss = player->GetSummonedCreatureByEntry(98831))
                         boss->Say(103763);
                    });
                npc->GetScheduler().Schedule(Milliseconds(31000), [npc, player](TaskContext context)
                     {
                    player->CastSpell(player, 199362, true);
                    });
                npc->GetScheduler().Schedule(Milliseconds(32000), [npc, player](TaskContext context)
                     {
                    player->CastSpell(player, 194854, true);
                    player->RemoveAura(204833);
                                            /*
                                            ServerToClient: SMSG_NEW_WORLD (0x25A9) Length: 36 ConnIdx: 1 Time: 09/19/2016 20:35:43.164 Number: 6065
                                            Map: 1522 (-Unknown-)
                                            Position: X: 2964.78 Y: 1128.12 Z: 206.71 O: 3.600265
                                            */
                                                //to do teleport
                        player->TeleportTo(1522, Position(2964.78f, 1128.12f, 206.71f, 3.600265f));
                    });
                
                    }
             if (Creature* npc = player->FindNearestCreature(101457, 50.0f, true))
                 {
                npc->GetScheduler().Schedule(Milliseconds(4000), [](TaskContext context)
                     {
                    GetContextCreature()->Say(103751);
                    });
                npc->GetScheduler().Schedule(Milliseconds(16000), [player](TaskContext context)
                     {
                    GetContextCreature()->Say(103757);
                                        //player cast 199989
                                            //all fire despawn 35845
                        if (Creature* door = player->GetSummonedCreatureByEntry(98831))
                         door->DespawnOrUnsummon();
                    player->SummonCreature(98831, Position(-867.6754f, 4439.961f, 696.7425f, 0.7974676f));
                                        //GetContextCreature()->Say(103747);
                        
                                            ///summon door //door 101433 delay 9000 cast 222203
                        if (Creature* boss = player->GetSummonedCreatureByEntry(98831))
                         {
                        boss->Say(103758);
                        boss->CastSpell(boss, 200011, true);
                                                /*
                                                98797 move Points: X: -846.1344 Y: 4459.916 Z: 696.0439
                                                101454 Points: X: -841.886 Y: 4445.958 Z: 696.023
                                                101455 Points: X: -859.4317 Y: 4467.347 Z: 696.0369
                                                101456 Points: X: -842.5834 Y: 4453.837 Z: 696.0247
                                                101457 Points: X: -868.1119 Y: 4467.236 Z: 696.6078
                        
                                                98797 ,101454,101455,101456,101457,player cast 204833
                                                */
                            player->CastSpell(player, 204833, true);
                                                //98797 ,101454,101455,101456,101457
                            if (Creature* npc1 = player->FindNearestCreature(98797, 50.0f, true))
                             npc1->CastSpell(npc1, 204833, true);
                        if (Creature* npc1 = player->FindNearestCreature(101454, 50.0f, true))
                             npc1->CastSpell(npc1, 204833, true);
                        if (Creature* npc1 = player->FindNearestCreature(101455, 50.0f, true))
                             npc1->CastSpell(npc1, 204833, true);
                        if (Creature* npc1 = player->FindNearestCreature(101456, 50.0f, true))
                             npc1->CastSpell(npc1, 204833, true);
                        if (Creature* npc1 = player->FindNearestCreature(1014567, 50.0f, true))
                             npc1->CastSpell(npc1, 204833, true);
                        }
                    
                        });
                }
             if (Creature* npc = player->FindNearestCreature(101456, 50.0f, true))
                 {
                npc->GetScheduler().Schedule(Milliseconds(8000), [](TaskContext context)
                     {
                    GetContextCreature()->Say(103753);
                    });
                npc->GetScheduler().Schedule(Milliseconds(12000), [](TaskContext context)
                     {
                    GetContextCreature()->Say(103755);
                    });
                }
            
                
                
                }
         }
 private:
    };

struct npc_calydus_101097 : public ScriptedAI
 {
    npc_calydus_101097(Creature * creature) : ScriptedAI(creature) { Initialize(); }
    
        void sQuestAccept(Player * player, Quest const* quest) override
         {
        if (quest->GetQuestId() == QUEST_THE_TOME_OF_BLIGHTED_IMPLEMENTS)
             {
            me->GetScheduler().Schedule(1s, 2s, [player](TaskContext context)
                 {
                player->CastSpell(player, SPELL_PLAYER_CHOICE_WARLOCK, true);
                });
            }
         }
    
        void sQuestReward(Player * player, Quest const* quest, uint32 /*opt*/)  override
         {
        if (quest->GetQuestId() == QUEST_THE_NEW_BLOOD)
             {
            if (const Quest* quest = sObjectMgr->GetQuestTemplate(QUEST_THE_TOME_OF_BLIGHTED_IMPLEMENTS))
                 player->AddQuest(quest, me);
            }
         }
    
        void Initialize()
         {
        IsLock = false;
        }
    
        void MoveInLineOfSight(Unit * who) override
         {
        if (!who || !who->IsInWorld())
             return;
        if (!me->IsWithinDist(who, 15.0f, false))
             return;
        Player * player = who->GetCharmerOrOwnerPlayerOrPlayerItself();
        if (!player)
             return;
        if (!IsLock && player->HasQuest(QUEST_THE_NEW_BLOOD))
             {
            IsLock = true;
            player->KilledMonsterCredit(101150);
            me->Say(103567);
                        //103568
                me->GetScheduler().Schedule(Milliseconds(4000), [](TaskContext context)
                     {
                GetContextCreature()->Say(103567);
                });
            }
         }
    
        void sGossipSelect(Player * player, uint32 /*menuId*/, uint32 gossipListId)
         {
        CloseGossipMenuFor(player);
        player->CastSpell(player, SPELL_PLAYER_CHOICE_WARLOCK, true);
        }
     };

class warlock_weapons_of_legend_player_choice : public PlayerScript {
public:
    warlock_weapons_of_legend_player_choice() : PlayerScript("warlock_weapons_of_legend_player_choice") {}
    
        void OnPlayerChoiceResponse(Player * player, uint32 choiceId, uint32 responseId)
         {
        if (player->getClass() != CLASS_WARLOCK)
             return;
        if (choiceId != PLAYER_CHOICE_WARLOCK_SELECTION)
             return;
        
            TC_LOG_ERROR("server.worldserver", "warlock_playerchoice %u, %u", choiceId, responseId);
        if (player->HasQuest(QUEST_THE_TOME_OF_BLIGHTED_IMPLEMENTS))
             player->KilledMonsterCredit(101095);
        if (player->HasQuest(QUEST_THE_TOME_OPENS_AGAIN))
             player->KilledMonsterCredit(112077);
        if (player->HasQuest(QUEST_A_GREATER_ARSENAL))
             player->KilledMonsterCredit(113792);
        
            switch (responseId)
            {
            case PLAYER_CHOICE_WARLOCK_AFFLICTION:
                if (player->GetQuestStatus(QUEST_ULTHALESH_THE_DEADWIND_HARVESTER) == QUEST_STATUS_NONE)
                     if (const Quest* quest = sObjectMgr->GetQuestTemplate(QUEST_ULTHALESH_THE_DEADWIND_HARVESTER))
                     player->AddQuest(quest, nullptr);
                break;
            case PLAYER_CHOICE_WARLOCK_DEMONOLOGY:
                if (player->GetQuestStatus(QUEST_RITUAL_REAGENTS) == QUEST_STATUS_NONE)
                     if (const Quest* quest = sObjectMgr->GetQuestTemplate(QUEST_RITUAL_REAGENTS))
                     player->AddQuest(quest, nullptr);
                break;
            case PLAYER_CHOICE_WARLOCK_DESTRUCTION:
                if (player->GetQuestStatus(QUEST_FINDING_THE_SCEPTER) == QUEST_STATUS_NONE)
                     if (const Quest* quest = sObjectMgr->GetQuestTemplate(QUEST_FINDING_THE_SCEPTER))
                     player->AddQuest(quest, nullptr);
                break;
            default:
                break;
                }
        
            }
    
};

void AddSC_class_hall_warlock()
{
    RegisterCreatureAI(npc_ritssyn_flamescowl_103506);
    new quest_phase_for_warlock();
    RegisterCreatureAI(npc_ritssyn_flamescowl_101456);
    RegisterCreatureAI(npc_summoning_portal_101433);
    RegisterCreatureAI(npc_calydus_101097);
    new warlock_weapons_of_legend_player_choice();
}
