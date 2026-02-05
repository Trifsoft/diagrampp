#ifndef COMPOSITE_COMMAND_H
#define COMPOSITE_COMMAND_H

#include "command.h"
#include <QVector>
#include <memory>

class CompositeCommand : public Command
{
public:
    CompositeCommand() = default;

    void add_command(const std::shared_ptr<Command>& command);

    bool execute() override;
    bool undo() override;

private:
    QVector<std::shared_ptr<Command>> m_commands;
};

#endif // COMPOSITE_COMMAND_H
