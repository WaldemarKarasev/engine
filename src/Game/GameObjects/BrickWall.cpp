#include "BrickWall.h"

#include "../../Resources/ResourceManager.h"
#include "../../Renderer/Sprite.h"

BrickWall::BrickWall(const EBrickWallType eBrickWallType, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
	: IGameObject(IGameObject::EObjectType::BrickWall, position, size, rotation, layer)
	, m_eCurrentBrickState{ EBrickState::Destroyed,
						    EBrickState::Destroyed, 
						    EBrickState::Destroyed, 
						    EBrickState::Destroyed }
	, m_blockOffsets { glm::vec2(0, m_size.y / 2.0f),
					   glm::vec2(m_size.x / 2.0f, m_size.y / 2.0f),
					   glm::vec2(0.0f, 0.0f),
					   glm::vec2(m_size.x / 2.0f, 0) }
{
	m_sprites[static_cast<size_t>(EBrickState::All)]				 = ResourceManager::getSprite("brickWall_All");
	m_sprites[static_cast<size_t>(EBrickState::TopLeft)]			 = ResourceManager::getSprite("brickWall_TopLeft");
	m_sprites[static_cast<size_t>(EBrickState::TopRight)]			 = ResourceManager::getSprite("brickWall_TopRight");
	m_sprites[static_cast<size_t>(EBrickState::Top)]				 = ResourceManager::getSprite("brickWall_Top");
	m_sprites[static_cast<size_t>(EBrickState::BottomLeft)]			 = ResourceManager::getSprite("brickWall_BottomLeft");
	m_sprites[static_cast<size_t>(EBrickState::Left)]				 = ResourceManager::getSprite("brickWall_Left");
	m_sprites[static_cast<size_t>(EBrickState::TopRight_BottomLeft)] = ResourceManager::getSprite("brickWall_TopRight_BottomLeft");
	m_sprites[static_cast<size_t>(EBrickState::Top_BottomLeft)]		 = ResourceManager::getSprite("brickWall_Top_BottomLeft");
	m_sprites[static_cast<size_t>(EBrickState::BottomRight)]		 = ResourceManager::getSprite("brickWall_BottomRight");
	m_sprites[static_cast<size_t>(EBrickState::TopLeft_BottomRight)] = ResourceManager::getSprite("brickWall_TopLeft_BottomRight");
	m_sprites[static_cast<size_t>(EBrickState::Right)]				 = ResourceManager::getSprite("brickWall_Right");
	m_sprites[static_cast<size_t>(EBrickState::Top_BottomRight)]	 = ResourceManager::getSprite("brickWall_Top_BottomRight");
	m_sprites[static_cast<size_t>(EBrickState::Bottom)]				 = ResourceManager::getSprite("brickWall_Bottom");
	m_sprites[static_cast<size_t>(EBrickState::TopLeft_Bottom)]		 = ResourceManager::getSprite("brickWall_TopLeft_Bottom");
	m_sprites[static_cast<size_t>(EBrickState::TopRight_Bottom)]	 = ResourceManager::getSprite("brickWall_TopRight_Bottom");

	//	std::array<EBrickState, 4> m_eCurrentBrickState;

	switch (eBrickWallType)
	{
	case EBrickWallType::All:
		m_eCurrentBrickState.fill(EBrickState::All);
		m_collides.emplace_back(glm::vec2(0), m_size);
		break;

	case EBrickWallType::Top:
		m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::TopLeft)] = EBrickState::All;
		m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::TopRight)] = EBrickState::All;
		m_collides.emplace_back(glm::vec2(0, m_size.y / 2), m_size);
		break;
	
	case EBrickWallType::Bottom:
		m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::BottomLeft)] = EBrickState::All;
		m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::BottomRight)] = EBrickState::All;
		m_collides.emplace_back(glm::vec2(0), glm::vec2(m_size.x, m_size.y / 2));
		break;

	case EBrickWallType::Left:
		m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::TopLeft)] = EBrickState::All;
		m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::BottomLeft)] = EBrickState::All;
		m_collides.emplace_back(glm::vec2(0), glm::vec2(m_size.x / 2, m_size.y));
		break;

	case EBrickWallType::Right:
		m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::TopRight)] = EBrickState::All;
		m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::BottomRight)] = EBrickState::All;
		m_collides.emplace_back(glm::vec2(m_size.x / 2, 0), m_size);
		break;

	case EBrickWallType::TopLeft:
		m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::TopLeft)] = EBrickState::All;
		m_collides.emplace_back(glm::vec2(0, m_size.y / 2), glm::vec2(m_size.x / 2, m_size.y));
		break;

	case EBrickWallType::TopRight:
		m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::TopRight)] = EBrickState::All;
		m_collides.emplace_back(glm::vec2(m_size.x / 2, m_size.y / 2), m_size);
		break;

	case EBrickWallType::BottomLeft:
		m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::BottomLeft)] = EBrickState::All;
		m_collides.emplace_back(glm::vec2(0), glm::vec2(m_size.x / 2, m_size.y / 2));
		break;

	case EBrickWallType::BottomRight:
		m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::BottomRight)] = EBrickState::All;
		m_collides.emplace_back(glm::vec2(m_size.x / 2, 0), glm::vec2(m_size.x, m_size.y / 2));
		break;
	}
}


// private func
void BrickWall::renderBrickWall(const EBrickLocation eBrickLocation) const
{
	const EBrickState state = m_eCurrentBrickState[static_cast<size_t>(eBrickLocation)];
	if (state != EBrickState::Destroyed)
	{
		m_sprites[static_cast<size_t>(state)]->render(m_position + m_blockOffsets[static_cast<size_t>(eBrickLocation)], m_size / 2.0f, m_rotation, m_layer);
	}
}

void BrickWall::render() const
{
	renderBrickWall(EBrickLocation::TopLeft);
	renderBrickWall(EBrickLocation::TopRight);
	renderBrickWall(EBrickLocation::BottomLeft);
	renderBrickWall(EBrickLocation::BottomRight);
}

void BrickWall::update(const double delta)
{
	// no update for this object
}