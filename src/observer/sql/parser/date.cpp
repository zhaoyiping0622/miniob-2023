#include "date.h"

#include "common/lang/comparator.h"
#include "common/log/log.h"
#include <cstdio>
#include <ctime>
#include <map>
#include <string>
#include <strings.h>

bool valid_tm(const std::tm &tm) {
  auto cpy = tm;

  // this two step process would correct any out of range values which may be present in cpy

  // http://en.cppreference.com/w/cpp/chrono/c/mktime
  //    note: "The values in time are permitted to be outside their normal ranges."
  const auto as_time_t = std::mktime(std::addressof(cpy));

  // http://en.cppreference.com/w/cpp/chrono/c/localtime
  cpy = *std::localtime(std::addressof(as_time_t));

  return tm.tm_mday == cpy.tm_mday && // valid day
         tm.tm_mon == cpy.tm_mon &&   // valid month
         tm.tm_year == cpy.tm_year && // valid year
         tm.tm_wday == cpy.tm_wday;   // valid day of week
}

Date::Date(const std::string &s, const std::string &format) {
  std::tm tm;
  bzero(&tm, sizeof(tm));
  char *tail = strptime(s.c_str(), format.c_str(), &tm);
  if (tail != s.c_str() + s.size() || !valid_tm(tm)) {
    value = -1;
  } else {
    value = mktime(&tm) / kSecondsInDay;
  }
}

static std::string trans(char c, struct tm &tm) {
  char buf[32];
  std::string tmp = "%";
  switch (c) {
  case '%': return "%";
  case 'M': c = 'B';
  case 'm':
  case 'd':
  case 'y':
  case 'F':
  case 'Y': {
    strftime(buf, sizeof(buf), (tmp + c).c_str(), &tm);
    return buf;
  }
  case 'D': {
    int t = tm.tm_mday;
    std::string ret;
    if (t / 10 == 1) {
      ret = "th";
    } else if (t % 10 == 1) {
      ret = "st";
    } else if (t % 10 == 2) {
      ret = "nd";
    } else if (t % 10 == 3) {
      ret = "rd";
    } else {
      ret = "th";
    }
    return std::to_string(t) + ret;
  }
  default: LOG_WARN("unsupported format char %%%c", c);
  }
  return std::string(1, c);
}

std::string Date::to_string(const Date &date, const std::string &format) {
  time_t t = date.value;
  t *= kSecondsInDay;
  struct tm tm;
  localtime_r(&t, &tm);
  std::string ret;
  bool flag = false;
  for (auto x : format) {
    if (flag) {
      ret += trans(x, tm);
      flag = false;
    } else if (x == '%') {
      flag = true;
    } else
      ret += x;
  }
  return ret;
}

int Date::compare_date(const Date *a, const Date *b) { return common::compare_int((void *)a, (void *)b); }
