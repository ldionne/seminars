//
// Examples to show the value category of different expressions.
//


struct Type { };


Type prvalue();
Type& lvalue();
Type&& xvalue();

int main() {
    // When an expression is an lvalue, we are able to take its address.
    //&prvalue(); // fails: prvalue() does not yield an lvalue.
    //&lvalue();  // works: lvalue() yields an lvalue.
    //&xvalue();  // fails: xvalue() does not yield an lvalue.

    Type&& named_xvalue_is_actually_an_lvalue = xvalue();
    // works: a named xvalue is actually an lvalue
    //&named_xvalue_is_actually_an_lvalue;
}
