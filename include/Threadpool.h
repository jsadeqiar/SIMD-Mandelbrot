#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace SM
{
    class Threadpool
    {
    private:
        std::vector<std::thread> threads_;
        std::mutex mutex_;
        std::condition_variable cv_;

        void IdleLoop();

        
    public:
        Threadpool();
        void StartPool();
        void QueueTask();
        bool IsPoolBusy() const;
        void StopPool();
        
        ~Threadpool();
    };
}
#endif