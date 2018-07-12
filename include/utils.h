#pragma once

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
    auto addIndex(const Indexes<Size> indexes, const Index index) {
        return std::tuple_cat(indexes, index);
    }
}

namespace print {
    template <typename T>
    int print(T arg) {
        std::cout << arg << " ";
        return 1;
    }

    template <typename... Args>
    void print(Args... args) {
        bool arr[] = { print(args)...  };
        (void)arr;
    }

    template <typename... Args>
    void print(size_t CurrSize, const char* str, Args... args) {
        std::cout << str << ". CurrSize = " << CurrSize << '\n';
        std::cout << " indexes: ";
        print(args...);
        std::cout << '\n';
    }
}