#include <memory>
#include <iostream>
#include <algorithm>

template <typename DataType, std::size_t default_value>
struct Proxy {

    using cell_type = typename DataType::value_type;

    Proxy(std::shared_ptr<DataType> data_)
    : data(data_)
    { 
        //std::cout << "Proxy ctor" << '\n';
    }

   friend std::ostream &operator<<(std::ostream &output, const Proxy<DataType, default_value> &instance) { 

        const auto idx_iter = std::find_if(instance.data->begin(), instance.data->end(), [&instance]
                           (const cell_type& el)
                           {
                               bool equal = (std::get<0>(el) == instance.row) && (std::get<1>(el) == instance.col);
                               return equal;
                           }
                          );


            if(idx_iter != instance.data->end())
            {
                auto el = *idx_iter;
                output << std::get<2>(el);
                return output; 
            }
            else
            {
                output << default_value;
                return output;
            }
      }

    operator int()  
    { 
        const auto idx_iter = std::find_if(data->begin(), data->end(), [&]
                           (const cell_type& el)
                           {
                               bool equal = (std::get<0>(el) == row) && (std::get<1>(el) == col);
                               return equal;
                           }
                          );

        if(idx_iter != data->end())
            value = std::get<2>(*idx_iter);
        else
            value = default_value;
        
        return value; 
    }


    auto operator = (std::size_t value_)  { //(value_type value)
        value = value_;
    const auto idx_iter = std::find_if(data->begin(), data->end(), [&]
                           (const cell_type& el)
                           {
                               bool equal = (std::get<0>(el) == row) && (std::get<1>(el) == col);
                               return equal;
                           }
                          );

        //std::cout << "Proxy = " << '\n';
        if(value == default_value) {
            if(idx_iter != data->end())
                data->erase(idx_iter);
        } else {
            //std::cout << " In insert" << '\n';
            if(idx_iter != data->end())
            {
                data->erase(idx_iter);
                data->insert(cell_type(row, col, value));
            } else {
                data->insert(cell_type(row, col, value));
            }
        }

        return *this;
    }

    std::size_t row;
    std::size_t col;

    private:
    std::size_t value{default_value};
    
    std::shared_ptr<DataType> data;
};

template <typename DataType, std::size_t default_value>
struct Row {

    Row(std::shared_ptr<DataType> data_)
    : data(data_)
    { 
        //std::cout << "Row ctor " << '\n';
    }

    auto operator [] (std::size_t col_) {
        //std::cout << "Row[] " << '\n';
        proxy.row = row;
        proxy.col = col_;
        return proxy;
    }

    //Row(Row const&) = delete;
    //Row& operator=(Row const&) = delete;

    std::size_t row;

    private:
    std::shared_ptr<DataType> data;
    Proxy<DataType, default_value> proxy{data};
};