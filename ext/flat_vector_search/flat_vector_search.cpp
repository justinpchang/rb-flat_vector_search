#ifndef FLAT_VECTOR_SEARCH_CPP
#define FLAT_VECTOR_SEARCH_CPP 1

#include <vector>

#include "flat_vector_search.hpp"

typedef struct Index
{
    int n_dims;
    std::vector<std::vector<float>> items;
};

void index_free(void *ptr)
{
    ruby_xfree(ptr);
}

size_t index_size(const void *ptr)
{
    return sizeof(*((Index *)ptr));
}

static const rb_data_type_t index_type = {
    .wrap_struct_name = "index",
    .function = {
        .dmark = NULL,
        .dfree = index_free,
        .dsize = index_size,
    },
    .data = NULL,
    .flags = RUBY_TYPED_FREE_IMMEDIATELY,
};

VALUE index_alloc(VALUE self)
{
    // data only contains (int) n_dims for now
    int *data = (int *)ruby_xmalloc(sizeof(Index));
    return TypedData_Wrap_Struct(self, &index_type, data);
}

Index *get_index(VALUE self)
{
    Index *ptr;
    TypedData_Get_Struct(self, Index, &index_type, ptr);
    return ptr;
}

VALUE index_init(VALUE self, VALUE _n_dims)
{
    Index *ptr = get_index(self);
    ptr->n_dims = NUM2INT(_n_dims);

    return self;
}

VALUE index_get_n_dims(VALUE self)
{
    Index *ptr;
    TypedData_Get_Struct(self, Index, &index_type, ptr);

    return INT2NUM(ptr->n_dims);
}

extern "C" void Init_flat_vector_search(void)
{
    VALUE rb_mFlatVectorSearch = rb_define_module("FlatVectorSearch");
    VALUE rb_cIndex = rb_define_class_under(rb_mFlatVectorSearch, "Index", rb_cObject);
    rb_define_alloc_func(rb_cIndex, index_alloc);
    rb_define_method(rb_cIndex, "initialize", RUBY_METHOD_FUNC(index_init), 1);
    rb_define_method(rb_cIndex, "get_n_dims", RUBY_METHOD_FUNC(index_get_n_dims), 0);
}

#endif /* FLAT_VECTOR_SEARCH_CPP */