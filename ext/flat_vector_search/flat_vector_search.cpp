#ifndef FLAT_VECTOR_SEARCH_CPP
#define FLAT_VECTOR_SEARCH_CPP 1

#include "flat_vector_search.hpp"

extern "C" void Init_flat_vector_search(void)
{
    VALUE rb_mFlatVectorSearch = rb_define_module("FlatVectorSearch");
}

#endif /* FLAT_VECTOR_SEARCH_CPP */