#include <catch2/catch_test_macros.hpp>
#include "validator.h"
#include "model/base/branches.h"
#include "model/elements/composition/composition.h"
#include "model/elements/composition/cpp_class.h"
#include "graph/diagram_graph.h"
#include <memory>

TEST_CASE("testvalidator::check_multiple_connection tests")
{
    SECTION("Given child with one connection to parent, "
            "When checking for multiple connections, "
            "Then returns true (count < 2)")
    {
        auto child = std::make_shared<CPPClass>("Child");
        auto parent = std::make_shared<CPPClass>("Parent");

        std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>> diagram = {
            {child, {{parent, BranchType::INHERITANCE}}}
        };

        REQUIRE(testvalidator::check_multiple_connection(child, parent, diagram) == true);
    }

    SECTION("Given child with two connections to parent, "
            "When checking for multiple connections, "
            "Then returns false (count >= 2)")
    {
        auto child = std::make_shared<CPPClass>("Child");
        auto parent = std::make_shared<CPPClass>("Parent");

        std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>> diagram = {
            {child, { {parent, BranchType::INHERITANCE}, {parent, BranchType::COMPOSITION} }}
        };

        REQUIRE(testvalidator::check_multiple_connection(child, parent, diagram) == false);
    }

    SECTION("Given child with no connection to parent, "
            "When checking for multiple connections, "
            "Then returns true (count = 0)")
    {
        auto child = std::make_shared<CPPClass>("Child");
        auto parent = std::make_shared<CPPClass>("Parent");
        auto other = std::make_shared<CPPClass>("Other");

        std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>> diagram = {
            {child, {{other, BranchType::INHERITANCE}}}
        };

        REQUIRE(testvalidator::check_multiple_connection(child, parent, diagram) == true);
    }
}

TEST_CASE("testvalidator::check_cycle tests")
{
    SECTION("Given a linear chain without cycle, "
            "When checking for cycles, "
            "Then returns false")
    {
        auto node1 = std::make_shared<CPPClass>("Node1");
        auto node2 = std::make_shared<CPPClass>("Node2");
        auto node3 = std::make_shared<CPPClass>("Node3");

        std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>> diagram = {
            {node1, {{node2, BranchType::INHERITANCE}}},
            {node2, {{node3, BranchType::INHERITANCE}}},
            {node3, {}}
        };

        REQUIRE(testvalidator::check_cycle(node1, BranchType::INHERITANCE, diagram) == false);
    }

    SECTION("Given a direct cycle (A -> A), "
            "When checking for cycles, "
            "Then returns true")
    {
        auto node1 = std::make_shared<CPPClass>("Node1");

        std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>> diagram = {
            {node1, {{node1, BranchType::INHERITANCE}}}
        };

        REQUIRE(testvalidator::check_cycle(node1, BranchType::INHERITANCE, diagram) == true);
    }

    SECTION("Given an indirect cycle (A -> B -> C -> A), "
            "When checking for cycles, "
            "Then returns true")
    {
        auto node1 = std::make_shared<CPPClass>("Node1");
        auto node2 = std::make_shared<CPPClass>("Node2");
        auto node3 = std::make_shared<CPPClass>("Node3");

        std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>> diagram = {
            {node1, {{node2, BranchType::INHERITANCE}}},
            {node2, {{node3, BranchType::INHERITANCE}}},
            {node3, {{node1, BranchType::INHERITANCE}}}
        };

        REQUIRE(testvalidator::check_cycle(node1, BranchType::INHERITANCE, diagram) == true);
    }

    SECTION("Given multiple branches without cycle, "
            "When checking for cycles, "
            "Then returns false")
    {
        auto node1 = std::make_shared<CPPClass>("Node1");
        auto node2 = std::make_shared<CPPClass>("Node2");
        auto node3 = std::make_shared<CPPClass>("Node3");
        auto node4 = std::make_shared<CPPClass>("Node4");

        std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>> diagram = {
            {node1, {{node2, BranchType::INHERITANCE}, {node3, BranchType::INHERITANCE}}},
            {node2, {{node4, BranchType::INHERITANCE}}},
            {node3, {{node4, BranchType::INHERITANCE}}},
            {node4, {}}
        };

        REQUIRE(testvalidator::check_cycle(node1, BranchType::INHERITANCE, diagram) == false);
    }
}

TEST_CASE("testvalidator::detect_diamond tests")
{
    SECTION("Given simple inheritance without diamond, "
            "When checking for diamond problem, "
            "Then returns false")
    {
        auto base = std::make_shared<CPPClass>("Base");
        auto derived1 = std::make_shared<CPPClass>("Derived1");
        auto derived2 = std::make_shared<CPPClass>("Derived2");

        std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>> diagram = {
            {derived1, {{base, BranchType::INHERITANCE}}},
            {derived2, {{base, BranchType::INHERITANCE}}},
            {base, {}}
        };

        REQUIRE(testvalidator::detect_diamond(derived1, diagram) == false);
    }

    SECTION("Given diamond inheritance (D -> B1 -> A and D -> B2 -> A), "
            "When checking for diamond problem, "
            "Then returns true")
    {
        // Arrange
        auto A = std::make_shared<CPPClass>("A");
        auto B1 = std::make_shared<CPPClass>("B1");
        auto B2 = std::make_shared<CPPClass>("B2");
        auto D = std::make_shared<CPPClass>("D");

        std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>> diagram = {
            {D, {{B1, BranchType::INHERITANCE}, {B2, BranchType::INHERITANCE}}},
            {B1, {{A, BranchType::INHERITANCE}}},
            {B2, {{A, BranchType::INHERITANCE}}},
            {A, {}}
        };

        REQUIRE(testvalidator::detect_diamond(D, diagram) == true);
    }

    SECTION("Given multiple inheritance without diamond, "
            "When checking for diamond problem, "
            "Then returns false")
    {
        auto A = std::make_shared<CPPClass>("A");
        auto B = std::make_shared<CPPClass>("B");
        auto C = std::make_shared<CPPClass>("C");
        auto D = std::make_shared<CPPClass>("D");

        std::map<SharedNodePtr, std::vector<std::pair<SharedNodePtr, BranchType>>> diagram = {
            {D, {{B, BranchType::INHERITANCE}, {C, BranchType::INHERITANCE}}},
            {B, {{A, BranchType::INHERITANCE}}},
            {C, {}},
            {A, {}}
        };

        REQUIRE(testvalidator::detect_diamond(D, diagram) == false);
    }
}
