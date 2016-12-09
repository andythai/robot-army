#include "window.h"

const char* window_title = "Project 3";

// Initializations
Skybox * skybox;
Cube * cube;
Sphere * sphere;
Group * robot;
Group * sphere_bounds;
MatrixTransform * sphere_matrix;
MatrixTransform * left_arm;
MatrixTransform * right_arm;
MatrixTransform * left_leg;
MatrixTransform * right_leg;
MatrixTransform * body;
MatrixTransform * head;
GLint skybox_shaderProgram;
GLint object_shaderProgram;

float rot_angle = 0.0f;
float rot_increment = 1.0f;

int grid_x = 0;
int grid_y = 0;
int grid_z = 0;
float scale_factor = 1;
glm::mat4 grid_rotation = glm::mat4(1.0f);
bool BOUNDING_SPHERES = false;
bool CULLING_MODE = false;

// FPS
double previous_time = glfwGetTime();
int frames = 0;

// On some systems you need to change this to the absolute path
#define SKYBOX_VERTEX_SHADER_PATH "../skybox.vert"
#define SKYBOX_FRAGMENT_SHADER_PATH "../skybox.frag"
#define OBJECT_VERTEX_SHADER_PATH "../object.vert"
#define OBJECT_FRAGMENT_SHADER_PATH "../object.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 50.0f, 40.0f);		// e  | Position of camera, z = 20.0f default, reset 1.0f
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
bool ROBOT_MODE = false;

