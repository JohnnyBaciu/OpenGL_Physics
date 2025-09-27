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

static void mouse_button_callback(GLFWwindow * window, int button, int action, int mods);

static void cursor_position_callback(GLFWwindow * window, double xpos, double ypos);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void draw_sliders(unsigned int EBO, unsigned int VAO2, float sliderValue, unsigned int VAO);

glm::vec3 floatToColor(float value);

int width = 800;
int length = 600;

float slider[] = {
	1.25f, 0.9f, 0.0f, 
	1.25f, 0.85f, 0.0f,
	-1.25f, 0.85f, 0.0f,
	-1.25, 0.9f, 0.0f
};

int sliderIndices[] = {
	0, 1, 3,
	1, 2, 3
};

float center[] = {-1.27f, 0.0f, 0.0f, 
		-1.14f, 0.0f, 0.0f, 
		-1.0f, 0.0f, 0.0f,
			-0.72f, 0.0f, 0.0f,
				-0.45f, 0.0f, 0.0f,
		-0.32f, 0.0f, 0.0f, 
		-0.19f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
			1.14f, 0.0f, 0.0f,
				0.72f, 0.0f, 0.0f,
			0.45f, 0.0f, 0.0f,
		0.32f, 0.0f, 0.0f,
		0.19f, 0.0f, 0.0f,
		-1.27f, 0.5f, 0.0f, 
		-1.14f, 0.5f, 0.0f, 
		-1.0f, 0.5f, 0.0f,
				-0.72f, 0.5f, 0.0f,
			-0.45f, 0.5f, 0.0f,
			-0.19f, 0.5f, 0.0f,
								0.72f, 0.5f, 0.0f,
							0.19f, 0.5f, 0.0f,
		0.065, 0.5f, 0.0f,
		
							-0.45f, 0.7f, 0.0f,
		-0.32f, 0.7f, 0.0f, 
			0.0f, 0.7f, 0.0f,
			1.14f, 0.7f, 0.0f,
				0.72f, 0.7f, 0.0f,
			0.45f, 0.7f, 0.0f,
		0.32f, 0.7f, 0.0f,
		0.19f, 0.7f, 0.0f,
	
		-1.27f, -0.5f, 0.0f, 
				-1.0f, -0.5f, 0.0f,
					-0.45f, -0.5f, 0.0f,
					0.0f, -0.5f, 0.0f,
									0.32f, -0.5f, 0.0f,

	
		-1.0f, -0.7f, 0.0f,
			
		-0.45f, -0.7f, 0.0f,
		-0.32f, -0.7f, 0.0f, 
		-0.19f, -0.7f, 0.0f,
		0.0f, -0.7f, 0.0f,

		1.14f, -0.7f, 0.0f,
		
		0.72f, -0.7f, 0.0f,
			0.45f, -0.7f, 0.0f,
	
	

		};

constexpr int numCircles = sizeof(center)/sizeof(center[0])/3;


glm::vec3 velocity[numCircles];
glm::vec3 deltaVelo[numCircles];
glm::vec3 position[numCircles];



float gravity;
Shader* globalShader;
float sliderValue = 0.0f;       // initial slider value (0.0 -> 1.0)
bool draggingSlider = false;     // true when mouse is dragging the slider
float sliderMinX = slider[6];       // left end in world coordinates
float sliderMaxX = slider[3];       // right end in world coordinates
float sliderY = slider[1] - std::abs((slider[4]-slider[1]) / 2);         // slider bar y position
									 //
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
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);



	int steps = 100;
	float radius = 0.0625f;


// Push center vertex once
myVector.push_back(0.0f);  // center x
myVector.push_back(0.0f);  // center y
myVector.push_back(0.0f);  // center z

