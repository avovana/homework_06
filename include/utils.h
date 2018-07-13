#pragma once
#include <string>

namespace indexes {
    template<typename T, unsigned N, typename... REST>
    struct generate_tuple_type {
        typedef typename generate_tuple_type<T, N-1, T, REST...>::type type;
    };

    template<typename T, typename... REST>
    struct generate_tuple_type<T, 0, REST...> {
        typedef std::tuple<REST...> type;
    };

    template<size_t Size>
    using Indexes = typename generate_tuple_type<size_t, Size>::type;

    using Index = typename generate_tuple_type<size_t, 1>::type;

    template<size_t Size>
    auto addIndex(const Indexes<Size> indexes, const size_t index) {
        return std::tuple_cat(indexes, std::tuple<size_t>(index));
    }

    template <typename T>
    T getElements(T arg) {
        return arg;
    }

    template <typename... Args>
    auto getElements(Args... args) {
        std::array<std::size_t, sizeof...(Args)> arr = { getElements(args)...  };

        std::string str;
        for(std::size_t i = 0; i < arr.size(); ++i) {
            if(i != 0)
                str += ", ";

            str += std::to_string(arr[i]);
        }

        return str;
    }

    template<size_t Size, size_t... Is>
    auto getElements_impl(const Indexes<Size>& indexes, std::index_sequence<Is...>) {
        return getElements(std::get<Is>(indexes)...);
    }

    template<size_t Size>
    auto getElementInString(const Indexes<Size> indexes) {
        auto seq = std::make_index_sequence<Size>();
        return getElements_impl<Size>(indexes, seq);
    }

    template<size_t Size>
    bool compareIndexes(const Indexes<Size> indexes, std::string elementsInString) {
        return getElementInString<Size>(indexes) == elementsInString;
    }
}