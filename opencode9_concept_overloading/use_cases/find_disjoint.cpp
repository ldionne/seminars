//!
//! This file implements the `find_disjoint` algorithm.
//!

#include <duck/random_access_iterator.hpp>

#include <boost/iterator/iterator_traits.hpp>
#include <boost/unordered_set.hpp>
#include <boost/utility/enable_if.hpp>
#include <iterator>
#include <vector>
#include <list>


namespace opencode9 {

namespace find_disjoint_detail {
    // Common code for both versions of the algorithm.
    template <typename Iterator, typename Set>
    Iterator find_disjoint_base(Iterator& first, Iterator& last, Set& seen) {
        auto curr = *first;
        seen.insert(curr);
        // For each in [first + 1, last)
        for (++first; first != last; ++first) {
            auto const& next = *first;
            if (curr != next) {
                // If it was seen before, we found the disjoint.
                if (!seen.insert(next).second)
                    return first;
                curr = next;
            }
        }
        return last;
    }

    struct half {
        template <typename T>
        T operator()(T t) const { return t / 2; }
    };
} // end namespace find_disjoint_detail

/**
 * Return an iterator to the first element in a range delimited by
 * [first, last) which is separated from a previous equal element
 * by at least one element.
 *
 * If no such element is found, an iterator to the past-the-end
 * element is returned.
 *
 * This algorithm makes at most n-1 comparisons and stores at most
 * n-1 elements in memory.
 *
 * The algorithm requires the following concepts:
 *   - Iterator must be a ReadableIterator.
 *   - value_type of the Iterator must be Hashable.
 *   - value_type of the Iterator must be EqualityComparable.
 */
template <typename Iterator>
typename boost::disable_if<
    duck::models<duck::RandomAccessIterator, Iterator>,
Iterator>::type find_disjoint(Iterator first, Iterator last) {
    typedef typename boost::iterator_value<Iterator>::type Value;

    if (first == last)
        return last;
    boost::unordered_set<Value> seen;
    return find_disjoint_detail::find_disjoint_base(first, last, seen);
}

/**
 * Specialization of the algorithm for random access iterators:
 *  - Pre-allocate the `unordered_set` so we (hopefully) don't have to rehash.
 *
 * @note The `InitialSize` functor is used to determine the initial size of
 *       the `unordered_set`. Halving the whole length of the range is
 *       arbitrary and it should be specialized as needed.
 */
template <typename Iterator, typename InitialSize = find_disjoint_detail::half>
typename boost::enable_if<
    duck::models<duck::RandomAccessIterator, Iterator>,
Iterator>::type find_disjoint(Iterator first, Iterator last) {
    typedef typename boost::iterator_value<Iterator>::type Value;

    // Never do anything for ranges of less than 3 elements since we can't
    // find disjoint elements in those.
    using std::distance;
    auto const len = distance(first, last);
    if (len < 3)
        return last;

    // If we have to do something, preallocate the hash table.
    boost::unordered_set<Value> seen(InitialSize()(len));
    return find_disjoint_detail::find_disjoint_base(first, last, seen);
}

} // end namespace opencode9


int main() {
    std::list<int> list{0, 1, 2, 3, 4, 5};
    std::vector<int> vec{0, 1, 2, 3, 4, 5};

    // calls the first find_disjoint
    opencode9::find_disjoint(begin(list), end(list));

    // calls the second find_disjoint
    opencode9::find_disjoint(begin(vec), end(vec));
}
