#include "Ice.h"

#include "../../Resources/ResourceManager.h"
#include "../../Renderer/Sprite.h"

Ice::Ice(const glm::vec2& position, const glm::vec2& size, const float rotation)
	: IGameObject(position, size, rotation)
	, m_sprite(ResourceManager::getSprite("ice"))
	, m_blockOffsets { glm::vec2(0, m_size.y / 2.0f),
					   glm::vec2(m_size.x / 2.0f, m_size.y / 2.0f),
					   glm::vec2(0.0f, 0.0f),
					   glm::vec2(m_size.x / 2.0f, 0) }
{
	
}


// private func
void Ice::renderBlock(const EBlockLocation eBlockLocation) const
{
	m_sprite->render(m_position + m_blockOffsets[static_cast<size_t>(eBlockLocation)], m_size / 2.0f, m_rotation);
	
}

void Ice::render() const
{
	renderBlock(EBlockLocation::TopLeft);
	renderBlock(EBlockLocation::TopRight);
	renderBlock(EBlockLocation::BottomLeft);
	renderBlock(EBlockLocation::BottomRight);
}
