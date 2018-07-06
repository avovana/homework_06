#include <iostream>
#include <string>
#include <tuple>

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

    //==========================================================
    
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
        std::cout << " values: ";
        print(args...);
        std::cout << '\n';
    }
}

template<size_t CurrSize, size_t Size, typename = void >
class Proxy {
    using gen_tuple = typename generate_tuple_type<size_t, CurrSize>::type;
    using gen_tuple_next = typename generate_tuple_type<size_t, CurrSize + 1>::type;
    
    public:
    
    template<typename... Args>
    Proxy(Args... args) : indexes(args...) {
        
        print(CurrSize, "basic template", args...);
    }
    
    auto operator[](const size_t index) const {
        auto new_indexes = std::tuple_cat (indexes, std::tuple<size_t>(index));
        auto seq = std::make_index_sequence<CurrSize + 1>();
        return prepare_for_constr(new_indexes, seq);
    }

    private:
    
    template<size_t... Is>
    auto prepare_for_constr(const gen_tuple_next& tuple, std::index_sequence<Is...>) const {
        return Proxy<CurrSize + 1, Size>(std::get<Is>(tuple)...);
    }

    gen_tuple indexes;
};

template<size_t CurrSize, size_t Size>
class Proxy<CurrSize, Size, typename std::enable_if<CurrSize == Size>::type> {
    using gen_tuple = typename generate_tuple_type<size_t, CurrSize>::type;
    using gen_tuple_next = typename generate_tuple_type<size_t, CurrSize + 1>::type;
    
    public:
    
    template<typename... Args>
    Proxy(Args... args) : indexes(args...) {
        print(CurrSize, "specialized template", args...);
    }
    
    auto operator[](const size_t index) const {
        auto new_indexes = std::tuple_cat (indexes, std::tuple<size_t>(index));
        const auto size = std::tuple_size<decltype(new_indexes)>::value;
        
        return std::get<size - 1>(new_indexes);
    }

    private:
    gen_tuple indexes;
};

template<size_t Size>
class Matrix {
    public:
    auto operator[](const size_t index) const {
        return Proxy<1, Size>{index};
    }
};

int main()
{
    const Matrix<3> mat{};
    std::cout << " " << mat[0][1][7][6];
}
