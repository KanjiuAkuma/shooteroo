#pragma once

/**
  * Created by Joscha Vack on 11/20/2020.
  *
  **/

#include <Engine.h>

#include <random>
#include <core/events/KeyEvent.h>
#include <core/events/MouseEvent.h>
#include <core/util/Shader.h>
#include <core/renderer/texture/Texture.h>

#include "entities/Player.h"
#include "entities/Projectile.h"
#include "entities/Bullet.h"
#include "entities/Hostile.h"
#include "settings/GameSettings.h"

using namespace Engine;

class Game : public Layer {
public:
    Game(GameSettings* settings, bool showSettings);
    ~Game() override;

    // public functions
    void addBullet(Bullet* entity);
    void removeBullet(Bullet* entity);

    void addHostile(Hostile* entity);
    void removeHostile(Hostile* entity);

    void addProjectile(Projectile* entity);
    void removeProjectile(Projectile* entity);

    void reset();

    // overwrites from Layer
    void onAttach() override;
    void onDetach() override;

    void OnEvent(Event& e) override;
    void onUpdate(float dt) override;
    void onImGuiRender() override;

    bool onKeyPressed(KeyPressedEvent& e);
    bool onMouseButtonPressed(MouseButtonPressedEvent& e);

    static Game& get() { return *instance; }

    static glm::vec2 mainAxis;

private:
    bool awaitPlay, running,
        showSettings, spawnHostiles, spawnProjectiles, renderHitbox,
        showHelp = false, invulnerable = false;
    float score = 0, dtMultiplier = 1.f;

    GameSettings* gameSettings;

    Player* player = nullptr;

    std::vector<Bullet*> bullets = std::vector<Bullet*>();
    std::vector<Hostile*> hostiles = std::vector<Hostile*>();
    std::vector<Projectile*> projectiles = std::vector<Projectile*>();

    float accumulatedProjectileProbability = 0;
    float accumulatedHostileProbability = 0;

    static Game* instance;

    // rendering

    GLuint quadVA, quadVB, quadIB;

    Texture *grassTex, *hitboxTex, *playerTex, *bulletTex, *hostileTex, *projectileTex;
        Util::Shader *shader;

    void spawnProjectile();
    void spawnHostile();

    void renderBackground();
    void renderEntity(const Entity* entity, const TextureSettings* textureSettings);

    void setPaused(bool paused);

};
