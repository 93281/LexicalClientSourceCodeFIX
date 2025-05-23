#include "Chat.h"
#include <random>

Chat::Chat() : Module("Chat", "Appends a suffix and bypass string to messages.", Category::CLIENT) {
    registerSetting(new SliderSetting<int>("Bypass Length", "Length of the bypass string.", &bypassLength, 5, 0, 16));
    registerSetting(new BoolSetting("Use Separator", "Toggle separator usage.", &useSeparator, true));
    registerSetting(new BoolSetting("Use Suffix", "Toggle suffix usage.", &useSuffix, true));
    registerSetting(new BoolSetting("Anti Filter", "Prevents your curse words from being filtered.", &antiFilter, true));
}

void Chat::onSendPacket(Packet* packet) {
    if (packet->getId() == PacketID::Text) {
        auto* textPacket = reinterpret_cast<TextPacket*>(packet);
        if (textPacket->mType == TextPacketType::Chat && !textPacket->mMessage.empty()) {
            std::string message = textPacket->mMessage;
            if (antiFilter) {
                static const std::vector<std::string> curseWords = {
                    "fuck", "shit", "bitch", "asshole", "bastard", "dick", "cunt", "nigger", "faggot", "ass", "damn", "nigga", "fag", "shitter", "retard"
                };
                for (const std::string& word : curseWords) {
                    size_t pos = 0;
                    while ((pos = message.find(word, pos)) != std::string::npos) {
                        std::string maskedWord = word;
                        size_t indexToMask = word.length() / 2;
                        maskedWord[indexToMask] = '*';
                        message.replace(pos, word.length(), maskedWord);
                        pos += maskedWord.length();
                    }
                }
            }


            std::string bypass = generateRandomString(bypassLength);
            std::string separator = useSeparator ? " | " : "";
            std::string finalMessage = message + separator;

            if (useSuffix) {
                finalMessage += suffixText + separator;
            }

            finalMessage += bypass;

            textPacket->mMessage = finalMessage;
            textPacket->mFilteredMessage = finalMessage;
            textPacket->mAuthor = "";
        }
    }
}

std::string Chat::generateRandomString(int length) {
    static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string result;
    result.reserve(length);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sizeof(alphanum) - 2);
    for (int i = 0; i < length; ++i) {
        result += alphanum[dis(gen)];
    }
    return result;
}
