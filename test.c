#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW

const GLuint WIDTH = 20000, HEIGHT = 10000;
const GLuint POSITION_WIDTH = WIDTH/10;
const GLuint POSITION_HEIGHT = HEIGHT/10;
const GLuint TOTAL_POSITIONS = POSITION_HEIGHT* POSITION_WIDTH;
int convertDenseToSparse(int * dense, int * sparse)
{
  int count = 0;
  for(int i; i<POSITION_HEIGHT;i++)
  {
    for(int j = 0; j<POSITION_WIDTH;j++)
    {
      if(dense[i*POSITION_WIDTH + j])
      {
        sparse[count*2]=j;
        sparse[count*2 + 1]=i;
        count++;
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

int main()
{

  printf("TOTAL_POSITIONS = %d, POSITION_HEIGHT = %d, POSITION_WIDTH = %d",TOTAL_POSITIONS,POSITION_HEIGHT,POSITION_WIDTH);
  int * frame = (int *)malloc(sizeof(int)*TOTAL_POSITIONS);
  int * sparseArray = (int *)malloc(sizeof(int)*2*TOTAL_POSITIONS);
  for(int i = 0; i<TOTAL_POSITIONS; i++)
  {
    frame[i]=rInt(100,10);
  }
  clock_t begin = clock();
  int count = convertDenseToSparse(frame,sparseArray);

  clock_t end = clock();
  double time_spent = (double)(end - begin)/(CLOCKS_PER_SEC);
  double frame_rate = 1/time_spent;
  printf("\n Time = %f , FR = %f ,Count = %d\n",time_spent, frame_rate,count);
}
