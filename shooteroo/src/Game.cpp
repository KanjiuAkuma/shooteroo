/**
  * Created by Joscha Vack on 11/20/2020.
  *
  **/

#include "Game.h"

#include <glm/ext.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <imgui.h>
#include <stb_image.h>

#include <Macros.h>
#include <core/logger/Logger.h>
#include <core/window/Input.h>
#include <core/events/MouseEvent.h>
#include <EngineUtil.h>

#define SPACE(h) ImGui::Dummy(ImVec2(0.f, h))
#define BOARD_DIMENSION 1e3f

#ifdef NDEBUG
#define SCREEN_PADDING 0.f
#else
#define SCREEN_PADDING .05f
#endif

Game* Game::instance = nullptr;
glm::vec2 Game::mainAxis = glm::vec2(1, 0);

Game::Game(GameSettings* gameSettings, bool showSettings) :
        showSettings(showSettings), spawnHostiles(!showSettings), spawnProjectiles(!showSettings),
        renderHitbox(showSettings), gameSettings(gameSettings) {
    CORE_ASSERT(!instance, "Application already exists!");
    instance = this;

    // init player
    player = new Player(&gameSettings->playerSettings, glm::vec2(0));
}

Game::~Game() {
    // clean up entities
    for (auto entity : hostiles) {
        delete entity;
    }
    hostiles.clear();
    for (auto bullet : bullets) {
        delete bullet;
    }
    bullets.clear();
    for (auto projectile : projectiles) {
        delete projectile;
    }
    projectiles.clear();

    delete player;
}

void Game::addBullet(Bullet* bullet) {
    ASSERT(std::find(bullets.begin(), bullets.end(), bullet) == bullets.end(), "Bullet already exists")
    bullets.push_back(bullet);
    LOG_DEBUG("Added bullet {}", bullet->toString());
}

void Game::removeBullet(Bullet* bullet) {
    auto it = std::find(bullets.begin(), bullets.end(), bullet);
    ASSERT(it != bullets.end(), "Bullet not found!")
    if (it != bullets.end()) {
        bullets.erase(it);
        LOG_DEBUG("Removed bullet {}", bullet->toString());
    }
}

void Game::addHostile(Hostile* hostile) {
    ASSERT(std::find(hostiles.begin(), hostiles.end(), hostile) == hostiles.end(), "Hostile already exists")
    hostiles.push_back(hostile);
    LOG_DEBUG("Added hostile {}", hostile->toString());
}

void Game::removeHostile(Hostile* hostile) {
    auto it = std::find(hostiles.begin(), hostiles.end(), hostile);
    ASSERT(it != hostiles.end(), "Hostile not found!")
    if (it != hostiles.end()) {
        hostiles.erase(it);
        LOG_DEBUG("Removed hostile {}", hostile->toString());
    }
}

void Game::addProjectile(Projectile* projectile) {
    ASSERT(std::find(projectiles.begin(), projectiles.end(), projectile) == projectiles.end(),
           "Projectile already exists");
    projectiles.push_back(projectile);
    LOG_DEBUG("Added projectile {}", projectile->toString());
}

void Game::removeProjectile(Projectile* projectile) {
    auto it = std::find(projectiles.begin(), projectiles.end(), projectile);
    ASSERT(it != projectiles.end(), "Projectile not found!")
    if (it != projectiles.end()) {
        projectiles.erase(it);
        LOG_DEBUG("Removed projectile {}", projectile->toString());
    }
}

void Game::reset() {
    // clear game
    for (auto entity : hostiles) {
        delete entity;
    }
    hostiles.clear();
    for (auto bullet : bullets) {
        delete bullet;
    }
    bullets.clear();
    for (auto projectile : projectiles) {
        delete projectile;
    }
    projectiles.clear();
    delete player;

    // create new player
    player = new Player(&gameSettings->playerSettings, glm::vec2(0));
    if (gameSettings->scoreSettings.highscore < score) {
        gameSettings->scoreSettings.highscore = score;
    }
    score = 0;
    LOG_DEBUG("Reset game");
}

