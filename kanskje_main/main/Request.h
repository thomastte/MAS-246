#ifndef REQUEST_H
#define REQUEST_H

enum class Dir { Up, Down };

class Request 
{
public:
    Request(int floor, Dir dir);
    int floor() const;
    Dir direction() const;

private:
    int floor_;
    Dir dir_;
};

#endif
