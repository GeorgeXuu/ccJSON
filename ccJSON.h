#ifndef CCJSON_H__
#define CCJSON_H__

// Commonly used headers.
#include <stdio.h>
#include <utime.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include <stdarg.h>
#include <errno.h>
#include <signal.h>
#include <netdb.h>
#include <locale.h>
#include <dirent.h>
#include <termios.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stddef.h>

#include <sys/syscall.h>

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <list>
#include <vector>
#include <deque>
#include <algorithm>
////////
// C++ headers 
#include <unordered_map>
#include <memory>
#include <bitset>
#include <forward_list>
#include <iterator>
#include <stack>
#include <map>
#include <set>
#include <sys/param.h>
#include <thread>
// 
///////////////////////////////////////////////

namespace ccJSON
{
  // See JSON documentation: There are seven types in JSON.
  enum Type { Null, False, True, Number, String, Array, Object };

  // Use a Tree-like data structure to represent each node in JSON.
  class Value
  {
  public:
    Type type;
    double number;
    inline ccJSON::Type get_type() const
    {
      return type;
    }
    std::string String;

  };

  int parse(ccJSON::Value &value, const char *jsontext);

  // overloading parse to pass C++ style string 
  int parse(ccJSON::Value &value, const std::string &jsontext);

  double get_number(const ccJSON::Value &value);
  std::string get_string(const ccJSON::Value &value);
  
  // return value of parse 
  enum 
  { 
    // parse_null_input = -1,
    parse_ok = 0, parse_expect_value, 
    parse_invalid_value, parse_root_not_singular,
    parse_number_too_big,
    parse_invalid_string
  };

  // ccJSON::Type get_type(const Value *value); 
  // Ref is better.
  // inline ccJSON::Type get_type(const Value &value)

};








#endif  // CCJSON_H__ 