void Game::onAttach() {
    LOG_DEBUG("Attaching game layer");
    running = false;
    awaitPlay = true;

    LOG_DEBUG("Setting up openGL");
    // init opengl stuff
    Engine::Util::enableGLDebugging();

    GL_CALL(glEnable(GL_BLEND));
    GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    LOG_DEBUG("Creating vertex array, vertex buffer, index buffer");
    // vertex array
    GL_CALL(glCreateVertexArrays(1, &quadVA))
    GL_CALL(glBindVertexArray(quadVA))

    // vertex buffer
    GL_CALL(glCreateBuffers(1, &quadVB))
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, quadVB))
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(EntityVertex) * 4, nullptr, GL_DYNAMIC_DRAW))

    GL_CALL(glEnableVertexArrayAttrib(quadVA, 0))
    GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(EntityVertex),
                                  (const void*) offsetof(EntityVertex, offset)))

    GL_CALL(glEnableVertexArrayAttrib(quadVA, 1))
    GL_CALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(EntityVertex),
                                  (const void*) offsetof(EntityVertex, position)))

    GL_CALL(glEnableVertexArrayAttrib(quadVA, 2))
    GL_CALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(EntityVertex),
                                  (const void*) offsetof(EntityVertex, texCoords)))

    // index buffer
    GL_CALL(glCreateBuffers(1, &quadIB))
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIB))

    int indices[6] = {
            0, 1, 2,
            2, 3, 0
    };
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6, indices, GL_STATIC_DRAW))

    LOG_DEBUG("Loading shader");
    shader = Engine::Util::Shader::fromGLSLTextFiles(
            R"(resources\shaders\shooteroo\entity.vshader)",
            R"(resources\shaders\shooteroo\entity.fshader)"
    );

    LOG_DEBUG("Loading texture");
    grassTex = new Texture(R"(resources\textures\shooteroo\grass_tex.png)");
    hitboxTex = new Texture(R"(resources\textures\shooteroo\hitbox_tex.png)");
    playerTex = new Texture(R"(resources\textures\shooteroo\player_tex.png)");
    bulletTex = new Texture(R"(resources\textures\shooteroo\bullet_tex.png)");
    projectileTex = new Texture(R"(resources\textures\shooteroo\projectile_tex.png)");
    hostileTex = new Texture(R"(resources\textures\shooteroo\hostile_tex.png)");

    LOG_DEBUG("Attached game layer");
}

void Game::onDetach() {
    LOG_DEBUG("Detaching game layer");
    running = false;
    awaitPlay = false;

    LOG_DEBUG("Deleting vertex array, vertex buffer, index buffer");
    GL_CALL(glDeleteVertexArrays(1, &quadVA));
    GL_CALL(glDeleteBuffers(1, &quadVB));
    GL_CALL(glDeleteBuffers(1, &quadIB));

    LOG_DEBUG("Deleting textures");
    delete grassTex;
    delete hitboxTex;
    delete playerTex;
    delete bulletTex;
    delete hostileTex;
    delete projectileTex;

    LOG_DEBUG("Deleting shader");
    delete shader;
}

void Game::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(Game::onKeyPressed));
    dispatcher.dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(Game::onMouseButtonPressed));
}

