//
// Created by wuyuanyi on 13/07/19.
//

#include "block.h"


class AsyncImageFrameDisplayBlock : public Block {
 private:

 public:
  explicit AsyncImageFrameDisplayBlock(SimStruct &S) : Block(S) {

  }
  ~AsyncImageFrameDisplayBlock() override {

  }
  void onStart() override {
  }
  void onTerminate() override {
  }
  void onUpdate() override {

  }
  void onOutput() override {

  }
};

std::shared_ptr<Block> createBlock(SimStruct *S) {
    return std::make_shared<AsyncImageFrameDisplayBlock>(*S);
}