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

    template<size_t Size>
    using Indexes = typename generate_tuple_type<size_t, Size>::type;
    
    using Index = typename generate_tuple_type<size_t, 1>::type;
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
class DataAcessor {
    using DataPointerType = std::shared_ptr<Container>;

    public:

    template<typename... Args>
    DataAcessor(DataPointerType data_, Args... args) : data(data_), indexes(args...) {
        //print(CurrSize, "ProxyData ctor", args...);
    }

    friend bool operator== (const DataAcessor<CurrSize, Container>& instance, typename Container::mapped_type value)
    {
        typename Container::mapped_type currValue{};
        
        auto idx_iter = instance.data->find(instance.indexes);
        
        if(idx_iter != instance.data->end())
            currValue = idx_iter->second;
            
        return currValue == value;
    }

    friend std::ostream &operator<<(std::ostream &output, const DataAcessor<CurrSize, Container>& instance) {
    
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
    Indexes<CurrSize> indexes;
};

template<size_t CurrSize, size_t Size, typename Container, typename = void >
class Proxy {
    using DataPointer = std::shared_ptr<Container>;
    
    public:
    template<typename... Args>
    Proxy(DataPointer data_, Args... args) : data(data_), indexes(args...) {
        //print(CurrSize, "basic template", args...);
    }
    
    auto operator[](const size_t index) const {
        auto newIndexes = addIndex(indexes, index);
        auto seq = std::make_index_sequence<CurrSize + 1>();
        return createProxy(newIndexes, seq);
    }

    private:
    template<size_t... Is>
    auto createProxy(const Indexes<CurrSize + 1>& newIndexes, std::index_sequence<Is...>) const {
        return Proxy<CurrSize + 1, Size, Container>(data, std::get<Is>(newIndexes)...);
    }
    
    auto addIndex(const Indexes<CurrSize> currentIndexes, const Index newIndex) const {
        return std::tuple_cat(currentIndexes, newIndex);
    }

    DataPointer data;
    Indexes<CurrSize> indexes;
};

template<size_t CurrSize, size_t Size, typename Container>
class Proxy<CurrSize, Size, Container, typename std::enable_if<CurrSize == Size - 1>::type> {
    using DataPointer = std::shared_ptr<Container>;
    
    public:
    template<typename... Args>
    Proxy(DataPointer data_, Args... args) : data(data_), indexes(args...) {
        //print(CurrSize, "specialized template", args...);
    }
    
    auto operator[](const size_t index) const {
        auto newIndexes = addIndex(indexes, index);
        auto seq = std::make_index_sequence<CurrSize + 1>();
        
        return createDataAccessor(newIndexes, seq);
    }

    private:
    DataPointer data;
    Indexes<CurrSize> indexes;
    
    template<size_t... Is>
    auto createDataAccessor(const Indexes<CurrSize + 1>& newIndexes, std::index_sequence<Is...>) const {
        return DataAcessor<CurrSize + 1, Container>(data, std::get<Is>(newIndexes)...);
    }
    
    auto addIndex(const Indexes<CurrSize> currentIndexes, const Index newIndex) const {
        return std::tuple_cat(currentIndexes, newIndex);
    }
};
