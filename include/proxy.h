#pragma once

#include <iostream>
#include <string>
#include <tuple>
#include <memory>

#include "data_acessor.h"
#include "utils.h"

using namespace indexes;

template<size_t CurrSize, size_t Size, typename DataType, typename = void >
class Proxy {
    using Container = typename DataType::Container;
    using DataPointer = std::weak_ptr<Container>;
    
    public:
    template<typename... Args>
    Proxy(DataPointer pData, Args... args) : pData{pData}, indexes(args...)
    { }
    
    auto operator[](const size_t index) const {
        auto newIndexes = addIndex(indexes, index);
        auto seq = std::make_index_sequence<CurrSize + 1>();
        return createProxy(newIndexes, seq);
    }

    private:
    template<size_t... Is>
    auto createProxy(const Indexes<CurrSize + 1>& newIndexes, std::index_sequence<Is...>) const {
        return Proxy<CurrSize + 1, Size, DataType>(pData, std::get<Is>(newIndexes)...);
    }
    
    auto addIndex(const Indexes<CurrSize> currentIndexes, const Index newIndex) const {
        return std::tuple_cat(currentIndexes, newIndex);
    }

    DataPointer pData;
    Indexes<CurrSize> indexes;
};

template<size_t CurrSize, size_t Size, typename DataType>
class Proxy<CurrSize, Size, DataType, typename std::enable_if<CurrSize == Size - 1>::type> {
    using Container = typename DataType::Container;
    using DataPointer = std::weak_ptr<Container>;
    
    public:
    template<typename... Args>
    Proxy(DataPointer pData, Args... args) : pData{pData}, indexes(args...)
    { }
    
    auto operator[](const size_t index) const {
        auto newIndexes = addIndex(indexes, index);
        auto seq = std::make_index_sequence<CurrSize + 1>();
        
        return createDataAccessor(newIndexes, seq);
    }

    private:
    template<size_t... Is>
    auto createDataAccessor(const Indexes<CurrSize + 1>& newIndexes, std::index_sequence<Is...>) const {
        return DataAcessor<CurrSize + 1, DataType>(pData, std::get<Is>(newIndexes)...);
    }
    
    auto addIndex(const Indexes<CurrSize> currentIndexes, const Index newIndex) const {
        return std::tuple_cat(currentIndexes, newIndex);
    }

    DataPointer pData;
    Indexes<CurrSize> indexes;
};
