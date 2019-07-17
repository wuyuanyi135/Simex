//
// Created by wuyuanyi on 17/07/19.
//
#include <QtWidgets/QApplication>
#include <QDebug>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_semaphore.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <iostream>
#include "WorkerThread.h"
#include "NumberDisplay.h"

using namespace boost::interprocess;

enum ArgvEnum {
  ProcessName = 0,
  SharedMemoryName,
  SharedMutexName,
  SharedSemaphoreName,
};

void errorExit(std::string msg) {
    std::cerr << msg << std::endl;
    exit(-1);
}

int main(int argc, char **argv) {
    if (argc != 4) {
        errorExit("3 arguments expected");
    }

    managed_shared_memory managed_shm(open_only, argv[SharedMemoryName]);
    std::pair<double *, std::size_t> p = managed_shm.find<double>("data");
    if (p.first == nullptr) {
        errorExit("'data' segment is not created!");
    }
    named_semaphore named_sem(open_only, argv[SharedSemaphoreName]);
    named_mutex named_mtx(open_only, argv[SharedMutexName]);
    // TODO: check

    QApplication a(argc, argv);
    qInfo() << "Application Started!";

    WorkerThread wt(nullptr, named_mtx, named_sem, p.first);

    NumberDisplay display(nullptr);
    QObject::connect(&wt, &WorkerThread::DataReceived, &display, &NumberDisplay::updateContent);
    display.show();
    wt.start();

    int ret = a.exec();
    wt.stop();
    wt.wait();
    return ret;
}
