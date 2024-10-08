//
// Created by Süleyman Poyraz on 2.10.2024.
//

#pragma once
#include <config.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <vector>
#include <string>

struct DockNodeInfo {
    ImGuiID nodeID;
    int splitAxis;
    ImVec2 size;
    ImGuiDockNodeFlags sharedFlags;
    ImGuiDockNodeFlags localFlags;
    ImGuiDockNodeFlags mergedFlags;
    int state;
    std::vector<std::string> windows;
    DockNodeInfo* child[2] = { nullptr, nullptr }; // To hold child nodes
};

namespace layout {
    inline ConfigManager layoutConfig;

    void printAllDockNodesAsJson(ImGuiDockNode* node, std::string filename);
    void createDockLayoutFromJson(ImGuiID& dockspaceID,ImVec2 availableSpaceForDocking, std::string layoutJson);
}
