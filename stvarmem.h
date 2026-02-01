#ifndef STVARMEM_H
#define STVARMEM_H

template <typename T> struct varMem
{
    T Previous;
    T Current;

    void install(T value)
    {
        Previous = value;
        Current = value;
    }

    void store()
    {
        Previous = Current;
    }

    void push(T value)
    {
        store();
        Current = value;
    }

    bool check()
    {
        return (Previous == Current);
    }
};

#endif // STVARMEM_H