void Window::initialize_objects()
{
	// Initialize camera
	V = glm::lookAt(cam_pos, cam_look_at, cam_up);

	//cube2->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0, 0)) * cube2->toWorld;

	skybox = new Skybox(200);
	skybox->loadCubemap();

	robot = new Group();
	cube = new Cube();
	sphere = new Sphere();
	left_arm = new MatrixTransform();
	right_arm = new MatrixTransform();
	left_leg = new MatrixTransform();
	right_leg = new MatrixTransform();
	body = new MatrixTransform();
	head = new MatrixTransform();

	sphere_bounds = new Group();
	sphere_matrix = new MatrixTransform();
	sphere_matrix->setMatrix(glm::mat4(1.0f));
	sphere_matrix->addChild(sphere);
	sphere_bounds->addChild(sphere_matrix);
	

	// Set matrices

	// Left arm
	glm::mat4 left_arm_transform = glm::rotate(glm::mat4(1.0f), -35.0f / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
	left_arm_transform = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 0.0f, 0.0f)) * glm::mat4(1.0f) * left_arm_transform;
	left_arm_transform = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.5f, 1.0f)) * left_arm_transform;
	left_arm->setMatrix(left_arm_transform);

	// Body
	glm::mat4 body_transform = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 3.0f, 1.0f));
	body->setMatrix(body_transform);

	// Right arm
	glm::mat4 right_arm_transform = glm::rotate(glm::mat4(1.0f), 35.0f / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
	right_arm_transform = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 0.0f)) * glm::mat4(1.0f) * right_arm_transform;
	right_arm_transform = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.5f, 1.0f)) * right_arm_transform;
	right_arm->setMatrix(right_arm_transform);

	// Left leg
	glm::mat4 left_leg_transform = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, -5.0f, 0.0f));
	left_leg_transform = glm::scale(glm::mat4(1.0f), glm::vec3(0.60f, 1.5f, 0.6f)) * left_leg_transform;
	left_leg->setMatrix(left_leg_transform);

	// Right leg
	glm::mat4 right_leg_transform = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, -5.0f, 0.0f));
	right_leg_transform = glm::scale(glm::mat4(1.0f), glm::vec3(0.60f, 1.5f, 0.6f)) * right_leg_transform;
	right_leg->setMatrix(right_leg_transform);

	// Head
	glm::mat4 head_transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 5.5f, 0.0f));
	head_transform = glm::scale(glm::mat4(1.0f), glm::vec3(1.25f, 1.25f, 1.25f)) * head_transform;
	head->setMatrix(head_transform);

	left_arm->addChild(cube);
	robot->addChild(left_arm);

	body->addChild(cube);
	robot->addChild(body);

	right_arm->addChild(cube);
	robot->addChild(right_arm);

	left_leg->addChild(cube);
	robot->addChild(left_leg);

	right_leg->addChild(cube);
	robot->addChild(right_leg);
	
	head->addChild(cube);
	robot->addChild(head);

	// Load the shader program. Make sure you have the correct filepath up top
	skybox_shaderProgram = LoadShaders(SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH);
	object_shaderProgram = LoadShaders(OBJECT_VERTEX_SHADER_PATH, OBJECT_FRAGMENT_SHADER_PATH);

	sphere = new Sphere();

	cube->shaderProgram = object_shaderProgram;
	sphere->shaderProgram = object_shaderProgram;
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(skybox);
	delete(cube);
	delete(sphere);

	delete(left_arm);
	delete(right_arm);
	delete(left_leg);
	delete(right_leg);
	delete(head);
	delete(body);
	delete(sphere_matrix);

	delete(robot);
	delete(sphere_bounds);

	glDeleteProgram(skybox_shaderProgram);
	glDeleteProgram(object_shaderProgram);
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
		if (velocity > 0.0001 && !ROBOT_MODE) {
			glm::vec3 rot_axis;
			rot_axis = cross(old_location, location);
			velocity = velocity / (3.25);
			rot_axis[2] = 0.0f;
			//Window::V = glm::translate(glm::mat4(1.0f), -cam_pos) * Window::V;
			//Window::V = glm::rotate(glm::mat4(1.0f), velocity / 180.0f * glm::pi<float>(), rot_axis) * Window::V;
			//Window::V = glm::translate(glm::mat4(1.0f), cam_pos) * Window::V;

			cam_pos = glm::vec3(glm::rotate(glm::mat4(1.0f), velocity / 180.0f * glm::pi<float>(), rot_axis) * glm::vec4(cam_pos, 1));
			cam_up = glm::vec3(glm::rotate(glm::mat4(1.0f), velocity / 180.0f * glm::pi<float>(), rot_axis) * glm::vec4(cam_up, 1));
			//cam_look_at = glm::vec3(glm::rotate(glm::mat4(1.0f), velocity / 180.0f * glm::pi<float>(), rot_axis) * glm::vec4(cam_look_at, 1));
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);

		}
		else if (velocity > 0.0001 && ROBOT_MODE) {
			glm::vec3 rot_axis = cross(old_location, location);
			velocity = velocity;
			rot_axis[2] = 0.0f;
			grid_rotation = glm::rotate(glm::mat4(1.0f), velocity / 180.f * glm::pi<float>(), rot_axis);
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
	if (yoffset != 0 && !ROBOT_MODE) {
		//Window::V = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, (float)-yoffset)) * Window::V;
		cam_pos.z += -yoffset;
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);

	}
	else if (yoffset != 0 && ROBOT_MODE) {
		grid_z += -yoffset;
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
	//cube->update();
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID, skybox
	glUseProgram(skybox_shaderProgram);
	skybox->draw(skybox_shaderProgram);

	// Get FPS
	double time = glfwGetTime();
	frames++;
	if (time - previous_time >= 1.0) {
		std::cout << 1 / (double)frames << "s rendering time" << std::endl;
		frames = 0;
		previous_time = previous_time + 1.0;
		/*
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				std::cout << Window::V[j][i] << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		std::cout << V[2][0] << " " << V[2][1] << std::endl;
		*/
	}

	// Use object shader
	glDisable(GL_CULL_FACE);

	glUseProgram(object_shaderProgram);
	skybox->sendLight(object_shaderProgram);
	
	if (rot_angle >= 15.0f) {
		rot_increment = -0.1f;
	}
	else if (rot_angle <= -15.0f) {
		rot_increment = 0.1f;
	}
	rot_angle = rot_angle + rot_increment;

	// Left leg
	glm::mat4 animation_transform = glm::rotate(glm::mat4(1.0f), rot_increment / 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 reverse_animation_transform = glm::rotate(glm::mat4(1.0f), -rot_increment / 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));


	glm::mat4 left_leg_transform = animation_transform * grid_rotation * left_leg->M;
	left_leg->setMatrix(left_leg_transform);

	glm::mat4 right_leg_transform = reverse_animation_transform * grid_rotation * right_leg->M;
	right_leg->setMatrix(right_leg_transform);

	glm::mat4 left_arm_transform = reverse_animation_transform * grid_rotation * left_arm->M;
	left_arm->setMatrix(left_arm_transform);

	glm::mat4 right_arm_transform = animation_transform * grid_rotation * right_arm->M;
	right_arm->setMatrix(right_arm_transform);

	head->setMatrix(grid_rotation * head->M);
	body->setMatrix(grid_rotation * body->M);

	grid_rotation = glm::mat4(1.0f);

	// Parameter determines transformation of object
	glm::mat4 robot_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_factor, scale_factor, scale_factor));
	for (int i = 1; i < 11; i++) {
		for (int j = 1; j < 11; j++) {
			if (!CULLING_MODE)
				robot->draw(glm::translate(glm::mat4(1.0f), glm::vec3(20.0f * (i - 1) + grid_x, 0.0f + grid_y, 20.0f * (j - 1) + grid_z)) * robot_matrix);
			else {
				glm::vec3 coord_vec = glm::vec3((20.0f * (i - 1) + grid_x, 0.0f + grid_y, 20.0f * (j - 1) + grid_z));

				glm::vec3 d = normalize(cam_look_at - cam_pos);
				float nearDist = 0.1f;
				float farDist = 1000.0f;
				float fov = 45.0f;
				float ratio = width / height;

				float Hnear = 2.0f * tan(fov / 2.0f) * nearDist;
				float Wnear = Hnear * ratio;
				float Hfar = 2.0f * tan(fov / 2.0f) * farDist;
				float Wfar = Hfar * ratio;

				glm::vec3 p = cam_pos;
				glm::vec3 up = normalize(cam_up);
				glm::vec3 right = cross(up, d);

				glm::vec3 fc = p + d * farDist;

				//glm::vec3 ftl, ftr, fbl, fbr;

				
				glm::vec3 ftl = fc + (up * Hfar / 2.0f) - (right * Wfar / 2.0f);
				glm::vec3 ftr = fc + (up * Hfar / 2.0f) + (right * Wfar / 2.0f);
				glm::vec3 fbl = fc - (up * Hfar / 2.0f) - (right * Wfar / 2.0f);
				glm::vec3 fbr = fc - (up * Hfar / 2.0f) + (right * Wfar / 2.0f);
				
				
				glm::vec3 nc = p + d * nearDist;

				
				//glm::vec3 ntl, ntr, nbl, nbr;
				
				glm::vec3 ntl = nc + (up * Hnear / 2.0f) - (right * Wnear / 2.0f);
				glm::vec3 ntr = nc + (up * Hnear / 2.0f) + (right * Wnear / 2.0f);
				glm::vec3 nbl = nc - (up * Hnear / 2.0f) - (right * Wnear / 2.0f);
				glm::vec3 nbr = nc - (up * Hnear / 2.0f) + (right * Wnear / 2.0f);
				
				glm::vec3 a = normalize((nc + right * Wnear / 2.0f) - p);
				glm::vec3 normalRight = cross(up, a);

				/*
				glm::vec3 top = glm::vec3(ntr, ntl, ftl);
				glm::vec3 bottom = glm::vec3(nbl, nbr, fbr);
				glm::vec3 left = glm::vec3(ntl, nbl, fbl);
				glm::vec3 right_p = glm::vec3(nbr, ntr, fbr);
				glm::vec3 nearp = glm::vec3(ntl, ntr, nbr);
				glm::vec3 farp = glm::vec3(ftr, ftl, fbl);
				
				glm::vec3 plane[8] = { ftl, ftr, fbl, fbr, ntl, ntr, nbl, nbr };
				*/
				bool to_render = false;

				glm::vec3 x_p = coord_vec - ftr;
				glm::vec3 x_p2 = coord_vec - nbl;
				glm::vec3 n = { 0, 0, 1 };
				glm::vec3 n2 = { 0, 0, -1 };

				glm::vec3 nl = { 1, 0, 0 };
				glm::vec3 nr = { -1, 0, 0 };
				float near_plane_distance = dot(n2, x_p2);
				float far_plane_distance = dot(n, x_p);

				float left_plane_distance = dot(nl, x_p);
				float right_plane_distance = dot(nr, x_p);

				float top_plane_distance = dot(n2, x_p2);
				float bottom_plane_distance = dot(n, x_p);

				if (near_plane_distance >= 0 && far_plane_distance >= 0) {
					if (left_plane_distance >= 0 && right_plane_distance <= 0) {
						to_render = true;
					}
				}
				else if (cam_pos.z <= 0 && near_plane_distance < 0 && far_plane_distance < 0) {
					if (left_plane_distance <= 0 && right_plane_distance >= 0) {
						to_render = true;
					}
				}

				//for (int i = 0; i < 8; i++) {
				/*
					if (cam_pos.z < 20 && cam_pos.z > 0) {
						if ((coord_vec.z > ftl.z) && (coord_vec.z < 20 * nbl.z)) {
							to_render = true;
						}
					}
					else if (cam_pos.z > 0) {
						if ((coord_vec.z > ftl.z) && (coord_vec.z < nbl.z)) {
							to_render = true;
						}
					}
					else if (cam_pos.z < 0) {
						if ((coord_vec.z > -ftl.z) && (coord_vec.z < -nbl.z))
						{
							to_render = true;
						}
					}
					*/
					/*
					float p_distance = distance(plane[i], coord_vec);
					int radius = sphere->radius;
					if (p_distance < -radius) {
						to_render = false;
					}
					*/
				//}

				if (to_render == false) {

				}
				else {
					robot->draw(glm::translate(glm::mat4(1.0f), glm::vec3(20.0f * (i - 1) + grid_x, 0.0f + grid_y, 20.0f * (j - 1) + grid_z)) * robot_matrix);
				}
			}
			if (BOUNDING_SPHERES)
				sphere_bounds->draw(glm::translate(glm::mat4(1.0f), glm::vec3(20.0f * (i - 1) + grid_x, 0.0f + grid_y, 20.0f * (j - 1) + grid_z)));
		}
	}

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);



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

		else if (key == GLFW_KEY_H) {
			if (CULLING_MODE) {
				CULLING_MODE = false;
				std::cout << "Culling mode off." << std::endl;
			}
			else {
				CULLING_MODE = true;
				std::cout << "Culling mode on." << std::endl;
			}
		}

		else if (key == GLFW_KEY_C) {
			if (ROBOT_MODE == true) {
				ROBOT_MODE = false;
				std::cout << "Robot controls off." << std::endl;
			}
			else {
				ROBOT_MODE = true;
				std::cout << "Robot controls on." << std::endl;
			}
		}

		else if (key == GLFW_KEY_B) {
			if (BOUNDING_SPHERES == true) {
				BOUNDING_SPHERES = false;
			}
			else {
				BOUNDING_SPHERES = true;
			}
		}

		// RESET
		else if (key == GLFW_KEY_R) {
			//Window::V = glm::lookAt(glm::vec3(0.0f, 50.0f, 40.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			cam_pos = glm::vec3(0.0f, 50.0f, 40.0f);
			cam_up = glm::vec3(0.0f, 1.0f, 0.0f);
			cam_look_at = glm::vec3(0.0f, 0.0f, 0.0f);
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
			scale_factor = 1;
			grid_x = 0;
			grid_y = 0;
			grid_z = 0;
			grid_rotation = glm::mat4(1.0f);


			// Left arm
			glm::mat4 left_arm_transform = glm::rotate(glm::mat4(1.0f), -35.0f / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
			left_arm_transform = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 0.0f, 0.0f)) * glm::mat4(1.0f) * left_arm_transform;
			left_arm_transform = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.5f, 1.0f)) * left_arm_transform;
			left_arm->setMatrix(left_arm_transform);

			// Body
			glm::mat4 body_transform = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 3.0f, 1.0f));
			body->setMatrix(body_transform);

			// Right arm
			glm::mat4 right_arm_transform = glm::rotate(glm::mat4(1.0f), 35.0f / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
			right_arm_transform = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 0.0f)) * glm::mat4(1.0f) * right_arm_transform;
			right_arm_transform = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.5f, 1.0f)) * right_arm_transform;
			right_arm->setMatrix(right_arm_transform);

			// Left leg
			glm::mat4 left_leg_transform = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, -5.0f, 0.0f));
			left_leg_transform = glm::scale(glm::mat4(1.0f), glm::vec3(0.60f, 1.5f, 0.6f)) * left_leg_transform;
			left_leg->setMatrix(left_leg_transform);

			// Right leg
			glm::mat4 right_leg_transform = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, -5.0f, 0.0f));
			right_leg_transform = glm::scale(glm::mat4(1.0f), glm::vec3(0.60f, 1.5f, 0.6f)) * right_leg_transform;
			right_leg->setMatrix(right_leg_transform);

			// Head
			glm::mat4 head_transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 5.5f, 0.0f));
			head_transform = glm::scale(glm::mat4(1.0f), glm::vec3(1.25f, 1.25f, 1.25f)) * head_transform;
			head->setMatrix(head_transform);
		}

		else if (key == GLFW_KEY_S) {
			if (mods == GLFW_MOD_SHIFT) {
				scale_factor += 0.1f;
			}
			else {
				if (scale_factor - 0.1f >= 0) {
					scale_factor += -0.1f;
				}
			}
		}

		// X MOVE
		else if (key == GLFW_KEY_X) {
			// X
			if (mods == GLFW_MOD_SHIFT) {
				grid_x += 1;
			}
			// x
			else {
				grid_x += -1;
			}
		}
		// Y MOVE
		else if (key == GLFW_KEY_Y) {
			// Y
			if (mods == GLFW_MOD_SHIFT) {
				grid_y += 1;
			}
			// y
			else {
				grid_y += -1;
			}
		}
		// Z MOVE
		else if (key == GLFW_KEY_Z) {
			// Z
			if (mods == GLFW_MOD_SHIFT) {
				grid_z += 1;
			}
			// z
			else {
				grid_z += -1;
			}
		}
	}
}