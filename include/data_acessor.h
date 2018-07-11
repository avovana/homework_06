#pragma once

#include "utils.h"

using namespace indexes;

template <typename IndexType, typename DataType>
class Acessor {
    using Container = typename DataType::Container;
    using ElementType = typename DataType::ElementType;
    using Iterator = typename Container::iterator;
    using DataPointer = std::weak_ptr<Container>;
    using AcessorPointer = std::shared_ptr<Container>;

    public:
    Acessor(DataPointer pData) : pData{pData}
    { }

    bool dataAvailable() const {
        return !pData.expired();
    }

    bool elementExists(IndexType indexes) const {
        AcessorPointer acessor = pData.lock();
        iterator = acessor->find(indexes);

        return iterator != acessor->end();
    }

    void deleteElement() {
        AcessorPointer acessor = pData.lock();
        acessor->erase(iterator);
    }

    void setValue(IndexType indexes, ElementType value)
    {
        AcessorPointer acessor = pData.lock();
        (*acessor)[indexes] = value;
    }

    ElementType getValue () const {
        return iterator->second;
    }

    private:
    DataPointer pData;
    mutable Iterator iterator;
};


template<size_t Size, typename DataType>
class DataAcessor {
    using ThisClassType = DataAcessor<Size, DataType>;

    using Container = typename DataType::Container;
    using ElementType = typename DataType::ElementType;
    using IndexType = Indexes<Size>;

    using DataPointer = std::weak_ptr<Container>;

    public:

    template<typename... Args>
    DataAcessor(DataPointer pData, Args... args) : acessor{pData}, indexes(args...)
    { }

    friend bool operator== (const ThisClassType& instance, ElementType value) {

        if (!instance.acessor.dataAvailable())
            throw std::runtime_error("No data available");

        ElementType currValue{};
        
        if(instance.acessor.elementExists(instance.indexes))
            currValue = instance.acessor.getValue();
            
        return currValue == value;
    }

    friend std::ostream &operator<<(std::ostream &output, const ThisClassType& instance) {
    
        if (!instance.acessor.dataAvailable())
            throw std::runtime_error("No data available");

        if(instance.acessor.elementExists(instance.indexes))
            output << instance.acessor.getValue();
        else
            output << ElementType{};

        return output;
    }

    auto operator = (ElementType value) {

        if (!acessor.dataAvailable())
            throw std::runtime_error("No data available");

        if(value == ElementType{}) {
            
            if(acessor.elementExists(indexes))
                acessor.deleteElement();
                
        } else {
            acessor.setValue(indexes, value);
        }

        return *this;
    }

    private:
    Acessor<IndexType, DataType> acessor;
    IndexType indexes;
};