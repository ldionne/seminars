//!
//! This file implements the `my_distance` algorithm.
//!

#include <duck/random_access_iterator.hpp>
#include <duck/forward_iterator.hpp>

#include <boost/utility/enable_if.hpp>
#include <iterator>
#include <list>
#include <vector>


namespace opencode9 {

template <typename Iterator>
typename boost::enable_if<
    duck::models<duck::RandomAccessIterator, Iterator>,
typename std::iterator_traits<Iterator>::difference_type>::type
my_distance(Iterator first, Iterator last) {
    return last - first;
}

template <typename Iterator>
typename boost::enable_if<
    duck::is_forward_iterator<Iterator>,
typename boost::disable_if<
    duck::is_random_access_iterator<Iterator>,
typename std::iterator_traits<Iterator>::difference_type>::type>::type
my_distance(Iterator first, Iterator last) {
    typedef typename std::iterator_traits<Iterator>::difference_type Diff;
    Diff len = 0;
    while (first != last)
        ++first, ++len;
    return len;
}

} // end namespace opencode9


int main() {
    std::list<int> list{0, 1, 2, 3, 4, 5};
    std::vector<int> vec{0, 1, 2, 3, 4, 5};

    // calls my_distance for forward iterators
    opencode9::my_distance(begin(list), end(list));

    // calls my_distance for random access iterators
    opencode9::my_distance(begin(vec), end(vec));
}
