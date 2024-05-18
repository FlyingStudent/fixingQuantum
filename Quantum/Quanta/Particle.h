#pragma once
#include"Quantum/Core/TimeStep.h"
#include <glm/glm.hpp>
/*
A particle is the simplest object that can be simulated in the physics system.
*/
namespace Quantum {
	class Particle {
	public:
		Particle();
		~Particle();
	 /*
	 Integrates the particle forward in time by the given amount.
	 This function uses a Newton-Euler integration method, which is a linear approximation to the correct integral. For this reason it may be inaccurate in some cases.
	 */

	 void Integrate(TimeStep ts);
	 /*
	 sets the mass of the particle.
	 the new mass of the body. This may not be zero.
	 small masses can produce unstable rigid bodies under simulation.
	 */
	 void SetMass(const double mass);
	 /*
	 gets the mass of the particle.
	 */
	 double GetMass() const;
	 /*
	 sets the inverse mass of the particle.
	 the new inverse mass of the body. This may be zero, for a body with infinite mass( i.e. unmovable).
	 @warning This invalidates internal data for the particle.
	 Either an integration function, or the calculateInternals function should be called before trying to get any settings from the particle.
	 */
	 void SetInverseMass(const double inverseMass);
	 /*
	 gets the inverse mass of the particle.
	 @return the current inverse mass of the particle.
	 */
	 double GetInverseMass() const;
	 /*
	 
	 returns true if the mass of the particle is not-infinite.
	 */
	 bool HasFiniteMass() const;
	 /*
	 sets the damping of the particle.
	 */
	 void SetDamping(const double damping);
	 /*
	 gets the current damping value
	 */
	 double GetDamping() const;
	 /*
	 sets the position of the particle.
	 */
	 void SetPosition( const glm::vec3& position);
	 /*
	 fills the given vector with the position of the particle */
	 glm::vec3 GetPosition() const;
	 /*
	 sets the velocity of the particle.
	 */
	 void SetVelocity(const glm::vec3& velocity);
	 /*
	 gets the velocity of the particle.
	 */
	 glm::vec3 GetVelocity() const;
	 /*
	 * sets the constant acceleration of the particle.
	 */
	 void SetAcceleration(const glm::vec3& acceleration);
	 /*
	 gets the acceleration of the particle.
	 */
	 glm::vec3 GetAcceleration() const;
	 /*
	 * 
	 * 
	 Clears all the applied to the particle. This will be called automatically after each integration step. 
	 */
	 void ClearAccumulator();
	 /*
	 adds the given force to the particle, to be applied at the next iteration only
	 */
	 void AddForce(const glm::vec3& force);
	protected:
		/*
		Holds the linear position of the particle in world space.
		*/
		glm::vec3 m_Position = glm::vec3(0.0f);
		/*
		Holds the linear velocity of the particle in world space
		*/
		glm::vec3 m_Velocity = glm::vec3(0.0f);
		/*
		Holds the acceleration of the particle. This value can be used to set acceleration due to gravity (its primary use), or any other constant acceleration
		*/
		glm::vec3 m_Acceleration = glm::vec3(0.0f);
		/*
		Holds the amount of damping applied to linear motion. Damping is required to remove energy added through numerical instability in the integrator.
		*/
		double m_Damping;
		/*
		Holds the inverse of the mass of the particle. It is ore useful to hold the inverse mass because integration is simpler, and because in
		real-time simulation it is more useful to have objects with infinite mass (immovable) than zero mass ( completely unstable in numerical
		simulation)
		*/
		double m_InverseMass = 0.0f;

		/*
		Holds the accumulated force to be applied at the next
		simulation iteration only. This value is zeroed at each integration step.
		*/
		glm::vec3 m_ForceAccumulator = glm::vec3(0.0f);
		/* anti gravity to make stuff float if the space bar were pressed*/
		bool anti_Gravity = false;

	};

}
