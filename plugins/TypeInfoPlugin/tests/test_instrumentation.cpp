#include <iostream>
#include <string>

extern "C" void instrument_start(const char* functionName) {
  std::cout << "---> ENTER: " << functionName << std::endl;
}

extern "C" void instrument_end(const char* functionName) {
  std::cout << "<--- EXIT:  " << functionName << std::endl;
}

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
    
    ~Derived() {
        std::cout << "Derived destructor with value: " << value << std::endl;
    }
};

int main() {
    std::cout << "--- Program Start ---" << std::endl;
    Derived d(42);
    Base* b = &d;
    
    b->method1();
    b->method2();
    
    std::cout << "Value from getValue(): " << d.getValue() << std::endl;
    std::cout << "--- Program End ---" << std::endl;
    return 0;
}
