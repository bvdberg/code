#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <string>

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


template <typename T>
class OwningVector {
public:
    OwningVector(int initial = 32)
        : m_capacity(initial)
        , m_size(0)
    {
        printf("creating vector %p\n", this);
        m_data = (T**)malloc(sizeof(T*)*m_capacity);
    }
    OwningVector(OwningVector& rhs)
        : m_capacity(rhs.m_capacity)
        , m_size(rhs.m_size)
        , m_data(rhs.m_data)
    {
        fprintf(stderr, "COPY\n");
        rhs.m_capacity = 0;
        rhs.m_size = 0;
        rhs.m_data = 0;
        
    }
    ~OwningVector () {
        printf("deleting vector %p  size=%d\n", this, m_size);
        if (m_data) {
            for (int i=0; i<m_size; i++) delete m_data[i];
            free(m_data);
        }
    }
    T* operator[] (int index) {
        assert(index < m_size);
        return m_data[index];
    }
    void push_back(T* s) {
        if (m_size == m_capacity) {
            if (m_capacity == 0) m_capacity = 4;
            else m_capacity *= 2;
            T** data2 = (T**)malloc(sizeof(T*)*m_capacity);
            memcpy(data2, m_data, m_size*sizeof(T*));
            // data = data2
        }
        m_data[m_size] = s;
        m_size++;
    }
    bool empty() const { return m_size == 0; }
    int size() const { return m_size; }
private:
    int m_capacity;
    int m_size;
    T** m_data;
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

