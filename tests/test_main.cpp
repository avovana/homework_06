#include "matrix.h"

#define BOOST_TEST_MODULE test_main

#include <boost/test/unit_test.hpp>
#include <boost/test/execution_monitor.hpp> 
#include <boost/test/output_test_stream.hpp>

#include <string>

BOOST_AUTO_TEST_SUITE(test_suite_main)

BOOST_AUTO_TEST_CASE(matrix_assignement_operator)
{
    SparseMatrix<int> matrix;

    (matrix[2][8] = 99) = 88;

    BOOST_CHECK(matrix[2][8] == 88);
}

BOOST_AUTO_TEST_CASE(matrix_fill_content)
{
    SparseMatrix<int> matrix;

    for(int i = 0, j = 0; i <= 9; ++i, ++j)
    {
        matrix[i][j] = i;
        matrix[9 - i][j] = 9 - i;
    }

    BOOST_CHECK(matrix.size() == 18);
}

BOOST_AUTO_TEST_CASE(matrix_erase)
{
    const int defaulValue = 0;

    SparseMatrix<int> matrix;

    matrix[0][5] = 99;

    BOOST_CHECK(matrix.size() == 1);

    matrix[0][5] = defaulValue;

    BOOST_CHECK(matrix.size() == 0);
}

BOOST_AUTO_TEST_CASE(matrix_char_type)
{
    SparseMatrix<char> matrix;

    BOOST_CHECK(matrix.size() == 0);
    char ch = 'c';

    matrix[1][2] = ch;

    BOOST_CHECK(matrix[1][2] == ch);
    BOOST_CHECK(matrix.size() == 1);

    const char defaulValue{};
    matrix[1][2] = defaulValue;

    BOOST_CHECK(matrix.size() == 0);
    BOOST_CHECK(matrix[2][8] == defaulValue);
}

BOOST_AUTO_TEST_CASE(matrix_string_type)
{
    SparseMatrix<std::string> matrix;

    BOOST_CHECK(matrix.size() == 0);

    matrix[1][2] = "3";

    BOOST_CHECK(matrix.size() == 1);
    BOOST_CHECK(matrix[1][2] == "3");

    const std::string defaulValue{};
    matrix[1][2] = defaulValue;

    BOOST_CHECK(matrix.size() == 0);
    BOOST_CHECK(matrix[1][2] == defaulValue);
}

BOOST_AUTO_TEST_CASE(matrix_multidimension)
{
    SparseMatrix<int, 4> matrix;

    BOOST_CHECK(matrix.size() == 0);

    matrix[1][2][3][4] = 3;

    BOOST_CHECK(matrix.size() == 1);
    BOOST_CHECK(matrix[1][2][3][4] == 3);

    const int defaulValue = 0;
    matrix[1][2][3][4] = defaulValue;

    BOOST_CHECK(matrix.size() == 0);
    BOOST_CHECK(matrix[1][2][3][4] == defaulValue);
}

BOOST_AUTO_TEST_CASE(matrix_implementation)
{
    SparseMatrix<int> matrix;

    auto savedProxy = matrix[1][2];

    savedProxy = 2;
    BOOST_CHECK(savedProxy == 2);
    BOOST_CHECK(matrix[1][2] == 2);
    BOOST_CHECK(matrix.size() == 1);

    const int defaultValue{};
    savedProxy = defaultValue;
    BOOST_CHECK(savedProxy == defaultValue);
    BOOST_CHECK(matrix[1][2] == defaultValue);
    BOOST_CHECK(matrix.size() == 0);
}

BOOST_AUTO_TEST_CASE(matrix_deleted_data)
{
    SparseMatrix<int> matrix;

    auto savedProxy = matrix[1][2];

    matrix.~SparseMatrix();

    BOOST_CHECK_THROW(savedProxy = 2, std::runtime_error);
}
BOOST_AUTO_TEST_SUITE_END()
