#pragma once

#include "utils.h"

using namespace indexes;

template <typename IndexType, typename DataType>
class DataAcessorImpl {
    using Container = typename DataType::Container;
    using ElementType = typename DataType::ElementType;
    using Iterator = typename Container::iterator;
    using DataPointer = std::weak_ptr<Container>;
    using AcessorPointer = std::shared_ptr<Container>;

    public:
    DataAcessorImpl(DataPointer pData) : pData{pData}
    { }

    ElementType getValue (IndexType indexes) const {

        if(pData.expired())
            throw std::runtime_error("No data available");

        AcessorPointer acessor = pData.lock();
        auto iterator = acessor->find(indexes);

        ElementType value{};

        if(iterator != acessor->end())
            value = iterator->second;

        return value;
    }

    void setValue(IndexType indexes, ElementType value) {

        if(pData.expired())
            throw std::runtime_error("No data available");

        AcessorPointer acessor = pData.lock();
        auto iterator = acessor->find(indexes);

        if(value == ElementType{})
        {
            if(iterator != acessor->end())
                acessor->erase(iterator);
        }
        else
        {
            (*acessor)[indexes] = value;
        }
    }

    private:
    DataPointer pData;
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
        return instance.acessor.getValue(instance.indexes) == value;
    }

    friend std::ostream &operator<<(std::ostream &output, const ThisClassType& instance) {
        output << instance.acessor.getValue(instance.indexes);

        return output;
    }

    auto operator = (ElementType value) {
        acessor.setValue(indexes, value);

        return *this;
    }

    private:
    DataAcessorImpl<IndexType, DataType> acessor;
    IndexType indexes;
};