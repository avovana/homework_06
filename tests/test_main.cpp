#include "matrix.h"

#define BOOST_TEST_MODULE test_main

#include <boost/test/unit_test.hpp>
#include <boost/test/execution_monitor.hpp> 
#include <boost/test/output_test_stream.hpp>

#include <string>
#include <sstream>

BOOST_AUTO_TEST_SUITE(test_suite_main)

BOOST_AUTO_TEST_CASE(index_generating)
{
    BOOST_STATIC_ASSERT(true == std::is_same< std::tuple<std::size_t>,                            Index>::value);
    BOOST_STATIC_ASSERT(true == std::is_same< std::tuple<std::size_t, std::size_t>,               Indexes<2>>::value);
    BOOST_STATIC_ASSERT(true == std::is_same< std::tuple<std::size_t, std::size_t, std::size_t>,  Indexes<3>>::value);

    using Concatenated2Indexes = decltype(addIndex<1>(Index{},      std::size_t{}));
    using Concatenated3Indexes = decltype(addIndex<2>(Indexes<2>{}, std::size_t{}));
    using Concatenated4Indexes = decltype(addIndex<3>(Indexes<3>{}, std::size_t{}));

    BOOST_STATIC_ASSERT(true == std::is_same< Concatenated2Indexes,                               Indexes<2> >::value );
    BOOST_STATIC_ASSERT(true == std::is_same< Concatenated3Indexes,                               Indexes<3> >::value );
    BOOST_STATIC_ASSERT(true == std::is_same< Concatenated4Indexes,                               Indexes<4> >::value );

    decltype(addIndex<1>(Index{}, std::size_t{})) indexes{5, 7};
    BOOST_CHECK(true == compareIndexes<2>(indexes, "5, 7"));

    auto indexes2 = addIndex<2>(indexes, std::size_t{13});
    BOOST_CHECK(true == compareIndexes<3>(indexes2, "5, 7, 13"));
}

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

    std::ostringstream oss;
    std::string result{
            "0 0 0 0 0 0 0 0 0 0\n"
            "0 1 0 0 0 0 0 0 1 0\n"
            "0 0 2 0 0 0 0 2 0 0\n"
            "0 0 0 3 0 0 3 0 0 0\n"
            "0 0 0 0 4 4 0 0 0 0\n"
            "0 0 0 0 5 5 0 0 0 0\n"
            "0 0 0 6 0 0 6 0 0 0\n"
            "0 0 7 0 0 0 0 7 0 0\n"
            "0 8 0 0 0 0 0 0 8 0\n"
            "9 0 0 0 0 0 0 0 0 9\n"
    };

    for(int i = 0; i <= 9; ++i) {
        for(int j = 0; j <= 9; ++j) {
            if(0 != j)
                oss << ' ';
            oss << matrix[i][j];
        }
        oss << '\n';
    }

    BOOST_CHECK_EQUAL(oss.str(), result);
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

BOOST_AUTO_TEST_CASE(matrix_iterator)
{
    SparseMatrix<int> matrix;

    matrix[3][3] = 55;
    BOOST_CHECK_EQUAL(matrix[3][3], 55);
    std::begin(matrix)->second = 44;
    BOOST_CHECK_EQUAL(matrix[3][3], 44);
}

BOOST_AUTO_TEST_CASE(matrix_const)
{
    const SparseMatrix<int> matrix;
    //matrix[100][100] = 314;         //compile error
}

BOOST_AUTO_TEST_CASE(matrix_const_iterator)
{
    SparseMatrix<int> matrix;
    //std::cbegin(matrix)->second = 12;         //compile error
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
