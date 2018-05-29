#include <memory>
#include <iostream>

template <typename DataType, std::size_t default_value>
struct Proxy {

    Proxy(std::shared_ptr<DataType> data_)
    : data(data_)
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

    typename DataType::key_type index; //const

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

    Proxy<DataType, default_value>& operator [] (std::size_t col_) {
        //std::cout << "Row[] " << '\n';
        proxy.index.row = row;
        proxy.index.col = col_;
        return proxy;
    }

    //Row(Row const&) = delete;
    //Row& operator=(Row const&) = delete;
    
    std::size_t row;

    private:
    std::shared_ptr<DataType> data;
    Proxy<DataType, default_value> proxy{data};
};