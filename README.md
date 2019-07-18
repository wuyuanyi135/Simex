# Simex
_C++ Simulink S-Function Framework_

## Usage

Follow the sample projects in `example` and `test`.

### CMakeLists.txt

```cmake
project(SimexExample)
cmake_minimum_required(VERSION 3.14)

add_compile_definitions(S_FUNCTION_NAME=SimexExample)  # 1. Define the S_FUNCTION_NAME the same as the target name


include(../CMakeLists.txt) # 2. include the project cmake file
include_directories(${SIMEX_INCLUDES}) # 3. include headers

set(SOURCES SimexExampleBlock.cpp)

set(CMAKE_BUILD_TYPE Debug)
matlab_add_mex(
        NAME SimexExample # 4. set the target name
        SRC ${SIMEX_SOURCES} ${SOURCES} # 5. include all sources
        LINK_TO ${Matlab_MEX_LIBRARY}
)
```

### C++ Implementation

```
#include "block.h" // 1. include the header

class SimexExampleBlock : public Block { // 2. inherit the Block class
 public:  // 3. implement the constructor, destructor, and callbacks.
  explicit SimexExampleBlock(SimStruct &S) : Block(S) {
      ssPrintf("Block created\n");
      int dim[] = {1};
      registerInputPort(SS_DOUBLE, 1, dim, COMPLEX_NO, FRAME_NO, true); // 4. register input, output, and parameters
      registerOutputPort(SS_DOUBLE, 1, dim, COMPLEX_NO, FRAME_NO);
  }
  ~SimexExampleBlock() override {
      ssPrintf("Block destroyed\n");
  }
  void onInitializeRuntime() override {
      Block::onInitializeRuntime();
  }
  void onStart() override {

  }
  void onTerminate() override {

  }
  void onUpdate() override {

  }
  void onOutput() override {
    OUTPUTPORT_TARGET(outputPorts[0], double*) = INPUTPORT_TARGET(inputPorts[0], double*) * 2; // 5. use macro INPUTPORT_TARGET to access the port data
  }
};

```