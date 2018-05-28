#include <map>

template <typename T> 
class Data
{
    struct Index {
        std::size_t row{};
        std::size_t col{};

        bool operator < (const Index& rhs) const {
            if(row < rhs.row) 
                return true;

            if(row == rhs.row && col < rhs.col) 
                return true;

            return false;
        }
    };   
    
    public:
    using index_type = Index;
    using value_type = T;
    using date_type = std::map<Index, value_type>;
};