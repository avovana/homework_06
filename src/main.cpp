#include "matrix.h"

int main(int, char**)
{
    try
    {
        SparseMatrix<int, 0> matrix(9, 9);

        for(int i = 0, j = 0; i <= 9; ++i, ++j)
        {
            matrix[i][j] = i;
            matrix[9 - i][j] = 9 - i;
        }

        std::cout << "Matrix fragment: " << '\n';
        for(int i = 1; i <= 8; ++i)
        {
            for(int j = 1; j <= 8; ++j)
            {
                std::cout << matrix[i][j] << " ";
            }
            std::cout << '\n';
        }

        std::cout << '\n' << "Matrix size = " << matrix.size() << '\n' << '\n';

        std::cout << "Matrix elements:" << '\n';
        for(auto element: matrix)
        {
            int x;
            int y;
            int v;
            std::tie(x, y, v) = element;
            std::cout << x << " " << y << " " << v << std::endl;
        }

        std::cout << '\n' << "Assignment operator demonstration: " << '\n';

        (matrix[2][8] = 99) = 88;
        assert(matrix[2][8] == 88);
        std::cout << "(matrix[2][8] = 99) = 88;" << '\n';
        std::cout << "matrix[2][8]: " << matrix[2][8] << '\n';
        std::cout << "Matrix size = " << matrix.size() << '\n';
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
