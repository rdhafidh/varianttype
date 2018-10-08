#include "varianttype.h"
#include <cfenv>

namespace val {
VariantType::VariantType() : type(TypeData::TYPE_IS_UNDEFINED) {
  intVar();
  vardata = std::monostate();
}

VariantType::VariantType(bool e) {
  vardata = (int64_t)e;
  type = TypeData::TYPE_IS_BOOL;
  intVar();
}

VariantType::VariantType(int64_t e) {
  vardata = e;
  type = TypeData::TYPE_IS_NUMBER;
  intVar();
}

VariantType::VariantType(const std::string &e) {
  intVar();
  vardata = e;
  type = TypeData::TYPE_IS_STRING;
}

VariantType::VariantType(const VectorData &vec) {
  intVar();
  vectordata = vec;
  type = TypeData::TYPE_IS_ARRAY;
}

VariantType::VariantType(const VariantType &anotherVar) {
  intVar();
  switch (anotherVar.getType()) {
    case TypeData::TYPE_IS_ARRAY:
      vectordata = anotherVar.getVector();
      type = anotherVar.getType();
      break;
    case TypeData::TYPE_IS_BOOL:
    case TypeData::TYPE_IS_NUMBER:
    case TypeData::TYPE_IS_STRING:
    case TypeData::TYPE_IS_DOUBLE:
      vardata = anotherVar.getVar();
      type = anotherVar.getType();
      break;
    default:
      vardata = std::monostate();
      type = TypeData::TYPE_IS_UNDEFINED;
      break;
  }
}

VariantType::VariantType(double n) {
  intVar();
  vardata = n;
  type = TypeData::TYPE_IS_DOUBLE;
}

VariantType &VariantType::operator=(const VariantType &anotherVar) {
  intVar();
  switch (anotherVar.getType()) {
    case TypeData::TYPE_IS_ARRAY:
      vectordata = anotherVar.getVector();
      type = anotherVar.getType();
      break;
    case TypeData::TYPE_IS_BOOL:
    case TypeData::TYPE_IS_NUMBER:
    case TypeData::TYPE_IS_STRING:
    case TypeData::TYPE_IS_DOUBLE:
      vardata = anotherVar.getVar();
      type = anotherVar.getType();
      break;
    default:
      vardata = std::monostate();
      type = TypeData::TYPE_IS_UNDEFINED;
      break;
  }
  return *this;
}

bool VariantType::operator==(const VariantType &anotherVar) {
  bool ok = (getType() == anotherVar.getType());
  if (ok) {
    if (getType() == TypeData::TYPE_IS_ARRAY) {
      ok = getVector() == anotherVar.getVector();
    } else {
      if (vardata.index() != 0) {
        ok = getVar() == anotherVar.getVar();
      }
    }
  }
  return ok;
}

int64_t VariantType::makeNumber(int n) { return (int64_t)n; }

double VariantType::makeDouble(float n) { return (double)n; }

bool VariantType::addVariantNumber(int64_t n, TypeErrorVariant *errorFlag) {
  if (type == TypeData::TYPE_IS_NUMBER) {
    boost::multiprecision::cpp_int left = std::get<int64_t>(vardata);
    boost::multiprecision::cpp_int right = n;
    left += right;
    if (left < minval || left > maxval) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_OVERFLOW;
      return false;
    }
    vardata = left.convert_to<int64_t>();
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }
  if (type == TypeData::TYPE_IS_DOUBLE) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double left = std::get<double>(vardata);
    left += n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
      return false;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
      return false;
    }
    vardata = left;
    type = TypeData::TYPE_IS_DOUBLE;
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }
  if (type == TypeData::TYPE_IS_BOOL) {
    boost::multiprecision::cpp_int left = std::get<int64_t>(vardata);
    boost::multiprecision::cpp_int right = n;
    left += right;
    if (left < minval || left > maxval) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_OVERFLOW;
      return false;
    }
    vardata = left.convert_to<int64_t>();
    type = TypeData::TYPE_IS_NUMBER;
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }

  *errorFlag = TypeErrorVariant::ERROR_UNSUPPORTED_ADD_ARITH_TYPES;
  return false;
}

