#ifndef FLAT_VECTOR_SEARCH_CPP
#define FLAT_VECTOR_SEARCH_CPP 1

#include "flat_vector_search.hpp"

static VALUE hello(VALUE self, VALUE name)
{
    return rb_str_new2("Hello world!");
}

extern "C" void Init_flat_vector_search(void)
{
    VALUE rb_mFlatVectorSearch = rb_define_module("FlatVectorSearch");
    VALUE rb_cTestClass = rb_define_class_under(rb_mFlatVectorSearch, "TestClass", rb_cObject);
    rb_define_singleton_method(rb_cTestClass, "hello", hello, 1);
}

#endif /* FLAT_VECTOR_SEARCH_CPP */