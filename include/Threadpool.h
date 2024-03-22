#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>

namespace SM
{
    class Threadpool
    {
    private:
        std::vector<std::thread> threads_;
        std::mutex mutex_;
        std::condition_variable cv_;
        std::queue<std::function<void()>> tasks_;

        void IdleLoop();
        
    public:
        Threadpool();
        void StartPool();
        void QueueTask(std::function<void()> task);
        bool IsPoolBusy() const;
        void StopPool();
        
        ~Threadpool();
    };
}
#endif