bool VariantType::subVariantNumber(int64_t n, TypeErrorVariant *errorFlag) {
  if (type == TypeData::TYPE_IS_NUMBER) {
    boost::multiprecision::cpp_int left = std::get<int64_t>(vardata);
    boost::multiprecision::cpp_int right = n;
    left -= right;
    if (left < minval || left > maxval) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_OVERFLOW;
      return false;
    }
    vardata = left.convert_to<int64_t>();
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }
  if (type == TypeData::TYPE_IS_DOUBLE) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double left = std::get<double>(vardata);
    left -= n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
      return false;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
      return false;
    }
    vardata = left;
    type = TypeData::TYPE_IS_DOUBLE;
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }
  if (type == TypeData::TYPE_IS_BOOL) {
    boost::multiprecision::cpp_int left = std::get<int64_t>(vardata);
    boost::multiprecision::cpp_int right = n;
    left -= right;
    if (left < minval || left > maxval) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_OVERFLOW;
      return false;
    }
    vardata = left.convert_to<int64_t>();
    type = TypeData::TYPE_IS_NUMBER;
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }

  *errorFlag = TypeErrorVariant::ERROR_UNSUPPORTED_SUB_ARITH_TYPES;
  return false;
}

bool VariantType::divVariantNumber(int64_t n, TypeErrorVariant *errorFlag) {
  if (n == 0) {
    *errorFlag = TypeErrorVariant::ERROR_DIVIDE_BY_ZERO;
    return false;
  }

  if (type == TypeData::TYPE_IS_NUMBER) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = (double)std::get<int64_t>(vardata) / n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
      return false;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
      return false;
    }
    vardata = ret;
    type = TypeData::TYPE_IS_DOUBLE;
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }
  if (type == TypeData::TYPE_IS_BOOL) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = (double)std::get<int64_t>(vardata) / n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
      return false;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
      return false;
    }
    vardata = ret;
    type = TypeData::TYPE_IS_DOUBLE;
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }
  if (type == TypeData::TYPE_IS_DOUBLE) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = (double)std::get<double>(vardata) / n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
      return false;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
      return false;
    }
    vardata = ret;
    type = TypeData::TYPE_IS_DOUBLE;
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }

  *errorFlag = TypeErrorVariant::ERROR_UNSUPPORTED_DIV_ARITH_TYPES;
  return false;
}

bool VariantType::addVariantBool(bool n, TypeErrorVariant *errorFlag) {
  if (type == TypeData::TYPE_IS_BOOL) {
    vardata = std::get<int64_t>(vardata) + n;
    type = TypeData::TYPE_IS_NUMBER;
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }
  if (type == TypeData::TYPE_IS_DOUBLE) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = (double)std::get<double>(vardata) + n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
      return false;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
      return false;
    }
    vardata = ret;
    type = TypeData::TYPE_IS_DOUBLE;
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }
  if (type == TypeData::TYPE_IS_NUMBER) {
    boost::multiprecision::cpp_int left = std::get<int64_t>(vardata);
    left += n;
    if (left > maxval) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_OVERFLOW;
      return false;
    }
    if (left < minval) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
      return false;
    }
    vardata = left.convert_to<int64_t>();
    type = TypeData::TYPE_IS_NUMBER;
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }

  *errorFlag = TypeErrorVariant::ERROR_UNSUPPORTED_ADD_ARITH_TYPES;
  return false;
}

bool VariantType::subVariantBool(bool n, TypeErrorVariant *errorFlag) {
  if (type == TypeData::TYPE_IS_BOOL) {
    vardata = std::get<int64_t>(vardata) - n;
    type = TypeData::TYPE_IS_NUMBER;
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }
  if (type == TypeData::TYPE_IS_DOUBLE) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = (double)std::get<double>(vardata) - n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
      return false;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
      return false;
    }
    vardata = ret;
    type = TypeData::TYPE_IS_DOUBLE;
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }
  if (type == TypeData::TYPE_IS_NUMBER) {
    boost::multiprecision::cpp_int left = std::get<int64_t>(vardata);
    left -= n;
    if (left > maxval) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_OVERFLOW;
      return false;
    }
    if (left < minval) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
      return false;
    }
    vardata = left.convert_to<int64_t>();
    type = TypeData::TYPE_IS_NUMBER;
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }

  *errorFlag = TypeErrorVariant::ERROR_UNSUPPORTED_SUB_ARITH_TYPES;
  return false;
}

