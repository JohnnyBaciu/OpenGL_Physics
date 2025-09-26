#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GlFW/glfw3.h>
#include <shader/shader.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <random>

static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

glm::vec3 floatToColor(float value);

int width = 800;
int length = 600;

float center[] = {-1.27f, 0.0f, 0.0f, 
		-1.14f, 0.0f, 0.0f, 
		-1.0f, 0.0f, 0.0f,
		-0.85f, 0.0f, 0.0f,
		-0.72f, 0.0f, 0.0f,
		-0.585f, 0.0f, 0.0f, 
		-0.45f, 0.0f, 0.0f,
		-0.32f, 0.0f, 0.0f, 
		-0.19f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.27f, 0.0f, 0.0f,
		1.14f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.85f, 0.0f, 0.0f,
		0.72f, 0.0f, 0.0f,
		0.585f, 0.0f, 0.0f,
		0.45f, 0.0f, 0.0f,
		0.32f, 0.0f, 0.0f,
		0.19f, 0.0f, 0.0f,
		0.065, 0.0f, 0.0f,
		-1.27f, 0.5f, 0.0f, 
		-1.14f, 0.5f, 0.0f, 
		-1.0f, 0.5f, 0.0f,
		-0.85f, 0.5f, 0.0f,
		-0.72f, 0.5f, 0.0f,
		-0.585f, 0.5f, 0.0f, 
		-0.45f, 0.5f, 0.0f,
		-0.32f, 0.5f, 0.0f, 
		-0.19f, 0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		1.27f, 0.5f, 0.0f,
		1.14f, 0.5f, 0.0f,
		1.0f, 0.5f, 0.0f,
		0.85f, 0.5f, 0.0f,
		0.72f, 0.5f, 0.0f,
		0.585f, 0.5f, 0.0f,
		0.45f, 0.5f, 0.0f,
		0.32f, 0.5f, 0.0f,
		0.19f, 0.5f, 0.0f,
		0.065, 0.5f, 0.0f,
		-1.14f, 0.7f, 0.0f, 
		-1.0f, 0.7f, 0.0f,
		-0.85f, 0.7f, 0.0f,
		-0.72f, 0.7f, 0.0f,
		-0.585f, 0.7f, 0.0f, 
		-0.45f, 0.7f, 0.0f,
		-0.32f, 0.7f, 0.0f, 
		-0.19f, 0.7f, 0.0f,
		0.0f, 0.7f, 0.0f,
		1.27f, 0.7f, 0.0f,
		1.14f, 0.7f, 0.0f,
		1.0f, 0.7f, 0.0f,
		0.85f, 0.7f, 0.0f,
		0.72f, 0.7f, 0.0f,
		0.585f, 0.7f, 0.0f,
		0.45f, 0.7f, 0.0f,
		0.32f, 0.7f, 0.0f,
		0.19f, 0.7f, 0.0f,
		0.065, 0.7f, 0.0f,


		-1.27f, -0.5f, 0.0f, 
		-1.14f, -0.5f, 0.0f, 
		-1.0f, -0.5f, 0.0f,
		-0.85f, -0.5f, 0.0f,
		-0.72f, -0.5f, 0.0f,
		-0.585f, -0.5f, 0.0f, 
		-0.45f, -0.5f, 0.0f,
		-0.32f, -0.5f, 0.0f, 
		-0.19f, -0.5f, 0.0f,
		0.0f, -0.5f, 0.0f,
		1.27f, -0.5f, 0.0f,
		1.14f, -0.5f, 0.0f,
		1.0f, -0.5f, 0.0f,
		0.85f, -0.5f, 0.0f,
		0.72f, -0.5f, 0.0f,
		0.585f, -0.5f, 0.0f,
		0.45f, -0.5f, 0.0f,
		0.32f, -0.5f, 0.0f,
		0.19f, -0.5f, 0.0f,
		0.065, -0.5f, 0.0f,
		-1.14f, -0.7f, 0.0f, 
		-1.0f, -0.7f, 0.0f,
		-0.85f, -0.7f, 0.0f,
		-0.72f, -0.7f, 0.0f,
		-0.585f, -0.7f, 0.0f, 
		-0.45f, -0.7f, 0.0f,
		-0.32f, -0.7f, 0.0f, 
		-0.19f, -0.7f, 0.0f,
		0.0f, -0.7f, 0.0f,
		1.27f, -0.7f, 0.0f,
		1.14f, -0.7f, 0.0f,
		1.0f, -0.7f, 0.0f,
		0.85f, -0.7f, 0.0f,
		0.72f, -0.7f, 0.0f,
		0.585f, -0.7f, 0.0f,
		0.45f, -0.7f, 0.0f,
		0.32f, -0.7f, 0.0f,
		0.19f, -0.7f, 0.0f,
		0.065, -0.7f, 0.0f


		};

constexpr int numCircles = sizeof(center)/sizeof(center[0])/3;


glm::vec3 velocity[numCircles];
glm::vec3 deltaVelo[numCircles];
glm::vec3 position[numCircles];




Shader* globalShader;
std::vector<float> myVector;

