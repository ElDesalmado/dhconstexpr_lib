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

#include <type_traits>
#include <tuple>

//tag for typenames
template <class T>
struct tag_type
{
    using type = T;
};

//tag for autos
template <auto val>
struct tag_auto
{
    constexpr static decltype(val) value = val;
};

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