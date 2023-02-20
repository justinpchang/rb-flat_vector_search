#ifndef FLAT_VECTOR_SEARCH_HPP
#define FLAT_VECTOR_SEARCH_HPP 1

#include <iostream>
#include <map>
#include <vector>
#include <string>

#include <ruby.h>
#include <typeinfo>

#include <metrics.hpp>

struct index_t
{
    int n_dims;
    std::map<std::string, std::vector<double>> items;
};

struct distance_t
{
    std::string key;
    double distance;

    distance_t(std::string _key, double _distance) : key(_key), distance(_distance){};
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
    new (ptr) index_t();
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

VALUE index_get_items(VALUE self)
{
    const int n_dims = get_index(self)->n_dims;
    const std::map<std::string, std::vector<double>> items = get_index(self)->items;
    VALUE rb_items_hash = rb_hash_new();

    for (auto const &[key, vec] : items)
    {
        VALUE rb_item_ary = rb_ary_new2(n_dims);

        for (int i = 0; i < vec.size(); i += 1)
        {
            rb_ary_store(rb_item_ary, i, DBL2NUM(vec[i]));
        }

        rb_hash_aset(rb_items_hash, rb_str_new2(key.c_str()), rb_item_ary);
    }

    return rb_items_hash;
}

VALUE index_get_item(VALUE self, VALUE _key)
{
    Check_Type(_key, T_STRING);

    const char *key = (char *)StringValueCStr(_key);
    const int n_dims = get_index(self)->n_dims;

    const std::vector<double> vec = get_index(self)->items[key];
    VALUE rb_item_ary = rb_ary_new2(n_dims);

    for (int i = 0; i < vec.size(); i += 1)
    {
        rb_ary_store(rb_item_ary, i, DBL2NUM(vec[i]));
    }

    return rb_item_ary;
}

VALUE index_add_item(VALUE self, VALUE _key, VALUE _vec)
{
    Check_Type(_key, T_STRING);
    Check_Type(_vec, T_ARRAY);

    const std::string key = (char *)StringValueCStr(_key);
    const int n_dims = get_index(self)->n_dims;

    if (n_dims != RARRAY_LEN(_vec))
    {
        rb_raise(rb_eArgError, "Item vector size does not match index dimensionality.");
    }

    std::vector<double> vec;
    for (int i = 0; i < n_dims; i += 1)
    {
        vec.push_back((double)NUM2DBL(rb_ary_entry(_vec, i)));
    }

    get_index(self)->items[key] = vec;

    return Qtrue;
}

VALUE index_remove_item(VALUE self, VALUE _key)
{
    Check_Type(_key, T_STRING);

    const std::string key = (char *)StringValueCStr(_key);
    index_t *index = get_index(self);

    std::map<std::string, std::vector<double>>::iterator iter = index->items.find(key);

    if (iter == index->items.end())
    {
        return Qfalse;
    }

    index->items.erase(iter);

    return Qtrue;
}

VALUE index_nearest_k_by_vector(VALUE self, VALUE _vec, VALUE _k)
{
    Check_Type(_vec, T_ARRAY);
    Check_Type(_k, T_FIXNUM);

    const int k = NUM2INT(_k);
    index_t *index = get_index(self);
    const int n_dims = index->n_dims;

    if (n_dims != RARRAY_LEN(_vec))
    {
        rb_raise(rb_eArgError, "Item vector size does not match index dimensionality.");
    }

    std::vector<double> test_vec;
    for (int i = 0; i < n_dims; i += 1)
    {
        test_vec.push_back((double)NUM2DBL(rb_ary_entry(_vec, i)));
    }

    std::vector<distance_t> distances;
    for (auto const &[key, vec] : index->items)
    {
        distances.emplace_back(key, cosine_distance(test_vec, vec, n_dims));
    }

    std::sort(distances.begin(), distances.end(),
              [](const auto &i, const auto &j)
              { return i.distance < j.distance; });

    VALUE rb_neighbors_ary = rb_ary_new2(k);
    for (int i = 0; i < k; i += 1)
    {
        rb_ary_store(rb_neighbors_ary, i, rb_str_new2(distances[i].key.c_str()));
    }

    return rb_neighbors_ary;
}

VALUE define_class(VALUE rb_mFlatVectorSearch)
{
    VALUE rb_cIndex = rb_define_class_under(rb_mFlatVectorSearch, "Index", rb_cObject);

    rb_define_alloc_func(rb_cIndex, index_alloc);
    rb_define_method(rb_cIndex, "initialize", RUBY_METHOD_FUNC(index_init), 1);

    rb_define_method(rb_cIndex, "get_n_dims", RUBY_METHOD_FUNC(index_get_n_dims), 0);
    rb_define_method(rb_cIndex, "get_items", RUBY_METHOD_FUNC(index_get_items), 0);
    rb_define_method(rb_cIndex, "get_item", RUBY_METHOD_FUNC(index_get_item), 1);
    rb_define_method(rb_cIndex, "add_item", RUBY_METHOD_FUNC(index_add_item), 2);
    rb_define_method(rb_cIndex, "remove_item", RUBY_METHOD_FUNC(index_remove_item), 1);

    rb_define_method(rb_cIndex, "nearest_k_by_vector", RUBY_METHOD_FUNC(index_nearest_k_by_vector), 2);

    return rb_cIndex;
}

#endif /* FLAT_VECTOR_SEARCH_HPP */