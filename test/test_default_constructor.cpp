#include <catch2/catch_test_macros.hpp>
#include <model/elements/constructor/default_constructor.h>
#include <model/base/visibility.h>

TEST_CASE("DefaultConstructor - with arguments") {
    DefaultConstructor ctor("MyClass", { new Argument("x", "int"), new Argument("y", "double") }, Visibility::Public);

    SECTION("declaration()") {
        REQUIRE(ctor.declaration() == "MyClass(int x, double y)");
    }
    SECTION("definition()") {
        REQUIRE(ctor.definition("MyClass") == "MyClass::MyClass(int x, double y) : x(x), y(y) {}");
    }
}

TEST_CASE("DefaultConstructor - no arguments") {
    DefaultConstructor ctor("MyClass", {}, Visibility::Public);

    SECTION("declaration()") {
        REQUIRE(ctor.declaration() == "MyClass()");
    }
    SECTION("definition()") {
        REQUIRE(ctor.definition("MyClass") == "MyClass::MyClass() {}");
    }
}
