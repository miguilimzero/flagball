#include <string.h>
#include <engine/shared/config.h>
#include <engine/server.h>
#include <game/mapitems.h>
#include <game/server/entities/character.h>
#include <game/server/player.h>
#include <game/server/gamecontext.h>
#include <game/collision.h>
#include "fb.h"

CBall::CBall(CGameWorld *pGameWorld, int Team, vec2 Pos)
: CFlag(pGameWorld, Team)
{
	m_StandPos = Pos;
	m_Pos = Pos;
}

void CBall::Reset()
{
	CFlag::Reset();
	m_pLastCarrier = NULL;
	m_DropTick = 0;
	m_IdleTick = -1;
	m_LastCarrierTeam = !m_Team;
}

void CBall::Tick()
{
	if (!m_pCarryingCharacter && !m_AtStand)
	{
		if (GameLayerClipped(m_Pos) || (m_IdleTick >= 0 && Server()->Tick() > m_IdleTick + Server()->TickSpeed() * g_Config.m_SvfbBallResetTime))
		{
			Reset();
			GameServer()->CreateSoundGlobal(SOUND_CTF_RETURN);
		}
		else
		{
			// do ball physics
			m_Vel.y += GameServer()->m_World.m_Core.m_Tuning.m_Gravity;
			if (m_Vel.x > -0.1f && m_Vel.x < 0.1f)
				m_Vel.x = 0.0f;
			else if (m_Vel.x < 0)
				m_Vel.x += 0.055f;
			else
				m_Vel.x -= 0.055f;

			float vy = m_Vel.y;
			GameServer()->Collision()->MoveBox(&m_Pos, &m_Vel, vec2(ms_PhysSize, ms_PhysSize), 0.5f);
			if (m_Vel.x == 0 && (vy > 0) != (m_Vel.y > 0) && abs(m_Vel.y) < GameServer()->m_World.m_Core.m_Tuning.m_Gravity)
			{
				if (m_IdleTick == -1)
					m_IdleTick = Server()->Tick();
			}
			else
				m_IdleTick = -1;
		}
	}
}

CGameControllerFB::CGameControllerFB(class CGameContext *pGameServer) 
: IGameController(pGameServer)
{
	m_apBalls[0] = NULL;
	m_apBalls[1] = NULL;
	m_pGameType = g_Config.m_SvfbInstagib ? "iFB" : "FB";
	m_GameFlags = GAMEFLAG_TEAMS | GAMEFLAG_FLAGS;
	m_apGoalPos[0] = vec2(0, 0);
	m_apGoalPos[1] = vec2(0, 0);
	m_NumGoals = 0;
	GameServer()->Tuning()->m_LaserDamage = g_Config.m_SvfbInstagib ? 10 : 5;
	
}

bool CGameControllerFB::OnEntity (int index, vec2 pos)
{
	
	//	don't add pickups in instagib
	if (strcmp (m_pGameType, "iFB") == 0 && index > ENTITY_FLAGSTAND_BLUE)
		return false;
	
	if (IGameController::OnEntity (index, pos))
		return true;
	
	//	add goal positions and balls
	switch (index)
	{
		case ENTITY_SPAWN:
			if (m_NumGoals < FB_MAX_GOALS)
				m_apGoalPos[m_NumGoals++] = pos;
			break;
		case ENTITY_FLAGSTAND_RED:
			if (!m_apBalls[TEAM_RED])
				GameServer()->m_World.InsertEntity(m_apBalls[TEAM_RED] = new CBall(&GameServer()->m_World, TEAM_RED, pos));
			break;
		case ENTITY_FLAGSTAND_BLUE:
			if (!m_apBalls[TEAM_BLUE])
				GameServer()->m_World.InsertEntity(m_apBalls[TEAM_BLUE] = new CBall(&GameServer()->m_World, TEAM_BLUE, pos));
			break;
		default:
			return false;
	}
	return true;
	
}

