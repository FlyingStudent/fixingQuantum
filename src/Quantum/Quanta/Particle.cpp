#include"qtpch.h"
#include"Particle.h"
#include<math.h>
#include"Quantum/Core/Input.h"
#include"Quantum/Core/KeyCodes.h"
namespace Quantum {
	Particle::Particle()
		:m_Damping(0.99f)
	{

	}
	Particle::~Particle()
	{

	}
	void Particle::Integrate(TimeStep ts)
	{
		//we don't integrate things with finite mass.
		if (m_InverseMass <= 0.0f) return;
		QT_CORE_ASSERT(ts >= 0, "Invalid time update!");
		//Update linear position.
		m_Position += m_Velocity * glm::vec3(ts);

	// Work out the acceleration from the force
			//(We'll add to this vector when we come to generate forces.)

	/*	glm::vec3 resultingAcceleration = m_Acceleration;
			resultingAcceleration += m_ForceAccumulator * glm::vec3(m_InverseMass);*/
	
	// Update linear velocity from the acceleration.
			m_Velocity += m_Acceleration + m_ForceAccumulator * glm::vec3(m_InverseMass) * glm::vec3(ts);
			m_Velocity *= powf(m_Damping, ts);
		//Clear the forces.
		ClearAccumulator();
		//TO DO: what does this method do in here.


		
	}
	void Particle::SetMass(const double mass)
	{
		QT_CORE_ASSERT(mass != 0, "Invalid Mass!");
		Particle::m_InverseMass = 1.0 / mass;
	}
	double Particle::GetMass() const
	{
		if (m_InverseMass == 0)
		{
			return std::numeric_limits<float>::max();
		}
		else {
			return 1.0 / m_InverseMass;
		}
	}
	void Particle::SetInverseMass(const double inverseMass)
	{
		Particle::m_InverseMass = inverseMass;
	}
	double Particle::GetInverseMass() const {
		return m_InverseMass;
	}
	//(TO DO: ) -- check if this  method gives the correct behaviour
	bool Particle::HasFiniteMass() const {
		return m_InverseMass >= 0.0f;
	}
	void Particle::SetDamping(const double damping)
	{
		Particle::m_Damping = damping;
	}
	double Particle::GetDamping() const
	{
		return m_Damping;
	}
	void Particle::SetPosition(const glm::vec3& position)
	{
		Particle::m_Position = position;
	}
	glm::vec3 Particle::GetPosition() const
	{
		return m_Position;
	}
	void Particle::SetVelocity(const glm::vec3& velocity)
	{
		Particle::m_Velocity = velocity;
	}
	glm::vec3 Particle::GetVelocity() const {
		return m_Velocity;
	}
	void Particle::SetAcceleration(const glm::vec3& acceleration)
	{
		Particle::m_Acceleration = acceleration;
	}
	glm::vec3 Particle::GetAcceleration() const {
		return m_Acceleration;
	}
	void Particle::ClearAccumulator()
	{
		m_ForceAccumulator = glm::vec3(0.0f);
	}
	void Particle::AddForce(const glm::vec3& force)
	{
		m_ForceAccumulator += force;
	}
	
}
