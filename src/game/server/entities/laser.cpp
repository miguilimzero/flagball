/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include "laser.h"
#include <game/server/gamemodes/fb.h>
#include <engine/shared/config.h>

CLaser::CLaser(CGameWorld *pGameWorld, vec2 Pos, vec2 Direction, float StartEnergy, int Owner)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_LASER)
{
	m_Pos = Pos;
	m_Owner = Owner;
	m_Energy = StartEnergy;
	m_Dir = Direction;
	m_Bounces = 0;
	m_EvalTick = 0;
	GameWorld()->InsertEntity(this);
	DoBounce();
}


bool CLaser::HitCharacter(vec2 From, vec2 To)
{
	vec2 At;
	CCharacter *OwnerChar = GameServer()->GetPlayerChar(m_Owner);
	CCharacter *Hit = GameServer()->m_World.IntersectCharacter(m_Pos, To, 0.f, At, OwnerChar);
	if(!Hit)
		return false;

	m_From = From;
	m_Pos = At;
	hit_ball(From, m_Pos);
	m_Energy = -1;		
	Hit->TakeDamage(vec2(0.f, 0.f), GameServer()->Tuning()->m_LaserDamage, m_Owner, WEAPON_RIFLE);
	return true;
}

void CLaser::hit_ball(vec2 from, vec2 to) {
 	if (g_Config.m_SvfbLaserMomentum == 0) return;
 	for (int i = 0; i < FB_MAX_BALLS; ++i)
 	{
 		CBall *ball = static_cast<CGameControllerFB *>(GameServer()->m_pController)->m_apBalls[i];
 		if (!ball || ball->m_pCarryingCharacter) continue;
 		vec2 ball_pos = ball->m_Pos + vec2(g_Config.m_SvfbBallOffsetX, -g_Config.m_SvfbBallOffsetY);
 		if (distance(ball_pos, closest_point_on_line(from, to, ball_pos)) < g_Config.m_SvfbBallRadius)
 		{
 			ball->m_Vel += normalize(to - from) * g_Config.m_SvfbLaserMomentum * 0.1f;
 			ball->m_DropTick = Server()->Tick();
 			ball->m_pLastCarrier = GameServer()->m_apPlayers[m_Owner];
 			ball->m_LastCarrierTeam = GameServer()->m_apPlayers[m_Owner]->GetTeam();
                        static_cast<CGameControllerFB *>(GameServer()->m_pController)->SetBallColor(ball, ball->m_LastCarrierTeam);
 		}
 	};
}; 

void CLaser::DoBounce()
{
	m_EvalTick = Server()->Tick();
	
	if(m_Energy < 0)
	{
		GameServer()->m_World.DestroyEntity(this);
		return;
	}
	
	vec2 To = m_Pos + m_Dir * m_Energy;
	vec2 OrgTo = To;
	
	if(GameServer()->Collision()->IntersectLine(m_Pos, To, 0x0, &To))
	{
		if(!HitCharacter(m_Pos, To))
		{
			hit_ball(m_Pos, To);
			// intersected
			m_From = m_Pos;
			m_Pos = To;

			vec2 TempPos = m_Pos;
			vec2 TempDir = m_Dir * 4.0f;
			
			GameServer()->Collision()->MovePoint(&TempPos, &TempDir, 1.0f, 0);
			m_Pos = TempPos;
			m_Dir = normalize(TempDir);
			
			m_Energy -= distance(m_From, m_Pos) + GameServer()->Tuning()->m_LaserBounceCost;
			m_Bounces++;
			
			if(m_Bounces > GameServer()->Tuning()->m_LaserBounceNum)
				m_Energy = -1;
				
			GameServer()->CreateSound(m_Pos, SOUND_RIFLE_BOUNCE);
		}
	}
	else
	{
		if(!HitCharacter(m_Pos, To))
		{
			hit_ball(m_Pos, To);
			m_From = m_Pos;
			m_Pos = To;
			m_Energy = -1;
		}
	}
}
	
void CLaser::Reset()
{
	GameServer()->m_World.DestroyEntity(this);
}

void CLaser::Tick()
{
	if(Server()->Tick() > m_EvalTick+(Server()->TickSpeed()*GameServer()->Tuning()->m_LaserBounceDelay)/1000.0f)
		DoBounce();
}

void CLaser::Snap(int SnappingClient)
{
	if(NetworkClipped(SnappingClient))
		return;

	CNetObj_Laser *pObj = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_ID, sizeof(CNetObj_Laser)));
	if(!pObj)
		return;

	pObj->m_X = (int)m_Pos.x;
	pObj->m_Y = (int)m_Pos.y;
	pObj->m_FromX = (int)m_From.x;
	pObj->m_FromY = (int)m_From.y;
	pObj->m_StartTick = m_EvalTick;
}
