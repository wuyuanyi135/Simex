//
// Created by wuyuanyi on 16/07/19.
//

#ifndef SIMEX_WORKERTHREAD_H
#define SIMEX_WORKERTHREAD_H

#include <QtCore/QThread>
#include <boost/interprocess/sync/named_mutex.hpp>
using namespace boost::interprocess;

class WorkerThread : public QThread {
 Q_OBJECT
 public:
  WorkerThread(QObject *parent,
                 named_mutex &mtx,
                 named_semaphore &sem,
                 double *dataPtr);

 private:
  named_mutex &mtx;
  named_semaphore &sem;
  double* dataPtr;

 protected:
  void run() override;

 signals:
  void DataReceived(double data);

 public:
  void stop();

 private:
  bool stopRequested{false};
};

#endif //SIMEX_WORKERTHREAD_H
