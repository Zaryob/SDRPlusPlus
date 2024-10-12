//
// Created by Süleyman Poyraz on 8.10.2024.
//

#pragma once

#include <json.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using json = nlohmann::json;

namespace layout {

  class LayoutBuilder {
  private:
    json dockWidgetConfig;
  public:
    LayoutBuilder() {
      dockWidgetConfig = json::object();
      setLocalFlags(ImGuiDockNodeFlags_None);
      setLocalFlagsInWindows(ImGuiDockNodeFlags_None);
      setMergedFlags(ImGuiDockNodeFlags_None);
      setSharedFlags(ImGuiDockNodeFlags_None);
      setState(ImGuiDockNodeState_Unknown);
      setSplitAxis(ImGuiAxis_None);
    }
    LayoutBuilder& setLocalFlags(ImGuiDockNodeFlags localFlags) {
      dockWidgetConfig["LocalFlags"] = localFlags;
      return *this;
    }

    LayoutBuilder& setLocalFlagsInWindows(ImGuiDockNodeFlags localFlagsInWindows) {
      dockWidgetConfig["LocalFlagsInWindows"] = localFlagsInWindows;
      return *this;
    }

    LayoutBuilder& setMergedFlags(ImGuiDockNodeFlags mergedFlags) {
      dockWidgetConfig["MergedFlags"] = mergedFlags;
      return *this;
    }

    LayoutBuilder& setNodeID(const std::string& nodeName) {
      dockWidgetConfig["NodeID"] = ImGui::GetID(nodeName.c_str());
      return *this;
    }

    LayoutBuilder& setSharedFlags(ImGuiDockNodeFlags sharedFlags) {
      dockWidgetConfig["SharedFlags"] = sharedFlags;
      return *this;
    }

    LayoutBuilder& setSize(float width, float height) {
      dockWidgetConfig["Size"] = {width, height};
      return *this;
    }

    LayoutBuilder& setSplitAxis(ImGuiAxis splitAxis) {
      dockWidgetConfig["SplitAxis"] = splitAxis;
      return *this;
    }

    LayoutBuilder& setState(ImGuiDockNodeState state) {
      dockWidgetConfig["State"] = state;
      return *this;
    }

    LayoutBuilder& addWindow(const std::string& windowName) {
      ImGuiID windowID = ImHashStr(windowName.c_str());
      dockWidgetConfig["Windows"].push_back({{"WindowID", windowID}, {"WindowName", windowName}});
      return *this;
    }

    LayoutBuilder& setChildNode1(json childNode) {
      dockWidgetConfig["ChildNode1"] = childNode;
      return *this;
    }

    LayoutBuilder& setChildNode2(json childNode) {
      dockWidgetConfig["ChildNode2"] = childNode;
      return *this;
    }

    json build() {
      return dockWidgetConfig;
    }
  };

}
