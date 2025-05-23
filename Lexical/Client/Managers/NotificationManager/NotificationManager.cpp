#include "NotificationManager.h"
#include "../../SDK/Game.h"
#include "../../Renderer/D2D.h"
#include "../../Utils/TimerUtil.h"

void NotificationManager::Render() {

    Vec2<float> windowSize = Game::clientInstance->guiData->windowSizeReal;
    float textSize = 1.f;
    float timeTextSize = 0.6f; 
    float textPaddingX = 4.f;
    float textPaddingY = 4.f;
    float textHeight = D2D::getTextHeight("", textSize);

    float offsetX = 12.f;
    float offsetY = 35.f;

    float posX = offsetX;
    float posY = windowSize.y - (textHeight + textPaddingY * 2.f) - offsetY;

    for (int i = 0; i < notifList.size(); i++) {
        auto& notif = notifList[i];
        std::string message = notif->message;
        float textWidth = D2D::getTextWidth(message, textSize);

        if (notif->duration == notif->maxDuration) {
            notif->pos = Vec2<float>(windowSize.x, posY);
        }

        if (notif->duration > 0.f) {
            Vec2<float> posTo = Vec2<float>(windowSize.x - offsetX - textWidth - (textPaddingX * 2.f), posY);
            notif->pos.x = Math::lerp(notif->pos.x, posTo.x, D2D::deltaTime * 8.f);
            notif->pos.y = Math::lerp(notif->pos.y, posTo.y, D2D::deltaTime * 8.f);
            notif->duration -= D2D::deltaTime;
        }
        else {
            if (notif->pos.x < windowSize.x) {
                notif->pos.x += notif->vel.x;
                notif->pos.y = Math::lerp(notif->pos.y, posY, D2D::deltaTime * 8.f);

                notif->vel.x += D2D::deltaTime * 30.f;
            }
            else {
                delete notif;
                notifList.erase(notifList.begin() + i);
                i--;
                continue;
            }
        }

        float anim = (windowSize.x - notif->pos.x) / (windowSize.x - (windowSize.x - offsetX - textWidth - (textPaddingX * 2.f)));
        if (anim > 1.f)
            anim = 1.f;

        float rectWidth = textWidth + (textPaddingX * 2.f) + 20.f;
        float rectHeight = textHeight + (textPaddingY * 2.f);

        Vec4<float> rectPos = Vec4<float>(notif->pos.x,
            notif->pos.y,
            notif->pos.x + rectWidth,
            notif->pos.y + rectHeight);

        Vec2<float> textPos = Vec2<float>(rectPos.x + textPaddingX, rectPos.y + textPaddingY);
        D2D::fillRectangle(rectPos, UIColor(0, 0, 0, (int)(135 * anim)));
        D2D::drawText(textPos, message, UIColor(255, 255, 255, (int)(255 * anim)), textSize);
        if (notif->duration > 0.f) {
            int secondsLeft = (int)std::ceil(notif->duration);
            std::string timeText = std::to_string(secondsLeft) + "s";

            float timeTextWidth = D2D::getTextWidth(timeText, timeTextSize);
            float timeTextHeight = D2D::getTextHeight(timeText, timeTextSize);
            float extraLeftPadding = 8.f;

            Vec2<float> timeTextPos = Vec2<float>(
                rectPos.z - timeTextWidth - textPaddingX - extraLeftPadding,
                rectPos.y + rectHeight - timeTextHeight - textPaddingY
            );
            D2D::drawText(timeTextPos, timeText, UIColor(200, 200, 200, (int)(200 * anim)), timeTextSize);
        }
        posY -= (rectHeight + 12.f * textSize) * anim;
    }
}
