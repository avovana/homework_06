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
    DataAcessor(DataPointer data_, Args... args) : data(data_), indexes(args...) 
    { }

    friend bool operator== (const DataAcessor<CurrSize, DataType>& instance, ElementType value) {
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
            output << pData->operator [](instance.indexes);
        else
            output << ElementType{};

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