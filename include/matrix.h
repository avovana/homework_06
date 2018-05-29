// Sparse Matrix
// v 0.9

#include <iostream>
#include <memory>

#include "data.h"
#include "index.h"
#include "iterator.h"

template <typename T, std::size_t default_value = -1>
class SparseMatrix {

private:
    using date_type = typename Data<T>::date_type;
    std::shared_ptr<date_type> data;

    public:
    SparseMatrix() : data(new date_type)
    {
        //std::cout << "Matrix ctor " << '\n';
    }
    
     auto operator [] (std::size_t row)
     {
        //std::cout << "Matrix[] " << '\n';
        return Row<date_type, default_value>(data, row);
     }

    auto size()
    {
        return data->size();
    }

    iterator<decltype(data->begin())> begin()
    {
        return iterator<decltype(data->begin())>(data->begin());
    }

    iterator<decltype(data->end())> end()
    {
        return iterator<decltype(data->begin())>(data->end());
    }
};
