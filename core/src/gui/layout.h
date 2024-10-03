//
// Created by Süleyman Poyraz on 2.10.2024.
//

#pragma once
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <vector>
#include <string>

struct DockWindow {
    ImGuiID windowID;
    std::string windowName;
};

struct DockNodeInfo {
    ImGuiID nodeID;
    int splitAxis;
    ImVec2 size;
    ImGuiDockNodeFlags sharedFlags;
    ImGuiDockNodeFlags localFlags;
    ImGuiDockNodeFlags mergedFlags;
    int state;
    std::vector<DockWindow> windows;
    DockNodeInfo* child[2] = { nullptr, nullptr }; // To hold child nodes
};

namespace layout {
    void printAllDockNodesAsJson(ImGuiDockNode* node, std::string filename);
    void createDockLayoutFromJson(ImGuiID& dockspaceID, std::string layoutJson);
}
