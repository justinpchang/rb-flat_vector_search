#ifndef FLAT_VECTOR_SEARCH_HPP
#define FLAT_VECTOR_SEARCH_HPP 1

#include <vector>

#include <ruby.h>
#include <typeinfo>

struct index_t
{
    int n_dims;
    std::vector<std::vector<double>> vecs;
};

void index_free(void *ptr)
{
    ruby_xfree(ptr);
}

size_t index_size(const void *ptr)
{
    return sizeof(*((index_t *)ptr));
}

static const rb_data_type_t rb_index_t = {
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
    index_t *ptr = (index_t *)ruby_xmalloc(sizeof(index_t));
    return TypedData_Wrap_Struct(self, &rb_index_t, ptr);
}

index_t *get_index(VALUE self)
{
    index_t *ptr;
    TypedData_Get_Struct(self, index_t, &rb_index_t, ptr);
    return ptr;
}

VALUE index_init(VALUE self, VALUE _n_dims)
{
    index_t *ptr = get_index(self);
    ptr->n_dims = NUM2INT(_n_dims);

    return self;
}

VALUE index_get_n_dims(VALUE self)
{
    index_t *ptr = get_index(self);

    return INT2NUM(ptr->n_dims);
}

VALUE index_add_item(VALUE self, VALUE _idx, VALUE _vec)
{
    const int32_t idx = (int32_t)NUM2INT(_idx);
    const int n_dims = get_index(self)->n_dims;

    if (!RB_TYPE_P(_vec, T_ARRAY))
    {
        rb_raise(rb_eArgError, "Item vector should be Array.");
    }

    if (n_dims != RARRAY_LEN(_vec))
    {
        rb_raise(rb_eArgError, "Item vector size does not match index dimensionality.");
    }

    std::vector<double> vec;
    for (int i = 0; i < n_dims; i += 1)
    {
        vec.push_back((double)NUM2DBL(rb_ary_entry(_vec, i)));
    }

    get_index(self)->vecs.push_back(vec);

    return Qtrue;
}

VALUE define_class(VALUE rb_mFlatVectorSearch)
{
    VALUE rb_cIndex = rb_define_class_under(rb_mFlatVectorSearch, "Index", rb_cObject);
    rb_define_alloc_func(rb_cIndex, index_alloc);
    rb_define_method(rb_cIndex, "initialize", RUBY_METHOD_FUNC(index_init), 1);
    rb_define_method(rb_cIndex, "get_n_dims", RUBY_METHOD_FUNC(index_get_n_dims), 0);
    rb_define_method(rb_cIndex, "add_item", RUBY_METHOD_FUNC(index_add_item), 2);
    return rb_cIndex;
}

#endif /* FLAT_VECTOR_SEARCH_HPP */