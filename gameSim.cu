#include "constants.h"

// struct C2Iter
// {
//   int * data;
//   int width;
//   int position;
//   __device__ int operator[] (int n)
//   {
//     return data[position+n];
//   }
// };
//
// typedef struct C2Iter C2Iter;
//
// struct C1Iter
// {
//   int * data;
//   int width, height;
//   int position;
//   __device__ C2Iter operator[] (int n)
//   {
//     C2Iter x;
//     x.position = this->position + n*this->width;
//     return x;
//   }
// };
//
// typedef struct C1Iter C1Iter;
// struct Cube
// {
//   int * data;
//   int width,height,depth;
//   __device__  C1Iter operator[] (int n)
//   {
//     C1Iter x;
//     x.position = n*this->height*this->width;
//     x.data = this->data;
//     x.width = this->width;
//     x.height = this->height;
//     return x;
//   }
// };
// typedef struct Cube Cube;
static int * deviceFrame;
static cudaDeviceProp properties;
extern void initDevice( int * initialFrame)
{
  cudaMalloc((void **)&deviceFrame,sizeof(int)*TOTAL_POSITIONS);
  cudaMemcpy(deviceFrame,initialFrame,sizeof(int)*TOTAL_POSITIONS,cudaMemcpyHostToDevice);
  cudaGetDeviceProperties(&properties,0);
}

extern void transformOperator(int * hostFrame)
{
  int area = properties.maxThreadsPerBlock / POSITION_WIDTH;

}
