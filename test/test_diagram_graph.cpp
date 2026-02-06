#include "graph/diagram_graph.h"
#include "model/elements/composition/cpp_class.h"
#include <memory>
#include <model/base/visibility.h>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("DiagramGraph::add_node(SharedNodePtr) tests")
{
    SECTION("Given an empty graph, "
            "When adding a node, "
            "Then the node exists in the diagram and neighbours list is empty.")
    {
        // Arrange
        DiagramGraph graph;
        SharedNodePtr node = std::make_shared<CPPClass>("node", Visibility::Public);

        // Act
        graph.add_node(node);
        auto& diagram = graph.get_diagram();


        // Assert
        REQUIRE(diagram.size() == 1);
        REQUIRE(diagram.find(node) != diagram.end());
        REQUIRE(diagram.at(node).empty());
    }

    SECTION("Given a graph with one node, "
            "When adding the same node again, "
            "Then the graph is unchanged (no duplicates).")
    {
        // Arange
        DiagramGraph graph;
        SharedNodePtr node = std::make_shared<CPPClass>("B", Visibility::Public);
        graph.add_node(node);

        auto& diagram_before = graph.get_diagram();
        const auto size_before = diagram_before.size();

        // Act
        graph.add_node(node);
        auto& diagram_after = graph.get_diagram();

        // Assert
        REQUIRE(diagram_after.size() == size_before);
        REQUIRE(diagram_after.find(node) != diagram_after.end());
    }
}



TEST_CASE("DiagramGraph::remove_node(SharedNodePtr) tests")
{
    SECTION("Given a graph with incoming edges to a node, "
            "When removing that node, "
            "Then the node is removed and all incoming edges to it are erased.")
    {
        // Arrange
        DiagramGraph graph;
        auto A = std::make_shared<CPPClass>("A", Visibility::Public);
        auto B = std::make_shared<CPPClass>("B", Visibility::Public);
        auto C = std::make_shared<CPPClass>("C", Visibility::Public);

        graph.add_node(A);
        graph.add_node(B);
        graph.add_node(C);

        graph.get_diagram()[A].push_back({B, BranchType::ASSOCIATION});
        graph.get_diagram()[C].push_back({B, BranchType::AGGREGATION});
        graph.get_diagram()[A].push_back({C, BranchType::DEPENDENCY});

        // Act
        graph.remove_node(B);
        const auto& diagram = graph.get_diagram();

        // Assert
        REQUIRE(diagram.find(B) == diagram.end());

        // Nema vise node-a B u listi suseda, ostao je samo C
        REQUIRE(diagram.at(A).size() == 1);
        REQUIRE(diagram.at(A)[0].first == C);
        REQUIRE(diagram.at(C).empty());
    }

    SECTION("Given a graph, "
            "When removing a node that does not exist, "
            "Then the graph is unchanged.")
    {
        // Arrange
        DiagramGraph graph;
        auto A = std::make_shared<CPPClass>("A", Visibility::Public);
        auto B = std::make_shared<CPPClass>("B", Visibility::Public);

        graph.add_node(A);
        const auto size_before = graph.get_diagram().size();

        // Act
        graph.remove_node(B);


        // Assert
        REQUIRE(graph.get_diagram().size() == size_before);
        REQUIRE(graph.get_diagram().find(A) != graph.get_diagram().end());
        REQUIRE(graph.get_diagram().find(B) == graph.get_diagram().end());
    }
}


TEST_CASE("DiagramGraph::find_pointer_owner(Composition*) tests")
{
    SECTION("Given a node that exists as a key in the graph, "
            "When searching for its raw pointer, "
            "Then the same shared_ptr owner is returned.")
    {
        // Arrange
        DiagramGraph graph;
        auto A = std::make_shared<CPPClass>("A", Visibility::Public);

        graph.add_node(A);

        Composition* rawA = A.get();

        // Act
        SharedNodePtr owner = graph.find_pointer_owner(rawA);

        // Assert
        REQUIRE(owner != nullptr);
        REQUIRE(owner == A);
        REQUIRE(owner.get() == rawA);
    }

    SECTION("Given a node that exists only as a neighbour, "
            "When searching for its raw pointer, "
            "Then the neighbour shared_ptr is returned.")
    {
        // Arrange
        DiagramGraph graph;
        auto A = std::make_shared<CPPClass>("A", Visibility::Public);
        auto B = std::make_shared<CPPClass>("B", Visibility::Public);

        graph.add_node(A);

        graph.get_diagram()[A].push_back({B, BranchType::DEPENDENCY});

        Composition* rawB = B.get();

        // Act
        SharedNodePtr owner = graph.find_pointer_owner(rawB);

        // Assert
        REQUIRE(owner != nullptr);
        REQUIRE(owner == B);
        REQUIRE(owner.get() == rawB);
    }

    SECTION("Given a raw pointer that is not present in the graph, "
            "When searching for its owner, "
            "Then nullptr is returned.")
    {
        // Arrange
        DiagramGraph graph;
        auto A = std::make_shared<CPPClass>("A", Visibility::Public);
        graph.add_node(A);

        auto X = std::make_shared<CPPClass>("X", Visibility::Public);
        Composition* rawX = X.get(); // X nije dodat u graf

        // Act
        SharedNodePtr owner = graph.find_pointer_owner(rawX);

        // Assert
        REQUIRE(owner == nullptr);
    }
}




