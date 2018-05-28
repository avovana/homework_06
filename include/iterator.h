#include <tuple>

template<typename Data>
class iterator 
{
    private:
    Data iter;
    
    public:
    iterator(Data iter_) 
        : iter(iter_)
        { }

    iterator& operator ++() {
        ++iter;
        return *this;
    }

    auto operator * ()
    {
        auto element = std::make_tuple (iter->first.row, iter->first.col, iter->second);
        return element;
    }

    bool operator != (const iterator& rhg) const {
        return iter != rhg.iter;
    }   

    
}; 
