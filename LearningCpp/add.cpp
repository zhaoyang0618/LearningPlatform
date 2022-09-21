#include <iostream>
#include <vector>
#include <set>
#include <ranges>
#include <atomic>

// concept to disable narrowing conversions:
template<typename From, typename To>
concept ConvertsWithoutNarrowing =
std::convertible_to<From, To> &&
	requires (From && x) {
		{ std::type_identity_t<To[]>{std::forward<From>(x)} }
		-> std::same_as<To[1]>;
};

// concept for container with push_back():
template<typename T>
concept SupportsPushBack = requires(T coll) {
	coll.push_back(std::declval<typename T::value_type>());
};

// add() for single values:
template<typename Coll, typename T>
	requires ConvertsWithoutNarrowing<T, typename Coll::value_type>
void add(Coll& coll, const T& val)
{
	if constexpr (SupportsPushBack<Coll>) {
		coll.push_back(val);
	}
	else {
		coll.insert(val);
	}
}

// add() for multiple values:
template<typename Coll, std::ranges::input_range T>
	requires ConvertsWithoutNarrowing<std::ranges::range_value_t<T>,
typename Coll::value_type>
void add(Coll& coll, const T& val)
{
	if constexpr (SupportsPushBack<Coll>) {
		coll.insert(coll.end(),
			std::ranges::begin(val), std::ranges::end(val));
	}
	else {
		coll.insert(std::ranges::begin(val), std::ranges::end(val));
	}
}

void testConceptsRequires()
{
	std::vector<int> iVec;
	add(iVec, 42); // OK: calls push_back() for T being int
	std::set<int> iSet;
	add(iSet, 42); // OK: calls insert() for T being int
	short s = 42;
	add(iVec, s); // OK: calls push_back() for T being short
	long long ll = 42;
	// add(iVec, ll); // ERROR: narrowing
	// add(iVec, 7.7); // ERROR: narrowing
	std::vector<double> dVec;
	add(dVec, 0.7); // OK: calls push_back() for floating-point types
	add(dVec, 0.7f); // OK: calls push_back() for floating-point types
	// add(dVec, 7); // ERROR: narrowing
	// insert collections:
	add(iVec, iSet); // OK: insert set elements into a vector
	add(iSet, iVec); // OK: insert vector elements into a set
	// can even insert raw array:
	int vals[] = { 0, 8, 18 };
	add(iVec, vals); // OK
	// add(dVec, vals); // ERROR: narrowing
}
