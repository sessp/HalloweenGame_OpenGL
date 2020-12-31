#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/model.h>
#include <unistd.h>

#include <iostream>
#include <string>

#define PI 3.14159265

// Box coordinate with 36 vertices.
// Every 3 coordinates will form 1 triangle.
// The last 2 columns represent texture coordinate for mapping.
float box[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow *window);
void register_texture(unsigned int * tex, std::string path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// camera
glm::vec3 camera_pos   = glm::vec3(0.0f, 0.5f,  3.0f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f); //z = -1.0f
glm::vec3 camera_up    = glm::vec3(0.0f, 1.0f,  0.0f);
glm::vec3 camera_jump = glm::vec3(0.0f,0.05f,0.0f);
glm::vec3 camera_fall = glm::vec3(0.0f,-0.05f,0.0f);

glm::vec3 camera_dir = camera_pos + camera_front;
glm::mat4 projection;
int pressedKey = 0;

//increase camera front by a small value when moving 
//add button delay to projection.
//add button delay to full bright. 
//Move light 
int projectionType = 0;
float yaw = 0.0f;
float pitch = 0.0f;
float pitch1 = 0.0f;
float yaw1 = 0.0f;

int jumpState = 0;
int zoomChange = 0;
int projectionChange = 0;

float fov   =  45.0f;
float ofov = 0.0f;
//Changed to 0, it was 10.0f

int DELAY_JUMP = 5;

// timing
float delta_time = 0.0f;	// time between current frame and last frame
float last_frame = 0.0f;

//Toggle (Animation or states)
bool BUTTON_PRESSED = false;
int BUTTON_DELAY = 0;
bool BUTTON_CLOSE_ENOUGH = false;

bool SHOW_COORDINATE = false;
int SHOW_DELAY = 0;

//Animation Variables
float curtin_rotate_y = 0.0;
float curtin_translate_y = 0.0;

// Countdown until the button trigger can be pressed again.
// This prevents accidental burst repeat clicking of the key.
void update_delay()
{
	if(BUTTON_DELAY > 0) BUTTON_DELAY -= 1;
	if(SHOW_DELAY > 0) SHOW_DELAY -= 1;
}

// Toggle button pressing only if the camera is close enough.
void toggle_button_distance(glm::vec3 button_pos)
{
	if(glm::length(camera_pos - button_pos) <= 1.6f)
		BUTTON_CLOSE_ENOUGH = true;
	else
		BUTTON_CLOSE_ENOUGH = false;
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
        Shader ourShader("sample1.vs", "sample1.fs");
	//Shader ourShader()
        //"resources/objects/nanosuit/nanosuit.obj"
	Model ourModel(FileSystem::getPath("resources/objects/tree/graved.obj"));
	//Has to be something wrong with settings/export settings. All settings vs no settings
	//Do you need the .blend file? 
	//lack of a mtl file? shouldn't matter but try anyways
	//doesn't have to have triangles? There's apparently an option so it creates triangles. 
	//Mesh setting? Doesn't need to contain normal vec setting, again there is a function. Seems as though meshes are quite important. 
	//Compare settings between the files. 

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

	unsigned int VBO_box, VAO_box;

	glGenVertexArrays(1, &VAO_box);
	glGenBuffers(1, &VBO_box);

	glBindVertexArray(VAO_box);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_box);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);


	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// load and create a texture 
	// -------------------------
	unsigned int tex_wood, tex_street, tex_grass, tex_marble, tex_curtin, tex_sky;
	unsigned int tex_red_dark, tex_red_bright, tex_red, tex_green, tex_blue;

	register_texture(&tex_wood,"resources/textures/wood2.jpg");
	register_texture(&tex_street,"resources/textures/street.png");
	register_texture(&tex_grass,"resources/textures/grass.jpg");
	register_texture(&tex_marble,"resources/textures/marble.jpg");
	register_texture(&tex_curtin,"resources/textures/curtin.jpg");
	register_texture(&tex_sky,"resources/textures/night_sky.jpg");
	register_texture(&tex_red_dark,"resources/textures/red_dark.jpg");
	register_texture(&tex_red_bright,"resources/textures/red_bright.jpg");
	register_texture(&tex_red,"resources/textures/red.jpg");
	register_texture(&tex_green,"resources/textures/green.jpg");
	register_texture(&tex_blue,"resources/textures/blue.jpg");

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use();

	// pass projection matrix to shader (as projection matrix rarely changes there's no need to do this per frame)
	// -----------------------------------------------------------------------------------------------------------
   	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 300.0f);//45.0f
	ourShader.setMat4("projection", projection);
	projectionType = 1;

        //Default Projection 

	// projectionType = 1;
	//projection = glm::ortho(-2.0f,2.0f,-2.0f,2.0f,-10.0f,20.0f);
	//ourShader.setMat4("projection", projection);
	//DEFAULT/STARTING PROJECTION 


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		delta_time = currentFrame - last_frame;
		last_frame = currentFrame;

		//update delay countdown
		update_delay();

		// input
		// -----
		process_input(window);
                
		if(zoomChange == 1)
		{
		    if(projectionType == 1)
		    {
		    	projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
                        ourShader.setMat4("projection", projection);
			zoomChange = 0;
		    }
		    if(projectionType == 2)
		    {
			    projection = glm::ortho(-10.0f/ofov,10.0f/ofov,-10.0f/ofov,10.0f/ofov,-10.0f,20.0f);
			    ourShader.setMat4("projection", projection);
			    zoomChange = 0;
		    }
		}

		if(projectionChange == 1)
		{
		   if(projectionType == 1)
		   {
		     projection = glm::ortho(-2.0f,2.0f,-2.0f,2.0f,-10.0f,20.0f);
		     projectionType = 2;
		     projectionChange = 0;
		     ourShader.setMat4("projection", projection);
		   }
		   else if(projectionType == 2)
		   {
		      	     projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);	                 
			     projectionType = 1;
                             projectionChange = 0;
			     ourShader.setMat4("projection", projection);

		   }
		}
                ourShader.setMat4("projection", projection);

		if(pressedKey == 1)
	        {
		  if((jumpState == 0))
	         	{
		           printf ("%s \n", "Executed up");
	                   camera_pos = camera_pos + camera_jump;
			   camera_dir = camera_dir + camera_jump; 
		           //camera_dir = camera_dir + glm::vec3(0.0f,0.1f,0.0f);

		        } 
		if(camera_pos.y > 1.0f)
		        {
		           printf ("%s \n", "Executed down");
		           jumpState = 1;
		        }
		if(jumpState == 1)
		        {
		        //camera_dir -= camera_dir + glm::vec3(0.0f,-0.25f,0.0f);
		    
		   	 printf ("%s \n", "Bill");
		    	 camera_pos = camera_pos + camera_fall;
			 camera_dir = camera_dir + camera_fall; 
		    	//-0.25

			}
		if(camera_pos.y < 0.5f)
			{
		   	jumpState = 2;
		   	pressedKey = 0;
		      
		   	//need some way to set it to 0 in the start. 
			}

		}


		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 


		// activate shader
		ourShader.use();

                ourShader.setMat4("projection", projection);


		// camera/view transformation
		glm::mat4 view = glm::lookAt(camera_pos,camera_dir, camera_up);
		ourShader.setMat4("view", view);

		//Load model
	        

		glm::mat4 model;
		/*model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
                model = glm::scale(model, glm::vec3(0.2f,0.2f,0.2f));*/
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
	/* Uncomment tangent stuff, it's a quick fix */

		//Texture stuff for obj goes near here?!?
		ourShader.setMat4("model",model);
		ourModel.Draw(ourShader);


		//declare transformation matrix + declaration should be here but redecluration error
	        model = glm::mat4();
		/*
		//example (remember, it is in column matrix position, so the order is reversed.)
		model = glm::translate(model, glm::vec3(1.0f, 2.0f, 3.0f)); 			// translate by (1.0, 2.0, 3.0)
		model = glm::scale(model, glm::vec3(2.0f, 5.0f, 3.0f)); 			// scale by (2.0, 5.0, 3.0) on X, Y, and Z respectively.
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));	// rotate 45 degree about Y-axis (0,1,0)
		*/

		//Draw objects
		//------------------------------------------------------------------------------------------
		
		//Sky
		glBindVertexArray(VAO_box);//This does not have to binded every time after first call, but just for consistency.

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_sky);

		model = glm::mat4();
		model = glm::scale(model, glm::vec3(200.0f, 200.0f, 200.0f));

		ourShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Coordinate System
		if(SHOW_COORDINATE == true)
		{
			
			glm::vec3 coord_scales[] = {
				glm::vec3( 100.0f,  0.02f,  0.02f),	//X
				glm::vec3( 0.02f,  100.0f,  0.02f),	//Y
				glm::vec3( 0.02f,  0.02f,  100.0f),	//Z
			};

			glBindVertexArray(VAO_box);

			glActiveTexture(GL_TEXTURE0);

			for(int tab = 0; tab < 3; tab++)
			{	
				if(tab == 0) glBindTexture(GL_TEXTURE_2D, tex_red); 	//X
				if(tab == 1) glBindTexture(GL_TEXTURE_2D, tex_green);	//Y
				if(tab == 2) glBindTexture(GL_TEXTURE_2D, tex_blue);	//Z

				model = glm::mat4();
				model = glm::scale(model, coord_scales[tab]);

				ourShader.setMat4("model", model);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

// Floor stuff
		//Street
		glBindVertexArray(VAO_box);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_street);

		model = glm::mat4();
		model = glm::scale(model, glm::vec3(3.0f, 0.0f, 7.0f));

		ourShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Grass
		glBindVertexArray(VAO_box);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_grass);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, -0.01f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f, 0.0f, 7.0f));

		ourShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Table (4 tall boxes for legs & 1 thin box as table top)
		glm::vec3 table_scales[] = {
			glm::vec3( 1.0f,  0.1f,  1.0f),	//top
			glm::vec3( 0.1f,  0.5f,  0.1f),//near left
			glm::vec3( 0.1f,  0.5f,  0.1f),	//near right
			glm::vec3( 0.1f,  0.5f,  0.1f),//far left
			glm::vec3( 0.1f,  0.5f,  0.1f),	//far right
		};
		glm::vec3 table_positions[] = {
			glm::vec3( 0.0f,  0.5f,  0.0f),		//top
			glm::vec3(-0.45f, 0.0f,  0.45f),	//near left
			glm::vec3( 0.45f, 0.0f,  0.45f),	//near right
			glm::vec3(-0.45f, 0.0f, -0.45f),	//far left
			glm::vec3( 0.45f, 0.0f, -0.45f),	//far right
		};

		glBindVertexArray(VAO_box);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_wood);

		for(int tab = 0; tab < 5; tab++)
		{	
			model = glm::mat4();
			model = glm::translate(model, table_positions[tab]);
			model = glm::scale(model, table_scales[tab]);
			model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));

			ourShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		//Button on table (1 big box & 1 small box as button)
		glm::vec3 button_scales[] = {
			glm::vec3( 0.2f,  0.12f,  0.2f),		//case
			glm::vec3( 0.12f,  0.12f,  0.12f),		//button
		};

		float red_button_height = 0.05f;
		if(BUTTON_PRESSED == true) {red_button_height -= 0.02f;}

		glm::vec3 button_positions[] = {
			glm::vec3( 0.0f,  0.0f,  0.0f),			//case
			glm::vec3( 0.0f,  red_button_height,  0.0f),	//button
		};

		glm::vec3 button_final_location = glm::vec3(0.0f, 0.56f, 0.25f);
		toggle_button_distance(button_final_location); 

		glBindVertexArray(VAO_box);
		
		for(int tab = 0; tab < 2; tab++)
		{	
			glActiveTexture(GL_TEXTURE0);
			if(tab == 0)
			{	
				glBindTexture(GL_TEXTURE_2D, tex_marble);
			}
			else
			{
				if(BUTTON_PRESSED == false)
				{
					glBindTexture(GL_TEXTURE_2D, tex_red_dark); 	// Not pressed
				}
				else
				{
					glBindTexture(GL_TEXTURE_2D, tex_red_bright);	// Pressed
				}
			}

			model = glm::mat4();
			model = glm::translate(model, button_final_location);
			model = glm::translate(model, button_positions[tab]);
			model = glm::scale(model, button_scales[tab]);
			model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));

			ourShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}



		//Curtin Logo
		glBindVertexArray(VAO_box);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_curtin);

		//transformation for animation
		if(BUTTON_PRESSED == true)
		{
			curtin_translate_y += 1.0f;
			curtin_rotate_y += 1.0f;
			if(abs(curtin_translate_y - 360.0f) <= 0.1f) curtin_translate_y = 0.0f;
			if(abs(curtin_rotate_y - 360.0f) <= 0.1f) curtin_rotate_y = 0.0f;
		}

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 0.8f + (0.1f * sin(curtin_translate_y * PI / 180.f)), -0.35f));
		model = glm::rotate(model, glm::radians(curtin_rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.0f));

		ourShader.setMat4("model", model);

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
	/* sin/cos(glfwGetTime()) times radius take out radians and replace with twofive and one. Maybe try to normalize it?!?? */
	float rotX = sin(glm::radians(90.0f)) * 0.1;
	float rotZ = cos(glm::radians(90.0f)) * 0.1;
    	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        	glfwSetWindowShouldClose(window, true);

	float cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		cameraSpeed = 2.5 * delta_time; 
	else
		cameraSpeed = 2.5 * delta_time * 2;	// double speed with "Shift" pressed


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera_pos += cameraSpeed * camera_front;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera_pos -= cameraSpeed * camera_front;

	if(glfwGetKey(window,GLFW_KEY_1) == GLFW_PRESS)
	{
	    if(pitch < 180.0)
            {
		glm::vec3 rotateUp;
		rotateUp.x = cos(glm::radians(90.0f));
		rotateUp.z = cos(glm::radians(90.0f));
	        rotateUp.y = sin(glm::radians(pitch));
		camera_dir += (cameraSpeed/2) * glm::normalize(rotateUp);
		pitch = pitch + 1.0;
	   }
        }

        if(glfwGetKey(window,GLFW_KEY_3) == GLFW_PRESS)
	{
	    if(pitch1 > -180.0)
            {
		glm::vec3 rotateUp;
		//-90.0f vs pitch1;
		rotateUp.x = cos(glm::radians(-90.0f));
		rotateUp.z = cos(glm::radians(-90.0f));
	        rotateUp.y = sin(glm::radians(pitch1));
		camera_dir += (cameraSpeed/2) * glm::normalize(rotateUp);
		pitch1 = pitch1 - 1.0;
	   /*Should be += */
	   }
        }

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
	     
             glm::vec3 rotateRight;
	     //printf("%f",yaw);
             rotateRight.x = cos(glm::radians(yaw1));
             rotateRight.z = sin(glm::radians(yaw1));
	     camera_dir -= (cameraSpeed/2) * glm::normalize(rotateRight);
	     yaw1 = yaw1 - 1.0;
	     /*
	      *  
             glm::vec3 rotateRight;
	     //printf("%f",yaw);
             rotateRight.x = cos(glm::radians(yaw1));
             rotateRight.z = sin(glm::radians(yaw1));
	     camera_dir -= (cameraSpeed/2) * glm::normalize(rotateRight);
	     yaw1 = yaw1 - 1.0; 

	      * */

	}

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{

          //Notify render loop that change has occured. 
	  if(projectionType == 2)
	  {  
	     projectionChange = 1;
	  }
	  else if(projectionType == 1)
	  {
		projectionChange = 1;
	  }

        }

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
	     /* adding to yaw */
             glm::vec3 rotateLeft;
	     //printf("%f",yaw);
             rotateLeft.x = cos(glm::radians(yaw));
             rotateLeft.z = sin(glm::radians(yaw));
	     camera_dir += (cameraSpeed/2) * glm::normalize(rotateLeft);
	     yaw = yaw + 1.0;
	     
	     /* cos 90*/
	     /* normalised rotateLeft vs non normalised vs add to current camera_Dir */
	     /* Do we really need to rotate z*/	     

	}
		
		/* You'll have to reset so the other functions work properly.  */
		/* -= vs += vs = */
		/* camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * cameraSpeed; */
		/* try just a number without rotZ. */
		/*camera_pos += glm::vec3(rotX, 0.0f,rotZ);*/
	       /* camera_front += glm::vec3(rotX, 0.0f,rotZ);*/
