#include <catch2/catch_test_macros.hpp>
#include <model/elements/field.h>
#include <model/base/visibility.h>

TEST_CASE("Field - declaration() returns correct decl") {
    Field f("x", "int", Visibility::Private);
    REQUIRE(f.declaration() == "int x");
}

TEST_CASE("Field - definition() returns std::nullopt") {
    Field f("x", "int", Visibility::Private);
    REQUIRE(f.definition("MyClass") == std::nullopt);
}
