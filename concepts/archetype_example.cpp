//
// Example of creating a custom concept archetype.
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
    static T a, b;
    void require_boolean(bool);
};


// We disable automatically generated methods because the goal of an
// archetype is to be a type meeting the _minimal_ requirements of a
// concept. Archetypes can then be used to check whether an algorithm's
// specified concepts really cover the whole algorithm by instantiating
// the algorithm with the archetype.
class equality_comparable_archetype {
    typedef equality_comparable_archetype Self;
    equality_comparable_archetype();
    equality_comparable_archetype(Self const&);
    Self& operator=(Self const&);
    ~equality_comparable_archetype();
};
bool operator==(equality_comparable_archetype const&,
                equality_comparable_archetype const&)
{ return true; }

bool operator!=(equality_comparable_archetype const&,
                equality_comparable_archetype const&)
{ return true; }


int main(int argc, char const* argv[]) {
    BOOST_CONCEPT_ASSERT((MyEqualityComparable<equality_comparable_archetype>));

    return 0;
}
