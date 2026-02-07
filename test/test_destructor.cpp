#include <catch2/catch_test_macros.hpp>
#include <model/elements/destructor.h>
#include <model/elements/field.h>
#include <model/base/visibility.h>

TEST_CASE("Destructor - default (no destruction fields)") {
    QVector<std::shared_ptr<Field>> fields = {
        std::make_shared<Field>("x", "int", Visibility::Private)
};
Destructor dtor("MyClass", fields);

SECTION("declaration()") {
    REQUIRE(dtor.declaration() == "~MyClass() = default");
}
SECTION("definition()") {
    REQUIRE(dtor.definition("MyClass") == std::nullopt);
}
}

TEST_CASE("Destructor - virtual, default") {
    QVector<std::shared_ptr<Field>> fields = {};
    Destructor dtor("MyClass", fields, true);

    SECTION("declaration()") {
        REQUIRE(dtor.declaration() == "virtual ~MyClass() = default");
    }
    SECTION("definition()") {
        REQUIRE(dtor.definition("MyClass") == std::nullopt);
    }
}

TEST_CASE("Destructor - non-default (has destruction)") {
    QVector<std::shared_ptr<Field>> fields = {
        std::make_shared<Field>("ptr", "int*", Visibility::Private, QString("delete ptr"))
};
Destructor dtor("MyClass", fields);

SECTION("declaration()") {
    REQUIRE(dtor.declaration() == "~MyClass()");
}
SECTION("definition()") {
    REQUIRE(dtor.definition("MyClass") == "~MyClass::MyClass() {\ndelete ptr\n}");
}
}