int CGameControllerFB::OnCharacterDeath(class CCharacter *victim, class CPlayer *killer, int weaponid)
{
	
	// do scoreing
	if (weaponid == WEAPON_SELF || weaponid == WEAPON_WORLD)
		victim->GetPlayer()->m_Score--;	// suicide or death-tile
	else if (killer && weaponid != WEAPON_GAME)
		{
			if (IsTeamplay() && victim->GetPlayer()->GetTeam() == killer->GetTeam())
				killer->m_Score--;	// teamkill
			else
				killer->m_Score++;	// normal kill
		}

	int mode = 0;
	if (killer)
	{
		CCharacter *const ch = killer->GetCharacter();
		if (ch && ch->m_pBall)
			mode |= 2;
	};
	if (victim->m_pBall)
	{
		victim->DropBall();
		if (weaponid != WEAPON_GAME) // exclude tee goal
			GameServer()->CreateSoundGlobal(SOUND_CTF_DROP);
		if (killer && killer->GetTeam() != victim->GetPlayer()->GetTeam())
			killer->m_Score++;
		mode |= 1;
	}

	return mode;
	
}

void CGameControllerFB::OnCharacterSpawn(class CCharacter *chr)
{
	
	// default health
	chr->m_Health = 10;
	
	if (strcmp (m_pGameType, "iFB") == 0)
	{
		// give instagib weapon
		chr->m_aWeapons[WEAPON_RIFLE].m_Got = 1;
		chr->m_aWeapons[WEAPON_RIFLE].m_Ammo = -1;
		chr->m_ActiveWeapon = WEAPON_RIFLE;
		chr->m_LastWeapon = WEAPON_RIFLE;
		chr->m_ReloadTimer = g_Config.m_SvfbSpawnWeaponDischarge * g_pData->m_Weapons.m_aId[WEAPON_RIFLE].m_Firedelay * Server()->TickSpeed() / (1000 * 100);
	}
	else
	{
		// give default weapons
		chr->m_aWeapons[WEAPON_HAMMER].m_Got = 1;
		chr->m_aWeapons[WEAPON_HAMMER].m_Ammo = -1;
		chr->m_aWeapons[WEAPON_GUN].m_Got = 1;
		chr->m_aWeapons[WEAPON_GUN].m_Ammo = 10;
	}
	
}

bool CGameControllerFB::HandleGoal (CBall *b, int goal)
{
	
	CPlayer *const p = b->m_pCarryingCharacter ? b->m_pCarryingCharacter->GetPlayer() : b->m_pLastCarrier;
	const int team = p ? p->GetTeam() : b->m_LastCarrierTeam;
	const bool own_goal = team == goal;
	const bool tee_goal = b->m_pCarryingCharacter;
	
	//	update scores
	if (own_goal)
	{
		if (!g_Config.m_SvfbOwngoal)
			return false;
		
		if (p)
		{
			p->m_Score -= g_Config.m_SvfbOwngoalPenalty;
			p->m_NumOwngoals++;
		
			// send warning to user
			char chat_msg[256];
			str_format (chat_msg, sizeof (chat_msg), "!!! Hey %s, you scored an own-goal! Put the ball into the OTHER goal !!!", Server()->ClientName(p->m_ClientID));
			GameServer()->SendChatTarget (p->m_ClientID, chat_msg);
		}
		// there's no sound like "wah...wah" ;)
		GameServer()->CreateSoundGlobal(SOUND_NINJA_HIT); //SOUND_PLAYER_PAIN_LONG
	}
	else
	{
		if (p)
			p->m_Score += tee_goal ? g_Config.m_SvfbScorePlayerTee : g_Config.m_SvfbScorePlayerBall;
			
		GameServer()->CreateSoundGlobal(SOUND_CTF_CAPTURE);
	}
	m_aTeamscore[goal ^ 1] += tee_goal ? g_Config.m_SvfbScoreTeamTee : g_Config.m_SvfbScoreTeamBall;
		
	// let the tee die if it jumped into goal... or when dumb own-goal
	if (p && (tee_goal || own_goal))
		p->KillCharacter (WEAPON_GAME);
	b->Reset();
	GameServer()->CreateSoundGlobal(SOUND_CTF_RETURN);
	
	// Cut off long nicknames... the message would jut out on screen!
	const int nick_length = 18;
	char nick[64] = {0};
	str_format (nick, sizeof (nick), "%s", p ? Server()->ClientName(p->m_ClientID) : "player left");
	if (str_length (nick) > nick_length)
	{
		nick[nick_length] = 0;
		str_append (nick, "...", sizeof (nick));
	}

	// Build broadcast-message
	char bc_msg[512] = {0};
	str_format (bc_msg, sizeof (bc_msg), "%s team (%s) scored with %s!", (goal ^ 1) == 0 ? "Red" : "Blue",
		nick, tee_goal ? "tee": "ball");

	// wired workaround for multiline broadcast-message center alignment
	// FIXME: can be done smarter!
	if (own_goal)
	{
		char line2[] = "\nPwned... that was an own-goal!";
		const int fillup = 2 * (str_length (bc_msg) - (str_length (line2) - 1));
		str_append (bc_msg, line2, sizeof (bc_msg));

		if (fillup > 0)
		{
			char *cur_pos = bc_msg + str_length (bc_msg);
			for (int l = 0; l < fillup; l++)
				*cur_pos++ = ' ';
			*cur_pos = 0;
		}
	}
	// send everyone the scoring-message
	GameServer()->SendBroadcast(bc_msg, -1);

	// kick unteachable player from server
	if (g_Config.m_SvfbOwngoalKick && p && p->m_NumOwngoals >= g_Config.m_SvfbOwngoalKick)
	{
		char chat_msg[256];
		str_format (chat_msg, sizeof (chat_msg), "%s was automatically kicked. Cause: Scored too many own-goals (%d).",
			Server()->ClientName(p->m_ClientID), g_Config.m_SvfbOwngoalKick);
			
		Server()->Kick(p->m_ClientID,
			"You scored too many own-goals! Please play the "
			"game as it is supposed to be. Thank you!");
		
		GameServer()->SendChat(-1, CGameContext::CHAT_ALL, chat_msg);
	}

	return true;
	
}

