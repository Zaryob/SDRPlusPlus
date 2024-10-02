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
            nodeInfo.windows.push_back(window["WindowName"].get<std::string>());
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

void createDockFromInfo(ImGuiID dockspaceID, const DockNodeInfo& info, int child_num)
{
    // Set up the dock node (starting with the dockspace root node)
    if (info.child[0] != nullptr || info.child[1] != nullptr) {
        // Split the dock node if it has children
        ImGuiID nodeID = info.child[0] ? info.child[0]->nodeID : 0;
        ImGuiID parentID =info.nodeID;
        ImGuiDir splitDir;
        float splitRatio = 0.5f;

        if(info.splitAxis == -1) {
            splitDir = ImGuiDir_None;
        }
        else if(child_num == 0){
            splitDir = (ImGuiDir)(child_num + 2 * info.splitAxis);

        }
        else{
            splitDir = (ImGuiDir)(child_num + 2 * info.splitAxis);

        }
        if(info.splitAxis==0) {
            splitRatio = info.child[0]->size.x / (info.child[0]->size.x + info.child[1]->size.x);
        }
        else
        {
            splitRatio = info.child[0]->size.y / (info.child[0]->size.y + info.child[1]->size.y);
        }
        ImGui::DockBuilderSplitNode(info.nodeID, splitDir, splitRatio,
                                    &nodeID, &parentID);

        // Recursively handle child nodes
        if (info.child[0]) {
            createDockFromInfo(dockspaceID, *info.child[0], 0);
        }
        if (info.child[1]) {
            createDockFromInfo(dockspaceID, *info.child[1], 1);
        }
    } else {
        // If the node has no children, dock the windows
        for (const auto& window_name : info.windows) {
            ImGui::DockBuilderDockWindow(window_name.c_str(), info.nodeID);
        }
    }
}

void layout::createDockLayoutFromJson(ImGuiID dockspaceID, std::string filename) {
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

    if(root_node_info.nodeID != dockspaceID) {
        // Remove any existing dockspace and prepare for a new layout
        ImGuiID dockspace_id = root_node_info.nodeID ;
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

        ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing dockspace
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_None); // Add the main dockspace
        dockspaceID = dockspace_id;

    }
    // Rebuild the layout from the deserialized tree
    createDockFromInfo(dockspaceID, root_node_info, 0);

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