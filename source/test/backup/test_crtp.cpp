#include "mip.h"
#include <vector>

//----------------------------------------------------------------------

class BasePlugin {
public:
  virtual ~BasePlugin() {}
  virtual void func() = 0;
};

template <class T>
class Plugin : public BasePlugin {
public:
  void func() {
    static_cast<T*>(this)->func();
  }
};

class myPlugin1 : public Plugin<myPlugin1> {
public:
  void func() {
    printf("myPlugin1.func\n");
  }
};

class myPlugin2 : public Plugin<myPlugin2> {
public:
  void func() {
    printf("myPlugin2.func\n");
  }
};

//----------------------------------------------------------------------

//template <class T>
//class BaseModule {
//public:
//  void base_func()  { }
//  void process()    { static_cast<T*>(this)->process(); }
//};
//
//class Module : public BaseModule<Module> {
//public:
//  void process() {
//    base_func();
//  }
//};
//
//class Module2 : public BaseModule<Module> {
//private:
//  Module mod1 = {};
//public:
//  void process() {
//    mod1.process();
//    base_func();
//  }
//};

//----------------------------------------------------------------------

int main() {
  std::vector<BasePlugin*> plugins;
  plugins.push_back(new myPlugin1());
  plugins.push_back(new myPlugin2());
  for (uint32_t i=0; i<plugins.size(); i++) {
    plugins[i]->func();
  }
  return 0;
}

