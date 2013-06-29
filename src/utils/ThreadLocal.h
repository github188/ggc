#ifndef __THREADLOCAL_H_
#define __THREADLOCAL_H_

#include <boost/noncopyable.hpp>
#include <pthread.h>

namespace dbdky
{
    template<typename T>
    class ThreadLocal : boost::noncopyable
    {
    public:
        ThreadLocal()
        {
            pthread_key_create(&pkey_, &ThreadPool::destructor);
        }

        ~ThreadLocal()
        {
            pthread_key_delete(pkey_);
        }
 
        T& value()
        {
            T* perThreadValue = static_cast<T*>(pthread_getspecific(pkey_));
            if (!perThreadValue)
            {
                T* newObj = new T();
                pthread_setspecific(pkey_, newObj);
                perThreadValue = newObj;
            }

            return *perThreadValue;
        }

    private:
        static void destructor(void* x)
        {
            T* obj = static_cast<T*>(x);
            typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
            delete obj;
        }

    private:
        pthread_key_t pkey_;
    };
}

#endif
