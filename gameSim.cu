#include "constants.h"
#include <stdio.h>
struct cubearray
{
  int * data;
  int  position;
  __device__ __host__ int & operator[] (int3 index)
  {
    int cubeposition = index.z*POSITION_WIDTH*POSITION_HEIGHT + index.y*POSITION_WIDTH + index.z;
    printf("%d\n", cubeposition+position);
    return data[position+cubeposition];
  }
};
typedef struct cubearray cubearray;
 struct functor
{
  __device__ __host__  int operator() (cubearray input)
  {
    int count = input[make_int3(1,1,1)]+input[make_int3(1,1,0)]+input[make_int3(1,0,1)]+input[make_int3(0,1,1)];
    if(count>1)
    {
      return 0;
    }
    else{
      return 0;
    }
  }
};
typedef struct functor functor;
static int * deviceFrame;
static int * outputDeviceFrame;
extern void initDevice( int * initialFrame)
{
  cudaMalloc((void **)&deviceFrame,sizeof(int)*TOTAL_POSITIONS);
  cudaMemcpy(deviceFrame,initialFrame,sizeof(int)*TOTAL_POSITIONS,cudaMemcpyHostToDevice);
  cudaMalloc((void **)&outputDeviceFrame,sizeof(int)*TOTAL_POSITIONS);
  cudaMemcpy(outputDeviceFrame,initialFrame,sizeof(int)*TOTAL_POSITIONS,cudaMemcpyHostToDevice);

}
__global__ void transformKernel(int * deviceFrame , int * outputDeviceFrame)
{
  int absolutePosition = (blockIdx.y*gridDim.x + blockIdx.x)*1024+ threadIdx.x;

  if(absolutePosition>TOTAL_POSITIONS)
  {

    return;
  }
    // if(threadIdx.x<2)
    // printf("Absolute Position = %d , BlockIdx.(x,y) = (%d,%d) gridDim.x = %d\n",absolutePosition,blockIdx.x,blockIdx.y,gridDim.x);
  cubearray current_cube,output_cube;
  current_cube.data = deviceFrame;
  output_cube.data = outputDeviceFrame;
  output_cube.position = absolutePosition;
  current_cube.position = absolutePosition;
  output_cube[make_int3(1,1,1)]=0;

}
extern  void transformOperator(int * hostFrame)
{
  int blocks = ceil((float)TOTAL_POSITIONS/1024);
  int xblocks=1,yblocks=1;
  if(blocks>65536)
  {
    xblocks=65536;
    yblocks=blocks/65536;
  }
  else{
    xblocks = blocks;
  }
  // printf("Blocks = %d (x,y)=%d,%d\n",blocks,xblocks,yblocks);
  transformKernel<<<dim3(1,1),1024>>>(deviceFrame,outputDeviceFrame);
  cudaDeviceSynchronize();
  cudaMemcpy(hostFrame,outputDeviceFrame,sizeof(int)*TOTAL_POSITIONS,cudaMemcpyDeviceToHost);
  cudaMemcpyAsync(deviceFrame,outputDeviceFrame,sizeof(int)*TOTAL_POSITIONS,cudaMemcpyDeviceToDevice);

}
