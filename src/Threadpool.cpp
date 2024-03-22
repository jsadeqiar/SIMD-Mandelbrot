#include "../include/Threadpool.h"


namespace SM
{
    Threadpool::Threadpool()
    {
        StartPool();
        return;
    }

    void Threadpool::StartPool()
    {
        // get the number of threads supported by the system.
        unsigned int numThreads = std::thread::hardware_concurrency();

        // push these threads into the vector and set them to run the IdleLoop()
        for(int i = 0; i < numThreads; i++)
        {
            threads_.emplace_back(&IdleLoop);
        }

        return;
    }

    void Threadpool::QueueTask(std::function<void()> task)
    {
        // TODO

        return;
    }

    void Threadpool::IdleLoop()
    {
        // TODO:
        // std::function<void()> task;

        // threads will run forever in this function until shutdown_ flag is set.
        while(true)
        {
            // lock the mutex and wait (gives up the lock and blocks thread) on the cond_var until notified.
            // if condition returns true from wakeup, thread is unblocked and reacquires lock.
            {
                std::unique_lock<std::mutex> lock(mutex_);

                cv_.wait(lock, [this](){ return shutdown_ || !tasks_.empty() });

                // break out of while loop and return (called from StopPool() to shutdown threadpool)
                if(shutdown_)
                    break;

                // TODO:
                // SET TASK, POP TASK
            }
            // TODO:
            // RUN TASK
        }

        return;
    }

    bool Threadpool::IsPoolBusy() const
    {
        // if tasks queue is not empty, work is being done, return true;
        return !tasks_.empty();
    }

    void Threadpool::StopPool()
    {
        // lock the mutex and set shutdown flag to true.
        {
            std::unique_lock<std::mutex> lock(mutex_);

            shutdown_ = true;
        }

        // notify all threads (will all exit IdleLoop due to shutdown_ being true).
        cv_.notify_all();

        // join all threads and close pool.
        for(int i = 0; i < threads_.size(); i++)
        {
            threads_[i].join();
        }

        return;
    }
}