CC=g++
FRAMEWORK = OpenGL
OUTPUT = myrun
CUDAOUT = gameSim.o
LDFLAGS=-Wl,-rpath,/usr/local/cuda/lib
all:  cuda build

run: cuda build
	export LD_LIBRARY_PATH=/usr/local/cuda/lib:$(LD_LIBRARY_PATH)
	exec ./$(OUTPUT)

build : sim.cpp default.frag default.vs Camera.h Shader.h cuda
		$(CC) $(CUDAOUT)  sim.cpp  $(LDFLAGS) -L/usr/local/cuda/lib -L/usr/local/cuda-8.0/lib64 -lcuda  -lcudart -lglfw -lGL -lGLEW -o $(OUTPUT)
debug:  sim.cpp default.frag default.vs Camera.h Shader.h
		$(CC) $(CUDAOUT)  sim.cpp  $(LDFLAGS) -g  -Wall -L/usr/local/cuda/lib -l cuda  -l cudart -l glfw3 -l GLEW -framework $(FRAMEWORK) -o $(OUTPUT)
cuda : gameSim.cu
	nvcc -c gameSim.cu -o $(CUDAOUT)
clean :
	rm *.o
	rm $(OUTPUT)
