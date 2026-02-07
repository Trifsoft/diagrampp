#include <catch2/catch_test_macros.hpp>
#include <model/elements/constructor/copy_constructor.h>
#include <model/elements/field.h>
#include <model/base/visibility.h>

TEST_CASE("CopyConstructor - with fields") {
    QList<std::shared_ptr<Field>> fields = {
        std::make_shared<Field>("x", "int", Visibility::Private),
        std::make_shared<Field>("y", "double", Visibility::Private)
    };
    CopyConstructor ctor("MyClass", fields, Visibility::Public);

    SECTION("declaration()") {
        REQUIRE(ctor.declaration() == "MyClass(const MyClass& other)");
    }
    SECTION("definition()") {
        REQUIRE(ctor.definition("MyClass") == "MyClass::MyClass(const MyClass& other) : x(other.x), y(other.y) {}");
    }
}

TEST_CASE("CopyConstructor - no fields") {
    QList<std::shared_ptr<Field>> fields = {};
    CopyConstructor ctor("MyClass", fields, Visibility::Public);

    SECTION("declaration()") {
        REQUIRE(ctor.declaration() == "MyClass(const MyClass& other)");
    }
    SECTION("definition()") {
        REQUIRE(ctor.definition("MyClass") == "MyClass::MyClass(const MyClass& other) {}");
    }
}
