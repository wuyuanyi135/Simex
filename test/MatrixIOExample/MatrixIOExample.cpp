//
// Created by wuyua on 2019-07-08.
//


#include <simstruc.h>
#include <cpp_api/block_lifecycle.h>
#include "block.h"
#include "debug_utils.h"

#define OUT_HEIGHT 300
#define OUT_WIDTH 500
#define OUT_CH 3

/// Block
class MatrixIOBlock : public Block {
 public:
  MatrixIOBlock(SimStruct &S) : Block(S) {
      DEBUGV_LIFECYCLE_PRINTF("Block Initialized (MatrixIO)\n");

      // variable size input
      registerVariableSizedInputPort(SS_UINT8, COMPLEX_NO, FRAME_NO, true);
      int dim[] = {OUT_HEIGHT, OUT_WIDTH, OUT_CH};
      registerOutputPort(SS_UINT8, 3, dim, COMPLEX_NO, FRAME_NO);

      allowSignalsWithMoreThan2D = true;

      SampleTime st;
      st.sampleTime = 1;
      st.offset = 0;
      sampleTime.push_back(st);
  }

  void onStart() override {
  }

  void onTerminate() override {
  }

  void onUpdate() override {
      auto in = INPUTPORT_TYPED_POINTER(inputPorts[0], UINT8_T *);
      auto out = OUTPUTPORT_TYPED_POINTER(outputPorts[0], UINT8_T *);
      auto in_height = inputPorts[0]->dimension[0];
      auto in_width = inputPorts[0]->dimension[1];
      auto in_ch = inputPorts[0]->dimension[2];
      for (int c = 0; c < OUT_CH; ++c) {
          for (int w = 0; w < OUT_WIDTH; ++w) {
              for (int h = 0; h < OUT_HEIGHT; ++h) {
                  out[h + w * OUT_HEIGHT + c * OUT_WIDTH*OUT_HEIGHT] = in[h + w*in_height + c*in_width*in_height];
              }
          }
      }
  }

  void onOutput() override {
  }

  ~MatrixIOBlock() {
      DEBUGV_LIFECYCLE_PRINTF("MatrixIOBlock destroyed");
  }
};

std::shared_ptr<Block> createBlock(SimStruct *S) {
    return std::make_shared<MatrixIOBlock>(*S);
}