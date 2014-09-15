#ifndef DV2520_SINGLETON_H
#define DV2520_SINGLETON_H

template<class T>
class Singleton {
public:
    static T& get() {
        static T singleInstance;
        return singleInstance;
    }
protected:
private:
    Singleton() {}
    ~Singleton() {}
};

#endif // DV2520_SINGLETON_H