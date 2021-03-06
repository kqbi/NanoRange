// Range v3 library
//
//  Copyright Eric Niebler 2014
//  Copyright Casey Carter 2015
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3

//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <nanorange/algorithm/min.hpp>
#include <memory>
#include <random>
#include <numeric>
#include <algorithm>
#include "../catch.hpp"
#include "../test_utils.hpp"
#include "../test_iterators.hpp"

namespace stl2 = nano;

namespace {

std::mt19937 gen;

template <class Iter, class Sent = Iter>
void
test_iter(Iter first, Sent last)
{
	REQUIRE(first != last);
	auto rng = stl2::subrange(first, last);
	auto v1 = stl2::min(rng);
	for (Iter i = first; i != last; ++i)
		CHECK(!(*i < v1));
}

template <class Iter, class Sent = Iter>
void
test_iter(unsigned N)
{
	REQUIRE(N > 0);
	std::unique_ptr<int[]> a{new int[N]};
	std::iota(a.get(), a.get()+N, 0);
	std::shuffle(a.get(), a.get()+N, gen);
	test_iter(Iter(a.get()), Sent(a.get()+N));
}

template <class Iter, class Sent = Iter>
void
test_iter()
{
	test_iter<Iter, Sent>(1);
	test_iter<Iter, Sent>(2);
	test_iter<Iter, Sent>(3);
	test_iter<Iter, Sent>(10);
	test_iter<Iter, Sent>(1000);
}

template <class Iter, class Sent = Iter>
void
test_iter_comp(Iter first, Sent last)
{
	REQUIRE(first != last);
	auto rng = stl2::subrange(first, last);
	auto v = stl2::min(rng, std::greater<int>());
	for (Iter i = first; i != last; ++i)
		CHECK(!std::greater<int>()(*i, v));
}

template <class Iter, class Sent = Iter>
void
test_iter_comp(unsigned N)
{
	REQUIRE(N > 0);
	std::unique_ptr<int[]> a{new int[N]};
	std::iota(a.get(), a.get()+N, 0);
	std::shuffle(a.get(), a.get()+N, gen);
	test_iter_comp(Iter(a.get()), Sent(a.get()+N));
}

template <class Iter, class Sent = Iter>
void
test_iter_comp()
{
	test_iter_comp<Iter, Sent>(1);
	test_iter_comp<Iter, Sent>(2);
	test_iter_comp<Iter, Sent>(3);
	test_iter_comp<Iter, Sent>(10);
	test_iter_comp<Iter, Sent>(1000);
}

void test_ilist()
{
	auto ilist = {5, 4, 3, 2, 1};
	CHECK(stl2::min(ilist) == 1);
	CHECK(stl2::min(ilist, stl2::greater{}) == 5);
	CHECK(stl2::min({1, 2, 3, 4, 5}) == 1);
	CHECK(stl2::min({1, 2, 3, 4, 5}, std::greater<int>{}) == 5);
}

struct S
{
	int i;
};

}

TEST_CASE("alg.min")
{
	test_iter<input_iterator<const int*> >();
	test_iter<forward_iterator<const int*> >();
	test_iter<bidirectional_iterator<const int*> >();
	test_iter<random_access_iterator<const int*> >();
	test_iter<const int*>();
	test_iter<input_iterator<const int*>, sentinel<const int*>>();
	test_iter<forward_iterator<const int*>, sentinel<const int*>>();
	test_iter<bidirectional_iterator<const int*>, sentinel<const int*>>();
	test_iter<random_access_iterator<const int*>, sentinel<const int*>>();

	test_iter_comp<input_iterator<const int*> >();
	test_iter_comp<forward_iterator<const int*> >();
	test_iter_comp<bidirectional_iterator<const int*> >();
	test_iter_comp<random_access_iterator<const int*> >();
	test_iter_comp<const int*>();
	test_iter_comp<input_iterator<const int*>, sentinel<const int*>>();
	test_iter_comp<forward_iterator<const int*>, sentinel<const int*>>();
	test_iter_comp<bidirectional_iterator<const int*>, sentinel<const int*>>();
	test_iter_comp<random_access_iterator<const int*>, sentinel<const int*>>();

	// Works with initializer lists?
	test_ilist();

	// Works with projections?
	S s[] = {S{1},S{2},S{3},S{4},S{-4},S{5},S{6},S{7},S{8},S{9}};
	S v = stl2::min(s, std::less<int>{}, &S::i);
	CHECK(v.i == -4);
}
