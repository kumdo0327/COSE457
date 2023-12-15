#ifndef __SINGLETON_H__
#define __SINGLETON_H__


template<class T>
class Singleton
{
public:
    static T& getInstance() { static T* singleton = new T(); return *singleton; }
protected:
    Singleton() = default;
private:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;
};
#endif
