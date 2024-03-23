#include "../include/Threadpool.h"

namespace SM
{
    Threadpool::Threadpool()
    {
        return;
    }

    bool Threadpool::StartPool()
    {
        // get the number of threads supported by the system.
        unsigned int numThreads = std::thread::hardware_concurrency();

        // push these threads into the vector and set them to run the IdleLoop()
        for(int i = 0; i < numThreads; i++)
        {
            threads_.emplace_back(&Threadpool::IdleLoop, this);
        }

        return true;
    }

    void Threadpool::QueueTask(std::function<void()> task)
    {
        // lock the mutex and push a task
        {
            std::unique_lock<std::mutex> lock(mutex_);

            tasks_.push(task);
            tasks_pending_++;
        }

        // notify a thread 
        cv_.notify_one();

        return;
    }

    void Threadpool::IdleLoop()
    {
        
        std::function<void()> task;

        // threads will run forever in this function until shutdown_ flag is set.
        while(true)
        {
            // lock the mutex and wait (gives up the lock and blocks thread) on the cond_var until notified.
            // if condition returns true from wakeup, thread is unblocked and reacquires lock.
            {
                std::unique_lock<std::mutex> lock(mutex_);

                cv_.wait(lock, [this](){ return shutdown_ || !tasks_.empty(); });

                // break out of while loop and return (called from StopPool() to shutdown threadpool)
                if(shutdown_)
                    break;

                task = tasks_.front();
                tasks_.pop();
            }
            task();
            tasks_pending_--;
        }

        return;
    }

    bool Threadpool::IsPoolBusy() const
    {
        // if there are any active jobs RUNNING, return true.
        return tasks_pending_;
    }

    bool Threadpool::StopPool()
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
        
        threads_.clear();

        return true;
    }

    int Threadpool::GetThreadCount() const
    {
        return threads_.size();
    }
}