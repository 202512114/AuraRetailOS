#pragma once
namespace aura {
class Command {
public:
    virtual ~Command() {}
    virtual void execute() = 0;
};
} // namespace aura
