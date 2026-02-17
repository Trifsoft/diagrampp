#ifndef CONNECTION_H
#define CONNECTION_H

#include <view/arrow.h>
#include <view/line.h>

class Connection: public QObject {
    Q_OBJECT
public:
    Arrow* arrow;
    Line* line;
    Connection(Arrow*, Line*);
    ~Connection();

    BranchType branchType() const;
};
#endif
