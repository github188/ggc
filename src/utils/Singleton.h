#ifndef __SINGLETON_H_
#define __SINGLETON_H_

#include <boost/noncopyable.h>
#include <pthread.h>
#include <stdlib.h>

namespace dbdky
{
    template<typename T>
    class Singleton : boost::noncopyable
    {
    public:
        static T& instance()
        {
            pthread_once(&ponce_, &singleton::init);
            return *value_;
        }

    private:
        Singleton();
        ~Singleton();

        static void init()
        {
            value_ = new T();
            ::atexit(destroy);
        }

        static void destroy()
        {
            typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
            delete value_;
        }

    private:
        static pthread_once_t ponce_;
        static T* value_;
    };

    template<typename T>
    pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;
    
    template<typename T>
    T* Singleton<T>::value_ = NULL;
}


#endif
