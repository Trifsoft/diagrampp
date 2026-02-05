#include "commandManager/composite_command.h"

void CompositeCommand::add_command(const std::shared_ptr<Command>& command)
{
    if (command) {
        m_commands.append(command);
    }
}

void CompositeCommand::execute()
{
    for (const auto& command : m_commands) {
        if (command) {
            command->execute();
        }
    }
}

void CompositeCommand::undo()
{
    for (int i = m_commands.size() - 1; i >= 0; --i) {
        auto& command = m_commands[i];
        if (command) {
            command->undo();
        }
    }
}
