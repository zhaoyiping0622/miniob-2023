/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by WangYunlai on 2023/06/28.
//

#include "sql/parser/value.h"
#include "common/lang/comparator.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "sql/parser/date.h"
#include "storage/field/field.h"
#include <compare>
#include <memory>
#include <sstream>
#include <vector>

const char *ATTR_TYPE_NAME[] = {"undefined", "chars", "ints", "dates", "floats", "nulls", "lists", "booleans"};

const char *attr_type_to_string(AttrType type) {
  if (type >= UNDEFINED && type <= BOOLEANS) {
    return ATTR_TYPE_NAME[type];
  }
  return "unknown";
}
AttrType attr_type_from_string(const char *s) {
  for (unsigned int i = 0; i < sizeof(ATTR_TYPE_NAME) / sizeof(ATTR_TYPE_NAME[0]); i++) {
    if (0 == strcmp(ATTR_TYPE_NAME[i], s)) {
      return (AttrType)i;
    }
  }
  return UNDEFINED;
}

Value::Value(int val) { set_int(val); }

Value::Value(float val) { set_float(val); }

Value::Value(bool val) { set_boolean(val); }

Value::Value(const char *s, int len /*= 0*/) { set_string(s, len); }

Value::Value(Date date) { set_date(date); }

Value::Value(std::set<ValueList> &list) { set_list(list); }

void Value::set_data(char *data, int length) {
  switch (attr_type()) {
  case CHARS: {
    set_string(data, length);
  } break;
  case INTS: {
    num_value_.int_value_ = *(int *)data;
    length_ = length;
  } break;
  case FLOATS: {
    num_value_.float_value_ = *(float *)data;
    length_ = length;
  } break;
  case BOOLEANS: {
    num_value_.bool_value_ = *(int *)data != 0;
    length_ = length;
  } break;
  case DATES: {
    num_value_.date_value_ = *(Date *)data;
    length_ = length;
  } break;
  default: {
    LOG_WARN("unknown data type: %d", attr_type_);
  } break;
  }
}
void Value::set_int(int val) {
  attr_type_ = INTS;
  num_value_.int_value_ = val;
  length_ = sizeof(val);
}

void Value::set_float(float val) {
  attr_type_ = FLOATS;
  num_value_.float_value_ = val;
  length_ = sizeof(val);
}
void Value::set_boolean(bool val) {
  attr_type_ = BOOLEANS;
  num_value_.bool_value_ = val;
  length_ = sizeof(val);
}
void Value::set_string(const char *s, int len /*= 0*/) {
  attr_type_ = CHARS;
  if (len > 0) {
    len = strnlen(s, len);
    str_value_.assign(s, len);
  } else {
    str_value_.assign(s);
  }
  length_ = str_value_.length();
}
void Value::set_date(Date date) {
  attr_type_ = DATES;
  num_value_.date_value_ = date;
  length_ = sizeof(date);
}

void Value::set_null() {
  attr_type_ = NULLS;
  length_ = 1;
}

void Value::set_list(const std::set<ValueList> &list) {
  attr_type_ = LISTS;
  list_value_ = std::make_shared<std::set<ValueList>>(list);
}

void Value::set_value(const Value &value) {
  switch (value.attr_type()) {
  case NULLS: {
    set_null();
  } break;
  case INTS: {
    set_int(value.get_int());
  } break;
  case FLOATS: {
    set_float(value.get_float());
  } break;
  case CHARS: {
    set_string(value.get_string().c_str());
  } break;
  case BOOLEANS: {
    set_boolean(value.get_boolean());
  } break;
  case DATES: {
    set_date(value.get_date());
  } break;
  case UNDEFINED: {
    ASSERT(false, "got an invalid value type");
  } break;
  case LISTS: {
    set_list(*value.get_list());
  } break;
  }
}

const char *Value::data() const {
  switch (attr_type()) {
  case CHARS: {
    return str_value_.c_str();
  } break;
  default: {
    return (const char *)&num_value_;
  } break;
  }
}

