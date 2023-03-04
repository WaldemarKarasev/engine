#include "Level.h"

//#include "../Resources/ResourceManager.h"
#include "GameObjects/BrickWall.h"
#include "GameObjects/BetonWall.h"
#include "GameObjects/Water.h"
#include "GameObjects/Trees.h"
#include "GameObjects/Ice.h"
#include "GameObjects/Eagle.h"
#include "GameObjects/Border.h"

#include <iostream>


std::shared_ptr<IGameObject> createGameObjectFromDescription(const char description, const glm::vec2& position, const glm::vec2& size, const float rotation = 0.0f)
{
	switch (description)
	{
	case  '0':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::Right, position, size, rotation, 0.0f);
	case  '1':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::Bottom, position, size, rotation, 0.0f);
	case  '2':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::Left, position, size, rotation, 0.0f);
	case  '3':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::Top, position, size, rotation, 0.0f);
	case  '4':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::All, position, size, rotation, 0.0f);
	case  'G':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::BottomLeft, position, size, rotation, 0.0f);
	case  'H':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::BottomRight, position, size, rotation, 0.0f);
	case  'I':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::TopLeft, position, size, rotation, 0.0f);
	case  'J':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::TopRight, position, size, rotation, 0.0f);
	
	case  '5':
		return std::make_shared<BetonWall>(BetonWall::EBetonWallType::Right, position, size, rotation, 0.0f);
	case  '6':
		return std::make_shared<BetonWall>(BetonWall::EBetonWallType::Bottom, position, size, rotation, 0.0f);
	case  '7':
		return std::make_shared<BetonWall>(BetonWall::EBetonWallType::Left, position, size, rotation, 0.0f);
	case  '8':
		return std::make_shared<BetonWall>(BetonWall::EBetonWallType::Top, position, size, rotation, 0.0f);
	case  '9':
		return std::make_shared<BetonWall>(BetonWall::EBetonWallType::All, position, size, rotation, 0.0f);


	case  'A':
		return std::make_shared<Water>(position, size, rotation, 0.0f);
	case  'B':
		return std::make_shared<Trees>(position, size, rotation, 1.0f);
	case  'C':
		return std::make_shared<Ice>(position, size, rotation, -1.0f);
	case  'E':
		return std::make_shared<Eagle>(position, size, rotation, 0.0f);


	case  'D':
		return nullptr;
	default:
		std::cout << "Unknown GameObject description" << description << std::endl;
		break;
	}

	return nullptr;
}

Level::Level(const std::vector<std::string>& levelDescription)
{
	if (levelDescription.empty())
	{
		std::cout << "Empty level description!" << std::endl;
	}

	m_width = levelDescription[0].length();
	m_height = levelDescription.size();

	m_playerRespawn_1 = { BLOCK_SIZE * (m_width / 2 - 1), BLOCK_SIZE / 2 };
	m_playerRespawn_2 = { BLOCK_SIZE * (m_width / 2 + 3), BLOCK_SIZE / 2 };
	m_enemyRespawn_1 =  { BLOCK_SIZE,					  m_height * BLOCK_SIZE - BLOCK_SIZE / 2};
	m_enemyRespawn_2 =  { BLOCK_SIZE * (m_width / 2 + 1), m_height * BLOCK_SIZE - BLOCK_SIZE / 2 };
	m_enemyRespawn_3 =  { BLOCK_SIZE *  m_width,		  m_height * BLOCK_SIZE - BLOCK_SIZE / 2 };

	// + 4 is for 4 border sprites
	m_mapObjects.reserve(m_width * m_height + 4);
	unsigned int currentBottomOffset = static_cast<unsigned int>(BLOCK_SIZE * (m_height - 1) + BLOCK_SIZE / 2.0f);
	for (const std::string& currentRow : levelDescription)
	{
		unsigned int currentLeftOffset = BLOCK_SIZE;
		for (const char currentElement : currentRow)
		{
			switch (currentElement)
			{
			case 'K':
				m_playerRespawn_1 = { currentLeftOffset, currentBottomOffset };
				break;

			case 'L':
				m_playerRespawn_2 = { currentLeftOffset, currentBottomOffset };
				break;

			case 'M':
				m_enemyRespawn_1 = { currentLeftOffset, currentBottomOffset };
				break;

			case 'N':
				m_enemyRespawn_2 = { currentLeftOffset, currentBottomOffset };
				break;

			case 'O':
				m_enemyRespawn_3 = { currentLeftOffset, currentBottomOffset };
				break;

			default:
				m_mapObjects.emplace_back(createGameObjectFromDescription(currentElement, glm::vec2(currentLeftOffset, currentBottomOffset), glm::vec2(BLOCK_SIZE, BLOCK_SIZE), 0.0f));
				break;
			}

			currentLeftOffset += BLOCK_SIZE;
		}
		currentBottomOffset -= BLOCK_SIZE;
	}

	// bottom bordder
	m_mapObjects.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, 0.0f), glm::vec2(m_width * BLOCK_SIZE, BLOCK_SIZE / 2.0f), 0.0f, 0.0f));

	// top border
	m_mapObjects.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, m_height * BLOCK_SIZE + BLOCK_SIZE / 2.0f), glm::vec2(m_width * BLOCK_SIZE, BLOCK_SIZE / 2.0f), 0.0f, 0.0f));

	// left border
	m_mapObjects.emplace_back(std::make_shared<Border>(glm::vec2( 0.0f, 0.0f), glm::vec2(BLOCK_SIZE, (m_height + 1) * BLOCK_SIZE), 0.0f, 0.0f));

	// right border
	m_mapObjects.emplace_back(std::make_shared<Border>(glm::vec2((m_width + 1) * BLOCK_SIZE, 0.0f), glm::vec2(2.0f * BLOCK_SIZE, (m_height + 1) * BLOCK_SIZE), 0.0f, 0.0f));

}

void Level::render() const
{
	for (const auto& currentMaoObject : m_mapObjects)
	{
		if (currentMaoObject)
		{
			currentMaoObject->render();
		}
	}
}

void Level::update(const uint64_t delta)
{
	for (const auto& currentMaoObject : m_mapObjects)
	{
		if (currentMaoObject)
		{
			currentMaoObject->update(delta);
		}
	}
}

size_t Level::getLevelWidth() const
{
	return (m_width + 3) * BLOCK_SIZE;
}

size_t Level::getLevelHeight() const
{
	return (m_height + 1) * BLOCK_SIZE;
}
