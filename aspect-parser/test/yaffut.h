// Copyright 2006 Rutger E.W. van Beusekom.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef __YAFFUT_H__
#define __YAFFUT_H__

#include <cxxabi.h>

#include <cmath>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <sys/time.h>

#define ANSI_BLACK "\033[22;30m"
#define ANSI_RED "\033[22;31m"
#define ANSI_GREEN "\033[22;32m"
#define ANSI_BROWN "\033[22;33m"
#define ANSI_BLUE "\033[22;34m"
#define ANSI_MAGENTA "\033[22;35m"
#define ANSI_CYAN "\033[22;36m"
#define ANSI_GREY "\033[22;37m"
#define ANSI_DARKGREY "\033[01;30m"
#define ANSI_BRED "\033[01;31m"
#define ANSI_BGREEN "\033[01;32m"
#define ANSI_YELLOW "\033[01;33m"
#define ANSI_BBLUE "\033[01;34m"
#define ANSI_BMAGENTA "\033[01;35m"
#define ANSI_BCYAN "\033[01;36m"
#define ANSI_WHITE "\033[01;37m"
#define ANSI_NORMAL "\033[22;37m"
#define COLOR_AT     ANSI_YELLOW
#define COLOR_OK     ANSI_GREEN
#define COLOR_FAIL   ANSI_BRED
#define COLOR_NORMAL ANSI_NORMAL

#define YAFFUT_STRINGIZE(x) YAFFUT_STRINGIZE_(x)
#define YAFFUT_STRINGIZE_(x) #x

#define __YAFFUT_AT__ COLOR_AT __FILE__ ":" YAFFUT_STRINGIZE(__LINE__)": "COLOR_NORMAL
#ifndef __AT__
#define __AT__ __YAFFUT_AT__
#endif

