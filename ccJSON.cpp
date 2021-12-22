// Author: George Xu
// Date: 2021/12/21
#include "ccJSON.h"
#include <assert.h>
#define EXPECT(c, ch) do { assert(*c->json == (ch)); c->json++; } while(0)
#define ISDIGIT(ch)      ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch)  ((ch) >= '1' && (ch) <= '9')


struct Context
{
  const char *jsontext;
};


// Helper Function, not to exposed to user
inline void parse_white_space(struct Context &context);
inline int parse_value(struct Context &context, ccJSON::Value &value);
inline int parse_true(struct Context &context, ccJSON::Value &value);
inline int parse_false(struct Context &context, ccJSON::Value &value);
inline int parse_null(struct Context &context, ccJSON::Value &value);
int parse_string(struct Context &context, ccJSON::Value &value);
int parse_number(Context &context, ccJSON::Value &value);
void DeleteRChar(char *str, const char chr);
void DeleteLChar(char *str, const char chr);
void DeleteLRChar(char *str, const char chr);
//////

int ccJSON::parse(ccJSON::Value &value, const char *jsontext)
{
  struct Context context;
  context.jsontext = jsontext;
  int ret = -1;
  value.type = Null;
  parse_white_space(context);
  value.type = ccJSON::Null;
  parse_white_space(context);
  if((ret = parse_value(context, value)) == ccJSON::parse_ok)
  {
    parse_white_space(context);
    if(context.jsontext != '\0')
    {
      ret = ccJSON::parse_root_not_singular; 
    }
  }
  return ret;
}

inline void parse_white_space(struct Context &context)
{
  const char *dest = context.jsontext;
  while(*dest == ' ' || *dest == '\n' || *dest == '\t' || *dest == '\r')
  {
    dest++;
  }
  context.jsontext = dest;
  return ;
}

inline int parse_value(struct Context &context, ccJSON::Value &value)
{
  switch(*(context.jsontext))
  {
    /* Depend which word to parse by the first letter of the word */
    case 't':  return parse_true(context, value); 
    case 'f':  return parse_false(context, value); 
    case 'n':  return parse_null(context, value); 
    default:   return parse_number(context, value);
    case '\"': return parse_string(context, value);
    case '\0': return ccJSON::parse_expect_value;
  }
}

inline int parse_true(struct Context &context, ccJSON::Value &value)
{
  // does not parse 'true' successfully 
  if(context.jsontext[0] != 't' || context.jsontext[1] != 'r' || 
     context.jsontext[2] != 'u' || context.jsontext[3] != 'e'  )
  {
    return ccJSON::parse_invalid_value; 
  }
  else
  {
    context.jsontext += 4; // ptr offset as length(true) == 4
    value.type = ccJSON::True;
    return ccJSON::parse_ok;
  }
}

inline int parse_false(struct Context &context, ccJSON::Value &value)
{
  // does not parse 'false' successfully 
  if(context.jsontext[0] != 'f' ||
     context.jsontext[1] != 'a' ||
     context.jsontext[2] != 'l' ||
     context.jsontext[3] != 's' ||
     context.jsontext[4] != 'e'   )
  {
    return ccJSON::parse_invalid_value;
  }
  else
  {
    context.jsontext += 5; // ptr offset as length(false) == 5
    value.type = ccJSON::False;
    return ccJSON::parse_ok;
  }
}


inline int parse_null(struct Context &context, ccJSON::Value &value)
{
  // does not parse 'null' successfully 
  if(context.jsontext[0] != 'n' ||
     context.jsontext[1] != 'u' ||
     context.jsontext[2] != 'l' ||
     context.jsontext[3] != 'l'   )
  {
    return ccJSON::parse_invalid_value;
  }
  else
  {
    context.jsontext += 4; // ptr offset as length(null) == 4
    value.type = ccJSON::Null;
    return ccJSON::parse_ok;
  }
}

int ccJSON::parse(ccJSON::Value &value, const std::string &jsontext)
{
  return ccJSON::parse(value, jsontext.c_str());
}


double ccJSON::get_number(const ccJSON::Value &value) 
{
  assert(value.get_type() == ccJSON::Number);
  return value.number;
}

std::string ccJSON::get_string(const ccJSON::Value &value) 
{
  assert(value.get_type() == ccJSON::String);
  return value.String;
}

int parse_number(Context &context, ccJSON::Value &value) 
{
  const char* p = context.jsontext;
  if (*p == '-') p++;
  if (*p == '0') p++;
  else { 
      if (!ISDIGIT1TO9(*p)) return ccJSON::parse_invalid_value;
      for (p++; ISDIGIT(*p); p++);
  }
  if (*p == '.') {
      p++;
      if (!ISDIGIT(*p)) return ccJSON::parse_invalid_value;
      for (p++; ISDIGIT(*p); p++);
  }
  if (*p == 'e' || *p == 'E') {
      p++;
      if (*p == '+' || *p == '-') p++;
      if (!ISDIGIT(*p)) return ccJSON::parse_invalid_value;
      for (p++; ISDIGIT(*p); p++);
  }
  errno = 0;
  value.number = strtod(context.jsontext, NULL);
  if (errno == ERANGE && 
       (value.number == HUGE_VAL || value.number== -HUGE_VAL))
  {
    return ccJSON::parse_number_too_big; 
  }
  value.type = ccJSON::Number;
  context.jsontext = p;
  return ccJSON::parse_ok;
}

int parse_string(struct Context &context, ccJSON::Value &value)
{
  // 是的，我偷懒了。 // Yes, I am being lazy.
  // TODO: Fix bug, Need to consider all cases,
  // For example, need to consider the case where the string is incomplete
  // and it ends with escape char. eg. "hello, world\"
  // delete it will make the string invalid.
  char *temp = const_cast<char *>(context.jsontext);
  DeleteLRChar(temp, ' ');
  DeleteLRChar(temp, '"');
  auto wocao = strstr(static_cast<const char *>(temp), "\"");
  if(wocao != 0 && *--wocao != 92) // ascii of '\'
  {
    return ccJSON::parse_invalid_string;
  }
  else
  {
    value.String = temp;
  }
  return ccJSON::parse_ok;
}

void DeleteLRChar(char *str, const char chr)
{
  DeleteLChar(str, chr);
  DeleteRChar(str, chr);
  return ;
}

void DeleteLChar(char *str, const char chr)
{
 if (str == 0) return;
  if (strlen(str) == 0) return;
  
  char strTemp[strlen(str)+1];
  
  int iTemp=0;
  
  memset(strTemp,0,sizeof(strTemp));
  strcpy(strTemp,str);
  
  while ( strTemp[iTemp] == chr )  iTemp++;
  
  memset(str,0,strlen(str)+1);
  
  strcpy(str,strTemp+iTemp);
  
  return;
}

void DeleteRChar(char *str, const char chr)
{
  if (str == 0) return;
  if (strlen(str) == 0) return;

  int istrlen = strlen(str);

  while (istrlen>0)
  {
    if (str[istrlen-1] != chr) break;

    str[istrlen-1]=0;

    istrlen--;
  }
}
