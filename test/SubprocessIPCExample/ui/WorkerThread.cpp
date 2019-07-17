//
// Created by wuyuanyi on 16/07/19.
//
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_semaphore.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <QDebug>
#include "WorkerThread.h"


using namespace boost::interprocess;
using namespace boost::posix_time;

void WorkerThread::run() {
    qInfo() << "WorkerThread started";
    while (!stopRequested) {
        auto timeout = second_clock::local_time() + millisec(100);
        if (sem.timed_wait(timeout)) {
            mtx.lock();
            emit DataReceived(*dataPtr);
            mtx.unlock();
        }
    }
    qInfo() << "WorkerThread stopped";
}
WorkerThread::WorkerThread(QObject *parent, named_mutex &mtx, named_semaphore &sem, double *dataPtr)
    : QThread(parent), mtx(mtx), sem(sem), dataPtr(dataPtr) {

}
void WorkerThread::stop() {
    stopRequested = true;
    // call join after this!
}
