#include "Game.h"

#include "../Resources/ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
//#include <chrono>

Game::Game(const glm::ivec2& windowSize) 
    : m_eCurrentGameState(EGameState::Active)
    , m_windowSize(windowSize)
{
	m_keys.fill(false);
}

Game::~Game()
{

}

void Game::render()
{
    ResourceManager::getAnimatedSprite("NewAnimatedSprite")->render();
}

void Game::update(const uint64_t delta)
{
    ResourceManager::getAnimatedSprite("NewAnimatedSprite")->update(delta);
}

void Game::setKey(int key, const int action)
{
	m_keys[key] = action;
}

bool Game::init()
{
    auto pDefaultShaderProgram = ResourceManager::loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
    if (!pDefaultShaderProgram)
    {
        std::cout << "Can't create shader program: " << "DefaultShader" << std::endl;
        return false;
    }


    // Sprite initializing START
    auto pSpriteShaderProgram = ResourceManager::loadShaders("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
    if (!pSpriteShaderProgram)
    {
        std::cout << "Can't create shader program: " << "SpriteShader" << std::endl;
        return false;
    }

    auto tex = ResourceManager::loadTexture("DefaultTexture", "res/textures/map_16x16.png");




    // subtextures list
    std::vector<std::string> subTextureNames = { "eagle", "deadEagle", "star0", "star1", "star2", "star3", "star4", "star5" };
    auto pTextureAtlas = ResourceManager::loadTextureAtlas("DefaultTextureAtlas", "res/textures/map_16x16.png", std::move(subTextureNames), 16, 16);

    auto pSprite = ResourceManager::loadSprite("NewSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "eagle");
    pSprite->setPosition(glm::vec2(300, 100));

    auto pAnimatedSprite = ResourceManager::loadAnimatedSprite("NewAnimatedSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "eagle");
    pAnimatedSprite->setPosition(glm::vec2(300, 300));

    std::vector<std::pair<std::string, uint64_t>> eagleState;
    eagleState.emplace_back(std::make_pair<std::string, uint64_t>("eagle", 1000000000));
    eagleState.emplace_back(std::make_pair<std::string, uint64_t>("deadEagle", 1000000000));

    std::vector<std::pair<std::string, uint64_t>> starState;
    starState.emplace_back(std::make_pair<std::string, uint64_t>("star0", 1000000000));
    starState.emplace_back(std::make_pair<std::string, uint64_t>("star1", 1000000000));
    starState.emplace_back(std::make_pair<std::string, uint64_t>("star2", 1000000000));
    starState.emplace_back(std::make_pair<std::string, uint64_t>("star3", 1000000000));
    starState.emplace_back(std::make_pair<std::string, uint64_t>("star4", 1000000000));
    starState.emplace_back(std::make_pair<std::string, uint64_t>("star5", 1000000000));

    pAnimatedSprite->insertState("eagleState", std::move(eagleState));
    pAnimatedSprite->insertState("starState", std::move(starState));

    pAnimatedSprite->setState("eagleState");








    // Transformation matrix
    // for the 1st triangle
    glm::mat4 modelMatrix_1 = glm::mat4(1.f);
    modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.f, 200.f, 0.f));

    // for the 2nd triangle
    glm::mat4 modelMatrix_2 = glm::mat4(1.f);
    modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.f, 50.f, 0.f));

    // orthographic projection matrix
    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_windowSize.x), 0.f, static_cast<float>(m_windowSize.y), -100.f, 100.f);


    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);


    pDefaultShaderProgram->use();
    pDefaultShaderProgram->setInt("tex", 0);
    pDefaultShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    return true;

}