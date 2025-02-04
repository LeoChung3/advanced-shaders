#pragma once

#include <glm/glm.hpp>

#include "gameObjects/actor.hpp"
#include "windows/GLFWWindowImpl.hpp"
#include "core/log.hpp"

class CameraScript : public Script
{
public:
	CameraScript(Actor& actor, GLFWWindowImpl& win, const glm::vec3& movementSpeed, float turnSpeed) :
		Script(actor),
		m_winRef(win),
		m_movementSpeed(movementSpeed),
		m_turnSpeed(turnSpeed)
	{}
	virtual void onUpdate(float timestep) override;
	virtual void onKeyPress(KeyPressedEvent& e) override {};
	virtual void onKeyRelease(KeyReleasedEvent& e) override {};
private:
	GLFWWindowImpl& m_winRef;
	glm::vec3 m_movementSpeed{ 30.f, 30.f, 30.f };
	float m_turnSpeed{ 1.5f };
};
