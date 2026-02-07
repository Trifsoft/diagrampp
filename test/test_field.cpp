#include <catch2/catch_test_macros.hpp>
#include <model/elements/field.h>
#include <model/base/visibility.h>

TEST_CASE("Field") {
    Field f("x", "int", Visibility::Private);
    SECTION("declaration()") {
        REQUIRE(f.declaration() == "int x");
    }
    SECTION("definition()") {
        REQUIRE(f.definition("MyClass") == std::nullopt);
    }
}