void Game::onUpdate(float dt) {
    dt *= dtMultiplier;
    // only update if game is running
    if (running && !showHelp) {
        // update player
        player->onUpdate(dt);

        // update hostiles and check if player dies
        for (auto& hostile : hostiles) {
            hostile->setTargetLocation(player->getPosition());
            hostile->onUpdate(dt);
            if (glm::length(player->getPosition() - hostile->getPosition()) < player->getSize() + hostile->getSize()) {
                // player got hit, game over
                if (!invulnerable) {
                    LOG_DEBUG("Game over, player died to a hostile");
                    running = false;
                    break;
                }
            }
        }

        // update projectiles and check if player dies
        glm::vec2 screenBounds((1.1f - SCREEN_PADDING) * (float) Engine::Application::get().getWindow().getViewportWidth() / BOARD_DIMENSION,
                               (1.1f - SCREEN_PADDING) * (float) Engine::Application::get().getWindow().getViewportHeight() / BOARD_DIMENSION);
        for (auto& projectile : projectiles) {
            projectile->onUpdate(dt);
            if (glm::length(player->getPosition() - projectile->getPosition()) <
                player->getSize() + projectile->getSize()) {
                // player got hit, game over
                if (!invulnerable) {
                    LOG_DEBUG("Game over, player died to a projectile");
                    running = false;
                    break;
                }
            }

            // check if projectile left the screen
            glm::vec2 pos = glm::abs(projectile->getPosition()) - projectile->getSize();

            if (screenBounds.x < pos.x || screenBounds.y < pos.y) {
                removeProjectile(projectile);
            }
        }

        // update bullets
        for (auto& bullet : bullets) {
            bullet->onUpdate(dt);
        }

        // check bullet-hostile collision
        for (auto& bullet : bullets) {
            for (auto& hostile: hostiles) {
                if (glm::length(hostile->getPosition() - bullet->getPosition()) <
                    hostile->getSize() + bullet->getSize()) {

                    // hostile got hit, remove bullet and hostile
                    score += gameSettings->scoreSettings.killIncrement;
                    LOG_DEBUG("Hostile {} got hit by bullet {}", hostile->toString(), bullet->toString());
                    hostile->onHit();
                    bullet->onHit();
                    break;
                }
            }
        }

        // maybe spawn new bullets and hostiles
        static std::default_random_engine e;
        static std::uniform_real_distribution<float> dis;

        accumulatedProjectileProbability += dis(e) * dt;
        if (1 <= accumulatedProjectileProbability) accumulatedProjectileProbability = 0;
        accumulatedHostileProbability += dis(e) * dt;
        if (1 <= accumulatedHostileProbability) accumulatedHostileProbability = 0;

        if (spawnProjectiles &&
            (1.f - gameSettings->projectileSettings.probability) <= accumulatedProjectileProbability) {
            accumulatedProjectileProbability = 0;

            spawnProjectile();
        }
        if (spawnHostiles && (1.f - gameSettings->hostileSettings.probability) <= accumulatedHostileProbability) {
            accumulatedHostileProbability = 0;

            spawnHostile();
        }

        score += dt * gameSettings->scoreSettings.timeIncrement;
    }

    // clear buffer
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // bind entity shader
    GL_CALL(glUseProgram(shader->getRendererID()));

    // bind vertex array, vertex buffer, index buffer
    GL_CALL(glBindVertexArray(quadVA))
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadVB))
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIB))

    // get tex uniform locations
    int projLocation = glGetUniformLocation(shader->getRendererID(), "u_proj");
    int texLocation = glGetUniformLocation(shader->getRendererID(), "u_texture");

    GL_CALL(glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f))));

    grassTex->bind(0);
    GL_CALL(glUniform1i(texLocation, 0))
    renderBackground();

    // calc and push proj matrix
    glm::vec2 viewportSize = Engine::Application::get().getWindow().getViewportSize();
    GL_CALL(glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(
            glm::scale(glm::mat4(1.f),
                       glm::vec3(
                               BOARD_DIMENSION / Application::get().getWindow().getViewportWidth(),
                               BOARD_DIMENSION / Application::get().getWindow().getViewportHeight(),
                               1.f
                       )))));

    // for debugging: render all elements with background texture
    if (renderHitbox) {
        const TextureSettings hitboxTextureSettings = {"hitbox_tex.png", 1.f, 0.f, 0.f, 0.f};

        hitboxTex->bind(0);
        GL_CALL(glUniform1i(texLocation, 0))
        renderEntity(player, &hitboxTextureSettings);

        for (auto& bullet : bullets) {
            renderEntity(bullet, &hitboxTextureSettings);
        }

        for (auto& hostile : hostiles) {
            renderEntity(hostile, &hitboxTextureSettings);
        }

        for (auto& projectile : projectiles) {
            renderEntity(projectile, &hitboxTextureSettings);
        }
    }

    playerTex->bind(0);
    GL_CALL(glUniform1i(texLocation, 0))
    renderEntity(player, &gameSettings->playerSettings.texture);

    bulletTex->bind(0);
    GL_CALL(glUniform1i(texLocation, 0))
    for (auto& bullet : bullets) {
        renderEntity(bullet, &gameSettings->playerSettings.weaponSettings.texture);
    }

    hostileTex->bind(0);
    GL_CALL(glUniform1i(texLocation, 0))
    for (auto& hostile : hostiles) {
        renderEntity(hostile, &gameSettings->hostileSettings.texture);
    }

    projectileTex->bind(0);
    GL_CALL(glUniform1i(texLocation, 0))
    for (auto& projectile : projectiles) {
        renderEntity(projectile, &gameSettings->projectileSettings.texture);
    }
}