void CGameControllerFB::OnDisconnect (CPlayer *const p)
{
	CCharacter* ch = p->GetCharacter();
	for (int i = 0; i < FB_MAX_BALLS; ++i)
	{
		if (m_apBalls[i])
		{
			if (ch && m_apBalls[i]->m_pCarryingCharacter == ch)
				ch->ReleaseBall();
			if (m_apBalls[i]->m_pLastCarrier == p)
				m_apBalls[i]->m_pLastCarrier = NULL;
		}
	}
}

template <typename T> void swap(T& x, T& y)
{
	T z = x;
	x = y;
	y = z;
}

void CGameControllerFB::SetBallColor(CBall *&ball, int team)
{
	/* The flag color cannot be changed by mere assignment to m_Team. When
	 * someone carries a repainted flag, it causes lags due to the way flag
	 * position interpolation is implemented in CItems::RenderFlag in
	 * game/client/components/items.cpp. A hackish workaround is to swap
	 * the flags when the other color is requested, but we should take
	 * special care for the maps where both flags are available */

	if (!ball || ball->m_Team == team) return;
	CBall *b = m_apBalls[team];
	if (!b->m_AtStand) return;

	swap(b->m_StandPos, ball->m_StandPos);
	b->m_AtStand            = false;
	b->m_Pos                = ball->m_Pos;
	b->m_Vel                = ball->m_Vel;
	b->m_pCarryingCharacter = ball->m_pCarryingCharacter;
	b->m_pLastCarrier       = ball->m_pLastCarrier;
	b->m_LastCarrierTeam    = ball->m_LastCarrierTeam;
	b->m_DropTick           = ball->m_DropTick;
	b->m_GrabTick           = ball->m_GrabTick;
	b->m_IdleTick           = ball->m_IdleTick;

	ball->Reset();
	ball = b;
}

void CGameControllerFB::CreateBallGrabSound(int team)
{
	for (int c = 0; c < MAX_CLIENTS; ++c)
	{
		CPlayer *p = GameServer()->m_apPlayers[c];
		if (!p)
			continue;
		GameServer()->CreateSoundGlobal(p->GetTeam() == team ? SOUND_CTF_GRAB_PL : SOUND_CTF_GRAB_EN, p->GetCID());
	}
}

void CGameControllerFB::GrantBall(CCharacter *ch, CBall *b)
{
	int team = ch->GetPlayer()->GetTeam();
	if (b->m_AtStand)
	{
		m_aTeamscore[team] += g_Config.m_SvfbScoreTeamStand;
		ch->GetPlayer()->m_Score += g_Config.m_SvfbScorePlayerStand;
		b->m_AtStand = 0;
	}
	b->m_pCarryingCharacter = ch;
	b->m_Pos = ch->m_Pos;
	if (g_Config.m_SvfbLaserMomentum) 
		SetBallColor(b, team);
	ch->m_pBall = b;
	ch->BecomeNinja();
	CreateBallGrabSound(team);
}


