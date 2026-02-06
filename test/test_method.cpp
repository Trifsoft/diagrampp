#include <catch2/catch_test_macros.hpp>
#include <model/elements/method.h>

TEST_CASE("Method - Regular") {
    Method m("method", "void", Visibility::Private, MethodKind::Regular, { new Argument("bar", "int"), new Argument("foo", "double")});

    SECTION("declaration()") {
        REQUIRE(m.declaration() == "void method(int bar, double foo)");
    }
    SECTION("definition()") {
        REQUIRE(m.definition("MyClass") == "void MyClass::method(int bar, double foo)\n{\n\n}");
    }
}
TEST_CASE("Method - Regular Virtual") {
    SECTION("RegularVirtual") {
        Method m("method", "void", Visibility::Private, MethodKind::RegularVirtual, { new Argument("bar", "int"), new Argument("foo", "double")});

        SECTION("declaration()") {
            REQUIRE(m.declaration() == "virtual void method(int bar, double foo)");
        }
        SECTION("definition()") {
            REQUIRE(m.definition("MyClass") == "void MyClass::method(int bar, double foo)\n{\n\n}");
        }
    }
}
TEST_CASE("Method - Pure Virtual") {
    Method m("method", "void", Visibility::Private, MethodKind::PureVirtual, { new Argument("bar", "int"), new Argument("foo", "double")});

    SECTION("declaration()") {
        REQUIRE(m.declaration() == "virtual void method(int bar, double foo) = 0");
    }
    SECTION("definition() returns std::nullopt") {
        REQUIRE(m.definition("MyClass") == std::nullopt);
    }
}
