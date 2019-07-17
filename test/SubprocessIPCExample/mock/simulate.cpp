//
// Created by wuyuanyi on 17/07/19.
//
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/named_semaphore.hpp>
#include <boost/process.hpp>

#include <string>
#include <thread>
#include <memory>
#include <iostream>

#define MUTEX_NAME "Simulate_Shared_Mutex_Name"
#define MEMORY_NAME "Simulate_Shared_Memory_Name"
#define SEMAPHORE_NAME "Simulate_Semaphore_Name"

using namespace boost::interprocess;
using namespace boost::process;

void wrapper() {
    try {
        auto shm = std::unique_ptr<managed_shared_memory, std::function<void(managed_shared_memory *)>>(
            new managed_shared_memory(create_only, MEMORY_NAME, 4096),
            [](managed_shared_memory *m) {
              shared_memory_object::remove(MEMORY_NAME);
              std::cout << "removed shared memory: " << MEMORY_NAME << std::endl;
              delete m;
            }
        );
        std::cout << "Created shared memory: " << MEMORY_NAME << std::endl;

        auto sem = std::unique_ptr<named_semaphore, std::function<void(named_semaphore *)>>(
            new named_semaphore(create_only, SEMAPHORE_NAME, 0),
            [](named_semaphore *m) {
              named_semaphore::remove(SEMAPHORE_NAME);
              std::cout << "removed shared semaphore: " << SEMAPHORE_NAME << std::endl;
              delete m;
            }
        );
        std::cout << "Created shared semaphore: " << SEMAPHORE_NAME << std::endl;

        auto mtx = std::unique_ptr<named_mutex, std::function<void(named_mutex *)>>(
            new named_mutex(create_only, MUTEX_NAME),
            [](named_mutex *m) {
              named_mutex::remove(MUTEX_NAME);
              std::cout << "removed shared mutex: " << MUTEX_NAME << std::endl;
              delete m;
            }
        );
        std::cout << "Created shared mutex: " << MUTEX_NAME << std::endl;

        double *data = shm->construct<double>("data")();
        *data = 0;

        ipstream stdoutStream;
        ipstream stderrStream;

        child childProc("./SubprocessIPCDisplay", MEMORY_NAME, MUTEX_NAME, SEMAPHORE_NAME,
                        std_out > stdout,
                        std_err > stdout);


        std::thread th([data, &sem, &mtx]() {
          for (int j = 0; j < 5; ++j) {
              mtx->lock();
              *data = static_cast<double> (j);
              mtx->unlock();
              sem->post();
              std::cout << "Issued " << j << std::endl;
              std::this_thread::sleep_for(std::chrono::seconds(1));
          }
        });

        th.join();
        childProc.terminate();
        childProc.wait();

    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

}
int main() {
    wrapper();
    return 0;
}