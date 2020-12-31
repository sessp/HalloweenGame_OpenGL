#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/model.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include <iostream>
#include <string>

#define PI 3.14159265

// Box coordinate with 36 vertices.
// Every 3 coordinates will form 1 triangle.
// The last 2 columns represent texture coordinate for mapping.
float box[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

//Variables associated with the attenuation + accessing the arrays.
float linear[]{0.7, 0.35, 0.22, 0.14, 0.09, 0.07, 0.045, 0.027, 0.022, 0.014, 0.007, 0.0014};
float quadratic[]{1.8,0.44,0.20,0.07,0.032,0.017,0.0075,0.0028,0.0019,0.0007,0.0002,0.000007};
int currentIndex = 0;
int maxIndex = 11;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow *window);
unsigned int loadTexture(char const * path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;


// camera stuff
glm::vec3 camera_pos   = glm::vec3(0.0f, 0.9f,  3.0f);//z was 3.0
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up    = glm::vec3(0.0f, 1.0f,  0.0f);
glm::vec3 camera_dir = camera_pos + camera_front;
glm::vec3 camera_jump = glm::vec3(0.0f,0.05f,0.0f);
glm::vec3 camera_fall = glm::vec3(0.0f,-0.05f,0.0f);
glm::mat4 projection;

//Used for animation
glm::vec3 applyTo(0.0f,0.0f,0.0f);

float animatedIndex = 0.0f;
float animatedIndex1 = 0.0f;
float animatedSkelIndex = 0.0f;

//Used as booleans. 

int lightMoveWithPlayer = 0;
int moveState = 1; 
int moveAcrossState = 1; 

//Yaw and pitch values
float y = -90.0f;
float p = 0.0f;


// lighting
glm::vec3 light_pos(0.0f, 0.5f, 0.1f);
glm::vec3 light_move(0.0f,-2.0f, 0.1f);

//More animating variables. 
float world_rotate_y = 0.0f;

glm::vec3 moveUp(0.0f, 0.0f, 0.0f);
glm::vec3 moveAcross(0.0f,0.0f,0.0f);


// timing
float delta_time = 0.0f;	// time between current frame and last frame
float last_frame = 0.0f;

//Toggle (Animation or states)
bool BUTTON_PRESSED = false;
int BUTTON_DELAY = 0;
int TOGGLEF_DELAY = 0;
int TOGGLEO_DELAY = 0;
int TOGGLEK_DELAY = 0;
int TOGGLEL_DELAY = 0;
int TOGGLE1_DELAY = 0;
int TOGGLE3_DELAY = 0;
int TOGGLEP_DELAY = 0;
int SPACE_DELAY = 0;
bool BUTTON_CLOSE_ENOUGH = false;
bool INTERACTION_CLOSE_ENOUGH = false;
int ZOOM_DELAY = 0;

//More booleans to notify render loop.
int pressedKey = 0;
int projectionType = 0;

int jumpState = 0;
int zoomChange = 0;
int projectionChange = 0;

//Fov/Zoom stuff. 
float fov = 45.0f;
float ofov = 1.0f;
int SHOW_DELAY = 0;

int brightnessChange = 0;
 
int lightPositionChanged = 0;

float cameraSpeed = 2.5 * delta_time * 2;	

// Countdown until the button trigger can be pressed again.
// This prevents accidental burst repeat clicking of the key.
void update_delay()
{
	if(BUTTON_DELAY > 0) BUTTON_DELAY -= 1;
	if(SHOW_DELAY > 0) SHOW_DELAY -= 1;
	if(ZOOM_DELAY > 0) ZOOM_DELAY -= 1;
	if(TOGGLEF_DELAY > 0) TOGGLEF_DELAY -= 1;
	if(TOGGLEO_DELAY > 0) TOGGLEO_DELAY -= 1;
	if(TOGGLEK_DELAY > 0) TOGGLEK_DELAY -= 1;
	if(TOGGLEL_DELAY > 0) TOGGLEL_DELAY -= 1;
	if(TOGGLE1_DELAY > 0) TOGGLE1_DELAY -= 1;
	if(TOGGLE3_DELAY > 0) TOGGLE3_DELAY -= 1;
	if(TOGGLEP_DELAY > 0) TOGGLEP_DELAY -= 1;
	if(SPACE_DELAY > 0) SPACE_DELAY -= 1;



}
//Check interaction distance based on object position. 
void check_interaction_distance(glm::vec3 object_pos)
{
	if(glm::length(camera_pos - object_pos) <= 2.0f)
		INTERACTION_CLOSE_ENOUGH = true;
	else
		INTERACTION_CLOSE_ENOUGH = false;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement for OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Tutorial", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	Shader lighting_shader("./sample2.vs", "./sample2.fs");
	Shader lamp_shader("./lamp.vs", "./lamp.fs");

	//fulltree
	//ghost 
	//doorrender
	//graved
	//fulloilrendtext
	//gatetextured
	Model ourGhost(FileSystem::getPath("resources/objects/tree/ghost.obj"));
	Model ourGrave(FileSystem::getPath("resources/objects/tree/graved.obj"));
	Model door(FileSystem::getPath("resources/objects/tree/doorrender.obj"));
        Model door1(FileSystem::getPath("resources/objects/tree/doorrender.obj"));
	Model ourSkeleton(FileSystem::getPath("resources/objects/tree/skelleton.obj"));
	Model ourSign(FileSystem::getPath("resources/objects/tree/signfinal.obj"));
        Model ourHouse(FileSystem::getPath("resources/objects/tree/housetextured.obj"));
	Model ourFence(FileSystem::getPath("resources/objects/tree/gatefinal.obj"));
	Model ourTree(FileSystem::getPath("resources/objects/tree/fulltree.obj"));
	Model oilLamp(FileSystem::getPath("resources/objects/tree/fulloilrendtext.obj"));



 

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

	unsigned int VBO_box, VAO_box;

	glGenVertexArrays(1, &VAO_box);
	glGenBuffers(1, &VBO_box);

	glBindVertexArray(VAO_box);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_box);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);

	//vertex coordinates
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//normal vectors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);



	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int VAO_light;
	glGenVertexArrays(1, &VAO_light);
	glBindVertexArray(VAO_light);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_box);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);





	// load and create a texture 
	// -------------------------
	unsigned int tex_wood_diffuse, tex_street_diffuse, tex_grass_diffuse, tex_marble_diffuse, tex_curtin_diffuse;
	unsigned int tex_wood_specular, tex_street_specular, tex_grass_specular, tex_marble_specular, tex_curtin_specular;

	unsigned int tex_red_dark_diffuse, tex_red_bright_diffuse, tex_red_diffuse, tex_green_diffuse, tex_blue_diffuse;
	unsigned int tex_red_dark_specular, tex_red_bright_specular, tex_red_specular, tex_green_specular, tex_blue_specular;

	tex_grass_diffuse = loadTexture(FileSystem::getPath("resources/textures/grass.jpg").c_str());
	tex_grass_specular = loadTexture(FileSystem::getPath("resources/textures/grass_specular.jpg").c_str());
	



	
	//shader configuration -------------------------------------------------------------------------------------------
	lighting_shader.use();

	
	// -----------------------------------------------------------------------------------------------------------
	//Set default projection + update projection type. 
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 300.0f);
	

	lighting_shader.setMat4("projection", projection);
	
	projectionType = 1;



	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		//Get frame + Update Delay
		float currentFrame = glfwGetTime();
		delta_time = currentFrame - last_frame;
		last_frame = currentFrame;
		update_delay();

		// Handle Input
		process_input(window);

		//If zoom has been changed, check projection type, if ortho, zoom in with ortho algorithm. If perspective, zoom in with perspective algorithm. Reset zoom change. 
		if(zoomChange == 1)
		{
		    if(projectionType == 1)
		    {
		    	projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			zoomChange = 0;

		    }
		    if(projectionType == 2)
		    {
			    projection = glm::ortho(-10.0f/ofov,10.0f/ofov,-10.0f/ofov,10.0f/ofov,-10.0f,20.0f);
			    zoomChange = 0;
		    }
		}

		//If projection has been changed, if projection is perspective switch to ortho, if projection is orth switch to perspective. 
		if(projectionChange == 1)
		{
		   if(projectionType == 1)
		   {
		     projection = glm::ortho(-2.0f,2.0f,-2.0f,2.0f,-10.0f,20.0f);
		     projectionType = 2;
		     projectionChange = 0;
		     lighting_shader.setMat4("projection", projection);

		   }
		   else if(projectionType == 2)
		   {
		      	     projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);	                 
			     projectionType = 1;
                             projectionChange = 0;
			     lighting_shader.setMat4("projection", projection);


		   }
		}
               lighting_shader.setMat4("projection", projection);




	       //Jump function, it's a bit buggy as you can't use W/S when jumping but it does work if you stand still. 

		if(pressedKey == 1)
	        {
		  if((jumpState == 0))
	         	{
	                   camera_pos = camera_pos + camera_jump;
			   camera_dir = camera_dir + camera_jump; 

		        } 
		if(camera_pos.y > 2.0f)
		        {
		           jumpState = 1;
		        }
		if(jumpState == 1)
		        {
		    	 camera_pos = camera_pos + camera_fall;
			 camera_dir = camera_dir + camera_fall; 

			}
		if(camera_pos.y < 1.0f)
			{
		   	jumpState = 2;
		   	pressedKey = 0;
		      
			}

		}


		


		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 


		// activate shaders, set make sure we have the correct projection matrix, camera position, set light position. 
		lighting_shader.use();
		lighting_shader.setMat4("projection",projection);
		glm::mat4 view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
		lighting_shader.setMat4("view", view);
		lighting_shader.setVec3("light.position", light_pos);
        	lighting_shader.setVec3("viewPos", camera_pos);

                //Global Attenuation Values. Small ambient setting. For attenuation constant never changes and you cycle through array depending on <K> and <L>
		lighting_shader.setVec3("light.ambient",0.1f, 0.1f, 0.1f);
		lighting_shader.setFloat("light.constantAtt", 1.0f);
                lighting_shader.setFloat("light.linearAtt", linear[currentIndex]);
                lighting_shader.setFloat("light.quadraticAtt", quadratic[currentIndex]);

		// Global light properties for all objects:
		lighting_shader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
		lighting_shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);


		//Suppose to be the <O> function. It works but only when you're at the brightest value in the array, ie you click k until it is max.
		//I have no idea why and I didn't have enough time but i assume it's the other attenuation values/diffuse/specular messing around with
		//the ambient in the shader, because of the attenuation formula. 

                if(brightnessChange == 1)
		{
			lighting_shader.setVec3("light.ambient",1.0f, 1.0f, 1.0f);
                   		   
		}
		


		// material properties
        	lighting_shader.setFloat("material.shininess", 65.0f);


		
		
                //Animation for ghost. This makes him move up + also rotate. 
		glm::mat4 model;
		if((moveUp.y < 1.5f) && (moveState == 1))
		{
		      moveUp.y += 0.05f;
		}
		else
	        {
		      moveState = 0;
		      moveUp.y -= 0.05f;
		}
		if((moveUp.y < -2.0f))
		{
		     moveState = 1; 
		}

		world_rotate_y += 1.0f;
		if(abs(world_rotate_y - 360.0f) <= 0.1f)
		{
		   world_rotate_y = 0.0f;
		}
		applyTo.y += 1.0f;

		//This is the start of my object creation + animation, all necessary commentary is in my report so there is no comments for a bit. 
		//Ghosts 
		//Just a note after I finished writing the report I realised that  ghosts would also glow so I didn't say that in my illumination effects justification.
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::translate(model, moveUp);

		model = glm::rotate(model,glm::radians(world_rotate_y),applyTo);
		model = glm::scale(model, glm::vec3(3.75f, 3.75f, 3.75f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));


	        lighting_shader.setMat4("model",model);
		
		lighting_shader.setVec3("material.specular",0.2f, 0.2f, 0.2f);
	        lighting_shader.setVec3("light.specular",0.2f, 0.2f, 0.2f);
		lighting_shader.setVec3("light.ambient",0.8f, 0.8f, 0.8f);
                lighting_shader.setVec3("material.ambient",0.8f, 0.8f, 0.8f);

		
		ourGhost.Draw(lighting_shader);

		lighting_shader.setVec3("material.specular",1.0f, 1.0f, 1.0f);    
                lighting_shader.setVec3("light.specular",1.0f, 1.0f, 1.0f);
		lighting_shader.setVec3("light.ambient",0.1f, 0.1f, 0.1f);
                lighting_shader.setVec3("material.ambient",0.1f, 0.1f, 0.1f);


		 model = glm::mat4();

		 model = glm::translate(model, glm::vec3(-4.0f, 0.0f, 0.0f));
		model = glm::translate(model, moveUp);

		model = glm::rotate(model,glm::radians(world_rotate_y),applyTo);
		model = glm::scale(model, glm::vec3(3.75f, 3.75f, 3.75f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));


	        lighting_shader.setMat4("model",model);
		
		lighting_shader.setVec3("material.specular",0.0f, 0.0f, 0.0f);
	        lighting_shader.setVec3("light.specular",0.0f, 0.0f, 0.0f);
		lighting_shader.setVec3("light.ambient",0.8f, 0.8f, 0.8f);
                lighting_shader.setVec3("material.ambient",0.8f, 0.8f, 0.8f);

		ourGhost.Draw(lighting_shader);
		lighting_shader.setVec3("light.ambient",0.1f, 0.1f, 0.1f);
                lighting_shader.setVec3("material.ambient",0.1f, 0.1f, 0.1f);

		lighting_shader.setVec3("material.specular",1.0f, 1.0f, 1.0f);    
                lighting_shader.setVec3("light.specular",1.0f, 1.0f, 1.0f);

		 model = glm::mat4();

		model = glm::translate(model, glm::vec3(-4.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));

	        lighting_shader.setMat4("model",model);
		
		lighting_shader.setVec3("material.specular",0.0f, 0.0f, 0.0f);
	        lighting_shader.setVec3("light.specular",0.0f, 0.0f, 0.0f);   	
		ourGrave.Draw(lighting_shader);

		lighting_shader.setVec3("material.specular",1.0f, 1.0f, 1.0f);    
                lighting_shader.setVec3("light.specular",1.0f, 1.0f, 1.0f); 

		model = glm::mat4();

		model = glm::mat4();

		 model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
		model = glm::translate(model, moveUp);

		model = glm::rotate(model,glm::radians(world_rotate_y),applyTo);
		model = glm::scale(model, glm::vec3(3.75f, 3.75f, 3.75f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));


	        lighting_shader.setMat4("model",model);
		
		lighting_shader.setVec3("material.specular",0.0f, 0.0f, 0.0f);
	        lighting_shader.setVec3("light.specular",0.0f, 0.0f, 0.0f);
		lighting_shader.setVec3("light.ambient",0.8f, 0.8f, 0.8f);
                lighting_shader.setVec3("material.ambient",0.8f, 0.8f, 0.8f);

		ourGhost.Draw(lighting_shader);
		lighting_shader.setVec3("light.ambient",0.1f, 0.1f, 0.1f);
                lighting_shader.setVec3("material.ambient",0.1f, 0.1f, 0.1f);

		lighting_shader.setVec3("material.specular",1.0f, 1.0f, 1.0f);    
                lighting_shader.setVec3("light.specular",1.0f, 1.0f, 1.0f);

		 model = glm::mat4();

		
		//Grave Stone
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));

	        lighting_shader.setMat4("model",model);
		
		lighting_shader.setVec3("material.specular",0.0f, 0.0f, 0.0f);
	        lighting_shader.setVec3("light.specular",0.0f, 0.0f, 0.0f);   	
		ourGrave.Draw(lighting_shader);

		lighting_shader.setVec3("material.specular",1.0f, 1.0f, 1.0f);    
                lighting_shader.setVec3("light.specular",1.0f, 1.0f, 1.0f); 

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(1.75f, 1.75f, 1.75f));

	        lighting_shader.setMat4("model",model);
		
		lighting_shader.setVec3("material.specular",0.0f, 0.0f, 0.0f);
	        lighting_shader.setVec3("light.specular",0.0f, 0.0f, 0.0f);   	
		ourTree.Draw(lighting_shader);

		lighting_shader.setVec3("material.specular",1.0f, 1.0f, 1.0f);    
                lighting_shader.setVec3("light.specular",1.0f, 1.0f, 1.0f); 

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(-6.0f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(1.75f, 1.75f, 1.75f));

	        lighting_shader.setMat4("model",model);
		
		lighting_shader.setVec3("material.specular",0.0f, 0.0f, 0.0f);
	        lighting_shader.setVec3("light.specular",0.0f, 0.0f, 0.0f);   	
		ourTree.Draw(lighting_shader);

		lighting_shader.setVec3("material.specular",1.0f, 1.0f, 1.0f);    
                lighting_shader.setVec3("light.specular",1.0f, 1.0f, 1.0f); 

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(-6.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.75f, 1.75f, 1.75f));

	        lighting_shader.setMat4("model",model);
		
		lighting_shader.setVec3("material.specular",0.0f, 0.0f, 0.0f);
	        lighting_shader.setVec3("light.specular",0.0f, 0.0f, 0.0f);   	
		ourTree.Draw(lighting_shader);

		lighting_shader.setVec3("material.specular",1.0f, 1.0f, 1.0f);    
                lighting_shader.setVec3("light.specular",1.0f, 1.0f, 1.0f); 

		model = glm::mat4();
		//Tree

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.75f, 1.75f, 1.75f));

	        lighting_shader.setMat4("model",model);
		
		lighting_shader.setVec3("material.specular",0.0f, 0.0f, 0.0f);
	        lighting_shader.setVec3("light.specular",0.0f, 0.0f, 0.0f);   	
		ourTree.Draw(lighting_shader);

		lighting_shader.setVec3("material.specular",1.0f, 1.0f, 1.0f);    
                lighting_shader.setVec3("light.specular",1.0f, 1.0f, 1.0f); 

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 4.0f));
		model = glm::scale(model, glm::vec3(1.75f, 1.75f, 1.75f));

	        lighting_shader.setMat4("model",model);
		
		lighting_shader.setVec3("material.specular",0.0f, 0.0f, 0.0f);
	        lighting_shader.setVec3("light.specular",0.0f, 0.0f, 0.0f);   	
		ourTree.Draw(lighting_shader);

		lighting_shader.setVec3("material.specular",1.0f, 1.0f, 1.0f);    
                lighting_shader.setVec3("light.specular",1.0f, 1.0f, 1.0f); 

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(-6.0f, 0.0f, 4.0f));
		model = glm::scale(model, glm::vec3(1.75f, 1.75f, 1.75f));

	        lighting_shader.setMat4("model",model);
		
		lighting_shader.setVec3("material.specular",0.0f, 0.0f, 0.0f);
	        lighting_shader.setVec3("light.specular",0.0f, 0.0f, 0.0f);   	
		ourTree.Draw(lighting_shader);

		lighting_shader.setVec3("material.specular",1.0f, 1.0f, 1.0f);    
                lighting_shader.setVec3("light.specular",1.0f, 1.0f, 1.0f); 

		model = glm::mat4();







		
	

                model = glm::mat4();
		
		//Gravestone -> Material:Stone. 
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));

	        lighting_shader.setMat4("model",model);
		//Remove if issues:
		lighting_shader.setVec3("material.specular",0.0f, 0.0f, 0.0f);
	        lighting_shader.setVec3("light.specular",0.0f, 0.0f, 0.0f);   	
		ourGrave.Draw(lighting_shader);
		float l = length(light_pos - glm::vec3(-1.0f,0.0f,0.0f));


		lighting_shader.setVec3("material.specular",1.0f, 1.0f, 1.0f);    
                lighting_shader.setVec3("light.specular",1.0f, 1.0f, 1.0f); 

		model = glm::mat4();

		
                model = glm::translate(model, glm::vec3(-0.4f, 0.0f, -8.5f));
	        check_interaction_distance(glm::vec3(0.0f,0.0f,-8.0f));
		if(INTERACTION_CLOSE_ENOUGH == true)
		{
		     
		     model = glm::translate(model, glm::vec3(-0.4f, 0.0f, -0.4f));
		     if(animatedIndex1 >= 90.0f)
		     {
		         animatedIndex1 = 0.0f;
		     }
		     
		     animatedIndex1 = animatedIndex1 + 1.0;
		     if(animatedIndex1 <= 90.0f)
		     {
		           model = glm::rotate(model, glm::radians(animatedIndex1), glm::vec3(0.0, 1.0, 0.0));
		     }


		}
		model = glm::scale(model, glm::vec3(1.75f, 1.75f, 1.75f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
                lighting_shader.setMat4("model",model);
                door.Draw(lighting_shader);
 
		if(animatedIndex <= -90.0f)
		{
		     animatedIndex = 0.0f;
		}
		
	        model = glm::mat4();

		//Doors + Door animation. Check if interaction is close enough, if true open the doors 
		model = glm::translate(model, glm::vec3(0.4f, 0.0f, -8.5f));
	        check_interaction_distance(glm::vec3(0.0f,0.0f,-8.0f));
		if(INTERACTION_CLOSE_ENOUGH == true)
		{
		     
		     model = glm::translate(model, glm::vec3(0.4f, 0.0f, -0.4f));
		     animatedIndex = animatedIndex - 1.0;
		     if(animatedIndex >= -90.0f)
		     {
		           model = glm::rotate(model, glm::radians(animatedIndex), glm::vec3(0.0, 1.0, 0.0));
		     }
		     


		}
		model = glm::scale(model, glm::vec3(1.75f, 1.75f, 1.75f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
                lighting_shader.setMat4("model",model);
                door1.Draw(lighting_shader);



		model = glm::mat4();

		/* Skeleton Object + Skeleton Animation */
		//If he hasn't gone far enough in positive x, go more! If he has gone far enough, start going on negative x. If you've gone too far on the negative
		//x side go back! Repeat. 

		if((moveAcross.x < 5.0f) && (moveAcrossState == 1))
		{
		      moveAcross.x += 0.05f;
		}
		else
	        {
		      moveAcrossState = 0;
		      moveAcross.x -= 0.05f;
		}
		
		if((moveAcross.x < -5.0f) && (moveAcrossState == 0))
		{
		     moveAcrossState = 1;
		}

		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 4.0f));
		model = glm::translate(model, moveAcross);
		//Make him face where he is walking to make it more realistic. 
	
		if(moveAcrossState == 1)
		{
         		    model = glm::rotate(model, glm::radians(animatedSkelIndex), glm::vec3(0.0, 1.0, 0.0));
		}
		else
		{
		    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
		}
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
                lighting_shader.setMat4("model",model);
                ourSkeleton.Draw(lighting_shader);

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 2.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
                lighting_shader.setMat4("model",model);
                ourSign.Draw(lighting_shader);

                model = glm::mat4();
 

                /* House */ 

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
		model = glm::scale(model, glm::vec3(1.60f, 1.60f, 1.60f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
                lighting_shader.setMat4("model",model);

		lighting_shader.setVec3("material.specular",0.1f, 0.1f, 0.1f);    
                lighting_shader.setVec3("light.specular",0.1f, 0.1f, 0.1f); 

                ourHouse.Draw(lighting_shader);

		lighting_shader.setVec3("material.specular",0.0f, 0.0f, 0.0f);    
                lighting_shader.setVec3("light.specular",0.0f, 0.0f, 0.0f); 

		model = glm::mat4();

		/* Front fences */

		model = glm::translate(model, glm::vec3(-9.1f, -0.5f, -25.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
                lighting_shader.setMat4("model",model);
		lighting_shader.setVec3("material.specular",1.0f, 1.0f, 1.0f);    
                lighting_shader.setVec3("light.specular",1.0f, 1.0f, 1.0f); 
                ourFence.Draw(lighting_shader);

		model = glm::mat4();


		model = glm::translate(model, glm::vec3(-6.1f, -0.5f, -25.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);

		model = glm::mat4();


                model = glm::translate(model, glm::vec3(-3.1f, -0.5f, -25.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);

		model = glm::mat4();


		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -25.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);

		model = glm::mat4();


		model = glm::translate(model, glm::vec3(3.1f, -0.5f, -25.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);

		model = glm::mat4();


		model = glm::translate(model, glm::vec3(6.1f, -0.5f, -25.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);

		model = glm::mat4();


		model = glm::translate(model, glm::vec3(9.1f, -0.5f, -25.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);

		model = glm::mat4();


		model = glm::translate(model, glm::vec3(12.1f, -0.5f, -25.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);

		model = glm::mat4();


		model = glm::translate(model, glm::vec3(15.1f, -0.5f, -25.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);


		model = glm::mat4();

		/* Back fences */
		model = glm::translate(model, glm::vec3(-9.1f, -0.5f, 25.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);

		model = glm::mat4();


		model = glm::translate(model, glm::vec3(-6.1f, -0.5f, 25.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);

		model = glm::mat4();


                model = glm::translate(model, glm::vec3(-3.1f, -0.5f, 25.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);

		model = glm::mat4();


		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 25.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);

		model = glm::mat4();


		model = glm::translate(model, glm::vec3(3.1f, -0.5f, 25.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);

		model = glm::mat4();


		model = glm::translate(model, glm::vec3(6.1f, -0.5f, 25.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);

		model = glm::mat4();


		model = glm::translate(model, glm::vec3(9.1f, -0.5f, 25.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);

		model = glm::mat4();


		model = glm::translate(model, glm::vec3(12.1f, -0.5f, 25.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);

		model = glm::mat4();


		model = glm::translate(model, glm::vec3(15.1f, -0.5f, 25.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);


		model = glm::mat4();


                /* Left Fences */

		model = glm::translate(model, glm::vec3(-11.9f, -0.5f, -9.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(-11.9f, -0.5f, -3.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(-11.9f, -0.5f, -6.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);	

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(-11.9f, -0.5f, -12.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);	

		model = glm::mat4();


		model = glm::translate(model, glm::vec3(-11.9f, -0.5f, -15.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);	

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(-11.9f, -0.5f, -18.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);	

		model = glm::mat4();


		model = glm::translate(model, glm::vec3(-11.9f, -0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(-11.9f, -0.5f, 3.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(-11.9f, -0.5f, 6.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);	

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(-11.9f, -0.5f, 9.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);	

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(-11.9f, -0.5f, 12.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);	

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(-11.9f, -0.5f, 15.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);	

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(-11.9f, -0.5f, 18.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);	

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(-11.9f, -0.5f, 21.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);	

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(-11.9f, -0.5f, 24.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);	

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(-11.9f, -0.5f, 27.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);	


		model = glm::mat4();

		/* Right Fences */


		model = glm::translate(model, glm::vec3(11.9f, -0.5f, -9.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);

                

			model = glm::mat4();

		model = glm::translate(model, glm::vec3(11.9f, -0.5f, -3.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(11.9f, -0.5f, -6.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);	

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(11.9f, -0.5f, -12.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);	

		model = glm::mat4();


		model = glm::translate(model, glm::vec3(11.9f, -0.5f, -15.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);	

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(11.9f, -0.5f, -18.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);	

		model = glm::mat4();


		model = glm::translate(model, glm::vec3(11.9f, -0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(11.9f, -0.5f, 3.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(11.9f, -0.5f, 6.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);	

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(11.9f, -0.5f, 9.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);	

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(11.9f, -0.5f, 12.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);	

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(11.9f, -0.5f, 15.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);	

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(11.9f, -0.5f, 18.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);	

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(11.9f, -0.5f, 21.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);	

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(11.9f, -0.5f, 24.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);	

		model = glm::mat4();

		model = glm::translate(model, glm::vec3(11.9f, -0.5f, 27.1f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		
                lighting_shader.setMat4("model",model);
                ourFence.Draw(lighting_shader);	



		/////////////////////////////////////////////////////////////////////////////////////////
		
		
		model = glm::mat4();
		
		/* Oil Lamp */
		model = glm::translate(model, light_pos);
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
                lighting_shader.setMat4("model",model);
		
		lighting_shader.setVec3("material.ambient",0.9f, 0.9f, 0.9f);
	        lighting_shader.setVec3("light.ambient",0.7f, 0.7f, 0.7f);

                oilLamp.Draw(lighting_shader);

		lighting_shader.setVec3("light.ambient",0.1f, 0.1f, 0.1f);
		lighting_shader.setVec3("material.ambient",0.1f, 0.1f, 0.1f);



		model = glm::mat4();





		//Draw objects
		//------------------------------------------------------------------------------------------

		//Antoni's grass + light source code, I just removed his other objects, no other changes. 
		glBindVertexArray(VAO_box);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_grass_diffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex_grass_specular);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, -0.01f, 0.0f));
		model = glm::scale(model, glm::vec3(24.0f, 0.001f, 50.0f));

		lighting_shader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);




		// Draw the light source
		lamp_shader.use();
		
		lamp_shader.setMat4("projection", projection);
		lamp_shader.setMat4("view", view);
		model = glm::mat4();
		model = glm::translate(model, light_pos);
	        if(lightPositionChanged == 1)
		{
	           model += glm::translate(model, (cameraSpeed * camera_front));
		   lightPositionChanged = 0;
		}
		model = glm::scale(model, glm::vec3(0.01f)); // a smaller cube
		lamp_shader.setMat4("model", model);

		 
		if(BUTTON_PRESSED == true) lamp_shader.setFloat("intensity", 1.0); //should be 1
		else lamp_shader.setFloat("intensity", 0.3);

		glBindVertexArray(VAO_light);
		glDrawArrays(GL_TRIANGLES, 0, 36);








		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO_box);
	glDeleteBuffers(1, &VBO_box);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void process_input(GLFWwindow *window)
	{
    	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        	glfwSetWindowShouldClose(window, true); 

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		cameraSpeed = 2.5 * delta_time; 
	else
		cameraSpeed = 2.5 * delta_time * 2;	// double speed with "Shift" pressed


	//Move forward with <W>
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		camera_pos += cameraSpeed * camera_front;
		camera_pos.y = 1; 
		if(lightMoveWithPlayer == 1)
		{
	           light_pos += cameraSpeed * camera_front;
		   light_pos.y = 0.5; 
		   lightPositionChanged = 1; 
		}
	}
                
	//Move back with <S>
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		
		camera_pos -= cameraSpeed * camera_front;
		camera_pos.y = 1.0; 

	        if(lightMoveWithPlayer == 1)
		{
	           light_pos -= cameraSpeed * camera_front;
		   light_pos.y = 0.5;
		   lightPositionChanged = 1; 

		}
        }
	//Rotate across with <A>. 
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		 y -= cameraSpeed * 20;
		 glm::vec3 front;
		 front.x = cos(glm::radians(y)) * cos(glm::radians(p));
		 front.y = sin(glm::radians(p));
		 front.z = sin(glm::radians(y)) * cos(glm::radians(p));
		 camera_front = glm::normalize(front);
	}
        
	//Rotate up/down with <E>/<Q>
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		 p -= cameraSpeed * 20;
		 glm::vec3 front;
		 front.x = cos(glm::radians(y)) * cos(glm::radians(p));
		 front.y = sin(glm::radians(p));
		 front.z = sin(glm::radians(y)) * cos(glm::radians(p));
		 camera_front = glm::normalize(front);
	    
			
	}

	//Rotate up/down with <E>/<Q>
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		
		 p += cameraSpeed * 20;
		 glm::vec3 front;
		 front.x = cos(glm::radians(y)) * cos(glm::radians(p));
		 front.y = sin(glm::radians(p));
		 front.z = sin(glm::radians(y)) * cos(glm::radians(p));
		 camera_front = glm::normalize(front);
	     
			
	}


	//Rotate with <D>
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		
                 y += cameraSpeed * 20;
		 glm::vec3 front;
		 front.x = cos(glm::radians(y)) * cos(glm::radians(p));
		 front.y = sin(glm::radians(p));
		 front.z = sin(glm::radians(y)) * cos(glm::radians(p));
		 camera_front = glm::normalize(front);

		
	}

	//Toggles if the player is holding light or not, notifies render loop of change. 
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && TOGGLEF_DELAY == 0){
		TOGGLEF_DELAY = 5;
		if(lightMoveWithPlayer == 1)
		{
		   lightMoveWithPlayer = 0;
		}
		else if(lightMoveWithPlayer == 0)
		{
		   lightMoveWithPlayer = 1;
		}
	        
	}
//Updates brightness delay, which in turn notifies renderloop of a brightness change. 
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS  && TOGGLEO_DELAY == 0)
	{
	      TOGGLEO_DELAY = 5;
              if(brightnessChange == 0)
	      {  
	        brightnessChange = 1;
	      }
	      else if(brightnessChange == 1)
	      {
		brightnessChange = 0;
              }
	}
//Toggle projection. Just notifies renderloop that a projection change has occured. projectionChange. The ifs are just there to test the projectionType variable
//and serve no other purpose/function in this method. 
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && TOGGLEP_DELAY == 0)
	{
	  TOGGLEP_DELAY = 5;
          
	  if(projectionType == 2)
	  {  
	     projectionChange = 1;
	  }
	  else if(projectionType == 1)
	  {
		projectionChange = 1;
	  }

        }

	//Zoom out/in. Updates FOV value accordingly. Updates zoomChange to notify render loop of change.

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
	TOGGLE1_DELAY = 5;
	if(projectionType == 1)
	{
		if (fov >= 1.0f && fov <= 45.0f)
                   fov -= 0.5f;
                if (fov <= 1.0f)
                   fov = 1.0f;
                if (fov >= 45.0f)
		{
                   fov = 45.0f;
		}
		
	
		zoomChange = 1;

	}
	else if(projectionType == 2)
	{
	     ofov += 0.5;
	     zoomChange = 1;
	}

	ZOOM_DELAY = 30;
	}
	//Zoom out/in. Updates FOV value accordingly. Updates zoomChange to notify render loop of change.
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
	TOGGLE3_DELAY = 5;
	if(projectionType == 1)
	{
	        if (fov >= 1.0f && fov <= 45.0f)
                   fov += 0.5;
                if (fov <= 1.0f)
                   fov = 1.0f;
                if (fov >= 45.0f)
                   fov = 45.0f;
		zoomChange = 1;


	  }
	  else if(projectionType == 2)
	  {
	     projection = glm::ortho(-10.0f/ofov,10.0f/ofov,-10.0f/ofov,10.0f/ofov,-10.0f,20.0f);
	     ofov -= 0.5;
	     zoomChange = 1;
	  }
	}

	//Notifies renderLoop that player wants to jump and that jump key is pressed.
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && SPACE_DELAY == 0)
	{
		pressedKey = 1;
		jumpState = 0;

		SPACE_DELAY = 10;
	}


        //Increase brightness. Increment currentIndex BUT make sure stack overflow does not occur. 
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && TOGGLEK_DELAY == 0)
	{
		TOGGLEK_DELAY = 5;
		if(((currentIndex + 1) <= maxIndex) && ((currentIndex + 1) >= 0))
		{
		     currentIndex += 1;
		}
		
	}

	//Decrease brightness. Decrease currentIndex BUT make sure stack overflow does not occur. 
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && TOGGLEL_DELAY == 0)
	{
		TOGGLEL_DELAY = 5;
		if(((currentIndex - 1) <= maxIndex) && ((currentIndex - 1) >= 0))
		{
		     currentIndex -= 1;
		}
		
	}


}


/* This is all antoni's/learn opengl.*/
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
