export module renderer;

import std;

import <glad/glad.h>;
import <glm/vec3.hpp>;
import <glm/vec4.hpp>;
import <glm/gtc/matrix_transform.hpp>;

import camera;
import colors;
import shader;
import simulation;
import spring;
import object;
import weight;


/// <summary>
/// Responsible for rendering the scene using OpenGL
/// </summary>
export class Renderer
{
public:
	friend class GuiController;

	Renderer(int windowWidth, int windowHeight, Camera& camera, SimulationEngine& simulation) :
		windowWidth(windowWidth), windowHeight(windowHeight), camera(camera), simulation(simulation)
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_PROGRAM_POINT_SIZE);

		// Create objects
		spring = std::make_unique<Spring>();
		weight = std::make_unique<Weight>();
	}

	/// <summary>
	/// Render the scene
	/// </summary>
	void draw(const SimulationData& data) const
	{
		// Clear
		glViewport(0, 0, windowWidth, windowHeight);
		glClearColor(0.15f, 0.0f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use the data from the simulation
		auto springModel = glm::translate(glm::mat4{ 1.0f }, { 0.0f, -data.w, 0.0f }) * glm::scale(glm::mat4{ 1.0f }, { 1.0f, data.position - data.w, 1.0f});
		spring->setModel(springModel);
		auto weightModel = glm::translate(glm::mat4{ 1.0f }, { 0.0f, -data.position, 0.0f });
		weight->setModel(weightModel);

		// draw
		uniformColorShader->use();
		uniformColorShader->setMatrix("view", camera.getView());
		uniformColorShader->setMatrix("projection", camera.getProjection());

		spring->draw(uniformColorShader.get());
		weight->draw(uniformColorShader.get());
	}

	void setWindowSize(int width, int height)
	{
		windowWidth = width;
		windowHeight = height;
		glViewport(0, 0, windowWidth, windowHeight);
	}

private:
	Camera& camera;
	SimulationEngine& simulation;

	std::unique_ptr<Shader> uniformColorShader = std::make_unique<UniformColorShader>();

	std::unique_ptr<Object> spring;
	std::unique_ptr<Object> weight;

	int windowWidth;
	int windowHeight;
};