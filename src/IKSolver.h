#pragma once

#include <glad/glad.h>

class IKSolver
{
public:
	glm::vec2 first, second, last;

private:
	const GLfloat pi = glm::pi<GLfloat>();

public:
	void solve(const GLfloat l1, const GLfloat l2, const glm::vec2 base, const glm::vec2 target)
	{
		glm::vec2 end_effector = clamp_distance(target - base, l1, l2);
		GLfloat effector_vector_angle = atan2(end_effector.y, end_effector.x);
		effector_vector_angle = glm::clamp(effector_vector_angle, 0.0f, glm::two_pi<GLfloat>());

		const GLfloat effector_squared = end_effector.x * end_effector.x + end_effector.y * end_effector.y;

		// Calculate angles using the Law of Cosines
		GLfloat angle1 = acos((l1 * l1 - l2 * l2 + effector_squared) / (2.0f * l1 * sqrt(effector_squared))) + effector_vector_angle;
		if (std::isnan(angle1))
			angle1 = 0.0f;

		GLfloat angle2 = acos((l1 * l1 + l2 * l2 - effector_squared) / (2.0f * l1 * l2));
		if (std::isnan(angle1))
			angle1 = 0.0f;

		// Calculate positions
		first = base;
		second = first + glm::vec2(cos(angle1) * l1, sin(angle1) * l1);
		last = second + glm::vec2(cos(pi - (-angle2 - angle1)) * l2, sin(pi - (2 * pi - angle2 - angle1)) * l2);
	}

private:
	// Clamp to prevent stretching
	glm::vec2 clamp_distance(const glm::vec2 end_effector, const GLfloat l1, const GLfloat l2)
	{
		const GLfloat distance_clamped = std::max(std::abs(l1 - l2), std::min(l1 + l2, glm::length(end_effector)));
		return end_effector * distance_clamped / glm::length(end_effector);
	}
};
