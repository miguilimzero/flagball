/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_VARIABLES_H
#define GAME_VARIABLES_H
#undef GAME_VARIABLES_H // this file will be included several times


// client
MACRO_CONFIG_INT(ClPredict, cl_predict, 1, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Predict client movements")
MACRO_CONFIG_INT(ClNameplates, cl_nameplates, 1, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Show name plates")
MACRO_CONFIG_INT(ClNameplatesAlways, cl_nameplates_always, 1, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Always show name plates disregarding of distance")
MACRO_CONFIG_INT(ClNameplatesTeamcolors, cl_nameplates_teamcolors, 1, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Use team colors for name plates")
MACRO_CONFIG_INT(ClNameplatesSize, cl_nameplates_size, 50, 0, 100, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Size of the name plates from 0 to 100%")
MACRO_CONFIG_INT(ClAutoswitchWeapons, cl_autoswitch_weapons, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Auto switch weapon on pickup")

MACRO_CONFIG_INT(ClShowhud, cl_showhud, 1, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Show ingame HUD")
MACRO_CONFIG_INT(ClShowfps, cl_showfps, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Show ingame FPS counter")

MACRO_CONFIG_INT(ClAirjumpindicator, cl_airjumpindicator, 1, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")
MACRO_CONFIG_INT(ClThreadsoundloading, cl_threadsoundloading, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Load sound files threaded")

MACRO_CONFIG_INT(ClWarningTeambalance, cl_warning_teambalance, 1, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Warn about team balance")

MACRO_CONFIG_INT(ClMouseDeadzone, cl_mouse_deadzone, 300, 0, 0, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")
MACRO_CONFIG_INT(ClMouseFollowfactor, cl_mouse_followfactor, 60, 0, 200, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")
MACRO_CONFIG_INT(ClMouseMaxDistance, cl_mouse_max_distance, 800, 0, 0, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")

MACRO_CONFIG_INT(EdShowkeys, ed_showkeys, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")

//MACRO_CONFIG_INT(ClFlow, cl_flow, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")

MACRO_CONFIG_INT(ClShowWelcome, cl_show_welcome, 1, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")
MACRO_CONFIG_INT(ClMotdTime, cl_motd_time, 10, 0, 100, CFGFLAG_CLIENT|CFGFLAG_SAVE, "How long to show the server message of the day")

MACRO_CONFIG_STR(ClVersionServer, cl_version_server, 100, "version.teeworlds.com", CFGFLAG_CLIENT|CFGFLAG_SAVE, "Server to use to check for new versions")

MACRO_CONFIG_STR(ClLanguagefile, cl_languagefile, 255, "", CFGFLAG_CLIENT|CFGFLAG_SAVE, "What language file to use")

MACRO_CONFIG_INT(PlayerUseCustomColor, player_use_custom_color, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Toggles usage of custom colors")
MACRO_CONFIG_INT(PlayerColorBody, player_color_body, 65408, 0, 0xFFFFFF, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Player body color")
MACRO_CONFIG_INT(PlayerColorFeet, player_color_feet, 65408, 0, 0xFFFFFF, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Player feet color")
MACRO_CONFIG_STR(PlayerSkin, player_skin, 24, "default", CFGFLAG_CLIENT|CFGFLAG_SAVE, "Player skin")

MACRO_CONFIG_INT(UiPage, ui_page, 5, 0, 9, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Interface page")
MACRO_CONFIG_INT(UiToolboxPage, ui_toolbox_page, 0, 0, 2, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Toolbox page")
MACRO_CONFIG_STR(UiServerAddress, ui_server_address, 64, "localhost:8303", CFGFLAG_CLIENT|CFGFLAG_SAVE, "Interface server address")
MACRO_CONFIG_INT(UiScale, ui_scale, 100, 50, 150, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Interface scale")

MACRO_CONFIG_INT(UiColorHue, ui_color_hue, 160, 0, 255, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Interface color hue")
MACRO_CONFIG_INT(UiColorSat, ui_color_sat, 70, 0, 255, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Interface color saturation")
MACRO_CONFIG_INT(UiColorLht, ui_color_lht, 175, 0, 255, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Interface color lightness")
MACRO_CONFIG_INT(UiColorAlpha, ui_color_alpha, 228, 0, 255, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Interface alpha")

MACRO_CONFIG_INT(GfxNoclip, gfx_noclip, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Disable clipping")

// server
MACRO_CONFIG_INT(SvWarmup, sv_warmup, 0, 0, 0, CFGFLAG_SERVER, "Number of seconds to do warmup before round starts")
MACRO_CONFIG_STR(SvMotd, sv_motd, 900, "", CFGFLAG_SERVER, "Message of the day to display for the clients")
MACRO_CONFIG_INT(SvTeamdamage, sv_teamdamage, 0, 0, 1, CFGFLAG_SERVER, "Team damage")
MACRO_CONFIG_STR(SvMaprotation, sv_maprotation, 768, "", CFGFLAG_SERVER, "Maps to rotate between")
MACRO_CONFIG_INT(SvRoundsPerMap, sv_rounds_per_map, 1, 1, 100, CFGFLAG_SERVER, "Number of rounds on each map before rotating")
MACRO_CONFIG_INT(SvPowerups, sv_powerups, 1, 0, 1, CFGFLAG_SERVER, "Allow powerups like ninja")
MACRO_CONFIG_INT(SvScorelimit, sv_scorelimit, 20, 0, 1000, CFGFLAG_SERVER, "Score limit")
MACRO_CONFIG_INT(SvTimelimit, sv_timelimit, 0, 0, 1000, CFGFLAG_SERVER, "Time limit in minutes")
MACRO_CONFIG_STR(SvGametype, sv_gametype, 32, "fb", CFGFLAG_SERVER, "Game type (dm, tdm, ctf, fb, ifb)")
MACRO_CONFIG_INT(SvTournamentMode, sv_tournament_mode, 0, 0, 1, CFGFLAG_SERVER, "Tournament mode. When enabled, players joins the server as spectator")
MACRO_CONFIG_INT(SvSpamprotection, sv_spamprotection, 1, 0, 1, CFGFLAG_SERVER, "Spam protection")

MACRO_CONFIG_INT(SvRespawnDelayTDM, sv_respawn_delay_tdm, 3, 0, 10, CFGFLAG_SERVER, "Time needed to respawn after death in tdm gametype")

MACRO_CONFIG_INT(SvSpectatorSlots, sv_spectator_slots, 0, 0, MAX_CLIENTS, CFGFLAG_SERVER, "Number of slots to reserve for spectators")
MACRO_CONFIG_INT(SvTeambalanceTime, sv_teambalance_time, 1, 0, 1000, CFGFLAG_SERVER, "How many minutes to wait before autobalancing teams")
MACRO_CONFIG_INT(SvInactiveKickTime, sv_inactivekick_time, 3, 0, 1000, CFGFLAG_SERVER, "How many minutes to wait before taking care of inactive players")
MACRO_CONFIG_INT(SvInactiveKick, sv_inactivekick, 1, 0, 2, CFGFLAG_SERVER, "How to deal with inactive players (0=move to spectator, 1=move to free spectator slot/kick, 2=kick)")

MACRO_CONFIG_INT(SvVoteSpectate, sv_vote_spectate, 1, 0, 1, CFGFLAG_SERVER, "Allow voting to move players to spectators")
MACRO_CONFIG_INT(SvVoteKick, sv_vote_kick, 1, 0, 1, CFGFLAG_SERVER, "Allow voting to kick players")
MACRO_CONFIG_INT(SvVoteKickMin, sv_vote_kick_min, 0, 0, MAX_CLIENTS, CFGFLAG_SERVER, "Minimum number of players required to start a kick vote")
MACRO_CONFIG_INT(SvVoteKickBantime, sv_vote_kick_bantime, 5, 0, 1440, CFGFLAG_SERVER, "The time to ban a player if kicked by vote. 0 makes it just use kick")

/*	flagball	*/
MACRO_CONFIG_INT(SvfbBallDamageLose, sv_fb_ball_damage_lose, 1, 0, 1, CFGFLAG_SERVER, "Lose ball on any damage")
MACRO_CONFIG_INT(SvfbBallResetTime, sv_fb_ball_reset_time, 30, 1, 500, CFGFLAG_SERVER, "Number of seconds needed to reset a lying ball")
MACRO_CONFIG_INT(SvfbBallVelocity, sv_fb_ball_velocity, 207, 10, 3000, CFGFLAG_SERVER, "Velocity of the ball after it got shot")
MACRO_CONFIG_INT(SvfbCampMaxtime, sv_fb_camp_maxtime, 0, 0, 100, CFGFLAG_SERVER, "Number of seconds allowed lingering at the same place")
MACRO_CONFIG_INT(SvfbCampThreshold, sv_fb_camp_threshold, 200, 64, 1000, CFGFLAG_SERVER, "Threshold for anti-camp distance")
MACRO_CONFIG_INT(SvfbDirectPass, sv_fb_direct_pass, 1, 0, 1, CFGFLAG_SERVER, "Let players pass ball with hook")
MACRO_CONFIG_INT(SvfbBallOffsetX, sv_fb_ball_offset_x, 0, 0, 1000, CFGFLAG_SERVER, "X offset of the hookable center of the ball with respect to the lower left corner of the image")
MACRO_CONFIG_INT(SvfbBallOffsetY, sv_fb_ball_offset_y, 38, 0, 1000, CFGFLAG_SERVER, "Y offset of the hookable center of the ball with respect to the lower left corner of the image")
MACRO_CONFIG_INT(SvfbBallRadius, sv_fb_ball_radius, 40, 0, 1000, CFGFLAG_SERVER, "Flag hookable radius")
MACRO_CONFIG_INT(SvfbLaserMomentum, sv_fb_laser_momentum, 207, -3000, 3000, CFGFLAG_SERVER, "Momentum passed to a flag when it is hit by laser")
MACRO_CONFIG_INT(SvfbGoalsize, sv_fb_goalsize, 64, 32, 500, CFGFLAG_SERVER, "Radius defining goal")
MACRO_CONFIG_INT(SvfbGoalcamp, sv_fb_goalcamp, 1, 0, 1, CFGFLAG_SERVER, "Activate goal camp protection")
MACRO_CONFIG_INT(SvfbGoalcampTime, sv_fb_goalcamp_time, 3, 1, 600, CFGFLAG_SERVER, "Number of seconds allowed lingering near a goal")
MACRO_CONFIG_INT(SvfbGoalcampFactor, sv_fb_goalcamp_factor, 40, 10, 100, CFGFLAG_SERVER, "Goal camp protection radius")
MACRO_CONFIG_INT(SvfbHookBall, sv_fb_hook_ball, 1, 0, 1, CFGFLAG_SERVER, "Make the ball hookable")
MACRO_CONFIG_INT(SvfbInstagib, sv_fb_instagib, 1, 0, 1, CFGFLAG_SERVER, "Activate instagib")
MACRO_CONFIG_INT(SvfbOwngoal, sv_fb_owngoal, 0, 0, 1, CFGFLAG_SERVER, "Allow own goals")
MACRO_CONFIG_INT(SvfbOwngoalKick, sv_fb_owngoal_kick, 2, 0, 20, CFGFLAG_SERVER, "Number of own goals allowed before kicking the player")
MACRO_CONFIG_INT(SvfbOwngoalPenalty, sv_fb_owngoal_penalty, 30, 0, 999, CFGFLAG_SERVER, "Negative score for own goal player")
MACRO_CONFIG_INT(SvfbOwngoalWarn, sv_fb_owngoal_warn, 0, 0, 1, CFGFLAG_SERVER, "Warn when player with ball is near own goal")
MACRO_CONFIG_INT(SvfbScorePlayerBall, sv_fb_score_player_ball, 5, 0, 100, CFGFLAG_SERVER, "Player score for ball goal")
MACRO_CONFIG_INT(SvfbScorePlayerStand, sv_fb_score_player_stand, 0, 0, 10, CFGFLAG_SERVER, "Player score for taking ball from stand")
MACRO_CONFIG_INT(SvfbScorePlayerTee, sv_fb_score_player_tee, 10, 0, 100, CFGFLAG_SERVER, "Player score for tee goal")
MACRO_CONFIG_INT(SvfbScoreTeamBall, sv_fb_score_team_ball, 50, 1, 100, CFGFLAG_SERVER, "Team score for ball goal")
MACRO_CONFIG_INT(SvfbScoreTeamStand, sv_fb_score_team_stand, 0, 0, 10, CFGFLAG_SERVER, "Team score for taking ball from stand")
MACRO_CONFIG_INT(SvfbScoreTeamTee, sv_fb_score_team_tee, 100, 1, 100, CFGFLAG_SERVER, "Team score for tee goal")
MACRO_CONFIG_STR(SvfbWelcomeMessage, sv_fb_welcome_message, 512, "", CFGFLAG_SERVER, "Message to display when clients join")
MACRO_CONFIG_INT(SvfbSelfKill, sv_fb_self_kill, 0, 0, 1, CFGFLAG_SERVER, "Allow self kill")
MACRO_CONFIG_INT(SvfbSpawnWeaponDischarge, sv_fb_spawn_weapon_discharge, 50, 0, 100, CFGFLAG_SERVER, "Amount of time players cannot fire after respawn (in percents of laser reloading time)")

// debug
#ifdef CONF_DEBUG // this one can crash the server if not used correctly
	MACRO_CONFIG_INT(DbgDummies, dbg_dummies, 0, 0, 15, CFGFLAG_SERVER, "")
#endif

MACRO_CONFIG_INT(DbgFocus, dbg_focus, 0, 0, 1, CFGFLAG_CLIENT, "")
MACRO_CONFIG_INT(DbgTuning, dbg_tuning, 0, 0, 1, CFGFLAG_CLIENT, "")
#endif
