//
// Example of creating custom concept specifications.
//

#include <boost/concept/assert.hpp>
#include <boost/concept/usage.hpp>


template <typename T>
struct MyEqualityComparable {
    BOOST_CONCEPT_USAGE(MyEqualityComparable) {
        require_boolean(a == b);
        require_boolean(a != b);
    }

private:
    T a, b;
    void require_boolean(bool);
};


struct no_comp { };

struct bad_comp { };
void operator==(bad_comp const&, bad_comp const&);
bool operator!=(bad_comp const&, bad_comp const&);


int main(int argc, char const* argv[]) {
    // Will work.
    BOOST_CONCEPT_ASSERT((MyEqualityComparable<int>));
    BOOST_CONCEPT_ASSERT((MyEqualityComparable<float>));

    // Will fail because no_comp can't be compared.
    //BOOST_CONCEPT_ASSERT((MyEqualityComparable<no_comp>));

    // Will fail because bad_comp::operator== does not return a boolean.
    //BOOST_CONCEPT_ASSERT((MyEqualityComparable<bad_comp>));

    return 0;
}
