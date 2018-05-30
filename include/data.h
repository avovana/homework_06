#include <tuple>
#include <set>

template <typename T> 
class Data
{
private:

    using cell_type = std::tuple<std::size_t, std::size_t, T>;

    struct less_compare
    {
        bool operator() (const cell_type& lhs, const cell_type& rhs) const
        {
            return lhs < rhs;
        }
    };

public:

    using date_type = std::set<cell_type, less_compare>;
};