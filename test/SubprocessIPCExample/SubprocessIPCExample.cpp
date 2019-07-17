//
// Created by wuyuanyi on 15/07/19.
//

#include "block.h"

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/named_semaphore.hpp>
#include <boost/process.hpp>
#include <boost/process/env.hpp>
#include <boost/process/environment.hpp>


#define MUTEX_NAME "Simulate_Shared_Mutex_Name"
#define MEMORY_NAME "Simulate_Shared_Memory_Name"
#define SEMAPHORE_NAME "Simulate_Semaphore_Name"

using namespace boost::interprocess;
using namespace boost::process;

class SubprocessIPCExample : public Block {

 public:
  std::unique_ptr<managed_shared_memory, std::function<void(managed_shared_memory *)>> shm;
  std::unique_ptr<named_semaphore, std::function<void(named_semaphore *)>> sem;
  std::unique_ptr<named_mutex, std::function<void(named_mutex *)>> mtx;
  double *data;
  child childProc;

 public:
  explicit SubprocessIPCExample(SimStruct &S) : Block(S) {
      int dims[] = {1};
      registerInputPort(SS_DOUBLE, 1, dims, COMPLEX_NO, FRAME_NO, 0);
  }
  void onStart() override {
      shm = std::unique_ptr<managed_shared_memory, std::function<void(managed_shared_memory *)>>(
          new managed_shared_memory(create_only, MEMORY_NAME, 4096),
          [](managed_shared_memory *m) {
            shared_memory_object::remove(MEMORY_NAME);
            delete m;
          }
      );

      sem = std::unique_ptr<named_semaphore, std::function<void(named_semaphore *)>>(
          new named_semaphore(create_only, SEMAPHORE_NAME, 0),
          [](named_semaphore *m) {
            named_semaphore::remove(SEMAPHORE_NAME);
            delete m;
          }
      );

      mtx = std::unique_ptr<named_mutex, std::function<void(named_mutex *)>>(
          new named_mutex(create_only, MUTEX_NAME),
          [](named_mutex *m) {
            named_mutex::remove(MUTEX_NAME);
            delete m;
          }
      );

      data = shm->construct<double>("data")();

      // Matlab may mask the qt library so override it here.
      childProc = child("./cmake-build-debug/test/SubprocessIPCExample/SubprocessIPCDisplay", MEMORY_NAME, MUTEX_NAME, SEMAPHORE_NAME, env["LD_LIBRARY_PATH"] = "/usr/lib/x86_64-linux-gnu",
                      std_out > "stdout.txt",
                      std_err > "stderr.txt");

  }
  void onTerminate() override {
      childProc.terminate();
      childProc.wait();
  }
  void onUpdate() override {
      mtx->lock();
      *data = INPUTPORT_TARGET(inputPorts[0], real64_T*);
      mtx->unlock();
      sem->post();
  }
  void onOutput() override {

  }


};

std::shared_ptr<Block> createBlock(SimStruct *S) {
    return std::make_shared<SubprocessIPCExample>(*S);
}