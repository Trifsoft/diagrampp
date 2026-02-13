#ifndef CONNECTION_H
#define CONNECTION_H

#include <view/arrow.h>
#include <view/line.h>

class Connection: public QObject {
    Q_OBJECT
    Arrow* arrow;
    Line* line;
public:
    Connection(Arrow*, Line*);
    ~Connection();
};
#endif