void Game::spawnProjectile() {
    glm::vec2 scale((1.f - SCREEN_PADDING) * (float) Engine::Application::get().getWindow().getViewportWidth() / BOARD_DIMENSION,
                    (1.f - SCREEN_PADDING) * (float) Engine::Application::get().getWindow().getViewportHeight() / BOARD_DIMENSION);
    static std::default_random_engine e;
    static std::uniform_real_distribution<float> dis;

    // spawn projectile
    float size = gameSettings->projectileSettings.size;

    float ra = (1.f + size) * (2.f * dis(e) - 1.f);
    float fa = (1.f + size) * (dis(e) <= .5f ? -1.f : 1.f);
    glm::vec2 pos = dis(e) <= .5f ? glm::vec2(ra, fa) : glm::vec2(fa, ra);
    pos.x *= scale.x;
    pos.y *= scale.y;

    glm::vec2 dir = glm::normalize(player->getPosition() - pos);
    float omega = -4.f + dis(e) * 8.f;

    LOG_DEBUG("Spawning projectile size={}, direction=[{}, {}], pos=[{}, {}]", size, dir.x, dir.y, pos.x, pos.y);

    auto* projectile = new Projectile(
            &gameSettings->projectileSettings,
            pos, dir, omega);
    addProjectile(projectile);
}

void Game::spawnHostile() {
    glm::vec2 scale((1.f - SCREEN_PADDING) * (float) Engine::Application::get().getWindow().getViewportWidth() / BOARD_DIMENSION,
                    (1.f - SCREEN_PADDING) * (float) Engine::Application::get().getWindow().getViewportHeight() / BOARD_DIMENSION);
    static std::default_random_engine e;
    static std::uniform_real_distribution<float> dis;

    // spawn hostile
    bool small = dis(e) < .5;
    const float* size = small ? &gameSettings->hostileSettings.sizeSmall : &gameSettings->hostileSettings.sizeBig;
    const float* speed = &gameSettings->hostileSettings.speed;
    float sizeV = *size;

    float ra = -(1.f + sizeV) + dis(e) * 2.f * (1.f + sizeV);
    float fa = (1.f + sizeV) * (dis(e) < .5 ? -1.f : 1.f);
    glm::vec2 pos = dis(e) <= .5f ? glm::vec2(ra, fa) : glm::vec2(fa, ra);
    pos.x *= scale.x;
    pos.y *= scale.y;
    LOG_DEBUG("Spawning hostile size={}, speed={}, pos=[{}, {}]", *size, *speed, pos.x, pos.y);

    auto* hostile = new Hostile(&gameSettings->hostileSettings, small, pos, player->getPosition());
    addHostile(hostile);
}

