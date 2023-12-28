#pragma once
#include <iostream>
#include <vector>
#include <iostream>
#include <vector>

#include "./Aim.hpp"
#include "../Cuore/Entities/LocalPlayer.hpp"
#include "../Cuore/Entities/Players.hpp"

#include "../Cuore/Util/MyDisplay.hpp"
#include "../Cuore/Util/Structs.hpp"
#include "../Cuore/Util/Offsets.hpp"
#include "../Cuore/Util/QAngle.hpp"
#include "../Cuore/Util/Resolver.hpp"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../Utilities/Config.hpp"

#include "../../Cuore/Entities/LocalPlayer.hpp"
#include "../../Cuore/Util/MyDisplay.hpp"
#include "../../Cuore/Util/Structs.hpp"
#include "../../Cuore/Util/Offsets.hpp"
#include "../../Cuore/Util/QAngle.hpp"
#include "../../Cuore/Util/Resolver.hpp"
#include "../../Cuore/Overlay/Rendering/Overlay.hpp"
#include "../../Cuore/Overlay/Rendering/Drawings.hpp"

// Geometry
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI / 180.f) )

struct Sense {
    MyDisplay* X11Display;
    Camera* GameCamera;
    Level* m_level;
    LocalPlayer* Myself;
    std::vector<Player*>* Players;
    std::chrono::milliseconds LastUpdateTime;
 

