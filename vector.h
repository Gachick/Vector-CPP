#include <iostream>
#include <memory>
#include <iostream>

//------------------------------------------------------------------------------

template <typename T>
class allocator
{
public:
    T *allocate(int n) { return static_cast<T *>(::operator new(n * sizeof(T))); }
    void deallocate(T *p) { ::operator delete(p); }
    void construct(T *p, const T &v) { new (p) T{v}; }
    void destroy(T *p) { p->~T(); }
};

//------------------------------------------------------------------------------

struct Range_error : std::out_of_range
{
    int index;
    Range_error(int i) : std::out_of_range{"Range error "}, index{i} {}
};

//------------------------------------------------------------------------------

template <typename T, typename A = allocator<T>>
class vector
{
    A alloc;
    int sz;
    T *elem;
    int space;

public:
    vector() : sz{0}, elem{nullptr}, space{0} {};
    explicit vector(int s, T def = T());
    vector(std::initializer_list<T> lst);

    vector(const vector &);
    vector &operator=(const vector &);

    vector(vector &&);
    vector &operator=(vector &&);

    ~vector();

    T &operator[](int n) { return elem[n]; };
    const T &operator[](int n) const { return elem[n]; };

    T &at(int n);
    const T &at(int n) const;

    int size() const { return sz; };
    int capacity() const { return space; };

    void resize(int newsize, T def = T());
    void push_back(const T &d);
    void reserve(int newalloc);

    friend std::ostream &operator<<(std::ostream &os, const vector &vec)
    {

        os << "[";
        for (int i = 0; i < vec.size(); ++i)
            os << vec[i] << ", ";
        if (vec.size() != 0)
            os << "\b\b";
        os << "]";
        return os;
    }
};
//------------------------------------------------------------------------------

template <typename T, typename A>
vector<T, A>::vector(int s, T def) : sz{s}, elem{nullptr}, space{s}
{
    if (s < 0)
    {
        throw std::length_error("Can't create a vector of negative lenght");
    }
    elem = alloc.allocate(s);
    for (T *p = elem; p != elem + s; ++p)
        alloc.construct(p, def);
}

template <typename T, typename A>
vector<T, A>::vector(std::initializer_list<T> lst) : sz{static_cast<int>(lst.size())}, elem{nullptr}, space{sz}
{
    elem = alloc.allocate(sz);

    T *p = elem;
    for (T item : lst)
    {
        alloc.construct(p, item);
        ++p;
    }
}

template <typename T, typename A>
vector<T, A>::vector(const vector<T, A> &a)
    : sz{a.size()}, elem{alloc.allocate(a.sz)}, space{a.size()}
{
    std::copy(a.elem, a.elem + a.sz, elem);
}

template <typename T, typename A>
vector<T, A>::vector(vector<T, A> &&a)
    : sz{a.sz}, elem{a.elem}, space{a.sz}
{
    a.sz = 0;
    a.elem = nullptr;
}

template <typename T, typename A>
vector<T, A> &vector<T, A>::operator=(vector<T, A> &&a)
{
    for (int i = 0; i < sz; ++i)
        alloc.destroy(&elem[i]);
    alloc.deallocate(elem);
    elem = a.elem;
    sz = a.sz;
    a.elem = nullptr;
    a.sz = 0;
    return *this;
}

template <typename T, typename A>
vector<T, A> &vector<T, A>::operator=(const vector<T, A> &a)
{
    if (this == &a)
        return *this;

    for (int i = 0; i < sz; ++i)
        alloc.destroy(&elem[i]);

    if (a.sz <= space)
    {
        for (int i = 0; i < a.sz; ++i)
            alloc.construct(&elem[i], a.elem[i]);
        sz = a.sz;
        return *this; // check alements after size
    }

    std::unique_ptr<T[]> p(alloc.allocate(a.sz));
    for (int i = 0; i < a.sz; ++i)
        alloc.construct(&p[i], a.elem[i]);
    std::cout << "whut";
    alloc.deallocate(elem);
    space = sz = a.sz;
    elem = p.release();
    return *this;
}

template <typename T, typename A>
T &vector<T, A>::at(int n)
{
    if (n < 0 || sz <= n)
        throw Range_error(n);
    return elem[n];
}

template <typename T, typename A>
void vector<T, A>::reserve(int newalloc)
{
    if (newalloc <= space)
        return;
    std::unique_ptr<T[]> p(alloc.allocate(newalloc));
    for (int i = 0; i < sz; ++i)
        alloc.construct(&p[i], elem[i]);
    for (int i = 0; i < sz; ++i)
        alloc.destroy(&elem[i]);
    alloc.deallocate(elem);
    elem = p.release();
    space = newalloc;
}

template <typename T, typename A>
void vector<T, A>::push_back(const T &val)
{
    if (space == 0)
        reserve(8);
    else if (sz == space)
        reserve(2 * space);
    alloc.construct(&elem[sz], val);
    ++sz;
}

template <typename T, typename A>
void vector<T, A>::resize(int newsize, T val)
{
    reserve(newsize);
    for (int i = sz; i < newsize; ++i)
        alloc.construct(&elem[i], val);
    for (int i = newsize; i < sz; ++i)
        alloc.destroy(&elem[i]);
    sz = newsize;
}

template <typename T, typename A>
vector<T, A>::~vector()
{
    for (int i = 0; i < sz; ++i)
        alloc.destroy(&elem[i]);
    alloc.deallocate(elem);
}

//------------------------------------------------------------------------------