#pragma once
#include <iostream>
#include <set>
#include <vector>
#include "../../Cuore/Util/Structs.hpp"
#include "../../Utilities/InputTypes.hpp"

namespace Modules {
    namespace Aimbot{
        //Aimbot
        bool Enabled = true;
        bool PredictMovement = true;
        bool PredictBulletDrop = true;
        bool AllowTargetSwitch = false;
        InputKeyType aimHotkey = InputKeyType::MOUSE_X2;
        float FinalDistance = 0;
        float Speed = 70;
        float Smooth = 25;
        float ExtraSmooth = 3000;
        float FOV = 7;
        float MinDistance = 1;
        float MaxDistance = 100;
        HitboxType Hitbox = HitboxType::UpperChest;
    } 

    namespace NoRecoil{
        bool Enabled = true;
        float PitchPower = 30;
        float YawPower = 30;
    }

    namespace ESP {
        bool Enabled = true;
        float GlowMaxDistance = 200;
        bool ItemGlow = true;
        bool DrawSeer = true;
        bool DrawTracers = false;
        bool DrawDistance = true;
        bool DrawNames = true;
        bool DrawFOVCircle = false;
        bool DrawRainbowCircle = true;
        bool ShowSpectators = true;
        bool printPlayersLvl = true;
        bool AimedAtOnly = true;
        bool DrawBox = true;
        float yOffset = -85.0f; 
        float xOffset = -30.0f;
        float yyOffset = 5.0f; 
        float xxOffset = 15.0f;
        float SeerMaxDistance = 200;
        float GameFOV = 120;
    } 

    namespace Radar {
        bool miniRadar = true;
        bool bigRadar = true;
        bool MiniMapGuides = true;
        float miniMapRange = 100;
        int minimapradardotsize1 = 5;
        int minimapradardotsize2 = 5;
        InputKeyType bigMapHotKey = InputKeyType::KEYBOARD_M;
    }

    namespace TriggerBot{
        bool Enabled = true;
        float TriggerbotRange = 150;
        float HipFireRange = 30;
        std::set<int> WeaponList = { 1, 84, 95, 86, 102, 94, 104, 88, 110, 89, 112, 106, 108, 91 };
    }

    namespace GameMode{
        bool threeManSquad = true;
    }
    
};

