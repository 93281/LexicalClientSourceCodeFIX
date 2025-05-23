#include "HitFX.h"
#include <cstdlib>

HitFX::HitFX() : Module("HitFX", "Spawns hit particles", Category::RENDER) {
    registerSetting(new SliderSetting<int>("Count", "Particles per hit", &particleCount, 10, 1, 100));
    registerSetting(new SliderSetting<float>("Lifetime", "How long they last", &particleLifetime, 1.0f, 0.1f, 5.0f));
}
void HitFX::onNormalTick(LocalPlayer* localPlayer) {
	if (!localPlayer) return;

	for (auto& entity : localPlayer->level->getRuntimeActorList()) {
		if (!TargetUtil::isTargetValid(entity, true)) continue;

        auto hurtTimeComp = entity->getComponent<MobHurtTimeComponent>();
        if (hurtTimeComp && hurtTimeComp->hurtTime > 0) {
			Vec3<float> center = entity->getPos();

			for (int i = 0; i < particleCount; ++i) {
				Vec3<float> randOffset = {
					((float)rand() / RAND_MAX - 0.5f) * 0.6f,
					((float)rand() / RAND_MAX) * 1.0f,
					((float)rand() / RAND_MAX - 0.5f) * 0.6f
				};
				Vec3<float> velocity = {
					((float)rand() / RAND_MAX - 0.5f) * 0.05f,
					((float)rand() / RAND_MAX) * 0.05f,
					((float)rand() / RAND_MAX - 0.5f) * 0.05f
				};

				Vec3<float> spawnPos = {
					center.x + randOffset.x,
					center.y + randOffset.y,
					center.z + randOffset.z
				};

				particles[entity].emplace_back(spawnPos, velocity, particleLifetime, color);
			}
		}
	}
}

void HitFX::onLevelRender() {
    float dt = D2D::deltaTime;

    for (auto it = particles.begin(); it != particles.end();) {
        auto& list = it->second;

        for (auto p = list.begin(); p != list.end();) {
            p->lifetime -= dt;

            p->pos.x += p->velocity.x;
            p->pos.y += p->velocity.y;
            p->pos.z += p->velocity.z;
            static Colors* colorMod = ModuleManager::getModule<Colors>();
            UIColor mainColor = colorMod->getColor(1 * colorMod->getSeperationValue());
            float alpha = (p->lifetime / p->maxLifetime) * p->color.a;
            UIColor fadedColor = UIColor(mainColor.r, mainColor.g, mainColor.b, (int)alpha);

            Vec3<float> boxMax = {
                p->pos.x + 0.05f,
                p->pos.y + 0.05f,
                p->pos.z + 0.05f
            };

            MCR::drawBox3dFilled(
                AABB(p->pos, boxMax),
                fadedColor, fadedColor
            );

            if (p->lifetime <= 0)
                p = list.erase(p);
            else
                ++p;
        }

        if (list.empty())
            it = particles.erase(it);
        else
            ++it;
    }
}
