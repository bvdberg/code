#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <string>

#include "OwningVector.h"

using namespace std;

/*
    - easy to transfer ownership
    - TODO make template class
*/

class Statement {
public:
    Statement(const char* name_) : name(name_) {
        printf("  creating Statement %s\n", name_);
    }
    ~Statement() {
        printf("  deleting Statement %s\n", name.c_str());
    }
    std::string name;
private:
    Statement(const Statement& rhs);
    Statement& operator= (const Statement& rhs);
};


typedef OwningVector<Statement> StmtList;

// no ownership change
void no_ownership(StmtList& v) {
    printf("no ownership() %p\n", &v);
}

void pass_ownership(StmtList v) {
    printf("pass ownership() %p\n", &v);
}

class Keeper {
public:
    Keeper(StmtList& v_) : v(v_) {}
private:
    StmtList v;
};

int main(int argc, const char *argv[])
{
    StmtList stmts;    
    stmts.push_back(new Statement("break"));
    stmts.push_back(new Statement("for"));
    stmts.push_back(new Statement("printf"));
    stmts.push_back(new Statement("while"));

    no_ownership(stmts);
    pass_ownership(stmts);

    Keeper* k;
    {
        StmtList s2;    
        s2.push_back(new Statement("11"));
        s2.push_back(new Statement("22"));
        s2.push_back(new Statement("33"));
        k = new Keeper(s2);
        printf("done\n");
    }
    delete k;


    return 0;
}

