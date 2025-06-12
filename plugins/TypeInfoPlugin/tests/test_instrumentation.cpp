#include <iostream>
#include <string>

class Base {
public:
    virtual void method1() {
        std::cout << "Base::method1()" << std::endl;
    }
    
    virtual void method2() = 0;
    
    virtual ~Base() {
        std::cout << "Base destructor" << std::endl;
    }
};

class Derived : public Base {
private:
    int value;
    
public:
    Derived(int val) : value(val) {
        std::cout << "Derived constructor with value: " << value << std::endl;
    }
    
    void method1() override {
        std::cout << "Derived::method1() with value: " << value << std::endl;
    }
    
    void method2() override {
        std::cout << "Derived::method2() with value: " << value << std::endl;
    }
    
    int getValue() const {
        return value;
    }
    
    ~Derived() override {
        std::cout << "Derived destructor with value: " << value << std::endl;
    }
};

template <typename T>
class Container {
private:
    T data;
    
public:
    Container(T val) : data(val) {}
    
    T getData() const {
        return data;
    }
    
    void setData(T val) {
        data = val;
    }
};

class Outer {
public:
    class Inner {
    private:
        int innerValue;
        
    public:
        Inner(int val) : innerValue(val) {}
        
        int getInnerValue() const {
            return innerValue;
        }
    };
    
    Inner createInner(int val) {
        return Inner(val);
    }
};

int main() {
    Derived d1(42);
    Derived d2(100);
    
    d1.method1();
    d1.method2();
    
    Container<int> intContainer(123);
    Container<std::string> strContainer("Hello");
    
    Outer outer;
    Outer::Inner inner = outer.createInner(99);
    
    return 0;
}