void CGameControllerFB::Tick()
{
	
	IGameController::Tick();
	DoTeamScoreWincheck();

	if (m_GameOverTick == -1 && m_RoundStartTick != Server()->Tick())
	{
		
		for (int i = 0; i < MAX_CLIENTS; ++i)
		{
			if (!GameServer()->m_apPlayers[i]) continue;

			CCharacter *ch = GameServer()->m_apPlayers[i]->GetCharacter();
			if (!ch || !ch->IsAlive())
				continue;

			// Check for a goal-camper
			if (g_Config.m_SvfbGoalcamp)
			{
				bool in_range = false;
				for (int goal = 0; goal < m_NumGoals; ++goal)
				{
					if (distance (ch->m_Pos, m_apGoalPos[goal]) <
						g_Config.m_SvfbGoalsize * static_cast<float> (g_Config.m_SvfbGoalcampFactor / 10.0f))  //	1.5f
					{
						// if in range, count up 2 ticks
						ch->m_GoalCampTick += 2;
						in_range = true;
						break;
					}
				}
		
				if (!in_range)
				{
					// if out of range, count down just 1 tick again (slower)
					if (ch->m_GoalCampTick)
						ch->m_GoalCampTick--;
				}
				else
				{	
					//	kill goal camping player
					if ((ch->m_GoalCampTick >> 1) >= Server()->TickSpeed() * g_Config.m_SvfbGoalcampTime)
					{
						GameServer()->m_apPlayers[i]->KillCharacter(WEAPON_GAME);
				
						char chat_msg[128];
						str_format (chat_msg, sizeof (chat_msg), "%s was caught camping near a goal...",
							Server()->ClientName(GameServer()->m_apPlayers[i]->m_ClientID));
						GameServer()->SendChat(-1, -2, chat_msg);
						continue;
					}
				}
			}

			// anti-camping
			if (g_Config.m_SvfbCampMaxtime)
			{
				if (distance (ch->m_Pos, ch->m_CampPos) <= g_Config.m_SvfbCampThreshold && !ch->m_pBall)
				{
					// if in range, count up 2 ticks
					ch->m_CampTick += 2;
				}
				else
				{
					// if out of range, count down just 1 tick again (slower)
					if (ch->m_CampTick)
						ch->m_CampTick--;
					else
						ch->m_CampPos = ch->m_Pos;
				}
	
				//	kill camping player
				if ((ch->m_CampTick >> 1) >= Server()->TickSpeed () * g_Config.m_SvfbCampMaxtime)
				{
					GameServer()->m_apPlayers[i]->KillCharacter(WEAPON_GAME);
					GameServer()->SendChat(-1, -2, "I'm a bloody camper");
					continue;
				}
			}
		}

		//	update balls
		for (int bi =  0; bi < FB_MAX_BALLS; ++bi)
		{
			CBall *b = m_apBalls[bi];

			if (!b)
				continue;

			//	update position here and not in CBall::Tick because balls tend to tick before characters
			if (b->m_pCarryingCharacter)
				b->m_Pos = b->m_pCarryingCharacter->m_Pos;

			//	check/handle goal
			bool is_goal = false;
			for (int g = 0; g < m_NumGoals; g++)
			{
				if (distance(b->m_Pos, m_apGoalPos[g]) < g_Config.m_SvfbGoalsize)
				{
					is_goal = HandleGoal(b, g);
					break;
				}
			}
			if (is_goal)
				continue;

			if (b->m_pCarryingCharacter)
			{	//	ball is carried by player
				//	warn ball-carrying player once when he is near his own goal
				CCharacter *ch = b->m_pCarryingCharacter;
				CPlayer *p = ch->GetPlayer();
				int t = p->GetTeam();
				if (g_Config.m_SvfbOwngoalWarn && !p->m_OwngoalWarned && t < m_NumGoals && distance(ch->m_Pos, m_apGoalPos[t]) < g_Config.m_SvfbGoalsize * 5)
				{
					GameServer()->SendBroadcast(
						"Attention! This is your team's goal\n"
						"you are approaching. You probably\n"
						"want to go to the other side!",
						p->m_ClientID);

					// play the server-chat sound
					GameServer()->CreateSound(ch->m_Pos, SOUND_CHAT_SERVER, CmaskOne(p->m_ClientID));
					p->m_OwngoalWarned = true;
				}
			}
			else
			{	//	ball is carried by none
				//	small delay for throwing ball a bit away before fetching again
				if (b->m_AtStand || Server()->Tick() >= b->m_DropTick + Server()->TickSpeed() * 0.2f)
				{
					//	find all players that can grab the ball
					CCharacter *close_characters[MAX_CLIENTS];
					int max_num = GameServer()->m_World.FindEntities(b->m_Pos, b->ms_PhysSize, (CEntity**)close_characters, MAX_CLIENTS, CGameWorld::ENTTYPE_CHARACTER);
					int curr_num = 0;
					for (int i = 0; i < max_num; ++i)
					{
						if (!close_characters[i]->IsAlive() || close_characters[i]->GetPlayer()->GetTeam() == -1 || GameServer()->Collision()->IntersectLine(b->m_Pos, close_characters[i]->m_Pos, NULL, NULL) ||
							// don't take flag if already have one (might be useful with two flags)
							m_apBalls[bi ^ 1]->m_pCarryingCharacter == close_characters[i]) 
							continue;
						close_characters[curr_num++] = close_characters[i];
					}
					
					if (!curr_num)
						continue;

					GrantBall(close_characters[Server()->Tick() % curr_num], b);
				}
			}
		}
	}
}

