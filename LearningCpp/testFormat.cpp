#include "testlang.h"
#include "print.h"

struct Frac {
    long n;
    long d;
};

template<>
struct std::formatter<Frac>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }
    template<typename FormatContext>
    auto format(const Frac& f, FormatContext& ctx) {
        return format_to(ctx.out(), "{0:d}/{1:d}",
            f.n, f.d);
    }
};

void testFormat()
{
    Frac f{ 5, 3 };
    print("Frac: {}\n", f);
}