void Game::renderBackground() {
    float width = 1.f - BOARD_DIMENSION * SCREEN_PADDING / (float) Engine::Application::get().getWindow().getViewportWidth();
    float height = 1.f - BOARD_DIMENSION * SCREEN_PADDING / (float) Engine::Application::get().getWindow().getViewportHeight();

    EntityVertex vertices[4] = {
            glm::vec2(-width, -height), glm::vec2(0), glm::vec2(0, 0),
            glm::vec2(width, -height), glm::vec2(0), glm::vec2(1, 0),
            glm::vec2(width, height), glm::vec2(0), glm::vec2(1, 1),
            glm::vec2(-width, height), glm::vec2(0), glm::vec2(0, 1),
    };

    GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(EntityVertex) * 4, vertices))

    // render player
    GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr))
}

void Game::renderEntity(const Entity* entity, const TextureSettings* textureSettings) {
    // buffer player vertex data
    const float size = entity->getSize();
    const float scale = textureSettings->scale;
    const float rotation = entity->getOrientation() + textureSettings->rotation;
    glm::vec2 position = entity->getPosition();
    glm::vec2 offset = glm::vec2(textureSettings->offsetX, textureSettings->offsetY);

    EntityVertex vertices[4] = {
            {glm::rotate((glm::vec2(-size, -size) + offset * size) * scale, rotation), position, glm::vec2(0, 0)},
            {glm::rotate((glm::vec2(size, -size) + offset * size) * scale, rotation),  position, glm::vec2(1, 0)},
            {glm::rotate((glm::vec2(size, size) + offset * size) * scale, rotation),   position, glm::vec2(1, 1)},
            {glm::rotate((glm::vec2(-size, size) + offset * size) * scale, rotation),  position, glm::vec2(0, 1)},
    };

    GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(EntityVertex) * 4, vertices))

    // render player
    GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr))
}

