#include <catch2/catch_test_macros.hpp>
#include <commandManager/command_manager.h>
#include <commandManager/add_node_command.h>
#include <commandManager/remove_node_command.h>
#include <commandManager/composite_command.h>
#include <model/elements/composition/cpp_class.h>
#include <model/base/visibility.h>
#include <memory>

class MockCommand : public Command {
public:
    int execute_count = 0;
    int undo_count = 0;
    bool execute_return_value = true;
    bool undo_return_value = true;

    bool execute() override { 
        execute_count++; 
        return execute_return_value;
    }
    
    bool undo() override { 
        undo_count++; 
        return undo_return_value;
    }
};

TEST_CASE("CommandManager") {
    CommandManager cm;

    SECTION("initially empty") {
        REQUIRE(cm.can_undo() == false);
        REQUIRE(cm.can_redo() == false);
    }

    SECTION("execute") {
        auto cmd = std::make_shared<MockCommand>();
        cm.execute(cmd);

        REQUIRE(cmd->execute_count == 1);
        REQUIRE(cm.can_undo() == true);
        REQUIRE(cm.can_redo() == false);
    }

    SECTION("undo") {
        auto cmd = std::make_shared<MockCommand>();
        cm.execute(cmd);
        cm.undo();

        REQUIRE(cmd->undo_count == 1);
        REQUIRE(cm.can_undo() == false);
        REQUIRE(cm.can_redo() == true);
    }

    SECTION("redo") {
        auto cmd = std::make_shared<MockCommand>();
        cm.execute(cmd);
        cm.undo();
        cm.redo();

        REQUIRE(cmd->execute_count == 2);
        REQUIRE(cm.can_undo() == true);
        REQUIRE(cm.can_redo() == false);
    }

    SECTION("execute after undo clears redo stack") {
        auto cmd1 = std::make_shared<MockCommand>();
        auto cmd2 = std::make_shared<MockCommand>();
        cm.execute(cmd1);
        cm.undo();
        REQUIRE(cm.can_redo() == true);

        cm.execute(cmd2);
        REQUIRE(cm.can_redo() == false);
    }

    SECTION("undo on empty stack is a no-op") {
        cm.undo();
        REQUIRE(cm.can_undo() == false);
        REQUIRE(cm.can_redo() == false);
    }

    SECTION("redo on empty stack is a no-op") {
        cm.redo();
        REQUIRE(cm.can_undo() == false);
        REQUIRE(cm.can_redo() == false);
    }

    SECTION("execute with nullptr is a no-op") {
        cm.execute(nullptr);
        REQUIRE(cm.can_undo() == false);
    }

    SECTION("execute returns false does not add to stack") {
        auto cmd = std::make_shared<MockCommand>();
        cmd->execute_return_value = false;
        cm.execute(cmd);

        REQUIRE(cmd->execute_count == 1);
        REQUIRE(cm.can_undo() == false);
    }
}

TEST_CASE("AddNodeCommand") {
    DiagramGraph graph;
    auto node = std::make_shared<CPPClass>("A", Visibility::Public);

    SECTION("execute adds node to graph") {
        AddNodeCommand cmd(&graph, node);
        bool result = cmd.execute();

        REQUIRE(result == true);

        auto& diagram = graph.get_diagram();
        REQUIRE(diagram.size() == 1);
        REQUIRE(diagram.find(node) != diagram.end());
    }

    SECTION("undo removes node from graph") {
        AddNodeCommand cmd(&graph, node);
        cmd.execute();
        cmd.undo();

        REQUIRE(graph.get_diagram().find(node) == graph.get_diagram().end());
    }

    SECTION("double execute is idempotent") {
        AddNodeCommand cmd(&graph, node);
        cmd.execute();
        cmd.execute();

        REQUIRE(graph.get_diagram().size() == 1);
    }
}

TEST_CASE("RemoveNodeCommand") {
    DiagramGraph graph;
    auto node = std::make_shared<CPPClass>("A", Visibility::Public);
    graph.add_node(node);

    SECTION("execute removes node from graph") {
        RemoveNodeCommand cmd(&graph, node);
        bool result = cmd.execute();

        REQUIRE(result == true);

        REQUIRE(graph.get_diagram().find(node) == graph.get_diagram().end());
    }

    SECTION("undo restores node to graph") {
        RemoveNodeCommand cmd(&graph, node);
        cmd.execute();
        cmd.undo();

        auto& diagram = graph.get_diagram();
        REQUIRE(diagram.size() == 1);
        REQUIRE(diagram.find(node) != diagram.end());
    }

    SECTION("double execute is idempotent") {
        RemoveNodeCommand cmd(&graph, node);
        cmd.execute();
        cmd.execute();

        REQUIRE(graph.get_diagram().empty());
    }
}

