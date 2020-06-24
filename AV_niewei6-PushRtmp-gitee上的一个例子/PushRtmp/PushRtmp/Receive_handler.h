#pragma once

class Receive_handler
{
public:
    Receive_handler(void){}
public:
    virtual ~Receive_handler(void){}
public:
        virtual bool receive_handler(const char *data,int size) = 0;
};
