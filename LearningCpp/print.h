#pragma once
#include <iostream>
#include <format>
#include <string_view>
using namespace std;

template<typename... Args>
void print(const string_view fmt_str, Args&&... args) {
    auto fmt_args{ make_format_args(args...) };
    string outstr{ vformat(fmt_str, fmt_args) };
    fputs(outstr.c_str(), stdout);
}
