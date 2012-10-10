//
// Example of using concept checks to catch errors early on.
//

#include <boost/concept/assert.hpp>
#include <boost/concept/requires.hpp>
#include <boost/concept_check.hpp>
#include <iostream>
#include <iterator>
#include <vector>


// Note: I _know_ this breaks if T is not comparable with
//       iterator_traits<Iterator>::value_type, but let's keep this simple.
template <typename Iterator, typename T>
Iterator my_find(Iterator first, Iterator last, T const& value) {
    BOOST_CONCEPT_ASSERT((boost::InputIterator<Iterator>));
    BOOST_CONCEPT_ASSERT((boost::EqualityComparable<T>));
    return std::find(first, last, value);
}

template <typename Iterator, typename T>
BOOST_CONCEPT_REQUIRES(
    ((boost::InputIterator<Iterator>))
    ((boost::EqualityComparable<T>)),
(Iterator)) my_other_find(Iterator first, Iterator last, T const& value) {
    return std::find(first, last, value);
}


struct non_comparable { };

struct non_input_iterator : std::ostream_iterator<int> {
    non_input_iterator() : std::ostream_iterator<int>(std::cout) { }
};


int main(int argc, char const* argv[]) {
    std::vector<int> int_vec(10);
    std::vector<non_comparable> nocomp_vec(10);

    // Will work.
    my_find(int_vec.begin(), int_vec.end(), 0);
    my_other_find(int_vec.begin(), int_vec.end(), 0);

    // Will fail because *nocomp_vec.begin() is not EqualityComparable.
    //my_find(nocomp_vec.begin(), nocomp_vec.end(), non_comparable());
    //my_other_find(nocomp_vec.begin(), nocomp_vec.end(), non_comparable());

    // Will fail because non_input_iterator is not an InputIterator.
    //my_find(non_input_iterator(), non_input_iterator(), 0);
    //my_other_find(non_input_iterator(), non_input_iterator(), 0);

    return 0;
}
