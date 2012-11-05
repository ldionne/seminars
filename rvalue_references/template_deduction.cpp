//
// Examples of template deduction using rvalue references.
//


struct Type { };

template <typename T> void foo(T&&);


int main() {
    Type dummy;

    Type a;
    //foo(a); // a is an lvalue for obvious reasons
    // foo<Type&>           -> which gives a call to
    // foo<Type&>(Type& &&) -> reference collapsing rules give
    // foo<Type&>(Type&)


    Type& ra = dummy;
    //foo(ra); // ra is an lvalue for obvious reasons too
    // foo<Type& &>         -> reference collapsing rules give
    // foo<Type&>           -> which gives a call to
    // foo<Type&>(Type& &&) -> reference collapsing rules give
    // foo<Type&>(Type&)


    Type&& rra = Type();
    //foo(rra); // rra is a lvalue because it is a NAMED rvalue reference
    // foo<Type&& &>        -> reference collapsing rules give
    // foo<Type&>           -> which gives a call to
    // foo<Type&>(Type& &&) -> reference collapsing rules give
    // foo<Type&>(Type&)

    Type&& unnamed_rra();
    //foo(unnamed_rra()); // unnamed_rra() is an xvalue because it is an UNNAMED rvalue reference
    // foo<Type&&>              -> which gives a call to
    // foo<Type&&>(Type&& &&)   -> reference collapsing rules give
    // foo<Type&&>(Type&&)
}
