#include "Level.h"

#include "../Resources/ResourceManager.h"
#include "GameObjects/BrickWall.h"

#include <iostream>


static const unsigned int BLOCK_SIZE = 16;

std::shared_ptr<IGameObject> createGameObjectFromDescription(const char description, const glm::vec2& position, const glm::vec2& size, const float rotation = 0.0f)
{
	switch (description)
	{
	case  '0':
		return std::make_shared<BrickWall>(ResourceManager::getSprite("brickWall"), position, size, rotation);
	/*case  '1':
	case  '2':*/
	case  '3':
		return std::make_shared<BrickWall>(ResourceManager::getSprite("brickWall_top"), position, size, rotation);
	/*case  '4':
	case  '5':
	case  '6':
	case  '7':
	case  '8':
	case  '9':
	case  'A':
	case  'B':*/
	case  'C':
		return std::make_shared<BrickWall>(ResourceManager::getSprite("brickWall_bottom"), position, size, rotation);
	/*case  'D':
	case  'E':*/
	case  'F':
		return nullptr;
		//return std::make_shared<BrickWall>(ResourceManager::getSprite("nothing"), position, size, rotation);
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

	m_mapObjects.reserve(m_width * m_height);
	unsigned int currentBottomOffset = static_cast<unsigned int>(BLOCK_SIZE * (m_height - 1));
	for (const std::string& currentRow : levelDescription)
	{
		unsigned int currentLeftOffset = 0;
		for (const char currentElement : currentRow)
		{
			m_mapObjects.emplace_back(createGameObjectFromDescription(currentElement, glm::vec2(currentLeftOffset, currentBottomOffset), glm::vec2(BLOCK_SIZE, BLOCK_SIZE), 0.0f));

			currentLeftOffset += BLOCK_SIZE;
		}
		currentBottomOffset -= BLOCK_SIZE;
	}


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