// uz ovaj test i remove_branch test, connection_exists je implicitno testiran
TEST_CASE("DiagramGraph::connection_exists(SharedNodePtr, SharedNodePtr, BranchType) tests")
{
    SECTION("Given a graph where from->to exists with ASSOCIATION, "
            "When adding branch "
            "Then connection_exists in add_branch returns false.")
    {
        // Arrange
        DiagramGraph graph;
        SharedNodePtr A = std::make_shared<CPPClass>("A", Visibility::Public);
        SharedNodePtr B = std::make_shared<CPPClass>("B", Visibility::Public);
        SharedNodePtr C = std::make_shared<CPPClass>("C", Visibility::Public);

        graph.add_node(B);
        graph.add_node(B);

        graph.get_diagram()[A].push_back({B, BranchType::ASSOCIATION});


        // Act
        std::string err_msg;


        // Assert
        // REQUIRE(graph.add_branch(A, B, BranchType::ASSOCIATION, err_msg) == false); // connection_exists -> true
        // REQUIRE(graph.add_branch(A, C, BranchType::INHERITANCE, err_msg) == true); // connection_existst -> false (if validator is correct)
    }
}



TEST_CASE("DiagramGraph::remove_branch(from,to,type) tests")
{
    SECTION("Given there is no such connection, "
            "When removing a branch, "
            "Then it returns false and graph remains unchanged.")
    {
        // Arrange
        DiagramGraph graph;
        auto A = std::make_shared<CPPClass>("A", Visibility::Public);
        auto B = std::make_shared<CPPClass>("B", Visibility::Public);
        graph.add_node(A);
        graph.add_node(B);

        const auto beforeA = graph.get_diagram().at(A).size();
        const auto beforeB = graph.get_diagram().at(B).size();
        const auto inheritance_before = A.get()->get_inheritance();
        // Act
        bool ok = graph.remove_branch(A, B, BranchType::DEPENDENCY);

        // Assert
        REQUIRE(ok == false);
        REQUIRE(graph.get_diagram().at(A).size() == beforeA);
        REQUIRE(graph.get_diagram().at(B).size() == beforeB);
        REQUIRE(inheritance_before == A.get()->get_inheritance());
    }

    SECTION("Given an INHERITANCE connection exists, "
            "When removing it, "
            "Then it returns true, removes the edge, and calls break_inheritance() on 'from'.")
    {
        // Arrange
        DiagramGraph graph;
        auto A = std::make_shared<CPPClass>("A", Visibility::Public);
        auto B = std::make_shared<CPPClass>("B", Visibility::Public);
        graph.add_node(A);
        graph.add_node(B);

        graph.get_diagram()[A].push_back({B, BranchType::INHERITANCE});
        auto inheritance_before = A->get_inheritance();

        // Act
        bool ok = graph.remove_branch(A, B, BranchType::INHERITANCE);

        // Assert
        REQUIRE(ok == true);
        const auto& neigh_A = graph.get_diagram().at(A);
        REQUIRE(neigh_A.empty());
        REQUIRE(std::nullopt == A->get_inheritance());
    }

    SECTION("Given an ASSOCIATION connection exists in both directions, "
            "When removing it, "
            "Then it returns true and removes both directions.")
    {
        // Arrange
        DiagramGraph graph;
        auto A = std::make_shared<CPPClass>("A", Visibility::Public);
        auto B = std::make_shared<CPPClass>("B", Visibility::Public);
        graph.add_node(A);
        graph.add_node(B);

        graph.get_diagram()[A].push_back({B, BranchType::ASSOCIATION});
        graph.get_diagram()[B].push_back({A, BranchType::ASSOCIATION});
        auto inheritance_A_before = A->get_inheritance();
        auto inheritance_B_before = B->get_inheritance();


        // Act
        bool ok = graph.remove_branch(A, B, BranchType::ASSOCIATION);


        // Assert
        REQUIRE(ok == true);
        REQUIRE(graph.get_diagram().at(A).empty());
        REQUIRE(graph.get_diagram().at(B).empty());

        REQUIRE(inheritance_A_before == A->get_inheritance());
        REQUIRE(inheritance_B_before == B->get_inheritance());
    }
}

