#pragma once

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