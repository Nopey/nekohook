/*
 * hooks.h
 *
 *  Created on: Jan 8, 2017
 *      Author: nullifiedcat
 */

#pragma once
//Source SDK
#include <platform.h>
#include <basehandle.h>
#include <engine/ivmodelrender.h>
#include <inputsystem/ButtonCode.h>
#include <mathlib.h>
#include <game/shared/shareddefs.h>
#include <game/shared/usercmd.h>
//#include <game/client/iinput.h>
#include <inetmessage.h>
#include <icliententity.h>
#include <igameevents.h>
#include <view_shared.h>
#include <igameevents.h>
#include <ienginevgui.h>
#include <steam/steamclientpublic.h>
#include <inetmsghandler.h>
#include <cdll_int.h>

#include "../interfaces.hpp"

typedef bool (*CreateMove_t)(void *, float, CUserCmd *);
typedef void (*PaintTraverse_t)(void *, unsigned int, bool, bool);
typedef bool (*CanPacket_t)(void *);
typedef int (*IN_KeyEvent_t)(void *, int, int, const char *);
typedef bool (*SendNetMsg_t)(void *, INetMessage &, bool, bool);
typedef void (*Shutdown_t)(void *, const char *);
typedef void (*OverrideView_t)(void *, CViewSetup *);
typedef bool (*DispatchUserMessage_t)(void *, int, bf_read &);
typedef void (*FrameStageNotify_t)(void *, int);
typedef void (*LevelInit_t)(void *, const char *);
typedef void (*LevelShutdown_t)(void *);
typedef void (*BeginFrame_t)(IStudioRender *);
typedef bool (*CanInspect_t)(IClientEntity *);
typedef void (*DrawModelExecute_t)(IVModelRender *, const DrawModelState_t &,
                                   const ModelRenderInfo_t &, matrix3x4_t *);
typedef CUserCmd *(*GetUserCmd_t)(IInput *, int);
typedef const char *(*GetClientName_t)(CBaseClientState *);
typedef bool (*ProcessSetConVar_t)(CBaseClientState *, NET_SetConVar *);
typedef bool (*ProcessGetCvarValue_t)(CBaseClientState *, SVC_GetCvarValue *);
typedef void (*Paint_t)(IEngineVGui *, PaintMode_t);

const char *GetClientName_hook(CBaseClientState *_this);
bool ProcessSetConVar_hook(CBaseClientState *_this, NET_SetConVar *msg);
bool ProcessGetCvarValue_hook(CBaseClientState *_this, SVC_GetCvarValue *msg);
// typedef void(*CInput__CreateMove_t)(void*, int, float, bool);
// void CInput__CreateMove_hook(void*, int sequence_number, float
// input_sample_frametime, bool active);
typedef const char *(*GetFriendPersonaName_t)(ISteamFriends *, CSteamID);
const char *GetFriendPersonaName_hook(ISteamFriends *_this, CSteamID steamID);

typedef void (*FireGameEvent_t)(void *_this, IGameEvent *event);
void FireGameEvent_hook(void *_this, IGameEvent *event);

CUserCmd *GetUserCmd_hook(IInput *, int);
void DrawModelExecute_hook(IVModelRender *_this, const DrawModelState_t &state,
                           const ModelRenderInfo_t &info, matrix3x4_t *matrix);

void Paint_hook(IEngineVGui *_this, PaintMode_t mode);
