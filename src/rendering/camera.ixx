export module camera;

import std;

import <glm/vec3.hpp>;
import <glm/mat4x4.hpp>;
import <glm/gtc/constants.hpp>;
import <glm/ext/matrix_clip_space.hpp>;
import <glm/ext/matrix_transform.hpp>;


export class Camera
{
public:
	Camera(const glm::vec3& initialPosition, const glm::vec3& center) : translation(initialPosition), center(center)
	{
		update();
	}

	inline const glm::vec3& getPosition() const
	{
		return translation;
	}

	inline const glm::mat4& getView() const
	{
		return view;
	}

	inline const glm::mat4& getProjection() const
	{
		return projection;
	}

private:
	friend class GuiController;

	inline static const glm::vec3 initialRight{ 1.0f, 0.0f, 0.0f };
	inline static const glm::vec3 initialUp{ 0.0f, 1.0f, 0.0f };
	inline static const glm::vec3 initialFront{ 0.0f, 0.0f, -1.0f };

	glm::vec3 translation;
	glm::vec3 center;

	glm::mat4 view{ 1.0f };
	glm::mat4 projection { 1.0f };

	void update()
	{
		view = glm::lookAt(translation, center, initialUp);
	}
};