for(int j = 0; j <= steps; j++){ // <= steps ensures closure
    float angle = 2.0f * 3.14159265f * ((float)j / (float)steps);
    float x = sin(angle);
    float y = cos(angle);
    myVector.push_back(x);
    myVector.push_back(y);
    myVector.push_back(0.0f);
}



	unsigned int VAO2;
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);
	unsigned int VBO2;
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(slider), slider, GL_STATIC_DRAW);
	glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sliderIndices), sliderIndices, GL_STATIC_DRAW);
	glBindVertexArray(0);


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
	shader.setFloat("radius", radius);
	glBindVertexArray(0);





	float lastTime = (float)glfwGetTime();
	float frameTime = 0.016f;
	float accumulator = 0.0f;

	while (!glfwWindowShouldClose(window)){
		float currentTime = (float)glfwGetTime();
		float deltaTime = currentTime - lastTime;
		accumulator += deltaTime;
		lastTime = currentTime;

		if(accumulator >= frameTime){


		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shader.use();
		glBindVertexArray(VAO);
	       	glm::mat4 view = glm::mat4(1.0f);	
		shader.setMat4("view", view);
		shader.setFloat("radius", radius);
		float aspect = ((float)width/(float)length);
		glm::mat4 model = glm::mat4(1.0f); 
		


		for(int i = 0; i < numCircles; i++){
			position[i] += velocity[i]*frameTime;
		}



		for(int i = 0; i < numCircles; i++){
			if (position[i].y < -1+radius && velocity[i].y < 0){ velocity[i].y *= -0.99f; position[i].y = -1+radius;}
			if(position[i].x < -aspect+radius){ velocity[i].x *= -0.99f; position[i].x = -aspect+radius;}
			if(position[i].x > aspect-radius){ velocity[i].x *= -0.99f; position[i].x = aspect-radius;}
			if(position[i].y > 1-radius){ velocity[i].y *= -0.99f; position[i].y = 1-radius;}
						}


		for(int i = 0; i < numCircles; i++){
			deltaVelo[i].y = gravity * frameTime;
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



				glm::vec3 finalVelocity1 = tangent1 + nVelocity2 * 0.91f;
				glm::vec3 finalVelocity2 = tangent2 + nVelocity1 * 0.91f;

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


		draw_sliders(EBO, VAO2, sliderValue, VAO);



		
		glfwSwapBuffers(window);
		accumulator -= frameTime;

	}
	glfwPollEvents();
	glBindVertexArray(0);

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
	
		velocity[0].x = -0.3f;
		velocity[0].y = 0.9f;
		velocity[1].y = 0.08f;
		velocity[4] = glm::vec3(0.5f, 0.2f, 0.0f);
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




void draw_sliders(unsigned int EBO, unsigned int VAO2, float sliderValue, unsigned int VAO) {
    globalShader->use();

    // Draw the slider bar
    globalShader->setMat4("model", glm::mat4(1.0f));
    globalShader->setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
    globalShader->setFloat("radius", 1.0f);
    glBindVertexArray(VAO2);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Draw the dragger circle
    glm::mat4 model = glm::mat4(1.0f);

    float sliderY = slider[1] - std::abs((slider[4]-slider[1]) / 2);

    model = glm::translate(model, glm::vec3(sliderValue, sliderY, 0.0f));
    globalShader->setMat4("model", model);
    globalShader->setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f)); // red dragger
    globalShader->setFloat("radius", 0.03f);

    // Bind the VAO of your circle (assume it's called VAO_circle)
    glBindVertexArray(VAO);  
    glDrawArrays(GL_TRIANGLE_FAN, 0, myVector.size()/3);

    gravity = sliderValue*4;
}



void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	//std::cout << "mouse pressed" << std::endl;
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);

            // Convert mouseX to NDC / world coordinates
            int width, height;
            glfwGetWindowSize(window, &width, &height);
            float ndcX = ((float)mouseX - (width / 2.0f)) * (2.0f / height);
	    //std::cout << "ndcx at: " << ndcX << std::endl;

            // Check if click is close to the slider bar
            if (std::abs(ndcX - sliderValue) < 0.05f) {
                draggingSlider = true; // start dragging
		//std::cout << "button pressed" << std::endl;

            }
        } else if (action == GLFW_RELEASE) {
            draggingSlider = false; // stop dragging
		//std::cout << "mouse released" << std::endl;
        }
    }
}




void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!draggingSlider) return;

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Map mouse x to world coordinates
   float ndcX = ((float)xpos - (width / 2.0f)) * (2.0f / height);

    //std::cout << "dragging ndcx: " << ndcX << std::endl;

    // Clamp to slider range
    if (ndcX < sliderMinX) ndcX = sliderMinX;
    if (ndcX > sliderMaxX) ndcX = sliderMaxX;

    // Convert to 0.0 -> 1.0 slider value
    sliderValue = ndcX;
}
