// Sparse Matrix
// v 0.98
#include <iostream>
#include <memory>
#include <map>
#include <utility>

#include "proxy.h"
#include "data.h"

template <typename ElementType, std::size_t Size = 2>
class SparseMatrix {

private:
    using DataType = Data<ElementType, Size>;
    using Container = typename DataType::Container;
    
    using iterator = typename Container::iterator;
    using DataPointer = std::shared_ptr<Container>;

    DataPointer data;

public:
    SparseMatrix() : data(new Container)
    {
        std::cout << "Matrix ctor " << '\n';
    }

    auto operator [] (std::size_t index) const
    {
        return Proxy<1, Size, DataType>(data, index);
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
