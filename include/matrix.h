// Sparse Matrix
// v 0.99

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
    { }

    auto operator [] (std::size_t index) const
    {
        auto ptr = std::weak_ptr<Container>{data};

        return Proxy<1, Size, DataType>(ptr, index);
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
