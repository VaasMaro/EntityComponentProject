#include "Arrow.h"
#include "LifeComponent.h"
Arrow::Arrow()
{
}

Arrow::Arrow(PositionComponent *position, ProjectileGraphicsComponent *graphics, ProjectilePhysicsComponent *physics, Damage damage, ManagersAccess *managerAccess, const float &rotation)
{
	// manager Access
	m_managerAccess = managerAccess;
	// Rotation
	m_rotation = rotation;
	// Position Component
	m_position = position;
	// Graphics
	m_graphics = graphics;
	m_graphics->setId(m_managerAccess->drawManager->subscribe(m_graphics));
	m_graphics->setRotation(m_rotation);
	// Damage
	m_rotation = (m_rotation + 90) * 0.01745329252; // degree to radian
	sf::Vector2f direction(sin(m_rotation), -cos(m_rotation));
	m_damage = damage;
	m_damage.direction = direction;
	// physics
	m_physics = physics;

	m_physics->initialize(m_managerAccess, direction);
	m_physics->setId(m_managerAccess->physicsHandler->subscribe(m_physics));
}

Arrow::~Arrow()
{
	delete m_position;
	m_managerAccess->physicsHandler->reject(m_physics->getId());
	delete m_physics;
	m_managerAccess->drawManager->reject(m_graphics->getId());
	delete m_graphics;
}

void Arrow::update(sf::Time time)
{

	if (m_physics->isTouching())
	{
		m_fadeTime += time;
		if (m_fadeTime.asSeconds() > 3.0f)
		{
			alive = false;
		}
	}
	else
	{
		m_physics->update(time, *m_position);

	}
	m_graphics->setPosition(*m_position);
	m_graphics->update(time);
	if (m_physics->getEntityHit() != nullptr)
	{
		m_physics->getEntityHit()->getLifeComponent()->hit(m_damage);
		alive = false;
	}
}

