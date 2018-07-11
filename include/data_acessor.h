#pragma once

#include "utils.h"

using namespace indexes;

template<size_t CurrSize, typename DataType>
class DataAcessor {
    using Container = typename DataType::Container;
    using ElementType = typename DataType::ElementType;
    using DataPointer = std::weak_ptr<Container>;
    using Acessor = std::shared_ptr<Container>;

    public:

    template<typename... Args>
    DataAcessor(DataPointer pData, Args... args) : pData{pData}, indexes(args...)
    { }

    friend bool operator== (const DataAcessor<CurrSize, DataType>& instance, ElementType value) {

        if (!instance.dataAvailable())
            throw std::runtime_error("No data available");

        ElementType currValue{};
        
        auto acessor = instance.getDataAcessor();
        auto indexIterator = acessor->find(instance.indexes);
        
        if(indexIterator != acessor->end())
            currValue = indexIterator->second;
            
        return currValue == value;
    }

    friend std::ostream &operator<<(std::ostream &output, const DataAcessor<CurrSize, DataType>& instance) {
    
        if (!instance.dataAvailable())
            throw std::runtime_error("No data available");

        auto acessor = instance.getDataAcessor();
        auto indexIterator = acessor->find(instance.indexes);

        if(indexIterator != acessor->end())
            output << acessor->operator [](instance.indexes);
        else
            output << ElementType{};

        return output;
    }

    auto operator = (typename Container::mapped_type value) {

        if (!dataAvailable())
            throw std::runtime_error("No data available");

        auto acessor = getDataAcessor();
        ElementType default_value{};

        if(value == default_value) {
            auto indexIterator = acessor->find(indexes);
            
            if(indexIterator != acessor->end())
                acessor->erase(indexIterator);
                
        } else {
            (*acessor)[indexes] = value;
        }

        return *this;
    }

    private:
    bool dataAvailable() const {
        return !pData.expired();
    }

    Acessor getDataAcessor() const {
        return pData.lock();
    }

    DataPointer pData;
    Indexes<CurrSize> indexes;
};