    Sense(MyDisplay* X11Display, std::vector<Player*>* Players, Camera* GameCamera, Level* level, LocalPlayer* Myself) {
        this->X11Display = X11Display;
        this->Players = Players;
        this->GameCamera = GameCamera;
        this->m_level = level;
        this->Myself = Myself;
    }
    void RenderDrawings(ImDrawList* Canvas, Aimbot* AimAssistState, LocalPlayer* Myself, Overlay OverlayWindow) {
        int ScreenWidth;
        int ScreenHeight;
        OverlayWindow.GetScreenResolution(ScreenWidth, ScreenHeight);
        
        // Draw Spectators
        if (Modules::ESP::ShowSpectators) {
            ImVec2 Center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(ImVec2(0.0f, Center.y), ImGuiCond_Once, ImVec2(0.02f, 0.5f));
            ImGui::SetNextWindowBgAlpha(0.3f);
            ImGui::Begin("Spectators", nullptr, ImGuiWindowFlags_AlwaysAutoResize | 
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoSavedSettings | 
                ImGuiWindowFlags_NoMove | 
                ImGuiWindowFlags_NoInputs | 
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoScrollbar);
            int spectatorcount = 0;
            std::vector<std::string> TempSpectators;
            std::vector<std::string> spectatorlist;
            for (int i = 0; i < Players->size(); i++) {
                Player* p = Players->at(i);
                float targetyaw = p->ViewYaw; // get Yaw player
                float localyaw= Myself->ViewYaw; // get Yaw LocalPlayer
                //printf("LocalViewYaw: %f == PlayerViewYaw: %f \n", localyaw, targetyaw);
                if (targetyaw == localyaw && p->Health == 0){
                    spectatorcount++;
                    
                    std::string namePlayer = p->GetPlayerName();    
                    spectatorlist.push_back(namePlayer);
                }            
            }
            const auto spectatorlist_size = static_cast<int>(TempSpectators.size());

            ImGui::Text("Spectators: ");
            ImGui::SameLine(); ImGui::TextColored(spectatorcount > 0 ? ImVec4(1, 0.343, 0.475, 1) : ImVec4(0.4, 1, 0.343, 1), "%d", spectatorcount);
            if (static_cast<int>(spectatorlist.size()) > 0) {
                ImGui::Separator();
                for (int i = 0; i < static_cast<int>(spectatorlist.size()); i++) {
                    ImGui::TextColored(ImVec4(1, 0.343, 0.475, 1), "> %s", spectatorlist.at(i).c_str());
                }
            }
            ImGui::End();
        }

        //====================---------[PRINTLEVELS]-------------------------==================

        if(Modules::ESP::printPlayersLvl){
        
            ImVec2 Center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(ImVec2(0.0f, Center.y), ImGuiCond_Once, ImVec2(0.02f, 0.5f));
            ImGui::SetNextWindowBgAlpha(0.3f);
            ImGui::Begin("Players", nullptr, ImGuiWindowFlags_AlwaysAutoResize | 
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoSavedSettings | 
                ImGuiWindowFlags_NoMove | 
                ImGuiWindowFlags_NoInputs | 
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoScrollbar);
            int playerLvl = 0;
            int playerCount = 0;
            std::string namePlayer;
            std::string modelName;
            for (auto i = 0; i < Players->size(); i++)
            {
                Player *p = Players->at(i);
                if(!p->IsDead && p->IsPlayer()){
                    playerLvl = p->GetPlayerLevel();
                    namePlayer = p->GetPlayerName();
                    modelName = p->getPlayerModelName();
                    playerCount ++;
                    ImGui::Separator();
                    ImGui::TextColored(ImVec4(1, 0, 0, 1), " [%i]", (i + 1));
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(0, 1, 0, 1), ">   [%s]", namePlayer.c_str());
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(0, 1, 1, 1), ">    [%s]", modelName.c_str());
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(1, 0.5, 0, 1), ">   [%i]", playerLvl);
                }
            } 
            ImGui::End();           
                   
        }

        // Draw White FOV Circle
        if (Modules::ESP::DrawFOVCircle && Myself->IsCombatReady()) {
            float FOV = Modules::Aimbot::FOV/.89f;
            if(Myself->IsZooming){FOV += 3.50f;}
            float Radius = tanf(DEG2RAD(FOV) / 2) / tanf(DEG2RAD(120) / 2) * ScreenWidth;
            Drawings::DrawCircle(Canvas, Vector2D(ScreenWidth / 2, ScreenHeight / 2), Radius, 40, ImColor(255, 255, 255), 2);
        }

        // Draw Rainbow FOV Circle
        if (Modules::ESP::DrawRainbowCircle && Myself->IsCombatReady()) {
            float FOV = Modules::Aimbot::FOV/.89f;
            if(Myself->IsZooming){FOV += 3.50f;}
            float Radius = tanf(DEG2RAD(FOV) / 2) / tanf(DEG2RAD(120) / 2) * ScreenWidth;
            float radius = Radius;
            Drawings::DrawDynamicRainbowCircle(Canvas, Vector2D(ScreenWidth / 2, ScreenHeight / 2), radius);
        }

        // Draw lot of things
        for (int i = 0; i < Players->size(); i++) {
            Player* p = Players->at(i);
            if (!p->IsCombatReady() || !p->IsVisible || !p->IsHostile || p->DistanceToLocalPlayer > (Conversion::ToGameUnits(Modules::ESP::SeerMaxDistance)) || Myself->BasePointer == p->BasePointer) continue;

            // Tracer
            if (Modules::ESP::DrawTracers) {
                Vector2D chestScreenPosition;
                GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::UpperChest), chestScreenPosition);
                if (!chestScreenPosition.IsZeroVector()) {
                    int x = (int)(ScreenWidth * 0.5f);
                    Drawings::DrawLine(Canvas, Vector2D(x, ScreenHeight), chestScreenPosition, 1.5f, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    Drawings::DrawCircleFilled(Canvas, chestScreenPosition, 2, 10, ImColor(255, 255, 255));
                }
            }
            // Distance
            if (Modules::ESP::DrawDistance) {
                Vector2D originScreenPosition;
                GameCamera->WorldToScreen(p->LocalOrigin.Add(Vector3D(0, 15, 0)), originScreenPosition);
                if (!originScreenPosition.IsZeroVector()) {
                    Drawings::DrawText(Canvas, originScreenPosition.Add(Vector2D(5, 0)), std::to_string((int)Conversion::ToMeters(p->DistanceToLocalPlayer)).c_str(), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), true, true, false);
                }
            }
            
            // Draw Name
            if (Modules::ESP::DrawNames && !p->IsDummy()) {
                Vector2D originScreenPosition;
                GameCamera->WorldToScreen(p->LocalOrigin.Add(Vector3D(0, 15, 0)), originScreenPosition);
                if (!originScreenPosition.IsZeroVector()) {
                    Drawings::DrawText(Canvas, originScreenPosition.Add(Vector2D(5, -70)), p->GetPlayerName().c_str(), ImVec4(0.3, 1, 0.64, 1), true, true, true);
                }
            }
            // Draw Rectangle On Locked on Target
            if (AimAssistState->TargetSelected && AimAssistState->CurrentTarget && Modules::ESP::DrawBox && Modules::ESP::AimedAtOnly)
	    	{      		
		        Vector2D headScreenPosition, chestScreenPosition;
		        GameCamera->WorldToScreen(AimAssistState->CurrentTarget->GetBonePosition(HitboxType::Head), headScreenPosition);
		        GameCamera->WorldToScreen(AimAssistState->CurrentTarget->GetBonePosition(HitboxType::UpperChest), chestScreenPosition);
		        chestScreenPosition.y -=Modules::ESP::yOffset;
		        chestScreenPosition.x -=Modules::ESP::xOffset;
		        headScreenPosition.y-=Modules::ESP::yyOffset;
		        headScreenPosition.x-=Modules::ESP::xxOffset;
		        if (p->IsLockedOn) 
		        {
		        	Drawings::DrawBox(Canvas, chestScreenPosition, headScreenPosition, ImColor(255,0,0), 2);
		        } else if (!p->IsLockedOn)  Drawings::DrawBox(Canvas, chestScreenPosition, headScreenPosition, ImColor(255, 255, 255), 2);
            }
            // Draw Rectangle On All Targets
            if (Modules::ESP::DrawBox && !Modules::ESP::AimedAtOnly)
	    	{       		
		        Vector2D headScreenPosition, chestScreenPosition;
		        GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Head), headScreenPosition);
		        GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::UpperChest), chestScreenPosition);
		        chestScreenPosition.y -=Modules::ESP::yOffset;
		        chestScreenPosition.x -=Modules::ESP::xOffset;
		        headScreenPosition.y-=Modules::ESP::yyOffset;
		        headScreenPosition.x-=Modules::ESP::xxOffset;
		        if (p->IsLockedOn) 
		        {
		        	Drawings::DrawBox(Canvas, chestScreenPosition, headScreenPosition, ImColor(255,0,0), 2);
		        } else if (!p->IsLockedOn)  Drawings::DrawBox(Canvas, chestScreenPosition, headScreenPosition, ImColor(255, 255, 255), 2);   
            }
            // Draw Seer for Everyone
            if (Modules::ESP::DrawSeer && !Modules::ESP::AimedAtOnly) {
                Vector2D headScreenPosition;
                GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Head), headScreenPosition);
                if (!headScreenPosition.IsZeroVector())
                    Drawings::DrawSeer(Canvas, headScreenPosition.x, headScreenPosition.y - 20, p->Shield, p->MaxShield, p->Health);
            }
            // Draw Seer on locked target
            if (AimAssistState->TargetSelected && AimAssistState->CurrentTarget && Modules::ESP::DrawSeer && Modules::ESP::AimedAtOnly) {
                Vector2D headScreenPosition;
                GameCamera->WorldToScreen(AimAssistState->CurrentTarget->GetBonePosition(HitboxType::Head), headScreenPosition);
                if (headScreenPosition.IsZeroVector())
                    return;
                Drawings::DrawSeer(Canvas, headScreenPosition.x, headScreenPosition.y - 20, AimAssistState->CurrentTarget->Shield, AimAssistState->CurrentTarget->MaxShield, AimAssistState->CurrentTarget->Health);
                return;
            }
        }   
    }

    void Update() {
        double senseMaxRange = 250;
        double senseMaxRangeOverWall = 250;
  
        if (!m_level->playable)
            return;

        for (int i = 0; i < Players->size(); i++) {
                    Player* p = Players->at(i);
            if (!p->IsValid())
                continue;
            
            if (p->friendly) {
                p->setGlowEnable(1);
                p->setGlowThroughWall(1);
                int healthShield = p->Health + p->Shield;
                p->setCustomGlow(healthShield, true, true);
                continue;
            }
            double distance = math::calculateDistanceInMeters(
                Myself->LocalOrigin.x,
                Myself->LocalOrigin.y, 
                Myself->LocalOrigin.z,
                p->LocalOrigin.x,
                p->LocalOrigin.y,
                p->LocalOrigin.z);
            if (p->IsVisible && !p->IsKnocked && distance < Conversion::ToGameUnits(Modules::ESP::GlowMaxDistance)) {
                p->setGlowEnable(1);
                p->setGlowThroughWall(1);
                int healthShield = p->Health + p->Shield;
                p->setCustomGlow(healthShield, true, false);
            } 
            if (!p->IsVisible && !p->IsKnocked && distance < Conversion::ToGameUnits(Modules::ESP::GlowMaxDistance)){
                p->setGlowEnable(1);
                p->setGlowThroughWall(1);  
                int healthShield = p->Health + p->Shield; 
                p->setCustomGlow(healthShield, true, false);
            } 
            else if (distance < senseMaxRangeOverWall) {
                p->setGlowEnable(1);
                p->setGlowThroughWall(1);
                p->setCustomGlow(0, false, false);
            } else if (p->getGlowEnable() == 1 && p->getGlowThroughWall() == 1) {
                p->setGlowEnable(0);
                p->setGlowThroughWall(0);
            }
        }
    }
};

namespace fs = std::filesystem;

std::vector<std::string> GetImageFilesFromFolder(const std::string& folderPath) {
    std::vector<std::string> imageFiles;

    try {
        for (const auto& entry : fs::directory_iterator(folderPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".png") {
                imageFiles.push_back(entry.path().filename().string());
            }
        }
    } catch (const std::exception& e) {
        // Handle exceptions if any
        // You may want to log the error or display an error message
    }

    return imageFiles;
}