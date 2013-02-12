# Static typing in C++

In C++, you have to declare the type of a variable before using it.

    #include <string>

    void func(int some_integer) {
        // do stuff
    }

    int main() {
        int i = 0;
        char c = 'a';
        std::string s = "abc";
        func(i);    // works
        func(s);    // fails at compile time: can't convert string to int
    }

------------------------------------------------------------------------------

# Overloading

If we wanted to have a `func` that works both for strings and integers, we
would have to write one `func` for each type. That is called overloading.

    #include <string>

    void func(int some_integer) {
        // do something
    }

    void func(std::string some_string) {
        // do something else
    }

    int main() {
        int i = 0;
        std::string s = "abc";

        // Here, the compiler will look at both implementations of func and
        // find the most suited. This is called overload resolution.
        func(i);    // calls func(int)
        func(s);    // calls func(string)
    }

------------------------------------------------------------------------------

# Templates

Sometimes, we would like to have something more flexible. For example, some
functions are applicable to any type that can be added. We can use
templates to achieve this:

    #include <vector>

    template <typename T> // T can be any type
    T add(T a, T b) {
        return a + b;
    }

    int main() {
        // The compiler will create (we say instantiate) a
        // `int add(int, int)` function and that function will
        // be called when the program is run.
        int three = add(1, 2);

        // Ditto, but for `float add(float, float)`
        float zero_point_five = add(0.2, 0.3);

        // Here, the compiler will try to instantiate a
        // `vector<int> add(vector<int>, vector<int>)`
        // function, but there will be a compilation error when the function
        // is instantiated because `a + b` is not a valid expression for
        // vectors in C++.
        std::vector<int> a{0, 1, 2}, b{3, 4, 5};
        std::vector<int> c = add(a, b);
    }

------------------------------------------------------------------------------

# More templates

One last thing before we go deeper: we can mix overloads and templates.
So we could, for instance, change the add function so it works on vectors.
Disclaimer: This is for illustration; I don't actually recommend doing this.

    #include <vector>

    // Like the previous example
    template <typename T>
    T add(T a, T b) {
        return a + b;
    }

    // This function will be selected for vectors of any type `T`.
    template <typename T>
    std::vector<T> add(std::vector<T> a, std::vector<T> b) {
        std::vector<T> result = a;
        result.insert(result.end(), b.begin(), b.end());
        return result;
    }

    int main() {
        int three = add(1, 2);  // like before
        std::vector<int> a{1, 2, 3}, b{4, 5, 6};
        std::vector<int> c = add(a, b);  // now calls our overload
    }

------------------------------------------------------------------------------

# Principle of SFINAE

Now, that's pretty cool and all, but imagine we would like to pick a
different overload based on properties more complex than the sheer
identity of a type. With what we got right now, it's impossible.

Recall the overload resolution? The compiler will select the function best
suited for the job.
A corollary of this is:

 - The compiler will simply ignore overloads that generate invalid function
   types when substituting the types in the template. It will simply remove
   these unfit functions from the set of possible overloads without
   triggering a compilation error. This principle is called SFINAE
   (Substitution Failure Is Not An Error) and is used extensively for
   C++ metaprogramming.

        // Now, this function can only be selected if the `T` type has a
        // nested type named `process_me_fast`. Otherwise, T::process_me_fast
        // will refer to something that does not exist and the compiler will
        // be forced not to consider this function for overload resolution.
        template <typename T>
        typename T::process_me_fast func(T t) {
            // extra fast implementation!
        }

        // Same here, but for types that have a nested type named `process_me_slowly`.
        template <typename T>
        typename T::process_me_slowly func(T t) {
            // take your time!
        }

        struct Fast {
            // Note: This declares `process_me_fast` as an alias for `void` inside
            //       the `Fast` class. So the expression `Fast::process_me_fast`
            //       refers to that type (i.e. `void`).
            typedef void process_me_fast;
        };

        struct Slow {
            typedef void process_me_slowly;
        };

        int main() {
            Fast fast;
            Slow slow;

            func(fast); // calls the first implementation
            func(slow); // calls the second
        }

------------------------------------------------------------------------------

# Metafunctions and SFINAE

How could we make this more generally useful?
Imagine we could create a special type that only has some nested type
if some condition is satisfied.

    // First, let's create two dummy types representing true and false.
    struct true_ { };
    struct false_ { };

    // This is a template type that has a nested type named `type`.
    // It is parametrized by two other types, namely `Condition` and `T`,
    // however `Condition` does not do anything for now.
    template <typename Condition, typename T>
    struct enable_if {
        typedef T type;
    };

    // Now, using template specialization, we can write a version of the
    // template that should be used for specific values of the template
    // parameters:
    template <typename T>
    struct enable_if<false_, T> {
        // No nested type if we were instantiated like enable_if<false_, T>!
    };

    // By the same mechanism, we may also define a `disable_if` type that is
    // symmetrical to `enable_if`:
    template<typename Condition, typename T>
    struct disable_if {
        typedef T type;
    };
    template<typename T> struct disable_if<true_, T> { };

So the `enable_if` class always have a nested type named `type`,
except when the `Condition` it was instantiated with is `false_`.
This is very powerful.

    // If `T::should_process_fast` is true_, the `enable_if` part is a valid
    // type. Otherwise, it isn't and the overload is not considered.
    template <typename T>
    typename enable_if<typename T::should_process_fast, void>::type func(T t) {
        // fast implementation
    }

    // Note: We _must_ disable this overload to avoid ambiguous overload
    //       resolution where two or more functions are valid candidates.
    template <typename T>
    typename disable_if<typename T::should_process_fast, void>::type func(T t) {
        // slow implementation
    }

    struct Fast {
        typedef true_ should_process_fast;
    };

    struct Slow {
        typedef false_ should_process_fast;
    };

    int main() {
        Fast fast;
        Slow slow;

        func(fast); // first implementation
        func(slow); // second implementation
    }

------------------------------------------------------------------------------

# Concepts

A concept is a specification of the interface that must be supported
by a type. A type that has that interface is said to _model_ the concept.

An example of a concept would be `Container`. A container is any type
for which the following expressions are valid:

    // Creates an empty container.
    MyContainer cont;

    // Nested type representing the type of the elements in the container.
    typedef MyContainer::value_type TypeOfTheElements;

    // Returns an iterator (a "pointer") to the first element in the container.
    MyContainer::iterator begin = cont.begin();

    // Returns the number of elements in the container.
    MyContainer::size_type size = cont.size();

    // and so on...

------------------------------------------------------------------------------

# Putting it together

Imagine if we could have a metafunction that is true if and only if a
type models a given concept. We could use that metafunction in constructs
like enable_if and friends.

I implemented a small library that does just that:
`duck::models<Concept, T>` is a metafunction that is true if and only if `T`
is a model of the `Concept` concept.

------------------------------------------------------------------------------

# Use cases

See code examples in use_cases/.
