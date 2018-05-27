#include "matrix.h"

#define BOOST_TEST_MODULE test_main

#include <boost/test/unit_test.hpp>
#include <boost/test/execution_monitor.hpp> 
#include <boost/test/output_test_stream.hpp>


BOOST_AUTO_TEST_SUITE(test_suite_main)

BOOST_AUTO_TEST_CASE(matrix_assignement_operator)
{
    SparseMatrix<int, 0> matrix;

    (matrix[2][8] = 99) = 88;

    BOOST_CHECK(matrix[2][8] == 88);
}

BOOST_AUTO_TEST_CASE(matrix_fill_content)
{
    SparseMatrix<int, 0> matrix;

    for(int i = 0, j = 0; i <= 9; ++i, ++j)
    {
        matrix[i][j] = i;
        matrix[9 - i][j] = 9 - i;
    }

    BOOST_CHECK(matrix.size() == 18);
}

BOOST_AUTO_TEST_CASE(matrix_output_content)
{
    SparseMatrix<int, 0> matrix;

    int row = 1;
    int col = 23;
    int value = 587;
    matrix[row][col] = value;

    BOOST_CHECK(matrix.size() == 1);

    for(auto element: matrix)
    {
        int r;
        int c;
        int v;
        std::tie(r, c, v) = element;

        BOOST_CHECK(r == row);
        BOOST_CHECK(c == col);
        BOOST_CHECK(v == value);
    }
}

BOOST_AUTO_TEST_CASE(matrix_erase)
{
    const int defaulValue = 0;

    SparseMatrix<int, defaulValue> matrix;

    matrix[0][5] = 99;

    BOOST_CHECK(matrix.size() == 1);

    matrix[0][5] = defaulValue;

    BOOST_CHECK(matrix.size() == 0);
}

BOOST_AUTO_TEST_SUITE_END()
