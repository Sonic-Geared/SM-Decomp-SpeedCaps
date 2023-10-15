#include "../GameAPI/C/GameAPI/Game.h"

#include "Objects/Player.h"

#if RETRO_USE_MOD_LOADER
DLLExport bool32 LinkModLogic(EngineInfo *info, const char *id);
#endif

void InitModAPI(void) {
    Player_HandleGroundMovement                 = Mod.GetPublicFunction(NULL, "Player_HandleGroundMovement");
    Player_HandleAirFriction            = Mod.GetPublicFunction(NULL, "Player_HandleAirFriction");

    Mod.RegisterStateHook(Player_HandleGroundMovement, Player_HandleGroundMovement_Hook, true);
    Mod.RegisterStateHook(Player_HandleAirFriction, Player_HandleAirFriction_Hook, true);

    MOD_REGISTER_OBJECT_HOOK(Player);
}
	

#if RETRO_USE_MOD_LOADER
#define ADD_PUBLIC_FUNC(func) Mod.AddPublicFunction(#func, (void *)(func))

void InitModAPI(void);

bool32 LinkModLogic(EngineInfo *info, const char *id)
{
#if MANIA_USE_PLUS
    LinkGameLogicDLL(info);
#else
    LinkGameLogicDLL(*info);
#endif

    globals = Mod.GetGlobals();

    modID = id;

    InitModAPI();

    return true;
}
#endif
