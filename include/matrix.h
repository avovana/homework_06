// Sparse Matrix
// v 0.8

#include <iostream>
#include <cassert>

#include <map>
#include <tuple>

template <typename T, int default_value = -1> 
struct SparseMatrix
{
    class iterator;

    SparseMatrix(std::size_t nrows, std::size_t ncols) 
        : nrows(nrows)
        , ncols(ncols) 
        { }

    struct Index
    {
        std::size_t row{};
        std::size_t col{};

        bool operator < (const Index& rhs) const
        {
            if(row < rhs.row) 
            {
                return true;
            }

            if(row == rhs.row && col < rhs.col) 
            {
                return true;
            }

            return false;
        }
    };   

    struct Proxy
    {
        Proxy(SparseMatrix& mtx_) : mtx(mtx_)
        { }

        operator int() const 
        { 
            return defV; 
        }

        Proxy& operator = (int value) 
        { 
            defV = value;

            if(value == default_value)
            {
                const auto idx_iter = mtx.data.find(currIdx);

                if(idx_iter != mtx.data.end())
                {
                    mtx.data.erase(idx_iter);
                }
            }
            else
            {
                mtx.data[currIdx] = value;
            }

            return *this;
        }
        
        void setRow(int row)
        {
            currIdx.row = row;
        }
        
        void setCol(int col)
        {
            currIdx.col = col;
        }
        
        void updateValue()
        {
            const auto idx_iter = mtx.data.find(currIdx) ;

            defV = idx_iter != mtx.data.end() ? idx_iter->second : default_value;
        }

        private:
        T defV{default_value};
        SparseMatrix& mtx;
        Index currIdx{};
    };
    
    struct Row
    {
        Row(Proxy& currEl_) 
            : currEl(currEl_)
        { }
        
        Proxy& operator [] (int col) 
        {
            currEl.setCol(col);
            currEl.updateValue();
            
            return currEl;
        }
        
        private:
        Proxy& currEl;
    };
    
    Row& operator [] (int row_) 
    {
        currEl.setRow(row_);
        return row;
    }
    
    auto size()
    {
        return data.size();
    }
    
    iterator begin() 
    {
        return iterator(data.begin()); 
    }

    iterator end() 
    {
        return iterator(data.end());
    }
    
    private:
    
    std::size_t nrows;
    std::size_t ncols;
    using index_type = Index;
    using value_type = T;
    using date_type = std::map<index_type, value_type>;
    
    date_type data;
    Proxy currEl{*this};
    Row row{currEl};
    
    public:
    class iterator
    {
        public:
        iterator(decltype(data.begin()) iter_) 
            : iter(iter_)
            { }

        iterator& operator ++() 
        {
            ++iter;
            return *this;
        }

        std::tuple<std::size_t, std::size_t, T> operator * () 
        {
            auto element = std::make_tuple (iter->first.row, iter->first.col, iter->second);
            return element;
            //return iter->second;
        }

        bool operator != (const iterator& rhg) const
        {
          return iter != rhg.iter;
        }   

        private:
        decltype(data.begin()) iter;
    }; 
};



