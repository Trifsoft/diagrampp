#include "commandManager/composite_command.h"

void CompositeCommand::add_command(const std::shared_ptr<Command>& command)
{
    if (command) {
        m_commands.append(command);
    }
}

bool CompositeCommand::execute()
{
    bool all_succeeded = true;
    for (const auto& command : m_commands) {
        if (command) {
            if (!command->execute()) {
                all_succeeded = false;
            }
        }
    }
    return all_succeeded && !m_commands.isEmpty();
}

bool CompositeCommand::undo()
{
    bool all_succeeded = true;
    for (int i = m_commands.size() - 1; i >= 0; --i) {
        auto& command = m_commands[i];
        if (command) {
            if (!command->undo()) {
                all_succeeded = false;
            }
        }
    }
    return all_succeeded && !m_commands.isEmpty();
}
