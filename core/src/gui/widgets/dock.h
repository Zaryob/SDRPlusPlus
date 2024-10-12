#pragma once
#include <string>
#include <vector>
#include <map>
#include <module.h>

#define MAX_MENU_COUNT 1024

class Dock {
public:
  Dock();

  struct DockOption_t {
    std::string name;
    bool open;
  };

  struct DockItem_t {
    void (*drawHandler)(void* ctx);
    void* ctx;
    ModuleManager::Instance* inst;
  };

  void registerEntry(std::string name, void (*drawHandler)(void* ctx), void* ctx = NULL, ModuleManager::Instance* inst = NULL);
  void removeEntry(std::string name);
  bool draw();

  std::vector<DockOption_t> dock_windows;

  bool locked = false;

private:
  bool isInOrderList(std::string name);


  std::map<std::string, DockItem_t> items;

  float headerTops[MAX_MENU_COUNT];
  int optionIDs[MAX_MENU_COUNT];
};