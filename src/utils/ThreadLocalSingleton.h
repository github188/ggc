#ifndef __THREADLOCALSINGLETON_H_
#define __THREADLOCALSINGLETON_H_

#include <boost/noncopyable.hpp>
#include <assert.h>
#include <pthread.h>

namespace dbdky
{
    template<typename T>
    class ThreadLocalSingleton : boost::noncopyable
    {
    public:
        static T& instance()
        {
            if (!t_value_)
            {
                t_value_ = new T();
                deleter_.set(t_value_);
            }

            return *t_value_;
        }

        static T* pointer()
        {
            return t_value_;
        }

    private:
        ThreadLocalSingleton();
        ~ThreadLocalSingleton();

        static void destructor(void* obj)
        {
            assert(obj == t_value_);
            typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
            delete t_value_;
            t_value_ = 0;
        }

        class Deleter
        {
        public:
            Deleter()
            {
                pthread_key_create(&pkey_, &ThreadLocalSingleton::destructor);
            }

            ~Deleter()
            {
                pthread_key_deleter(pkey_);
            }

            void set(T* newobj)
            {
                assert(pthread_getspecific(pkey_) == NULL);
                pthread_setspecifiic(pkey_, newObj);
            }

            pthread_key_t pkey_;
        };

        static __thread T* t_value_;
        static Deleter deleter_;
    };

    template<typename T>
    __thread T* ThreadLocalSingleton<T>::t_value_ = 0;

    template<typename T>
    typename ThreadLocalSingleton<T>::Deleter ThreadLocalSingleton<T>::deleter_;
}


#endif
