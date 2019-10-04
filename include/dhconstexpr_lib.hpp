#pragma once

/*MIT License

Copyright (c) 2019 ElDesalmado

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#ifndef _TYPE_TRAITS_
#include <type_traits>
#endif

#ifndef _TUPLE_
#include <tuple>
#endif

//tag for typenames
template <class T>
struct type_t
{
    using type = T;
};

//tag for autos
template <auto val>
using auto_t = std::integral_constant<decltype(val), val>;

//this is a workaround for vs express 17 (because const char* fails for auto_t)
template <const char* val>
using char_t = std::integral_constant<const char*, val>;

constexpr bool str_equal(const char *a, const char *b)
{
	return *a == *b && (*a == '\0' || str_equal(a + 1, b + 1));
}


//generic pair
template <typename key_tag, typename val_tag>
struct cexpr_pair
{
    using key = key_tag;
    using value = val_tag;
};

template <class ... cexpr_pairs>
class cexpr_generic_map
{
    template <typename cexpr_tag_key, size_t iter = 0>
    constexpr static auto Find()
    {
        //failed to find by key
        if constexpr (iter == sizeof...(cexpr_pairs))
            return cexpr_pair<cexpr_tag_key, void>();
        else
        {
            typedef std::tuple_element_t<iter, std::tuple<cexpr_pairs...>> cur_pair;
            if constexpr (std::is_same_v<cexpr_tag_key, cur_pair::key>)
                return cur_pair();
            else 
                return Find<cexpr_tag_key, iter + 1>();
        }
    }

public:

    template <typename tag_key>
    using found_pair = decltype(Find<tag_key>());
};


template <typename T, size_t sz, class iseq = decltype(std::make_index_sequence<sz>())>
struct gen_tuple;

template <typename T, size_t sz, size_t ... indx>
struct gen_tuple<T, sz, std::index_sequence<indx...>>
{
    template <size_t>
    using T_ = T;

    using tuple = std::tuple<T_<indx>...>;
};

template <typename T, size_t sz>
using n_tuple_t = typename gen_tuple<T, sz>::tuple;

template <typename To, typename From>
using to_type = To;

template <size_t indx, typename ... types>
using nth_type = typename std::tuple_element_t<indx, std::tuple<types...>>;

template <auto val, auto ... compare>
inline constexpr bool is_any = ((val == compare) || ...);

template <typename ... Types>
inline constexpr bool all_same_v = sizeof...(Types) ? (std::is_same_v<std::tuple_element_t<0, std::tuple<Types...>>, Types> && ...) : false;

template <bool ... vals>
constexpr inline bool dh_conjunction = (vals && ...);

template <bool ... vals>
constexpr inline bool dh_disjunction = (vals || ...);

//function traits

template <auto func, class func_t = decltype(func)>
struct func_traits;

template <auto func, typename Ret, typename Cl, typename ... Args>
struct func_traits<func, Ret(Cl::*)(Args...)>
{
	using t_return = Ret;
	using t_class = Cl;
	using tuple_args = std::tuple<Args...>;
	constexpr static size_t args_count = sizeof...(Args);
};

template <auto func, typename Ret, typename ... Args>
struct func_traits<func, Ret(*)(Args...)>
{
	using t_return = Ret;
	using t_class = void;
	using tuple_args = std::tuple<Args...>;
	constexpr static size_t args_count = sizeof...(Args);
};