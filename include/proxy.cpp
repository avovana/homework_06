#include <iostream>
#include <string>
#include <tuple>
#include <memory>

namespace {
    template<typename T, unsigned N, typename... REST>
    struct generate_tuple_type
    {
        typedef typename generate_tuple_type<T, N-1, T, REST...>::type type;
    };

    template<typename T, typename... REST>
    struct generate_tuple_type<T, 0, REST...>
    {
    typedef std::tuple<REST...> type;
    };

//===================================================

    template <typename T>
    int print(T arg)
    {
        std::cout << arg << " ";
        return 1;
    }

    template <typename... Args>
    void print(Args... args)
    {
        bool arr[] = { print(args)...  };

        (void)arr;
    }

    template <typename... Args>
    void print(size_t CurrSize, const char* str, Args... args)
    {
        std::cout << str << ". CurrSize = " << CurrSize << '\n';
        std::cout << " indexes: ";
        print(args...);
        std::cout << '\n';
    }
}

template<size_t CurrSize, typename Container>
class ProxyData {
    using gen_tuple = typename generate_tuple_type<size_t, CurrSize>::type;
    using DataPointerType = std::shared_ptr<Container>;

    public:

    template<typename... Args>
    ProxyData(DataPointerType data_, Args... args) : data(data_), indexes(args...) {
        //print(CurrSize, "ProxyData ctor", args...);
    }

    friend bool operator== (const ProxyData<CurrSize, Container>& instance, typename Container::mapped_type value)
    {
        typename Container::mapped_type currValue{};
        
        auto idx_iter = instance.data->find(instance.indexes);
        
        if(idx_iter != instance.data->end())
            currValue = idx_iter->second;
            
        return currValue == value;
    }

    friend std::ostream &operator<<(std::ostream &output, const ProxyData<CurrSize, Container>& instance) {
    
        if(instance.data->find(instance.indexes) != instance.data->end())
        {
            //std::cout << "found" << '\n';
            output << instance.data->operator [](instance.indexes);// << '\n';
        }
        else
        {
            //std::cout << "not found" << '\n';
            typename Container::mapped_type mtype{};
            output << mtype;//  << '\n';
        }

        return output;
    }

    auto operator = (typename Container::mapped_type value) {

        typename Container::mapped_type default_value{};

        if(value == default_value) {
            auto idx_iter = data->find(indexes);
            
            if(idx_iter != data->end())
                data->erase(idx_iter);
                
        } else {
            (*data)[indexes] = value;
        }

        return *this;
    }

    private:
    DataPointerType data;
    gen_tuple indexes;
};

template<size_t CurrSize, size_t Size, typename Container, typename = void >
class Proxy {
    using gen_tuple = typename generate_tuple_type<size_t, CurrSize>::type;
    using gen_tuple_next = typename generate_tuple_type<size_t, CurrSize + 1>::type;
    using DataPointerType = std::shared_ptr<Container>;
    
    public:
    
    template<typename... Args>
    Proxy(DataPointerType data_, Args... args) : data(data_), indexes(args...) {
        
        print(CurrSize, "basic template", args...);
    }
    
    auto operator[](const size_t index) const {
        auto new_indexes = std::tuple_cat (indexes, std::tuple<size_t>(index));
        auto seq = std::make_index_sequence<CurrSize + 1>();
        return prepare_for_constr(new_indexes, seq);
    }

    private:
    
    template<size_t... Is>
    auto prepare_for_constr(const gen_tuple_next& tuple, std::index_sequence<Is...>) const {
        return Proxy<CurrSize + 1, Size, Container>(data, std::get<Is>(tuple)...);
    }

    DataPointerType data;
    gen_tuple indexes;
};

template<size_t CurrSize, size_t Size, typename Container>
class Proxy<CurrSize, Size, Container, typename std::enable_if<CurrSize == Size - 1>::type> {
    using gen_tuple = typename generate_tuple_type<size_t, CurrSize>::type;
    using gen_tuple_next = typename generate_tuple_type<size_t, CurrSize + 1>::type;
    using DataPointerType = std::shared_ptr<Container>;
    
    public:
    
    template<typename... Args>
    Proxy(DataPointerType data_, Args... args) : data(data_), indexes(args...) {
        print(CurrSize, "specialized template", args...);
        
        //for(auto el : *data)
            //std::cout << "index: " << el.first << ", " << "data: " << el.second << '\n';
    }
    
    auto operator[](const size_t index) const {
        auto new_indexes = std::tuple_cat(indexes, std::tuple<size_t>(index));
        auto seq = std::make_index_sequence<CurrSize + 1>();
        
        return prepare_for_constr(new_indexes, seq);
    }

    private:
    DataPointerType data;
    gen_tuple indexes;
    
    template<size_t... Is>
    auto prepare_for_constr(const gen_tuple_next& tuple, std::index_sequence<Is...>) const {
        return ProxyData<CurrSize + 1, Container>(data, std::get<Is>(tuple)...);
    }
};
