#include "window.h"

const char* window_title = "Project 3";

// Initializations
Cube * skybox;
GLint shaderProgram;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 1.0f);		// e  | Position of camera, z = 20.0f default
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at, z = 0.0f default
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

// Callback variables
float cursor_x = 0;
float cursor_y = 0;
glm::vec3 old_location;
bool lmb = false;
bool rmb = false;
bool button_down = false;

void Window::initialize_objects()
{
	skybox = new Cube(64);
	skybox->loadCubemap();

	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(skybox);
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

glm::vec3 Window::trackball(float x, float y)    // Use separate x and y values for the mouse location
{
	glm::vec3 v;    // Vector v is the synthesized 3D position of the mouse location on the trackball
	float d;     // this is the depth of the mouse location: the delta between the plane through the center of the trackball and the z position of the mouse
	v.x = (2.0f*x - Window::width) / Window::width;   // this calculates the mouse X position in trackball coordinates, which range from -1 to +1
	v.y = (Window::height - 2.0f*y) / Window::height;   // this does the equivalent to the above for the mouse Y position
	v.z = 0.0;   // initially the mouse z position is set to zero, but this will change below
	d = (float)v.length();    // this is the distance from the trackball's origin to the mouse location, without considering depth (=in the plane of the trackball's origin)
	d = (d<1.0) ? d : 1.0f;   // this limits d to values of 1.0 or less to avoid square roots of negative values in the following line
	v.z = (float)sqrtf(1.001 - d*d);  // this calculates the Z coordinate of the mouse position on the trackball, based on Pythagoras: v.z*v.z + d*d = 1*1
	v = normalize(v); // Still need to normalize, since we only capped d, not v.
	return v;  // return the mouse location on the surface of the trackball
}




/************************************
*		START NEW CALLBACKS			*
*									*
*************************************/



void Window::cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
	cursor_x = xpos;
	cursor_y = ypos;

	// Rotate camera
	if (lmb && button_down) {
		glm::vec3 location = trackball(cursor_x, cursor_y);
		glm::vec3 direction = location - old_location;
		float velocity = direction.length();
		// Velocity check
		if (velocity > 0.0001) {
			glm::vec3 rot_axis;
			rot_axis = cross(old_location, location);
			velocity = velocity / (3.25);
			rot_axis[2] = 0.0f;
			Window::V = glm::translate(glm::mat4(1.0f), -cam_pos) * Window::V;
			Window::V = glm::rotate(glm::mat4(1.0f), velocity / 180.0f * glm::pi<float>(), rot_axis) * Window::V;
			Window::V = glm::translate(glm::mat4(1.0f), cam_pos) * Window::V;
		}
	}
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) {
		button_down = true;
		old_location = trackball(cursor_x, cursor_y);

		if (button == GLFW_MOUSE_BUTTON_1) {
			lmb = true;
			rmb = false;
		}

		if (button == GLFW_MOUSE_BUTTON_2) {
			lmb = false;
			rmb = true;
		}
	}
	else {
		button_down = false;
		lmb = false;
		rmb = false;
	}
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset != 0) {
		Window::V = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, (float)-yoffset)) * Window::V;
		cam_pos.z += -yoffset;
	}
}


/************************************
*		END NEW CALLBACKS			*
*									*
*************************************/




void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		//V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	// Call the update function the cube
	skybox->update();
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID
	glUseProgram(shaderProgram);
	
	// Render the cube
	skybox->draw(shaderProgram);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		// RESET
		else if (key == GLFW_KEY_R) {
			Window::V = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			cam_pos = glm::vec3(0.0f, 0.0f, 1.0f);
		}

		// X MOVE
		else if (key == GLFW_KEY_X) {
			// X
			if (mods == GLFW_MOD_SHIFT) {
				Window::V = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * Window::V;
				cam_pos.x += 1.0f;
			}
			// x
			else {
				Window::V = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)) * Window::V;
				cam_pos.x += -1.0f;
			}
		}
		// Y MOVE
		else if (key == GLFW_KEY_Y) {
			// Y
			if (mods == GLFW_MOD_SHIFT) {
				Window::V = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * Window::V;
				cam_pos.y += 1.0f;
			}
			// y
			else {
				Window::V = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f)) * Window::V;
				cam_pos.y += -1.0f;
			}
		}
		// Z MOVE
		else if (key == GLFW_KEY_Z) {
			// Z
			if (mods == GLFW_MOD_SHIFT) {
				Window::V = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * Window::V;
				cam_pos.z += 1.0f;
			}
			// z
			else {
				Window::V = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f)) * Window::V;
				cam_pos.z += -1.0f;
			}
		}
	}
}