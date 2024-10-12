//
// Created by Süleyman Poyraz on 12.10.2024.
//

#include <gui/widgets/dock.h>

#include <gui/widgets/menu.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <gui/style.h>

Dock::Dock() {
}

void Dock::registerEntry(std::string name, void (*drawHandler)(void* ctx), void* ctx, ModuleManager::Instance* inst) {
    DockItem_t item;
    item.drawHandler = drawHandler;
    item.ctx = ctx;
    item.inst = inst;
    items[name] = item;
    if (!isInOrderList(name)) {
        DockOption_t opt;
        opt.name = name;
        opt.open = true;
        dock_windows.push_back(opt);
    }
}

void Dock::removeEntry(std::string name) {
    items.erase(name);
}

bool Dock::draw() {
    for (DockOption_t& opt : dock_windows) {
        if (items.find(opt.name) == items.end()) {
            continue;
        }
        DockItem_t item = items[opt.name];

        if (ImGui::Begin((opt.name).c_str())) {
            item.drawHandler(item.ctx);
        }
        ImGui::End();
    }
    return true;
}

bool Dock::isInOrderList(std::string name) {
    for (DockOption_t opt : dock_windows) {
        if (opt.name == name) {
            return true;
        }
    }
    return false;
}