int main() {


	for (int i = 0; i < numCircles; i++) {
		velocity[i] = glm::vec3(0.0f, 0.0f, 0.0f);
		deltaVelo[i] = glm::vec3(0.0f, 0.0f, 0.0f);
		position[i] = glm::vec3(center[(i+1)*3-3], center[(i+1)*3-2], center[(i+1)*3-1]);
	}



	if (!glfwInit()){
		std::cout << "glfw initialization failed!" << std::endl;
	}	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow(width, length, "Test", nullptr, nullptr);
	if (!window){
		std::cout << "window creation failed" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glViewport(0,0, width, length);



	int steps = 100;
	float radius = 0.0625f;


// Push center vertex once
myVector.push_back(0.0f);  // center x
myVector.push_back(0.0f);  // center y
myVector.push_back(0.0f);  // center z

for(int j = 0; j <= steps; j++){ // <= steps ensures closure
    float angle = 2.0f * 3.14159265f * ((float)j / (float)steps);
    float x = sin(angle) * radius;
    float y = cos(angle) * radius;
    myVector.push_back(x);
    myVector.push_back(y);
    myVector.push_back(0.0f);
}


	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, myVector.size()*sizeof(float), myVector.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	Shader shader("shader.vs", "shader.fs");
	globalShader = &shader;
	shader.use();
	glm::mat4 projection = glm::ortho(-((float)width/(float)length), ((float)width/(float)length), -1.0f, 1.0f, -1.0f, 1.0f );
	shader.setMat4("projection", projection);




	float lastTime = (float)glfwGetTime();
	float frameTime = 0.016f;
	float accumulator = 0.0f;

	while (!glfwWindowShouldClose(window)){
		float currentTime = (float)glfwGetTime();
		float deltaTime = currentTime - lastTime;
		accumulator += deltaTime;
		lastTime = glfwGetTime();

		if(accumulator >= frameTime){


		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shader.use();
	       	glm::mat4 view = glm::mat4(1.0f);	
		shader.setMat4("view", view);
		glBindVertexArray(VAO);
		float aspect = ((float)width/(float)length);
		glm::mat4 model = glm::mat4(1.0f); 
		


		for(int i = 0; i < numCircles; i++){
			position[i] += velocity[i];
		}


		for(int i = 0; i < numCircles; i++){
			if (position[i].y < -1+radius && velocity[i].y < 0){ velocity[i].y *= -0.99f; position[i].y = -1+radius;}
			if(position[i].x < -aspect+radius){ velocity[i].x *= -0.99f; position[i].x = -aspect+radius;}
			if(position[i].x > aspect-radius){ velocity[i].x *= -0.99f; position[i].x = aspect-radius;}
			if(position[i].y > 1-radius){ velocity[i].y *= -0.99f; position[i].y = 1-radius;}
		}


		for(int i = 0; i < numCircles; i++){
		/*gravity on/off*/	deltaVelo[i].y = 0.15f * frameTime;
			velocity[i] -= deltaVelo[i];	
		}


		for(int i = 0; i < numCircles; i++){
			for(int j = i+1; j<numCircles; j++){

				glm::vec3 distance = position[i]-position[j];
				float penetration = radius*2 - glm::length(distance);
				if(penetration < 0) continue;

				glm::vec3 normal = glm::normalize(distance);
				glm::vec3 correction = normal * (penetration / 2.0f);

				position[i] += correction;
				position[j] -= correction;
		
				glm::vec3 nVelocity1 = glm::dot(velocity[i], normal) * normal;
				glm::vec3 nVelocity2 = glm::dot(velocity[j], normal) * normal;

				glm::vec3 tangent1 = velocity[i] - nVelocity1;
				glm::vec3 tangent2 = velocity[j] - nVelocity2;



				glm::vec3 finalVelocity1 = tangent1 + nVelocity2 * 0.9f;
				glm::vec3 finalVelocity2 = tangent2 + nVelocity1 * 0.9f;

				velocity[i] = finalVelocity1;
				velocity[j] = finalVelocity2;




			}
		}


		for(int i = 0; i < numCircles; i++){
			model = glm::mat4(1.0f);
			model = glm::translate(model, position[i]);

			shader.setMat4("model", model);
			shader.setVec3("color", floatToColor(center[(i+1)*3-3]));
	

			glDrawArrays(GL_TRIANGLE_FAN, 0, myVector.size()/3);
		}

		
		glfwSwapBuffers(window);
		accumulator -= frameTime;

	}
	glfwPollEvents();
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	

	}


	glfwDestroyWindow(window);
	glfwTerminate();
	

	return 0;
}


static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if (key == GLFW_KEY_R && action == GLFW_PRESS){
		for (int i = 0; i < numCircles; i++) {
			velocity[i] = glm::vec3(0.0f, 0.0f, 0.0f);
			deltaVelo[i] = glm::vec3(0.0f, 0.0f, 0.0f);
			position[i] = glm::vec3(center[(i+1)*3-3], center[(i+1)*3-2], center[(i+1)*3-1]);
		}
	
		velocity[0].x = -0.001f;
		velocity[0].y = 0.0f;
		velocity[1].y = 0.0f;
		velocity[4] = glm::vec3(0.0f, 0.0f, 0.0f);


	}
	
	std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n";
	
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    float aspect = (float)width / (float)height;
    glm::mat4 projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);

    globalShader->use();
    globalShader->setMat4("projection", projection);
}


glm::vec3 floatToColor(float value) {
    // Map float to an integer seed
    unsigned int seed = static_cast<unsigned int>((value + 1.27f) * 10000); 

    std::mt19937 rng(seed); // deterministic RNG
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
	

    return glm::vec3(dist(rng), 0.4f, 0.5f);
}
