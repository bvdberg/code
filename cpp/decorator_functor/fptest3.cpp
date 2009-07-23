#include <stdio.h>
#include <string>

using namespace std;

class MudObject {
public:
	virtual void print() { printf("-> %s\n", __PRETTY_FUNCTION__); }
	void foo() { printf("-> %s\n", __PRETTY_FUNCTION__); }
};


class MudItem : public MudObject {
public:
	MudItem(int i) : num(i) {}
	virtual void print() { printf("-> %s (%d)\n", __PRETTY_FUNCTION__, num); }
	void get() { printf("-> %s (%d)\n", __PRETTY_FUNCTION__, num); num*=2; }
private:
	int num;
};


class MudCreature : public MudObject {
public:
	MudCreature(string n) : name(n) {}
	virtual void print() { printf("-> %s (%s)\n", __PRETTY_FUNCTION__, name.c_str()); }
	void fight() { printf("-> %s (%s)\n", __PRETTY_FUNCTION__, name.c_str()); }
private:
	string name;
};


class MudObjectFactory {
public:
    virtual ~MudObjectFactory() {}
    virtual MudObject& create() const = 0;
};

template <typename T>
class CustomObjectFactory : public MudObjectFactory {
public:
    CustomObjectFactory(const T& t) : prototype(t), function(0) {}
    virtual MudObject& create() const {
		T* t = new T(prototype);
		if (function) function(t);
		return *t;
	}

	typedef void (*Function)(T*);
	void addFunction(Function f_) {
		function=f_;
		printf("-> %s\n", __PRETTY_FUNCTION__);
	}
private:
    T prototype;
	Function function;
};


void function1(MudObject* o) { o->foo(); }

void function2(MudItem* i) { i->get(); }

void function3(MudCreature* c) { c->fight(); }


int main() {
	printf("item:\n");
	CustomObjectFactory<MudItem> itemFactory(MudItem(10));
	itemFactory.addFunction(function2);

	MudObject& obj = itemFactory.create();
	obj.print();

	printf("creature:\n");
	CustomObjectFactory<MudCreature> creatureFactory(MudCreature("tiger"));
	creatureFactory.addFunction(function3);

	MudObject& obj2 = creatureFactory.create();
	obj2.print();

	printf("object:\n");
	MudObject object;
	CustomObjectFactory<MudObject> objectFactory(object);
	objectFactory.addFunction(function1);

	MudObject& obj3 = objectFactory.create();
	obj3.print();


	return 0;
}

