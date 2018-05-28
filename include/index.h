#include <memory>
#include <iostream>

template <typename DataType, std::size_t default_value = -1>
struct Proxy {

    Proxy(std::shared_ptr<DataType> data_, std::size_t row, std::size_t col) 
    : data(data_)
    , index{row, col}
    { 
        //std::cout << "Proxy ctor" << '\n';
    }

   friend std::ostream &operator<<(std::ostream &output, const Proxy<DataType, default_value> &instance) { 

        const auto idx_iter = instance.data->find(instance.index);

            if(idx_iter != instance.data->end())
            {
                output << instance.data->at(instance.index);
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
        const auto idx_iter = data->find(index);
        
        if(idx_iter != data->end())
            value = data->at(index);
        else
            value = default_value;
        
        return value; 
    }


    auto operator = (std::size_t value_)  { //(value_type value)
        value = value_;

        //std::cout << "Proxy = " << '\n';
        if(value == default_value) {
            const auto idx_iter = data->find(index);

            if(idx_iter != data->end())
                data->erase(idx_iter);
        } else {
            //std::cout << " In insert" << '\n';
            (*data)[index] = value; //data->insert( std::make_pair(index, value) );
        }

        return *this;
    }

    private:
    std::size_t value{default_value};
    
    std::shared_ptr<DataType> data;
    typename DataType::key_type index; //const
};

template <typename DataType, std::size_t default_value = -1> 
struct Row {

    Row(std::shared_ptr<DataType> data_, std::size_t row_) 
    : data(data_)
    , row(row_)
    { 
        //std::cout << "Row ctor " << '\n';
    }

    auto operator [] (std::size_t col_) {
        //std::cout << "Row[] " << '\n';
        return Proxy<DataType, default_value>(data, row, col_);
    }

    //Row(Row const&) = delete;
    //Row& operator=(Row const&) = delete;
    
    private:
    std::shared_ptr<DataType> data;
    std::size_t row;
};