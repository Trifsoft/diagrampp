#include <view/connection.h>

Connection::Connection(Arrow* arrow, Line* line)
    : arrow(arrow)
    , line(line)
{
    arrow->setParent(this);
    line->setParent(this);
}

Connection::~Connection() {}