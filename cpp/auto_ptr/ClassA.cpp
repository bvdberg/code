#include <stdio.h>

#include "ClassA.h"
#include "ClassB.h"

ClassA::ClassA()
//    : pb( new ClassB() )
{
    printf("constructor ClassA\n");
    pb.reset(new ClassB());

    if(pb.get() == 0) printf("NULL\n");     //use get() to get pointer itself
    else printf("NOT NULL\n");
}


ClassA::~ClassA()
{
    printf("destructor ClassA\n");
}


void ClassA::init()
{
    printf("init()\n");
    pb.reset(new ClassB());
}


