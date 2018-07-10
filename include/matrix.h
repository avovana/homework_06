// Sparse Matrix
// v 0.98
#include <iostream>
#include <memory>
#include <map>
#include <utility>

#include "proxy.h"

template <typename ElementType, std::size_t Size = 2>
class SparseMatrix {

private:
    using Container = std::map<Indexes<Size>, ElementType>;
    
    using iterator = typename Container::iterator;
    using DataPointerType = std::shared_ptr<Container>;

    DataPointerType data;

public:
    SparseMatrix() : data(new Container)
    {
        std::cout << "Matrix ctor " << '\n';
    }

    auto operator [] (std::size_t index) const
    {
        return Proxy<1, Size, Container>(data, index);
    }

    auto size() const
    {
        return data->size();
    }

    iterator begin() const
    {
        return data->begin();
    }

    iterator end() const
    {
        return data->end();
    }
};