std::string Value::to_string() const {
  std::stringstream os;
  switch (attr_type()) {
  case INTS: {
    os << num_value_.int_value_;
  } break;
  case FLOATS: {
    os << common::double_to_str(num_value_.float_value_);
  } break;
  case BOOLEANS: {
    os << num_value_.bool_value_;
  } break;
  case CHARS: {
    os << str_value_;
  } break;
  case DATES: {
    os << Date::to_string(num_value_.date_value_);
  } break;
  case NULLS: {
    os << "NULL";
  } break;
  case LISTS: {
    os << "{";
    bool sep = false;
    for (auto &x : *list_value_) {
      if (sep)
        os << ",";
      os << x.to_string();
      sep = true;
    }
    os << "{";
  } break;
  default: {
    LOG_WARN("unsupported attr type: %d", attr_type_);
  } break;
  }
  return os.str();
}

int compare_by_ordering(std::strong_ordering order) {
  if (order == std::strong_ordering::equal || order == std::strong_ordering::equivalent) {
    return 0;
  } else if (order == std::strong_ordering::less) {
    return -1;
  } else if (order == std::strong_ordering::greater) {
    return 1;
  }
  return -1;
}

int compare_by_ordering(std::weak_ordering order) {
  if (order == std::weak_ordering::equivalent) {
    return 0;
  } else if (order == std::weak_ordering::less) {
    return -1;
  } else if (order == std::weak_ordering::greater) {
    return 1;
  }
  return -1;
}

int Value::compare(const Value &other) const {
  if (this->attr_type() == other.attr_type()) {
    switch (this->attr_type_) {
    case INTS: {
      return common::compare_int((void *)&this->num_value_.int_value_, (void *)&other.num_value_.int_value_);
    } break;
    case FLOATS: {
      return common::compare_float((void *)&this->num_value_.float_value_, (void *)&other.num_value_.float_value_);
    } break;
    case CHARS: {
      return common::compare_string((void *)this->str_value_.c_str(), this->str_value_.length(),
                                    (void *)other.str_value_.c_str(), other.str_value_.length());
    } break;
    case BOOLEANS: {
      return common::compare_int((void *)&this->num_value_.bool_value_, (void *)&other.num_value_.bool_value_);
    } break;
    case DATES: {
      return Date::compare_date(&num_value_.date_value_, &other.num_value_.date_value_);
    } break;
    case LISTS: {
      auto order = (*list_value_) <=> (*other.list_value_);
      return compare_by_ordering(order);
    } break;
    default: {
      LOG_WARN("unsupported type: %d", this->attr_type_);
    }
    }
  } else if (this->attr_type_ == INTS && other.attr_type_ == FLOATS) {
    float this_data = this->num_value_.int_value_;
    return common::compare_float((void *)&this_data, (void *)&other.num_value_.float_value_);
  } else if (this->attr_type_ == FLOATS && other.attr_type_ == INTS) {
    float other_data = other.num_value_.int_value_;
    return common::compare_float((void *)&this->num_value_.float_value_, (void *)&other_data);
  } else if (this->attr_type_ == CHARS && other.attr_type_ == DATES) {
    Date a = get_date();
    return Date::compare_date(&a, &other.num_value_.date_value_);
  } else if (this->attr_type_ == DATES && other.attr_type_ == CHARS) {
    Date b = other.get_date();
    return Date::compare_date(&num_value_.date_value_, &b);
  } else if (this->attr_type_ == LISTS) {
    auto list = get_list();
    if (list->size() != 1)
      return -1;
    return list->begin()->get_list().begin()->compare(other);
  } else if (other.attr_type_ == LISTS) {
    auto list = other.get_list();
    if (list->size() != 1)
      return -1;
    return this->compare(*list->begin()->get_list().begin());
  }
  LOG_WARN("not supported");
  return -1; // TODO return rc?
}

std::strong_ordering Value::operator<=>(const Value &value) const {
  int cmp = compare(value);
  if (cmp < 0) {
    return std::strong_ordering::less;
  }
  if (cmp > 0) {
    return std::strong_ordering::greater;
  }
  return std::strong_ordering::equal;
}

int Value::get_int() const {
  switch (attr_type()) {
  case CHARS: {
    try {
      return (int)(std::stol(str_value_));
    } catch (std::exception const &ex) {
      LOG_TRACE("failed to convert string to number. s=%s, ex=%s", str_value_.c_str(), ex.what());
      return 0;
    }
  }
  case INTS: {
    return num_value_.int_value_;
  }
  case FLOATS: {
    return (int)(num_value_.float_value_);
  }
  case BOOLEANS: {
    return (int)(num_value_.bool_value_);
  }
  default: {
    // DATES ignore
    LOG_WARN("unknown data type. type=%d", attr_type_);
    return 0;
  }
  }
  return 0;
}