void CGameControllerFB::Snap(int SnappingClient)
{
	
	IGameController::Snap(SnappingClient);

	CNetObj_GameData *pGameDataObj = (CNetObj_GameData *)Server()->SnapNewItem(NETOBJTYPE_GAMEDATA, 0, sizeof(CNetObj_GameData));
	if(!pGameDataObj)
		return;

	pGameDataObj->m_TeamscoreRed = m_aTeamscore[TEAM_RED];
	pGameDataObj->m_TeamscoreBlue = m_aTeamscore[TEAM_BLUE];
	
	if(m_apBalls[TEAM_RED])
	{
		if(m_apBalls[TEAM_RED]->m_AtStand)
		{
			pGameDataObj->m_FlagCarrierRed = FLAG_ATSTAND;
		}
		else if(m_apBalls[TEAM_RED]->m_pCarryingCharacter)
		{
			pGameDataObj->m_FlagCarrierRed = m_apBalls[TEAM_RED]->m_pCarryingCharacter->GetPlayer()->GetCID();
		}
		else
		{
			pGameDataObj->m_FlagCarrierRed = FLAG_TAKEN;
		}
	}
	else
	{
		pGameDataObj->m_FlagCarrierRed = FLAG_MISSING;	
	}
	
	if(m_apBalls[TEAM_BLUE])
	{
		if(m_apBalls[TEAM_BLUE]->m_AtStand)
		{
			pGameDataObj->m_FlagCarrierBlue = FLAG_ATSTAND;
		}
		else if(m_apBalls[TEAM_BLUE]->m_pCarryingCharacter)
		{
			pGameDataObj->m_FlagCarrierBlue = m_apBalls[TEAM_BLUE]->m_pCarryingCharacter->GetPlayer()->GetCID();
		}
		else
		{
			pGameDataObj->m_FlagCarrierBlue = FLAG_TAKEN;
		}
	}
	else
	{
		pGameDataObj->m_FlagCarrierBlue = FLAG_MISSING;
	}
}

bool CGameControllerFB::CanSpawn(int Team, vec2 *pOutPos)
{
	CSpawnEval Eval;
	
	// spectators can't spawn
	if(Team == TEAM_SPECTATORS)
		return false;
	
	Eval.m_FriendlyTeam = Team;
	
	// try own team only
	EvaluateSpawnType(&Eval, 1+(Team&1));

	// try harder
	if(!Eval.m_Got)
		FindFreeSpawn(&Eval, 1+(Team&1));
	
	*pOutPos = Eval.m_Pos;
	return Eval.m_Got;
}
