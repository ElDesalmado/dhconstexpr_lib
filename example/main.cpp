#include "dhconstexpr_lib.hpp"

#include <iostream>

struct abc
{
    void func1(int i, double d)
    {
        std::cout << i << '\n' << d << '\n';
    }

    int func2()
    {
        return 9;
    }
};

typedef cexpr_pair<auto_t<1>, auto_t<&abc::func1>> pair_func1;
typedef cexpr_pair<auto_t<2>, auto_t<&abc::func2>> pair_func2;
typedef cexpr_pair<type_t<int>, auto_t<&abc::func2>> int_func2;
typedef cexpr_pair<abc, auto_t<18>> custom1;
typedef cexpr_pair<int, abc> custom2;

typedef cexpr_generic_map<pair_func1, pair_func2, int_func2, custom1, custom2> map_funcs;

typedef cexpr_generic_map<
    //initial values
    cexpr_pair<auto_t<0>, auto_t<4>>,
    cexpr_pair<auto_t<1>, auto_t<8>>,
    cexpr_pair<auto_t<2>, auto_t<15>>,

    //inversed values
    cexpr_pair<auto_t<4>, auto_t<0>>,
    cexpr_pair<auto_t<8>, auto_t<1>>,
    cexpr_pair<auto_t<15>, auto_t<2>>
> map_inverse;

int main()
{
    abc a;
    (a.*map_funcs::found_pair<auto_t<1>>::value::value)(6, 3.28);                         // 6 3.28

    std::cout << (a.*map_funcs::found_pair<auto_t<2>>::value::value)() << std::endl;      // 9
    std::cout << (a.*map_funcs::found_pair<type_t<int>>::value::value)() << std::endl;    // 9 
    std::cout << map_funcs::found_pair<abc>::value::value << std::endl;                     // 18
    map_funcs::found_pair<int>::value().func1(4, 8.15162342);                               // 4 8.15162342

    static_assert(map_inverse::found_pair<auto_t<0>>::value::value == 4);
    static_assert(map_inverse::found_pair<auto_t<1>>::value::value == 8);
    static_assert(map_inverse::found_pair<auto_t<2>>::value::value == 15);

    static_assert(map_inverse::found_pair<auto_t<4>>::value::value == 0);
    static_assert(map_inverse::found_pair<auto_t<8>>::value::value == 1);
    static_assert(map_inverse::found_pair<auto_t<15>>::value::value == 2);


    std::cin.get();
    return 0;
}