float Value::get_float() const {
  switch (attr_type()) {
  case CHARS: {
    try {
      return std::stof(str_value_);
    } catch (std::exception const &ex) {
      LOG_TRACE("failed to convert string to float. s=%s, ex=%s", str_value_.c_str(), ex.what());
      return 0.0;
    }
  } break;
  case INTS: {
    return float(num_value_.int_value_);
  } break;
  case FLOATS: {
    return num_value_.float_value_;
  } break;
  case BOOLEANS: {
    return float(num_value_.bool_value_);
  } break;
  default: {
    // DATES ignore
    LOG_WARN("unknown data type. type=%d", attr_type_);
    return 0;
  }
  }
  return 0;
}

std::string Value::get_string() const { return this->to_string(); }

bool Value::get_boolean() const {
  switch (attr_type()) {
  case CHARS: {
    try {
      float val = std::stof(str_value_);
      if (val >= EPSILON || val <= -EPSILON) {
        return true;
      }

      int int_val = std::stol(str_value_);
      if (int_val != 0) {
        return true;
      }

      return !str_value_.empty();
    } catch (std::exception const &ex) {
      LOG_TRACE("failed to convert string to float or integer. s=%s, ex=%s", str_value_.c_str(), ex.what());
      return !str_value_.empty();
    }
  } break;
  case INTS: {
    return num_value_.int_value_ != 0;
  } break;
  case FLOATS: {
    float val = num_value_.float_value_;
    return val >= EPSILON || val <= -EPSILON;
  } break;
  case BOOLEANS: {
    return num_value_.bool_value_;
  } break;
  default: {
    // DATES ignore
    LOG_WARN("unknown data type. type=%d", attr_type_);
    return false;
  }
  }
  return false;
}

std::shared_ptr<std::set<ValueList>> Value::get_list() const { return list_value_; }

bool Value::convert(AttrType from, AttrType to, Value &value) {
  if (from == to) {
    return true;
  }
  if (from == CHARS && to == DATES) {
    Date date = value.get_date();
    if (date == INVALID_DATE)
      return false;
    value.set_date(date);
    return true;
  }
  if (from == INTS && to == FLOATS) {
    value.set_float(value.get_float());
    return true;
  }
  if (from == FLOATS && to == INTS) {
    value.set_int(value.get_int());
    return true;
  }
  return false;
}

Date Value::get_date() const {
  switch (attr_type()) {
  case DATES: return num_value_.date_value_;
  case CHARS: return Date(str_value_);
  default: return INVALID_DATE;
  }
}

AttrType AttrTypeCompare(AttrType a, AttrType b) {
  if (a == b)
    return a;
  if (a > b)
    std::swap(a, b);
  if (a == LISTS || b == LISTS) {
    return UNDEFINED;
  }
  switch (a) {
  case UNDEFINED:
  case NULLS: return NULLS;
  case CHARS: return b;
  case INTS: {
    if (b == DATES)
      return UNDEFINED;
    return b;
  }
  case DATES: return UNDEFINED;
  case FLOATS:
  case LISTS: return UNDEFINED;
  case BOOLEANS: return BOOLEANS;
  }
  return UNDEFINED;
}

bool Value::check_value(const Value &v) {
  return v.attr_type() != UNDEFINED && (v.attr_type() != CHARS || v.get_date() != INVALID_DATE);
}

std::string ValueList::to_string() const {
  std::stringstream ss;
  ss << "{";
  bool sep = false;
  for (auto x : list_) {
    if (sep)
      ss << ",";
    ss << x.to_string();
    sep = true;
  }
  ss << "}";
  return ss.str();
}

std::strong_ordering ValueList::operator<=>(const ValueList &other) const {
  int size = std::min(list_.size(), other.list_.size());
  for (int i = 0; i < size; i++) {
    auto cmp = list_[i] <=> other.list_[i];
    if (cmp != std::strong_ordering::equal) {
      return cmp;
    }
  }
  if (list_.size() < other.list_.size())
    return std::strong_ordering::less;
  else if (list_.size() == other.list_.size())
    return std::strong_ordering::equal;
  else
    return std::strong_ordering::greater;
}
