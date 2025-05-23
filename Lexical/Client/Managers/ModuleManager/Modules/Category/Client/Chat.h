#pragma once

#include "../../ModuleBase/Module.h"
#include <string>

class Chat : public Module {
public:
    Chat();
    ~Chat() {}
    void onSendPacket(Packet* packet) override;
private:
    std::string generateRandomString(int length);
    int bypassLength = 5;
    bool useSeparator = true;
    bool useSuffix = true;
    bool antiFilter = false;
    std::string suffixText = "Lexical";
};
