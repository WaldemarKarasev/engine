#include "Eagle.h"

#include "../../Resources/ResourceManager.h"
#include "../../Renderer/Sprite.h"

Eagle::Eagle(const glm::vec2& position, const glm::vec2& size, const float rotation)
	: IGameObject(position, size, rotation)
	, m_sprites{ ResourceManager::getSprite("eagle"),
				 ResourceManager::getSprite("eagle_dead")}
	, m_eCurrentstate(EEagleState::Alive)
{
	
}

void Eagle::render() const
{
	m_sprites[static_cast<size_t>(m_eCurrentstate)]->render(m_position, m_size, m_rotation);
}

void Eagle::update(const uint64_t delta)
{

}