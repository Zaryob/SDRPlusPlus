//
// Created by Süleyman Poyraz on 2.10.2024.
//

#include <gui/layout.h>
#include <json.hpp>
#include <utils/flog.h>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

json serializeDockNode(ImGuiDockNode* node)
{
    if (!node) return nullptr;

    json j;
    j["NodeID"] = node->ID;
    j["SplitAxis"] = node->SplitAxis;
    j["Size"] = { node->Size.x, node->Size.y };
    j["SharedFlags"] = node->SharedFlags;
    j["LocalFlags"] = node->LocalFlags;
    j["LocalFlagsInWindows"] = node->LocalFlagsInWindows;
    j["MergedFlags"] = node->MergedFlags;
    j["State"] = node->State;

    // Serialize docked windows
    if (!node->Windows.empty()) {
        j["Windows"] = json::array();
        for (ImGuiWindow* window : node->Windows) {
            json window_info;
            window_info["WindowID"] = window->ID;
            window_info["WindowName"] = window->Name;
            j["Windows"].push_back(window_info);
        }
    }

    // Serialize child nodes (if any)
    if (node->ChildNodes[0]) {
        j["ChildNode1"] = serializeDockNode(node->ChildNodes[0]);
    }
    if (node->ChildNodes[1]) {
        j["ChildNode2"] = serializeDockNode(node->ChildNodes[1]);
    }

    return j;
}

DockNodeInfo deserializeDockNode(const json& j)
{
    DockNodeInfo nodeInfo;
    nodeInfo.nodeID = j["NodeID"].get<ImGuiID>();
    nodeInfo.splitAxis = j["SplitAxis"].get<int>();
    nodeInfo.size = ImVec2(j["Size"][0].get<float>(), j["Size"][1].get<float>());
    nodeInfo.sharedFlags = j["SharedFlags"].get<ImGuiDockNodeFlags>();
    nodeInfo.localFlags = j["LocalFlags"].get<ImGuiDockNodeFlags>();
    nodeInfo.mergedFlags = j["MergedFlags"].get<ImGuiDockNodeFlags>();
    nodeInfo.state = j["State"].get<int>();

    // Deserialize the windows associated with the node
    if (j.contains("Windows")) {
        for (const auto& window : j["Windows"]) {
            nodeInfo.windows.push_back({window["WindowID"].get<ImGuiID>(),window["WindowName"].get<std::string>()});
        }
    }

    // Deserialize child nodes recursively
    if (j.contains("ChildNode1")) {
        nodeInfo.child[0] = new DockNodeInfo(deserializeDockNode(j["ChildNode1"]));
    }
    if (j.contains("ChildNode2")) {
        nodeInfo.child[1] = new DockNodeInfo(deserializeDockNode(j["ChildNode2"]));
    }

    return nodeInfo;
}

void createDockFromInfo(ImGuiID parentId, const DockNodeInfo& info)
{
    ImGuiID nodeId = info.nodeID;
    ImGuiContext& g = *GImGui;
    ImGuiDockNode* node = ImGui::DockContextFindNodeByID(&g, nodeId);
    ImGuiDockNode* p_node = ImGui::DockContextFindNodeByID(&g, parentId);
    node->Size = info.size;
    node->State = (ImGuiDockNodeState)info.state;
    node->SplitAxis = (ImGuiAxis)info.splitAxis;
    node->LocalFlags = info.localFlags;
    node->SharedFlags = info.sharedFlags;
    node->MergedFlags = info.mergedFlags;
    node->ParentNode = p_node;
    node->Windows.clear();

    for (const auto& window : info.windows) {
        ImGui::DockBuilderDockWindow(window.windowName.c_str(), window.windowID);
    }
    // Set up the dock node (starting with the dockspace root node)
    if (info.child[0] != nullptr && info.child[1] != nullptr) {
        // Split the dock node if it has children
        ImGuiID leftNodeId = info.child[0]->nodeID;
        ImGuiID rightNodeId = info.child[1]->nodeID;

        ImGui::DockBuilderAddNode(leftNodeId);
        ImGuiDockNode* lnode = ImGui::DockContextFindNodeByID(&g, leftNodeId);

        ImGui::DockBuilderAddNode(rightNodeId);
        ImGuiDockNode* rnode = ImGui::DockContextFindNodeByID(&g, rightNodeId);

        node->ChildNodes[0] = lnode;
        node->ChildNodes[1] = rnode;

        // Recursively handle child nodes
        if (info.child[0]) {
            createDockFromInfo(info.nodeID, *info.child[0]);
        }
        if (info.child[1]) {
            createDockFromInfo(info.nodeID, *info.child[1]);
        }
    }
}

void layout::createDockLayoutFromJson(ImGuiID& dockspaceID, std::string filename) {
    // Parse the JSON
    // Open the JSON file
    std::ifstream input_file(filename);
    if (!input_file.is_open()) {
        flog::error("Could not open file: {0}\n", filename.c_str());
        return;
    }

    // Parse the JSON
    json layout_json;
    input_file >> layout_json;

    // Deserialize root node
    DockNodeInfo root_node_info = deserializeDockNode(layout_json);

    // Remove any existing dockspace and prepare for a new layout
    if(dockspaceID != root_node_info.nodeID) {
        ImGui::DockBuilderRemoveNode(dockspaceID); // Clear out existing dockspace
    }

    ImGuiID dockspace_id = root_node_info.nodeID ;
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode );
    ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing dockspace

    ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing dockspace
    ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_None); // Add the main dockspace

    dockspaceID = dockspace_id;
    ImGui::DockBuilderFinish(dockspace_id);

    std::cout<<"DOCKBUILD"<<std::endl;

    // Rebuild the layout from the deserialized tree
    createDockFromInfo(dockspaceID, root_node_info);
    std::cout<<"DOCKBUILD2"<<std::endl;


}

void layout::printAllDockNodesAsJson(ImGuiDockNode* node, std::string filename) {
    if (node) {
        // Serialize the dock node tree into JSON
        json root_node_json = serializeDockNode(node);

        // Output JSON to console or file
        std::string json_output = root_node_json.dump(4); // 4 is the indentation level
        // Write the JSON string to a file
        std::cout << json_output<<std::endl;
        std::ofstream file(filename, std::ios::out);
        if (file.is_open()) {
            file << json_output;
            file.close();
            flog::info("JSON saved to file: {0}\n", filename.c_str());
        } else {
            flog::error("Could not open file for writing: {0}\n", filename.c_str());
        }
    } else {
        flog::error("DockSpace node not found.");
    }
}