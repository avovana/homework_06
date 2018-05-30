// Sparse Matrix
// v 0.96

#include <iostream>
#include <memory>

#include "data.h"
#include "index.h"

template <typename T, std::size_t default_value = -1>
class SparseMatrix {

private:
    using date_type = typename Data<T>::date_type;
    using iterator = typename date_type::iterator;

    std::shared_ptr<date_type> data;
    Row<date_type, default_value> row;

public:
    SparseMatrix() : data(new date_type), row{data}
    {
        //std::cout << "Matrix ctor " << '\n';
    }
    
     Row<date_type, default_value>& operator [] (std::size_t row_)
     {
        //std::cout << "Matrix[] " << '\n';
        row.row = row_;
        return row;
     }

    auto size()
    {
        return data->size();
    }

    iterator begin()
    {
        return data->begin();
    }

    iterator end()
    {
        return data->end();
    }
};
