#ifndef BAR_H
#define BAR_H

class Foo;

class Bar {
public:
    void add(Foo f);
private:
    void print(Foo f);  // no need for full include here
    //Foo m_f;        // if full member, need to include Foo.h
};

#endif