/*	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * cameraSpeed;*/
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
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
		//projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	
		zoomChange = 1;
		// ourShader.setMat4("projection", projection);

	}
	else if(projectionType == 2)
	{
	     //projection = glm::ortho(-10.0f/ofov,10.0f/ofov,-10.0f/ofov,10.0f/ofov,-10.0f,20.0f);
	     ofov += 0.5;
	     zoomChange = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	if(projectionType == 1)
	{
	        if (fov >= 1.0f && fov <= 45.0f)
                   fov += 0.5;
                if (fov <= 1.0f)
                   fov = 1.0f;
                if (fov >= 45.0f)
                   fov = 45.0f;
	//	projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		zoomChange = 1;
	//	 ourShader.setMat4("projection", projection);


	}else if(projectionType == 2)
	{
             //fix boundary limitation 
	     projection = glm::ortho(-10.0f/ofov,10.0f/ofov,-10.0f/ofov,10.0f/ofov,-10.0f,20.0f);
	     ofov -= 0.5;
	     zoomChange = 1;
	}	
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && BUTTON_DELAY == 0)
	{
		//&& BUTTON_DELAY == 0
		
		pressedKey = 1;
		jumpState = 0;

		BUTTON_DELAY = 10;
		/*
		if((jumpState == 0))
		{
		   printf ("%s \n", "Executed up");
	          camera_pos = camera_pos + camera_jump;
		  //camera_dir = camera_dir + glm::vec3(0.0f,0.25f,0.0f);

		} 
		if(camera_pos.y > 1.0f)
		{
		  printf ("%s \n", "Executed down");
		  jumpState = 1;
		}
		if(jumpState == 1)
		{
		    //camera_dir -= camera_dir + glm::vec3(0.0f,-0.25f,0.0f);
		    
		    printf ("%s \n", "Bill");
		    camera_pos = camera_pos + glm::vec3(0.0f,-0.25f,0.0f);
		    //-0.25

		}
		if(camera_pos.y < 0.2f)
		{
		   jumpState = 2;
		   pressedKey = 0;
		      
		   //need some way to set it to 0 in the start. 
		}*/

		/*
		if((camera_pos.y > 1.0f))
		{
		         //if((camera_dir.y > 0.0f) && (camera_pos.y > 0.0f))
			 //{
			//start jumping down
                        camera_dir -= camera_dir + glm::vec3(0.0f,-0.25f,0.0f);
			camera_pos -= camera_pos + glm::vec3(0.0f,-0.25f,0.0f);
			
			//camera_pos += camera_pos + glm::vec3(0.0f,-0.35f,0.0f);
			 //camera_pos = camera_pos -= camera_jump;
			// }
		}*/

		
		/*
		 * glm::mat4 view = glm::lookAt(camera_pos,camera_dir, camera_up);
		 * */

		//camera_dir = camera_dir - glm::vec3(0.0f,0.25f,0.0f);
		//camera_pos = camera_pos - glm::vec3(0.0f,0.25f,0.0f);

		//Create some kinf of a delay + remove them back down. 
		//camera_pos = camera_pos - glm::vec3(0.0f,0.125f,0.0f);
	}

	      
	//speed 
	//toggle red button
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && BUTTON_DELAY == 0 && BUTTON_CLOSE_ENOUGH == true)
	{
		BUTTON_DELAY = 20;
		if(BUTTON_PRESSED == false) 		
			BUTTON_PRESSED = true;
		else
			BUTTON_PRESSED = false;
	}

	//toggle coordinate visibility
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && SHOW_DELAY == 0)
	{
		SHOW_DELAY = 20;
		if(SHOW_COORDINATE == false) 		
			SHOW_COORDINATE = true;
		else
			SHOW_COORDINATE = false;
	}
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}



//registering a new texture. You can modify this function to allow for more configuration options.
void register_texture(unsigned int * tex, std::string path)
{
	glGenTextures(1, tex);
	glBindTexture(GL_TEXTURE_2D, *tex);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderColor [] = {1.0f, 1.0f, 1.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load(FileSystem::getPath(path).c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}
/*
 * Light Notes Colors:
 * Uses RGB
 * When defining LS; - Must have a light source colour
 * Multiplying Light colour x object rgb value = Colour reflected.
 *
 * Rf, Gf, Bf,
 * EG
 *
 * 0 1 0
 * 1 5 0.3
 * = 0.5 green 0 blue/red
 * */
/*
 *Possible fixes
 https://stackoverflow.com/questions/22276206/why-does-assimp-fail-to-generate-tangents
 Do textures, and all other data.
 try .3ds
 Else make tangents can be avoided by specifying texture coords. 
 convert num1//num2 to num1/num2/num3 in blender somehow. 
 * */
