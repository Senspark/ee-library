//
//  BoostExample.cpp
//  ee-library-test
//
//  Created by enrevol on 3/1/16.
//
//

#include "BoostExample.hpp"
#include "boost/algorithm/algorithm.hpp"
#include "boost/assert.hpp"

#include "EEHeader.hpp"

static const ee::EventInfo<int, int*, int&, const int&, std::string, std::string&, const std::string&> ExampleEvent("unikey_key");
static const ee::DataInfo<int, int, int, int> ExampleData("unique_key");

static void algorithm_example() {
    BOOST_ASSERT(boost::algorithm::power(2, 5) == 32);
    BOOST_ASSERT(boost::algorithm::power(2, 5, std::plus<int>()) == 10);
    
    ExampleData[1][1][2] = 2;
    ExampleData[2][5][2] = 1;
    
    int x = 0;
    std::string y = "1";
    ExampleEvent.dispatch(x, &x, x, x, y, y, y);
}

void boost_example() {
    algorithm_example();
}