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
#include <cstring>
#include <limits>
#include <memory>
#include <sstream>
#include <vector>

std::strong_ordering ValueComparator::compare(const Value &a, const Value &b) const {
  if (a.attr_type() != b.attr_type())
    return a.attr_type() <=> b.attr_type();
  return a <=> b;
}
std::strong_ordering ValueComparator::compare(const ValueList &a, const ValueList &b) const {
  auto &aa = a.get_list();
  auto &bb = b.get_list();
  for (int i = 0; i < aa.size() && i < bb.size(); i++) {
    auto order = compare(aa[i], bb[i]);
    if (order == std::strong_ordering::equal)
      continue;
    return order;
  }
  return aa.size() <=> bb.size();
}

bool ValueComparator::operator()(const Value &a, const Value &b) const {
  return compare(a, b) == std::strong_ordering::less;
}

bool ValueComparator::operator()(const ValueList &a, const ValueList &b) const {
  return compare(a, b) == std::strong_ordering::less;
}

const char *ATTR_TYPE_NAME[] = {"undefined", "chars", "ints", "dates", "floats", "texts", "nulls", "lists", "booleans"};

int attr_type_to_size(AttrType type) { return 4; }

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

Value::Value(ValueListMap &list) { set_list(list); }

void Value::set_data(char *data, int length) {
  switch (attr_type()) {
  case CHARS: {
    set_string(data, length);
  } break;
  case TEXTS: {
    num_value_.int_value_ = *(int *)data;
    length_ = length;
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
  str_value_.clear();
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

void Value::set_list(const ValueListMap &list) {
  attr_type_ = LISTS;
  list_value_ = std::make_shared<ValueListMap>(list);
  bool has_null = false;
  for (auto &v : list) {
    if (v.first.has_null()) {
      has_null = true;
      break;
    }
  }
  num_value_.bool_value_ = has_null;
}

void Value::set_text(const char *s) {
  std::string tmp;
  attr_type_ = TEXTS;
  tmp.resize(TEXT_SIZE, 0);
  strncpy(&tmp[0], s, TEXT_SIZE);
  str_value_.swap(tmp);
}

void Value::set_value(const Value &value) {
  if (&value == this)
    return;
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
  case TEXTS: {
    set_text(value.get_string().c_str());
  } break;
  }
}

const char *Value::data() const {
  switch (attr_type()) {
  case TEXTS:
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
  case TEXTS:
  case CHARS: {
    os << str_value_.c_str();
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
      os << x.first.to_string();
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
    case TEXTS:
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
      auto it1 = list_value_->begin();
      auto it2 = other.list_value_->begin();
      while (it1 != list_value_->end() && it2 != other.list_value_->end()) {
        auto order = *it1 <=> *it2;
        if (order != std::strong_ordering::equal) {
          return compare_by_ordering(order);
        }
      }
      return compare_by_ordering(list_value_->size() <=> other.list_value_->size());
    } break;
    case NULLS: {
      return 0;
    } break;
    default: {
      LOG_WARN("unsupported type: %d", this->attr_type_);
      return INVALID_COMPARE;
    }
    }
  }

  if (this->attr_type_ == LISTS) {
    auto list = get_list();
    if (list->size() != 1 || list->begin()->second != 1)
      return INVALID_COMPARE;
    return list->begin()->first.get_list().begin()->compare(other);
  } else if (other.attr_type_ == LISTS) {
    return -other.compare(*this);
  }

  auto target_type = AttrTypeCompare(this->attr_type_, other.attr_type_);
  if (target_type == UNDEFINED)
    return INVALID_COMPARE;

  Value a = *this;
  Value b = other;

  if (!convert(a.attr_type_, target_type, a) || !convert(b.attr_type_, target_type, b)) {
    return INVALID_COMPARE;
  }
  return a.compare(b);
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
  case TEXTS:
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
    return (int)(num_value_.float_value_ + 0.5);
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
  case TEXTS:
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
  case TEXTS:
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

bool Value::is_null() const { return attr_type_ == NULLS || attr_type_ == LISTS && num_value_.bool_value_; }

bool Value::get_only(Value &value) const {
  if (attr_type_ != LISTS) {
    value.set_value(*this);
    return true;
  }
  auto list = get_list();
  if (list->size() != 1 || list->begin()->second != 1)
    return false;
  const auto &value_list = list->begin()->first;
  auto &vec = value_list.get_list();
  if (vec.size() != 1)
    return false;
  value.set_value(vec[0]);
  return true;
}

std::shared_ptr<ValueListMap> Value::get_list() const { return list_value_; }

bool Value::convert(AttrType from, AttrType to, Value &value) {
  if (from == to) {
    return true;
  }
  if (from == CHARS) {
    if (to == DATES) {
      Date date = value.get_date();
      if (date == INVALID_DATE)
        return false;
      value.set_date(date);
      return true;
    } else if (to == TEXTS) {
      value.set_text(value.get_string().c_str());
      return true;
    } else if (to == INTS) {
      value.set_int(value.get_int());
      return true;
    } else if (to == FLOATS) {
      value.set_float(value.get_float());
      return true;
    }
  }
  if (from == LISTS) {
    Value new_value;
    if (value.get_only(new_value) && convert(new_value.attr_type(), to, new_value)) {
      value = new_value;
      return true;
    }
    return false;
  }
  if (to == CHARS) {
    if (from == LISTS)
      return false;
    if (from == NULLS)
      return false;
    value.set_string(value.to_string().c_str());
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
  case TEXTS:
  case CHARS: {
    if (b == INTS)
      return FLOATS;
    return b;
  }
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
