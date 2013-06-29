#ifndef __BLOCKINGQUEUE_H_
#define __BLOCKINGQUEUE_H_

#include <utils/Condition.h>
#include <utils/Mutex.h>

#include <boost/noncopyable.hpp>
#include <deque>
#include <assert.h>

namespace dbdky
{
    template<typename T>
    class BlockingQueue : boost::noncopyable
    {
    public:
        BlockingQueue()
            : mutex_(),
              notEmpty_(mutex_),
              queue_()
        {
        }

        void put(const T& x)
        {
            MutexLockGuard lock(mutex_);
            queue_.push_back(x);
            notEmpty_.notify();
        }

        T take()
        {
            MutexLockGuard lock(mutex_);
            while (queue_.empty())
            {
                notEmpty_.wait();
            }

            assert(!queue_.empty());
            T front(queue_.front());
            queue_.pop_front();
     
            return front;
        }

        size_t size() const
        {
            MutexLockGuard lock(mutex_);
     
            return queue_.size();
        }

    private:
        mutable MutexLock mutex_;
        Condition notEmpty_;
        std::deque<T> queue_;
    };
}


#endif
