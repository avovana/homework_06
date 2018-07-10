#pragma once

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

template<size_t CurrSize, typename DataType>
class DataAcessor {
    using Container = typename DataType::Container;
    using ElementType = typename DataType::ElementType;
    using DataPointer = std::weak_ptr<Container>;

    public:

    template<typename... Args>
    DataAcessor(DataPointer data_, Args... args) : data(data_), indexes(args...) {
        //print(CurrSize, "ProxyData ctor", args...);
    }

    friend bool operator== (const DataAcessor<CurrSize, DataType>& instance, ElementType value)
    {
        if (instance.data.expired())
            throw std::bad_weak_ptr();

        ElementType currValue{};
        
        std::shared_ptr<Container> pData = instance.data.lock();
        auto idx_iter = pData->find(instance.indexes);
        
        if(idx_iter != pData->end())
            currValue = idx_iter->second;
            
        return currValue == value;
    }

    friend std::ostream &operator<<(std::ostream &output, const DataAcessor<CurrSize, DataType>& instance) {
    
        if (instance.data.expired())
            throw std::bad_weak_ptr();

        std::shared_ptr<Container> pData = instance.data.lock();

        if(pData->find(instance.indexes) != pData->end())
        {
            //std::cout << "found" << '\n';
            output << pData->operator [](instance.indexes);// << '\n';
        }
        else
        {
            //std::cout << "not found" << '\n';
            ElementType mtype{};
            output << mtype;//  << '\n';
        }

        return output;
    }

    auto operator = (typename Container::mapped_type value) {

        if (data.expired())
            throw std::bad_weak_ptr();

        std::shared_ptr<Container> pData = data.lock();
        ElementType default_value{};

        if(value == default_value) {
            auto idx_iter = pData->find(indexes);
            
            if(idx_iter != pData->end())
                pData->erase(idx_iter);
                
        } else {
            (*pData)[indexes] = value;
        }

        return *this;
    }

    private:

    DataPointer data;
    Indexes<CurrSize> indexes;
};

template<size_t CurrSize, size_t Size, typename DataType, typename = void >
class Proxy {
    using Container = typename DataType::Container;
    using DataPointer = std::weak_ptr<Container>;
    
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
        return Proxy<CurrSize + 1, Size, DataType>(data, std::get<Is>(newIndexes)...);
    }
    
    auto addIndex(const Indexes<CurrSize> currentIndexes, const Index newIndex) const {
        return std::tuple_cat(currentIndexes, newIndex);
    }

    DataPointer data;
    Indexes<CurrSize> indexes;
};

template<size_t CurrSize, size_t Size, typename DataType>
class Proxy<CurrSize, Size, DataType, typename std::enable_if<CurrSize == Size - 1>::type> {
    using Container = typename DataType::Container;
    using DataPointer = std::weak_ptr<Container>;
    
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
        return DataAcessor<CurrSize + 1, DataType>(data, std::get<Is>(newIndexes)...);
    }
    
    auto addIndex(const Indexes<CurrSize> currentIndexes, const Index newIndex) const {
        return std::tuple_cat(currentIndexes, newIndex);
    }
};
