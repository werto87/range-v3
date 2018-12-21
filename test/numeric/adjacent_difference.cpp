// Range v3 library
//
//  Copyright Eric Niebler 2014-present
//  Copyright Gonzalo Brito Gadeschi 2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//
// Implementation based on the code in libc++
//   http://http://libcxx.llvm.org/

//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <range/v3/core.hpp>
#include <range/v3/numeric/adjacent_difference.hpp>
#include "../simple_test.hpp"
#include "../test_iterators.hpp"

struct S
{
    int i;
};

template<class InIter, class OutIter, class InSent = InIter> void test()
{
    using ranges::adjacent_difference;
    using ranges::make_subrange;
    { // iterator
        int ia[] = {15, 10, 6, 3, 1};
        int ir[] = {15, -5, -4, -3, -2};
        const unsigned s = sizeof(ia) / sizeof(ia[0]);
        int ib[s] = {0};
        auto r = adjacent_difference(InIter(ia), InSent(ia + s), OutIter(ib));
        CHECK(base(r.in) == ia + s);
        CHECK(base(r.out) == ib + s);
        for(unsigned i = 0; i < s; ++i)
        {
            CHECK(ib[i] == ir[i]);
        }
    }

    { // range + output iterator
        int ia[] = {15, 10, 6, 3, 1};
        int ir[] = {15, -5, -4, -3, -2};
        const unsigned s = sizeof(ia) / sizeof(ia[0]);
        int ib[s] = {0};
        auto rng = make_subrange(InIter(ia), InSent(ia + s));
        auto r = adjacent_difference(rng, OutIter(ib));
        CHECK(base(r.in) == ia + s);
        CHECK(base(r.out) == ib + s);
        for(unsigned i = 0; i < s; ++i)
        {
            CHECK(ib[i] == ir[i]);
        }
    }

    { // range + output range
        int ia[] = {15, 10, 6, 3, 1};
        int ir[] = {15, -5, -4, -3, -2};
        const unsigned s = sizeof(ia) / sizeof(ia[0]);
        int ib[s] = {0};
        auto rng = make_subrange(InIter(ia), InSent(ia + s));
        auto orng = make_subrange(OutIter(ib), OutIter(ib + s));
        auto r = adjacent_difference(rng, orng);
        CHECK(base(r.in) == ia + s);
        CHECK(base(r.out) == ib + s);
        for(unsigned i = 0; i < s; ++i)
        {
            CHECK(ib[i] == ir[i]);
        }
    }

    {
        int ia[] = {15, 10, 6, 3, 1};
        int ir[] = {15, 25, 16, 9, 4};
        const unsigned s = sizeof(ia) / sizeof(ia[0]);
        int ib[s] = {0};
        auto rng = make_subrange(InIter(ia), InSent(ia + s));
        auto orng = make_subrange(OutIter(ib), OutIter(ib + s));
        auto r = adjacent_difference(rng, orng, std::plus<int>());
        CHECK(base(r.in) == ia + s);
        CHECK(base(r.out) == ib + s);
        for(unsigned i = 0; i < s; ++i)
        {
            CHECK(ib[i] == ir[i]);
        }
    }
}

int main()
{
    test<input_iterator<const int *>, input_iterator<int *>>();
    test<input_iterator<const int *>, forward_iterator<int *>>();
    test<input_iterator<const int *>, bidirectional_iterator<int *>>();
    test<input_iterator<const int *>, random_access_iterator<int *>>();
    test<input_iterator<const int *>, int *>();

    test<forward_iterator<const int *>, input_iterator<int *>>();
    test<forward_iterator<const int *>, forward_iterator<int *>>();
    test<forward_iterator<const int *>, bidirectional_iterator<int *>>();
    test<forward_iterator<const int *>, random_access_iterator<int *>>();
    test<forward_iterator<const int *>, int *>();

    test<bidirectional_iterator<const int *>, input_iterator<int *>>();
    test<bidirectional_iterator<const int *>, forward_iterator<int *>>();
    test<bidirectional_iterator<const int *>, bidirectional_iterator<int *>>();
    test<bidirectional_iterator<const int *>, random_access_iterator<int *>>();
    test<bidirectional_iterator<const int *>, int *>();

    test<random_access_iterator<const int *>, input_iterator<int *>>();
    test<random_access_iterator<const int *>, forward_iterator<int *>>();
    test<random_access_iterator<const int *>, bidirectional_iterator<int *>>();
    test<random_access_iterator<const int *>, random_access_iterator<int *>>();
    test<random_access_iterator<const int *>, int *>();

    test<const int *, input_iterator<int *>>();
    test<const int *, forward_iterator<int *>>();
    test<const int *, bidirectional_iterator<int *>>();
    test<const int *, random_access_iterator<int *>>();
    test<const int *, int *>();

    using ranges::adjacent_difference;

    { // Test projections
        S ia[] = {{15}, {10}, {6}, {3}, {1}};
        int ir[] = {15, -5, -4, -3, -2};
        const unsigned s = sizeof(ir) / sizeof(ir[0]);
        int ib[s] = {0};
        auto r = adjacent_difference(ranges::begin(ia), ranges::begin(ia) + s,
                                     ranges::begin(ib), std::minus<int>(), &S::i);
        CHECK(base(r.in) == ia + s);
        CHECK(base(r.out) == ib + s);
        for(unsigned i = 0; i < s; ++i)
        {
            CHECK(ib[i] == ir[i]);
        }
    }

    { // Test BinaryOp
        int ia[] = {15, 10, 6, 3, 1};
        int ir[] = {15, 25, 16, 9, 4};
        const unsigned s = sizeof(ir) / sizeof(ir[0]);
        int ib[s] = {0};
        auto r = adjacent_difference(ia, ranges::begin(ib), std::plus<int>());
        CHECK(base(r.in) == ia + s);
        CHECK(base(r.out) == ib + s);
        for(unsigned i = 0; i < s; ++i)
        {
            CHECK(ib[i] == ir[i]);
        }
    }

    { // Test calling it with an array
        int ia[] = {15, 10, 6, 3, 1};
        int ir[] = {15, 25, 16, 9, 4};
        const unsigned s = sizeof(ir) / sizeof(ir[0]);
        int ib[s] = {0};
        auto r = adjacent_difference(ia, ib, std::plus<int>());
        CHECK(base(r.in) == ia + s);
        CHECK(base(r.out) == ib + s);
        for(unsigned i = 0; i < s; ++i)
        {
            CHECK(ib[i] == ir[i]);
        }
    }

    return ::test_result();
}
