//
// Examples of template deduction using rvalue references.
//


struct Type { };

template <typename T> void foo(T&&);


int main() {
    Type dummy;

    // Remember, because of the special template argument deduction, we add &
    // to the type before deducing when the expression from which we deduce is
    // an lvalue.

    Type a;
    // a is an lvalue for obvious reasons
    //foo(a);
    // foo<Type&>           -> which gives a call to
    // foo<Type&>(Type& &&) -> reference collapsing rules give
    // foo<Type&>(Type&)


    Type& ra = dummy;
    // ra is an lvalue for obvious reasons too
    //foo(ra);
    // foo<Type& &>         -> reference collapsing rules give
    // foo<Type&>           -> which gives a call to
    // foo<Type&>(Type& &&) -> reference collapsing rules give
    // foo<Type&>(Type&)

    // Note: Above, I am not SURE about how the collapsing of the 2 lvalue
    //       references work, but it does.

    Type&& rra = Type();
    // rra is an lvalue because it is a NAMED rvalue reference
    //foo(rra);
    // foo<Type&& &>        -> reference collapsing rules give
    // foo<Type&>           -> which gives a call to
    // foo<Type&>(Type& &&) -> reference collapsing rules give
    // foo<Type&>(Type&)

    Type&& unnamed_rra();
    // unnamed_rra() is an xvalue because it is an UNNAMED rvalue reference
    //foo(unnamed_rra());
    // foo<Type>            -> which gives a call to
    // foo<Type>(Type&&)

    Type prvalue();
    // prvalue() is a prvalue because it returns a temporary
    //foo(prvalue());
    // foo<Type>            -> which gives a call to
    // foo<Type>(Type&&)
}
