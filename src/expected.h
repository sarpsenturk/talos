#pragma once

// Since C++23 adds expected support to the standard library
// we "import" the expected names into our own namespace to simplify
// the transition once we can upgrade the project to use C++23

#include <version>
#ifdef __cpp_lib_expected
    #include <expected>
namespace talos {
    using std::expected;
    using std::unexpected;
    using std::bad_expected_access;
    using std::unexpect;
    using std::unexpect_t;
}
#else
    #include <tl/expected.hpp>
namespace talos {
    using tl::expected;
    using tl::unexpected;
    using tl::bad_expected_access;
    using tl::unexpect;
    using tl::unexpect_t;
}
#endif