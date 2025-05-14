
struct Human {
  unsigned age;
  unsigned height;
  virtual void sleep() = 0;
  virtual void eat() = 0;
};


struct Engineer : Human {
  unsigned salary;
  void sleep() override {}
  void eat() override {}
  void work() {}
};


class Vehicle {
private:
  int year;
  double price;
  char model[50];
  
public:
  Vehicle(int y, double p) : year(y), price(p) {}
  
  virtual void start() {}
  virtual void stop() {}
  
  int getYear() const { return year; }
  double getPrice() const { return price; }
};


class Car : public Vehicle {
private:
  int doors;
  
public:
  Car(int y, double p, int d) : Vehicle(y, p), doors(d) {}
  
  void start() override {}
  void stop() override {}
  
  int getDoors() const { return doors; }
};
