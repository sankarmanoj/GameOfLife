#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// #define DEBUG
// Other includes
#include "Shader.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//For usleep
#include "constants.h"
#include <unistd.h>
#include<time.h>
#include "gameSim.h"
#define MAX_PARTICLES 1000
#ifndef __APPLE__
#define nullptr NULL
#endif
int frameRate = 40;
int debugCount = 40;
int debugPrintCount = 4;
int numberParticles = 1024*1;
float bright = 1.0f;
//Init Camera
Camera camera(glm::vec3(0.0f,0.0f,25.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
bool paused = false;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
// Window dimensions

//Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
int convertDenseToSparse(int * dense, int * sparse)
{
  int count = 0;
  for(uint h = 0; h<POSITION_DEPTH;h++)
  {
    for(uint i =0 ; i<POSITION_HEIGHT;i++)
    {
      for(uint j = 0; j<POSITION_WIDTH;j++)
      {
        if(dense[h*POSITION_HEIGHT*POSITION_WIDTH + i*POSITION_WIDTH + j])
        {

          sparse[count*3]=j-HALF_POSITION_WIDTH;
          sparse[count*3 + 1]=i;
          sparse[count*3 + 2]=h+DEPTH_OFFSET;
          count++;
        }
      }
    }
  }
  return count;
}
GLuint rInt(GLuint Max,GLuint Threshold)
{
  GLuint value =  (GLuint)(((float)rand()/(float)RAND_MAX)*(float)Max);
  if(value>Threshold)
  return 1;

  return 0;
}

GLuint rInt(GLuint Max)
{
  return (GLuint)(((float)rand()/(float)RAND_MAX)*(float)Max);
}
GLfloat rFloat(GLuint Max)
{
   return (GLfloat)(((float)rand()/(float)RAND_MAX)*(float)Max);
}
// The MAIN function, from here we start the application and run the game loop
int main()
{
    //Seed Random Number Generator Function
    srand (time(NULL));

    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_POINT_SMOOTH);
    //Enable Point Drawing
    glEnable(GL_PROGRAM_POINT_SIZE);

    // Build and compile our shader program
    Shader ourShader("default.vs", "default.frag");
    GLuint currentNumberOfCells;
    GLint * cellFrame = (GLint*)malloc(sizeof(int)*TOTAL_POSITIONS);
    GLint * openGLSparseFrame = (GLint*)malloc(sizeof(int)*3*TOTAL_POSITIONS);

    for(uint i = 0; i<TOTAL_POSITIONS; i++)
    {
      cellFrame[i]=rInt(100,70);
    }
    currentNumberOfCells = convertDenseToSparse(cellFrame,openGLSparseFrame);
    printf("Current Number of Cells = %d\n",currentNumberOfCells);
    initDevice(cellFrame);


    // for(int i = 0; i<numberParticles*8;i++)
    // {
    //   std::cout<<sizeof(particles)<<"   "<<particles[i]<<"   "<<i<<std::endl;
    // }
    // X    Y   Z   VX    VY    VZ    Mass    Charge
    // 0    1   2   3     4     5     6       7
    // for (int i =0; i<8*numberParticles; i++)
    // {
    //   std::cout<<particles[i]<<":";
    //   if((i+1)%8==0)
    //   std::cout<<std::endl;
    // }
        GLuint VBO, VAO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);


    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,3*currentNumberOfCells*sizeof(GLint),openGLSparseFrame,GL_STREAM_DRAW);
    //Position Attributes
    glVertexAttribPointer(0,3,GL_INT,GL_FALSE,3*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);
    //Mass Atrribute
    // glVertexAttribPointer(1,1,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid *)(6*sizeof(GLfloat)));
    // glEnableVertexAttribArray(1);
    // //Charge Attribute
    // glVertexAttribPointer(2,1,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid *)(7*sizeof(GLfloat)));
    // glEnableVertexAttribArray(2);

    glBindVertexArray(0);


    int count = 0;
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        GLfloat currentFrame = glfwGetTime();

        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
      //  usleep(1000*(1000/frameRate-deltaTime));
        glfwPollEvents();


        Do_Movement();
      //  usleep(1000*20);
        // Render
        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        // Draw the triangle
        ourShader.Use();
        //Setup tranform matricies
        glm::mat4 view, projection,model;
        view = camera.GetViewMatrix();
        projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 1000.0f);
        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
        GLint cameraLoc = glGetUniformLocation(ourShader.Program, "camera");
        GLint brightLoc = glGetUniformLocation(ourShader.Program, "brightnessConstant");
        glUniformMatrix4fv(viewLoc,1,GL_FALSE,glm::value_ptr(view));
        glUniformMatrix4fv(projLoc,1,GL_FALSE,glm::value_ptr(projection));
        glUniform3f(cameraLoc,camera.Position.x,camera.Position.y,camera.Position.z);
        glUniform1f(brightLoc,bright);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBindVertexArray(VAO);
        //model = glm::rotate(model,(GLfloat)glfwGetTime()*20.0f,glm::vec3(0.0f,0.0f,1.0f));
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
        glBufferData(GL_ARRAY_BUFFER,3*currentNumberOfCells*sizeof(GLint),openGLSparseFrame,GL_STREAM_DRAW);
        glDrawArrays(GL_POINTS , 0, currentNumberOfCells);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        // Swap the screen buffers
        glfwSwapBuffers(window);
        count++;
        transformOperator(cellFrame);
        currentNumberOfCells = convertDenseToSparse(cellFrame,openGLSparseFrame);
        printf("Current Number of Cells = %d\n",currentNumberOfCells);

    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Moves/alters the camera positions based on user input
void Do_Movement()
{
    // Camera controls
    if(keys[GLFW_KEY_W])
      camera.ProcessKeyboard(FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
      camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
      camera.ProcessKeyboard(LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
      camera.ProcessKeyboard(RIGHT, deltaTime);
    if(keys[GLFW_KEY_R])
      bright *=1.2;
    if(keys[GLFW_KEY_F])
      bright /=1.2;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  //  std::cout << key << std::endl;
   if(key=='P')
   paused=true;
   else if (key=='U')
   paused = false;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if(action == GLFW_PRESS)
            keys[key] = true;
        else if(action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