void Game::onImGuiRender() {

    // push score styles
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(.7, .7, .7, 1));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, .1, .3, .3));
    // set up score window
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->Pos);
    ImGui::SetNextWindowSize(ImVec2(270.f, 45.f + (running ? 0.f : 20.f)));
    ImGui::Begin("Shooteroo", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);

    ImGui::Text("Press H to show help. %.1f FPS", ImGui::GetIO().Framerate);
    ImGui::Text("Score: %.2f (Best: %.2f)", score, gameSettings->scoreSettings.highscore);
    if (!running) {
        if (awaitPlay) {
            ImGui::Text("(Left-) Click anywhere to play");
        } else {
            ImGui::Text("(Left-) Click anywhere to play again");
        }
    }
    ImGui::End();

    // pop score window styles
    ImGui::PopStyleColor(2);

    if (showHelp) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(.7, .7, .7, 1));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, .1, .3, .6));

        auto mainViewport = ImGui::GetMainViewport();
        const float helpWindowWidth = 450;
        const float helpWindowHeight = 245;
        float helpWindowX = mainViewport->Pos.x + (mainViewport->Size.x - helpWindowWidth) / 2.f;
        float helpWindowY = mainViewport->Pos.y + (mainViewport->Size.y - helpWindowHeight) / 2.f;
        ImGui::SetNextWindowPos(ImVec2(helpWindowX, helpWindowY));
        ImGui::SetNextWindowSize(ImVec2(helpWindowWidth, helpWindowHeight));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.f, 1.f));

        ImGui::Begin("Help", nullptr,
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(.7, 1, .9, 1));
        ImGui::Dummy(ImVec2(130, 0));
        ImGui::SameLine(),
                ImGui::Text("Shooteroo by Kami-Kaze");
        ImGui::PopStyleColor();
        SPACE(2.f);
        ImGui::Separator();
        SPACE(8.f);
        ImGui::Text("Your objective is to survive as long as possible");
        SPACE(5.f);
        ImGui::Text("Fireballs will kill you if you touch them,");
        ImGui::Text("  so you better dodge those.");
        SPACE(5.f);
        ImGui::Text("Monsters, will kill you too if they reach you,");
        ImGui::Text("  but you can shoot those.");
        SPACE(5.f);
        ImGui::Text("Mouse Buttons:");
        ImGui::Text(" Right click: Move");
        ImGui::Text(" Left click: Shoot");
        SPACE(5.f);
        ImGui::Text("Keys:");
        ImGui::Text(" S: stop the player");
        ImGui::Text(" H: show this window");
        ImGui::Text(" O: open the option window");
        ImGui::Text(" Escape: close the game");

        ImGui::End();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(2);
    }


    if (showSettings) {
        ImGui::Begin("Settings");
        if (ImGui::BeginTabBar("TabBarName???")) {
            if (ImGui::BeginTabItem("General")) {
                ImGui::Checkbox("Invulnerable", &invulnerable);
                ImGui::SameLine();
                if (ImGui::Button("Restart Game")) {
                    running = false;
                    awaitPlay = true;
                    reset();
                }
                ImGui::DragFloat("Time multiplier", &dtMultiplier, 0.01f, .001f, 1.f);
                ImGui::DragFloat("Time increment", &gameSettings->scoreSettings.timeIncrement, .2, 0, 5);
                ImGui::DragFloat("Kill increment", &gameSettings->scoreSettings.killIncrement, .5, 0, 10);
                ImGui::Checkbox("Render hitboxes", &renderHitbox);
                if (ImGui::Button("Reset settings")) {
                    gameSettings->reset();
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Player settings")) {
                ImGui::DragFloat("Size", &gameSettings->playerSettings.size, .01, 0, 1);
                ImGui::DragFloat("Speed", &gameSettings->playerSettings.speed, .01, 0, 1);
                ImGui::DragFloat("Turning speed", &gameSettings->playerSettings.turningSpeed, .1, 0, 10);
                if (ImGui::CollapsingHeader("Texture")) {
                    ImGui::DragFloat("Scale", &gameSettings->playerSettings.texture.scale, 0.01, 0, 2);
                    ImGui::DragFloat("Rotation", &gameSettings->playerSettings.texture.rotation, 0.05, 0, 7);
                    ImVec2 texOffset = ImVec2(gameSettings->playerSettings.texture.offsetX,
                                              gameSettings->playerSettings.texture.offsetY);
                    if (ImGui::DragFloat2("Offset", &texOffset.x, 0.01, -.5, .5)) {
                        gameSettings->playerSettings.texture.offsetX = texOffset.x;
                        gameSettings->playerSettings.texture.offsetY = texOffset.y;
                    }
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Weapon settings")) {
                ImGui::DragFloat("Cooldown", &gameSettings->playerSettings.weaponSettings.cooldown, .1, 0, 2);
                ImGui::DragFloat("Range", &gameSettings->playerSettings.weaponSettings.pRange, .01, 0, 2);
                ImGui::DragFloat("Projectile size", &gameSettings->playerSettings.weaponSettings.pSize, .01, 0, 1);
                ImGui::DragFloat("Projectile speed", &gameSettings->playerSettings.weaponSettings.pSpeed, .01, 0, 1);
                if (ImGui::CollapsingHeader("Texture")) {
                    ImGui::DragFloat("Scale", &gameSettings->playerSettings.weaponSettings.texture.scale, 0.01, 0, 2);
                    ImGui::DragFloat("Rotation", &gameSettings->playerSettings.weaponSettings.texture.rotation, 0.05, 0,
                                     7);
                    ImVec2 texOffset = ImVec2(gameSettings->playerSettings.weaponSettings.texture.offsetX,
                                              gameSettings->playerSettings.weaponSettings.texture.offsetY);
                    if (ImGui::DragFloat2("Offset", &texOffset.x, 0.01, -.5, .5)) {
                        gameSettings->playerSettings.weaponSettings.texture.offsetX = texOffset.x;
                        gameSettings->playerSettings.weaponSettings.texture.offsetY = texOffset.y;
                    }
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Projectiles")) {
                ImGui::Checkbox("Spawn", &spawnProjectiles);
                ImGui::SameLine();
                if (ImGui::Button("Spawn single")) spawnProjectile();
                ImGui::DragFloat("Probability", &gameSettings->projectileSettings.probability, .05, 0, 1);
                ImGui::DragFloat("Size", &gameSettings->projectileSettings.size, .01, 0, 1);
                ImGui::DragFloat("Speed", &gameSettings->projectileSettings.speed, .01, 0, 1);
                if (ImGui::CollapsingHeader("Texture")) {
                    ImGui::DragFloat("Scale", &gameSettings->projectileSettings.texture.scale, 0.01, 0, 2);
                    ImGui::DragFloat("Rotation", &gameSettings->projectileSettings.texture.rotation, 0.05, 0, 7);
                    ImVec2 texOffset = ImVec2(gameSettings->projectileSettings.texture.offsetX,
                                              gameSettings->projectileSettings.texture.offsetY);
                    if (ImGui::DragFloat2("Offset", &texOffset.x, 0.01, -.5, .5)) {
                        gameSettings->projectileSettings.texture.offsetX = texOffset.x;
                        gameSettings->projectileSettings.texture.offsetY = texOffset.y;
                    }
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Hostiles")) {
                ImGui::Checkbox("Spawn", &spawnHostiles);
                ImGui::SameLine();
                if (ImGui::Button("Spawn single")) spawnHostile();
                ImGui::DragFloat("Probability", &gameSettings->hostileSettings.probability, .05, 0, 1);
                ImGui::DragFloat("Size small", &gameSettings->hostileSettings.sizeSmall, .01, 0, 1);
                ImGui::DragFloat("Size big", &gameSettings->hostileSettings.sizeBig, .01, 0, 1);
                ImGui::DragFloat("Speed", &gameSettings->hostileSettings.speed, .01, 0, 1);
                ImGui::DragFloat("Turning speed", &gameSettings->hostileSettings.turningSpeed, .01, 0, 1);
                if (ImGui::CollapsingHeader("Texture")) {
                    ImGui::DragFloat("Scale", &gameSettings->hostileSettings.texture.scale, 0.01, 0, 2);
                    ImGui::DragFloat("Rotation", &gameSettings->hostileSettings.texture.rotation, 0.05, 0, 7);
                    ImVec2 texOffset = ImVec2(gameSettings->hostileSettings.texture.offsetX,
                                              gameSettings->hostileSettings.texture.offsetY);
                    if (ImGui::DragFloat2("Offset", &texOffset.x, 0.01, -.5, .5)) {
                        gameSettings->hostileSettings.texture.offsetX = texOffset.x;
                        gameSettings->hostileSettings.texture.offsetY = texOffset.y;
                    }
                }
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
        ImGui::End();
    }

    // player props
}

bool Game::onKeyPressed(KeyPressedEvent& e) {
    if (e.getKeyCode() == Key::Escape) {
        Application::get().stop();
    } else if (e.getKeyCode() == Key::O) {
        showSettings = !showSettings;
        if (running) {
            setPaused(showSettings);
        }
    } else if (e.getKeyCode() == Key::H) {
        showHelp = !showHelp;
    }
    if (running) {
        if (e.getKeyCode() == Key::S) {
            player->stopMoving();
        }
    }
    return true;
}

bool Game::onMouseButtonPressed(MouseButtonPressedEvent& e) {
    if (showHelp && e.getMouseButton() == Mouse::Button0) {
        if (showHelp) {
            showHelp = false;
        }
    } else if (running) {
        glm::vec2 pos = Input::getMousePosition();
        pos.x *= (1.f) * (float) Engine::Application::get().getWindow().getViewportWidth() / BOARD_DIMENSION;
        pos.y *= (1.f) * (float) Engine::Application::get().getWindow().getViewportHeight() / BOARD_DIMENSION;
        if (e.getMouseButton() == Mouse::Button1) {
            player->setTargetLocation(pos);
        } else if (e.getMouseButton() == Mouse::Button0) {
            player->shoot(glm::normalize(pos - player->getPosition()));
        }
    } else if (e.getMouseButton() == Mouse::Button0) {
        if (awaitPlay) {
            setPaused(false);
        } else if (!running) {
            reset();
            running = true;
        }
    }
    return true;
}

void Game::setPaused(bool paused) {
    awaitPlay = paused;
    running = !paused;
}
