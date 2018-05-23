#include "matrix.h"

#define BOOST_TEST_MODULE test_main

#include <boost/test/unit_test.hpp>
#include <boost/test/execution_monitor.hpp> 
#include <boost/test/output_test_stream.hpp>


BOOST_AUTO_TEST_SUITE(test_suite_main)

BOOST_AUTO_TEST_CASE(test_matrix)
{
    SparseMatrix<int, 0> matrix(9, 9);

    (matrix[2][8] = 99) = 88;

    BOOST_CHECK(matrix[2][8] == 88);
}

BOOST_AUTO_TEST_SUITE_END()