bool VariantType::mulVariantBool(bool n, TypeErrorVariant *errorFlag) {
  if (type == TypeData::TYPE_IS_BOOL) {
    vardata = (int64_t)std::get<int64_t>(vardata) * n;
    type = TypeData::TYPE_IS_NUMBER;
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }
  if (type == TypeData::TYPE_IS_DOUBLE) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = (double)std::get<double>(vardata) * n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
      return false;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
      return false;
    }
    vardata = ret;
    type = TypeData::TYPE_IS_DOUBLE;
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }
  if (type == TypeData::TYPE_IS_NUMBER) {
    boost::multiprecision::cpp_int left = std::get<int64_t>(vardata);
    left *= n;
    if (left > maxval) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_OVERFLOW;
      return false;
    }
    if (left < minval) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
      return false;
    }
    vardata = left.convert_to<int64_t>();
    type = TypeData::TYPE_IS_NUMBER;
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }
  *errorFlag = TypeErrorVariant::ERROR_UNSUPPORTED_MUL_ARITH_TYPES;
  return false;
}

bool VariantType::divVariantBool(bool n, TypeErrorVariant *errorFlag) {
  if (n == 0) {
    *errorFlag = TypeErrorVariant::ERROR_DIVIDE_BY_ZERO;
    return false;
  }
  if (type == TypeData::TYPE_IS_BOOL || type == TypeData::TYPE_IS_NUMBER ||
      type == TypeData::TYPE_IS_DOUBLE) {
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }

  *errorFlag = TypeErrorVariant::ERROR_UNSUPPORTED_DIV_ARITH_TYPES;
  return false;
}

bool VariantType::addVariantDouble(double n, TypeErrorVariant *errorFlag) {
  if (type == TypeData::TYPE_IS_BOOL || type == TypeData::TYPE_IS_NUMBER) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = (double)std::get<int64_t>(vardata) + n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
      return false;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
      return false;
    }
    vardata = ret;
    type = TypeData::TYPE_IS_DOUBLE;
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }
  if (type == TypeData::TYPE_IS_DOUBLE) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = std::get<double>(vardata) + n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
      return false;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
      return false;
    }
    vardata = ret;
    type = TypeData::TYPE_IS_DOUBLE;
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }
  *errorFlag = TypeErrorVariant::ERROR_UNSUPPORTED_ADD_ARITH_TYPES;
  return false;
}

bool VariantType::subVariantDouble(double n, TypeErrorVariant *errorFlag) {
  if (type == TypeData::TYPE_IS_BOOL || type == TypeData::TYPE_IS_NUMBER) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = (double)std::get<int64_t>(vardata) - n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
      return false;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
      return false;
    }
    type = TypeData::TYPE_IS_DOUBLE;
    vardata = ret;
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }
  if (type == TypeData::TYPE_IS_DOUBLE) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = (double)std::get<double>(vardata) - n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
      return false;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
      return false;
    }
    type = TypeData::TYPE_IS_DOUBLE;
    vardata = ret;
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }
  *errorFlag = TypeErrorVariant::ERROR_UNSUPPORTED_SUB_ARITH_TYPES;
  return false;
}

bool VariantType::mulVariantDouble(double n, TypeErrorVariant *errorFlag) {
  if (type == TypeData::TYPE_IS_BOOL || type == TypeData::TYPE_IS_NUMBER) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = (double)std::get<int64_t>(vardata) * n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
      return false;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
      return false;
    }
    type = TypeData::TYPE_IS_NUMBER;
    vardata = ret;
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }
  if (type == TypeData::TYPE_IS_DOUBLE) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = std::get<double>(vardata) * n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
      return false;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
      return false;
    }
    type = TypeData::TYPE_IS_DOUBLE;
    vardata = ret;
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }
  *errorFlag = TypeErrorVariant::ERROR_UNSUPPORTED_MUL_ARITH_TYPES;
  return false;
}