#define YAFFUT_EQUAL(e,a) \
  yaffut::equal (e ,a , __YAFFUT_AT__, COLOR_FAIL"EQUAL(" #e " == " #a ") failed "COLOR_NORMAL)
#ifndef EQUAL
#define EQUAL YAFFUT_EQUAL
#endif

#define YAFFUT_UNEQUAL(e,a) \
  yaffut::unequal (e, a, __YAFFUT_AT__, COLOR_FAIL"UNEQUAL(" #e " != " #a ") failed "COLOR_NORMAL)
#ifndef UNEQUAL
#define UNEQUAL YAFFUT_UNEQUAL
#endif

#define YAFFUT_CHECK(e) \
  yaffut::check (e, __YAFFUT_AT__, COLOR_FAIL"CHECK(" #e ") failed "COLOR_NORMAL)
#ifndef CHECK
#define CHECK YAFFUT_CHECK
#endif

#define YAFFUT_FAIL(s) yaffut::fail (s, __YAFFUT_AT__);
#ifndef FAIL
#define FAIL YAFFUT_FAIL
#endif

#define YAFFUT_ASSERT_THROW(s, e) \
  try \
  { \
    s; \
    throw yaffut::failure (__YAFFUT_AT__,  #s COLOR_RED" failed to throw"ANSI_NORMAL); \
  } \
  catch(const e&){}
#ifndef ASSERT_THROW
#define ASSERT_THROW YAFFUT_ASSERT_THROW
#endif

namespace yaffut {

template <typename T>
std::string demangle()
{
  size_t sz;
  int status;
  char* ptr = abi::__cxa_demangle(typeid(T).name(), 0, &sz, &status);
  std::string name(ptr ? ptr : "", ptr ? strlen(ptr) : 0);
  if(ptr){ free(ptr); }
  std::string::size_type pos = name.rfind("::");
  if(pos != std::string::npos)
  {
    name = name.substr(pos + 2);
  }  
  return name;
}

struct ITest
{
  virtual ~ITest(){}
};

class Factory
{
public:
  typedef ITest* (*Create_t) ();
private:
  typedef std::map<std::string, Create_t> Tests_t;
  Tests_t m_Tests;
  size_t m_fail;
  size_t m_pass;
  u_int64_t m_startTime;
  u_int64_t m_endTime;
private:
  Factory(){}
  ~Factory(){}
  static bool EqualsSuiteName (std::string const &name, std::string const& s)
  {
    return name.find (':') >= name.length () - 2
      && s.substr (0, name.length ()) == name;
  }
  u_int64_t getCurrentTime() {
    struct timeval now;
    gettimeofday(&now, 0);
    u_int64_t now64 = now.tv_sec * 1000000 + now.tv_usec;
    return now64;
  }
public:
  static Factory& Instance()
  {
    static Factory instance;
    return instance;
  }
  void Register(const std::string& name, Create_t create)
  {
    m_Tests[name] = create;
  }
  size_t Fail () { return m_fail; }
  void List(const std::string& name)
  {
    for(Tests_t::const_iterator it = m_Tests.begin(); it != m_Tests.end(); ++it)
    {
      if(name.empty () || it->first == name
	 || EqualsSuiteName (name, it->first))
	std::cout << it->first << std::endl;
    }
  }
  void Run(const std::string& name)
  {
    for(Tests_t::const_iterator it = m_Tests.begin(); it != m_Tests.end(); ++it)
    {
      if("All" == name || it->first == name || EqualsSuiteName (name, it->first))
      {
        try
        {
          std::cout << std::endl << it->first << ' ' << std::flush;
          {
            std::auto_ptr<ITest> test(it->second());
          }
          std::cout << "[OK]" << std::flush;
          ++m_pass;
        }
        catch(const std::exception& e)
        {
          std::cout << COLOR_FAIL"[FAIL]\n"COLOR_NORMAL << e.what() << std::flush;
          ++m_fail;
        }
        catch(...)
        {
          std::cout << COLOR_FAIL"[FAIL] Unknown exception"COLOR_NORMAL << std::flush;
          ++m_fail;
        }
      }
    }
  }
  void Report ()
  {
    const size_t size = m_Tests.size();
    std::cout << std::endl;
    std::cout << "[TOTAL](" << m_pass + m_fail << '/' << size << ")" << std::endl;
    if (!m_fail) std::cout << COLOR_OK;
    std::cout << "[OK](" << m_pass << '/' << size << ")" << std::endl;
    if (!m_fail) std::cout << COLOR_NORMAL;
    if (m_fail)
      std::cout << COLOR_FAIL"[FAIL](" << m_fail << '/' << size << ")"COLOR_NORMAL << std::endl;
    u_int64_t diff = (m_endTime - m_startTime) / 1000;
    std::cout << "Elapsed time: " << diff << " ms." << std::endl;
  }
  int Main (int argc, const char* argv[])
  {
    if(argc > 1
       && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help"))
    {
      std::cout << "Yaffut - Yet Another Framework For Unit Testing.\n\n"
	"Usage: yaffut [OPTION] [Suite:|Suite::Test]...\n\n"
	"Options:\n"
	"  -h, --help  show this help\n"
	"  -l, --list  list test cases" << std::endl;
      return 0;
    }
    if(argc > 1
       && (std::string(argv[1]) == "-l" || std::string(argv[1]) == "--list"))
    {
      Factory::Instance().List(argc > 2 ? argv[2] : "");
      return 0;
    }

    const char* all[] = {"All"};
    const char** test = all;
    int num = 1;
    if(1 < argc)
    {
      test = argv;
      num = argc;
    }
   
    m_startTime = getCurrentTime(); 
    for(int i = 0; i < num; ++i)
    {
      try
      {
	    Factory::Instance().Run(test[i]);
      }
      catch(const std::exception& e)
      {
	    std::clog << e.what() << std::endl;
      }
    }
    m_endTime = getCurrentTime(); 

    Factory::Instance().Report ();
    return Factory::Instance().Fail ();
  }
};

class failure: public std::exception
{
  std::string failure_;
public:
  template <typename Expected, typename Actual>
  failure(const Expected& e, Actual& a, const char* at = "", const char* expr = "")
  {
    std::ostringstream os;
    os << at << expr << "\nexpected: "
       << "(" << demangle<Expected>() << ") " << e
       << " != actual: " << "(" << demangle<Actual>() << ") " << a;
    failure_ = os.str();
  }
  failure(const char* at = "", const char* expr = "")
  {
    std::ostringstream os;
    os << at << expr;
    failure_ = os.str();
  }
  virtual ~failure() throw() {}
  virtual const char* what() const throw() { return failure_.c_str(); }
};

template <typename Suite, typename Case>
struct Registrator
{
  Registrator()
  {
    Factory::Instance().Register(TestName(), Create);
  }
  const std::string& TestName()
  {
    static const std::string name(demangle<Suite>() + "::" + demangle<Case>());
    return name;
  }
  static ITest* Create()
  {
    return new Case;
  }
};

template <typename Suite, typename Case>
struct Test: public ITest, public Suite
{
  static Registrator<Suite, Case> s_Registrator;
  Test()
  : Suite()
  {
    Registrator<Suite, Case>* r = &s_Registrator;
    r = 0;
  }
  template <typename E, typename T>
  void assert_throw(void(T::*mf)(), const char* at)
  {
    try
    {
      (dynamic_cast<T*> (this)->*mf)();
      throw yaffut::failure (at, "statement failed to throw");
    }
    catch(const E&){}
  }
};

template <typename Suite, typename Case>
Registrator<Suite, Case> Test<Suite, Case>::s_Registrator;

template <typename Expected, typename Actual>
void equal(const Expected& e, const Actual& a, const char* at = "", const char* expr = "")
{
  if(e != a)
  {
    throw failure(e, a, at, expr);
  }
}
inline void equal(double e, double a, const char* at = "", const char* expr = "")
{
  double max = std::abs(std::max(e, a));
  max = max < 1.0 ? 1.0 : max;
  if(std::abs(e - a) > std::numeric_limits<double>::epsilon() * max)
  {
    throw failure(e, a, at, expr);
  }
}
inline void check(bool b, const char* at = "", const char* expr = "")
{ 
  if(!b)
  {
    throw failure(at, expr);
  }
}

template <typename Expected, typename Actual>
void unequal(const Expected& e, const Actual& a, const char* at = "", const char* expr = "")
{
  if(e == a)
  {
    throw failure(e, a, at, expr);
  }
}
inline void unequal(double e, double a, const char* at = "", const char* expr = "")
{
  double max = std::abs(std::max(e, a));
  max = max < 1.0 ? 1.0 : max;
  if(std::abs(e - a) <= std::numeric_limits<double>::epsilon() * max)
  {
    throw failure(e, a, at, expr);
  }
}

template <typename T>
void fail(const T& expr, const char* at = "")
{
  std::ostringstream os;
  os << expr;
  throw failure(at, os.str().c_str());
}

template <typename E>
void assert_throw(void(*pf)(), const char* at = "")
{
  try
  {
    (*pf)();
    throw failure (at, " statement failed to throw");
  }
  catch(const E&){}
}

//define catch-all suite
struct Suite {};

}

//and for those who prefer macro obscurity over more typing
#define TEST(Suite, Case)\
  namespace { struct Case: public yaffut::Test<Suite, Case>{ Case(); }; } \
  template struct yaffut::Test<Suite, Case>; Case::Case()

#define FUNC(Case)\
  namespace { struct Case: public yaffut::Test<yaffut::Suite, Case>{ Case(); }; } \
  template struct yaffut::Test<yaffut::Suite, Case>; Case::Case()

#ifdef YAFFUT_MAIN

#include <iostream>

int main(int argc, const char* argv[])
{
  std::cout << "pid(" << getpid() << ")" << std::endl;
  return yaffut::Factory::Instance().Main (argc, argv);
};

#endif /* YAFFUT_MAIN */

#define yaffut_main(argc, argv) yaffut::Factory::Instance().Main (argc, argv)

#endif