TEST_CASE("CompositeCommand") {
    SECTION("empty composite command") {
        CompositeCommand composite;
        
        REQUIRE(composite.execute() == false);
        REQUIRE(composite.undo() == false);
    }

    SECTION("execute all commands in order") {
        CompositeCommand composite;
        auto cmd1 = std::make_shared<MockCommand>();
        auto cmd2 = std::make_shared<MockCommand>();
        auto cmd3 = std::make_shared<MockCommand>();

        composite.add_command(cmd1);
        composite.add_command(cmd2);
        composite.add_command(cmd3);

        bool result = composite.execute();

        REQUIRE(result == true);
        REQUIRE(cmd1->execute_count == 1);
        REQUIRE(cmd2->execute_count == 1);
        REQUIRE(cmd3->execute_count == 1);
    }

    SECTION("undo all commands in reverse order") {
        CompositeCommand composite;
        auto cmd1 = std::make_shared<MockCommand>();
        auto cmd2 = std::make_shared<MockCommand>();
        auto cmd3 = std::make_shared<MockCommand>();

        composite.add_command(cmd1);
        composite.add_command(cmd2);
        composite.add_command(cmd3);

        composite.execute();
        bool result = composite.undo();

        REQUIRE(result == true);
        REQUIRE(cmd1->undo_count == 1);
        REQUIRE(cmd2->undo_count == 1);
        REQUIRE(cmd3->undo_count == 1);
    }

    SECTION("add nullptr command is ignored") {
        CompositeCommand composite;
        composite.add_command(nullptr);
        
        REQUIRE(composite.execute() == false);
    }

    SECTION("execute with graph operations") {
        DiagramGraph graph;
        auto node1 = std::make_shared<CPPClass>("A", Visibility::Public);
        auto node2 = std::make_shared<CPPClass>("B", Visibility::Public);

        CompositeCommand composite;
        composite.add_command(std::make_shared<AddNodeCommand>(&graph, node1));
        composite.add_command(std::make_shared<AddNodeCommand>(&graph, node2));

        bool result = composite.execute();

        REQUIRE(result == true);
        REQUIRE(graph.get_diagram().size() == 2);
        REQUIRE(graph.get_diagram().find(node1) != graph.get_diagram().end());
        REQUIRE(graph.get_diagram().find(node2) != graph.get_diagram().end());
    }

    SECTION("undo with graph operations") {
        DiagramGraph graph;
        auto node1 = std::make_shared<CPPClass>("A", Visibility::Public);
        auto node2 = std::make_shared<CPPClass>("B", Visibility::Public);

        CompositeCommand composite;
        composite.add_command(std::make_shared<AddNodeCommand>(&graph, node1));
        composite.add_command(std::make_shared<AddNodeCommand>(&graph, node2));

        composite.execute();
        bool result = composite.undo();

        REQUIRE(result == true);
        REQUIRE(graph.get_diagram().empty());
    }

    SECTION("composite with CommandManager") {
        CommandManager cm;
        DiagramGraph graph;
        auto node1 = std::make_shared<CPPClass>("A", Visibility::Public);
        auto node2 = std::make_shared<CPPClass>("B", Visibility::Public);

        auto composite = std::make_shared<CompositeCommand>();
        composite->add_command(std::make_shared<AddNodeCommand>(&graph, node1));
        composite->add_command(std::make_shared<AddNodeCommand>(&graph, node2));

        cm.execute(composite);

        REQUIRE(graph.get_diagram().size() == 2);
        REQUIRE(cm.can_undo() == true);

        cm.undo();
        REQUIRE(graph.get_diagram().empty());
        REQUIRE(cm.can_redo() == true);

        cm.redo();
        REQUIRE(graph.get_diagram().size() == 2);
    }

    SECTION("execute returns false if any command fails") {
        CompositeCommand composite;
        auto cmd1 = std::make_shared<MockCommand>();
        auto cmd2 = std::make_shared<MockCommand>();
        auto cmd3 = std::make_shared<MockCommand>();

        cmd2->execute_return_value = false;

        composite.add_command(cmd1);
        composite.add_command(cmd2);
        composite.add_command(cmd3);

        bool result = composite.execute();

        REQUIRE(result == false);
        REQUIRE(cmd1->execute_count == 1);
        REQUIRE(cmd2->execute_count == 1);
        REQUIRE(cmd3->execute_count == 1);
    }

    SECTION("undo returns false if any command undo fails") {
        CompositeCommand composite;
        auto cmd1 = std::make_shared<MockCommand>();
        auto cmd2 = std::make_shared<MockCommand>();
        auto cmd3 = std::make_shared<MockCommand>();

        cmd2->undo_return_value = false;

        composite.add_command(cmd1);
        composite.add_command(cmd2);
        composite.add_command(cmd3);

        composite.execute();
        bool result = composite.undo();

        REQUIRE(result == false);
        REQUIRE(cmd1->undo_count == 1);
        REQUIRE(cmd2->undo_count == 1);
        REQUIRE(cmd3->undo_count == 1);
    }
}