bool VariantType::divVariantDouble(double n, TypeErrorVariant *errorFlag) {
  if (n == 0) {
    *errorFlag = TypeErrorVariant::ERROR_DIVIDE_BY_ZERO;
    return false;
  }
  if (type == TypeData::TYPE_IS_BOOL || type == TypeData::TYPE_IS_NUMBER) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = (double)std::get<int64_t>(vardata) / n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
      return false;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
      return false;
    }
    type = TypeData::TYPE_IS_DOUBLE;
    vardata = ret;
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }
  if (type == TypeData::TYPE_IS_DOUBLE) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = (double)std::get<double>(vardata) / n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
      return false;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
      return false;
    }
    type = TypeData::TYPE_IS_DOUBLE;
    vardata = ret;
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }
  *errorFlag = TypeErrorVariant::ERROR_UNSUPPORTED_DIV_ARITH_TYPES;
  return false;
}

bool VariantType::addVariantString(const std::string &n,
                                   TypeErrorVariant *errorFlag) {
  if (type == TypeData::TYPE_IS_STRING) {
    std::string ret = std::get<std::string>(vardata);
    ret += n;
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }
  *errorFlag = TypeErrorVariant::ERROR_UNSUPPORTED_ADD_STRING_TYPES;
  return false;
}

bool VariantType::mulVariantNumber(int64_t n, TypeErrorVariant *errorFlag) {
  if (type == TypeData::TYPE_IS_NUMBER) {
    boost::multiprecision::cpp_int left = std::get<int64_t>(vardata);
    boost::multiprecision::cpp_int right = n;
    left *= right;
    if (left < minval || left > maxval) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_OVERFLOW;
      return false;
    }
    vardata = left.convert_to<int64_t>();
    return true;
  }
  if (type == TypeData::TYPE_IS_DOUBLE) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = (double)std::get<double>(vardata) * n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
      return false;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
      return false;
    }
    vardata = ret;
    type = TypeData::TYPE_IS_DOUBLE;
    *errorFlag = TypeErrorVariant::NO_ERROR;
    return true;
  }
  if (type == TypeData::TYPE_IS_BOOL) {
    boost::multiprecision::cpp_int left = std::get<int64_t>(vardata);
    boost::multiprecision::cpp_int right = n;
    left *= right;
    if (left < minval || left > maxval) {
      *errorFlag = TypeErrorVariant::ERROR_ARITH_OVERFLOW;
      return false;
    }
    vardata = left.convert_to<int64_t>();
    type = TypeData::TYPE_IS_NUMBER;
    return true;
  }

  *errorFlag = TypeErrorVariant::ERROR_UNSUPPORTED_ADD_ARITH_TYPES;
  return false;
}

VariantType::~VariantType() {}

TypeData VariantType::getType() const { return type; }

Var VariantType::getVar() const { return vardata; }

VectorData VariantType::getVector() const { return vectordata; }

VariantType &VariantType::operator<<(const std::string &str) {
  if (getType() != TypeData::TYPE_IS_STRING) {
    return *this;
  }
  vardata = std::get<std::string>(vardata) + str;
  return *this;
}

VariantType &VariantType::operator<<(const char *buf) {
  if (getType() != TypeData::TYPE_IS_STRING) {
    return *this;
  }
  vardata = std::get<std::string>(vardata) + buf;
  return *this;
}

VariantType &VariantType::operator<<(const VariantType &anotherVar) {
  switch (getType()) {
    case TypeData::TYPE_IS_ARRAY:
    case TypeData::TYPE_IS_UNDEFINED:
      vectordata.emplace_back(anotherVar);
      type = TypeData::TYPE_IS_ARRAY;
      break;
    default:
      break;
  }
  return *this;
}

void VariantType::intVar() {
  minval = std::numeric_limits<int64_t>::min();
  maxval = std::numeric_limits<int64_t>::max();
}

bool operator==(const Var &f1, const Var &f2) {
  bool ok = f1.index() == f2.index();
  if (ok) {
    switch (f1.index()) {
      case 0:
        break;
      case 1:
        ok = std::get<int64_t>(f1) == std::get<int64_t>(f2);
        break;
      case 2:
        ok = std::get<std::string>(f1) == std::get<std::string>(f2);
        break;
      case 3:
        ok = std::get<double>(f1) == std::get<double>(f2);
        break;
      default:
        ok = false;
        break;
    }
  }
  return ok;
}

