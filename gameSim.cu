#include "constants.h"
#include <stdio.h>

__constant__ float weights[27];
struct cubearray
{
  int * data;
  int  position;
  __device__ __host__ int & operator[] (int3 index)
  {
    int cubeposition = index.z*4000 + index.y*200 + index.x;
    return data[position + cubeposition];
  }
};
typedef struct cubearray cubearray;
 class functor
{
public:
  functor(float * host_weights)
  {
    cudaMemcpyToSymbol(weights,host_weights,sizeof(float)*27);
  }
  __device__  int operator() (cubearray input)
  {
    int pos;
    float val = 0;
    for(int i = -1; i<1; i++)
      for(int j = -1; j<1; j++)
        for(int k = -1; k<1; k++)
        {

          val += weights[pos]*(float)input[make_int3(k,j,i)];
          pos++;
        }
    if(val>0.8)
    {
      return 1;
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
  cudaMemset(outputDeviceFrame,0,sizeof(int)*TOTAL_POSITIONS);

}
__global__ void transformKernel(int * deviceFrame , int * outputDeviceFrame, functor x)
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
  output_cube[make_int3(0,0,0)]=x(current_cube);

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
  float hw [27] ;
  for(int i = 0; i<27;i++)
  {
    hw[i]=(float)rand()/RAND_MAX;
  }
  functor x(hw);
  // printf("Blocks = %d (x,y)=%d,%d\n",blocks,xblocks,yblocks);
  transformKernel<<<dim3(xblocks,yblocks),1024>>>(deviceFrame,outputDeviceFrame,x);
  cudaDeviceSynchronize();
  cudaMemcpy(hostFrame,outputDeviceFrame,sizeof(int)*TOTAL_POSITIONS,cudaMemcpyDeviceToHost);
  cudaMemcpyAsync(deviceFrame,outputDeviceFrame,sizeof(int)*TOTAL_POSITIONS,cudaMemcpyDeviceToDevice);
  // memset(hostFrame,0,sizeof(int)*TOTAL_POSITIONS);

}
