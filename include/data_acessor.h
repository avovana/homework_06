#pragma once

#include "utils.h"

using namespace indexes;

template<size_t CurrSize, typename DataType>
class DataAcessor {
    using Container = typename DataType::Container;
    using ElementType = typename DataType::ElementType;
    using DataPointer = std::weak_ptr<Container>;

    public:

    template<typename... Args>
    DataAcessor(DataPointer pData, Args... args) : pData{pData}, indexes(args...)
    { }

    friend bool operator== (const DataAcessor<CurrSize, DataType>& instance, ElementType value) {

        if (instance.pData.expired())
            throw std::bad_weak_ptr();

        ElementType currValue{};
        
        std::shared_ptr<Container> dataAcessor = instance.pData.lock();
        auto indexIterator = dataAcessor->find(instance.indexes);
        
        if(indexIterator != dataAcessor->end())
            currValue = indexIterator->second;
            
        return currValue == value;
    }

    friend std::ostream &operator<<(std::ostream &output, const DataAcessor<CurrSize, DataType>& instance) {
    
        if (instance.pData.expired())
            throw std::bad_weak_ptr();

        std::shared_ptr<Container> dataAcessor = instance.pData.lock();
        auto indexIterator = dataAcessor->find(instance.indexes);

        if(indexIterator != dataAcessor->end())
            output << dataAcessor->operator [](instance.indexes);
        else
            output << ElementType{};

        return output;
    }

    auto operator = (typename Container::mapped_type value) {

        if (pData.expired())
            throw std::bad_weak_ptr();

        std::shared_ptr<Container> dataAcessor = pData.lock();
        ElementType default_value{};

        if(value == default_value) {
            auto indexIterator = dataAcessor->find(indexes);
            
            if(indexIterator != dataAcessor->end())
                dataAcessor->erase(indexIterator);
                
        } else {
            (*dataAcessor)[indexes] = value;
        }

        return *this;
    }

    private:
    DataPointer pData;
    Indexes<CurrSize> indexes;
};