bool operator==(const VectorData &vec1, const VectorData &vec2) {
  bool ok = vec1.size() == vec2.size();
  if (ok) {
    for (decltype(vec1.size()) x = 0; x < vec1.size(); x++) {
      ok = vec1.at(x) == vec2.at(x);
      if (!ok) return false;
    }
  }
  return ok;
}
void printArray(std::ostream &sm, const VariantType &v) {
  auto vec = v.getVector();
  for (decltype(vec.size()) x = 0; x < vec.size(); x++) {
    switch (vec.at(x).getType()) {
      case TypeData::TYPE_IS_UNDEFINED:
        sm << " [undefined]";
        break;
      case TypeData::TYPE_IS_STRING:
        sm << " " << std::get<std::string>(vec.at(x).getVar());
        break;
      case TypeData::TYPE_IS_NUMBER:
        sm << " " << std::get<int64_t>(vec.at(x).getVar());
        break;
      case TypeData::TYPE_IS_DOUBLE:
        sm << " " << std::get<double>(vec.at(x).getVar());
        break;
      case TypeData::TYPE_IS_BOOL:
        sm << " " << std::boolalpha
           << (bool)std::get<int64_t>(vec.at(x).getVar());
        break;
      case TypeData::TYPE_IS_ARRAY:
        sm << "\nNested Array: ";
        printArray(sm, v);
        break;
      default:
        break;
    }
  }
  sm << " ";
}

std::ostream &operator<<(std::ostream &os, const VariantType &var) {
  switch (var.getType()) {
    case TypeData::TYPE_IS_UNDEFINED:
      os << " [undefined value] ";
      break;
    case TypeData::TYPE_IS_NUMBER:
      os << " " << std::get<int64_t>(var.getVar()) << " ";
      break;
    case TypeData::TYPE_IS_STRING:
      os << " " << std::get<std::string>(var.getVar()) << " ";
      break;
    case TypeData::TYPE_IS_DOUBLE:
      os << " " << std::get<double>(var.getVar()) << " ";
      break;
    case TypeData::TYPE_IS_BOOL:
      os << " " << std::boolalpha << (bool)std::get<int64_t>(var.getVar())
         << " ";
      break;
    case TypeData::TYPE_IS_ARRAY:
      printArray(os, var);
      break;
    default:
      break;
  }
  return os;
}

bool operator==(VariantType &var, bool e) {
  bool ok = (var.getType() == TypeData::TYPE_IS_BOOL);
  if (ok) {
    ok = (std::get<int64_t>(var.getVar()) == (int64_t)e);
  }
  return ok;
}

bool operator==(VariantType &var, const std::string &e) {
  bool ok = (var.getType() == TypeData::TYPE_IS_STRING);
  if (ok) {
    ok = (std::get<std::string>(var.getVar()) == e);
  }
  return ok;
}

bool operator==(VariantType &var, const char *e) {
  bool ok = (var.getType() == TypeData::TYPE_IS_STRING);
  if (ok) {
    ok = (std::get<std::string>(var.getVar()) == e);
  }
  return ok;
}

bool operator==(VariantType &var, int64_t e) {
  bool ok = (var.getType() == TypeData::TYPE_IS_NUMBER);
  if (ok) {
    ok = (std::get<int64_t>(var.getVar()) == e);
  }
  return ok;
}

bool operator==(VariantType &var, double e) {
  bool ok = (var.getType() == TypeData::TYPE_IS_DOUBLE);
  if (ok) {
    ok = (std::get<double>(var.getVar()) == e);
  }
  return ok;
}

bool operator==(VariantType &var, const VectorData &vec) {
  bool ok = (var.getType() == TypeData::TYPE_IS_ARRAY);
  if (ok) {
    auto vec1 = var.getVector();
    ok = vec1.size() == vec.size();
    if (ok) {
      for (decltype(vec1.size()) x = 0; x < vec1.size(); x++) {
        ok = vec1.at(x) == vec.at(x);
        if (!ok) return false;
      }
    }
  }
  return ok;
}

bool isOkTypeVariant(const TypeErrorVariant &type) {
  return type == TypeErrorVariant::NO_ERROR;
}

bool operator==(const VariantType &var1, const VariantType &var2) {
  return (var1.getType() == var2.getType() && var1.getVar() == var2.getVar());
}

}  // namespace val