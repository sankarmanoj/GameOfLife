#include "constants.h"
struct cubearray
{
  int * data;
  int  position;
  __device__ __host__ int & operator[] (int3 index)
  {
    int cubeposition = index.z*POSITION_WIDTH*POSITION_HEIGHT + index.y*POSITION_WIDTH + index.z;
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
      return 1;
    }
    else{
      return 1;
    }
  }
};
typedef struct functor functor;
static int * deviceFrame;
extern void initDevice( int * initialFrame)
{
  cudaMalloc((void **)&deviceFrame,sizeof(int)*TOTAL_POSITIONS);
  cudaMemcpy(deviceFrame,initialFrame,sizeof(int)*TOTAL_POSITIONS,cudaMemcpyHostToDevice);
}
__global__ void transformKernel(int * deviceFrame)
{
  int absolutePosition = (blockIdx.y*gridDim.x + blockIdx.x)*blockDim.x+ threadIdx.x;
  if(absolutePosition>TOTAL_POSITIONS)
  {
    return;
  }
  cubearray current_cube;
  current_cube.data = deviceFrame;
  current_cube.position = absolutePosition;
  functor x;
  current_cube[make_int3(1,1,1)] = x(current_cube);

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
  transformKernel<<<dim3(xblocks,yblocks),1024>>>(deviceFrame);
  cudaDeviceSynchronize();
  cudaMemcpy(hostFrame,deviceFrame,sizeof(int)*TOTAL_POSITIONS,cudaMemcpyDeviceToHost);


}
