#include "varianttype.h"
#include <cfenv>

val::VariantType::VariantType() : type(TypeData::TYPE_IS_UNDEFINED) {
  intVar();
  vardata = std::monostate();
}
namespace val {
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

VariantType::VariantType(const VariantType &anotherVar) { doClone(anotherVar); }

VariantType::VariantType(double n) {
  intVar();
  vardata = n;
  type = TypeData::TYPE_IS_DOUBLE;
}

VariantType &VariantType::operator=(const VariantType &anotherVar) {
  bool e = *this == anotherVar;
  if (!e) {
    doClone(anotherVar);
  }
  return *this;
}

bool VariantType::operator==(const VariantType &anotherVar) {
  bool ok = (getType() == anotherVar.getType());
  if (ok) {
    ok = getVar() == anotherVar.getVar();
  }
  return ok;
}

bool VariantType::operator!=(const VariantType &anotherVar) {
  return (*this == anotherVar) == false;
}

int64_t VariantType::makeNumber(int n) { return (int64_t)n; }

double VariantType::makeDouble(float n) { return (double)n; }

TypeErrorVariant VariantType::addVariantNumber(int64_t n) {
  if (type == TypeData::TYPE_IS_NUMBER) {
    boost::multiprecision::cpp_int left = std::get<int64_t>(vardata);
    boost::multiprecision::cpp_int right = n;
    left += right;
    if (left < minval || left > maxval) {
      return TypeErrorVariant::ERROR_ARITH_OVERFLOW;
    }
    vardata = left.convert_to<int64_t>();
    return TypeErrorVariant::NO_ERROR;
  }
  if (type == TypeData::TYPE_IS_DOUBLE) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double left = std::get<double>(vardata);
    left += n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
    }
    auto status = isInfNumber(left);
    if (!isOkTypeReturnVariant(status)) return status;

    vardata = left;
    type = TypeData::TYPE_IS_DOUBLE;
    return TypeErrorVariant::NO_ERROR;
  }
  if (type == TypeData::TYPE_IS_BOOL) {
    boost::multiprecision::cpp_int left = std::get<int64_t>(vardata);
    boost::multiprecision::cpp_int right = n;
    left += right;
    if (left < minval || left > maxval) {
      return TypeErrorVariant::ERROR_ARITH_OVERFLOW;
    }
    vardata = left.convert_to<int64_t>();
    type = TypeData::TYPE_IS_NUMBER;
    return TypeErrorVariant::NO_ERROR;
  }

  return TypeErrorVariant::ERROR_UNSUPPORTED_ADD_ARITH_TYPES;
}

TypeErrorVariant VariantType::subVariantNumber(int64_t n) {
  if (type == TypeData::TYPE_IS_NUMBER) {
    boost::multiprecision::cpp_int left = std::get<int64_t>(vardata);
    boost::multiprecision::cpp_int right = n;
    left -= right;
    if (left < minval || left > maxval) {
      return TypeErrorVariant::ERROR_ARITH_OVERFLOW;
    }
    vardata = left.convert_to<int64_t>();
    return TypeErrorVariant::NO_ERROR;
  }
  if (type == TypeData::TYPE_IS_DOUBLE) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double left = std::get<double>(vardata);
    left -= n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
    }
    auto status = isInfNumber(left);
    if (!isOkTypeReturnVariant(status)) return status;

    vardata = left;
    type = TypeData::TYPE_IS_DOUBLE;
    return TypeErrorVariant::NO_ERROR;
  }
  if (type == TypeData::TYPE_IS_BOOL) {
    boost::multiprecision::cpp_int left = std::get<int64_t>(vardata);
    boost::multiprecision::cpp_int right = n;
    left -= right;
    if (left < minval || left > maxval) {
      return TypeErrorVariant::ERROR_ARITH_OVERFLOW;
    }
    vardata = left.convert_to<int64_t>();
    type = TypeData::TYPE_IS_NUMBER;
    return TypeErrorVariant::NO_ERROR;
  }

  return TypeErrorVariant::ERROR_UNSUPPORTED_SUB_ARITH_TYPES;
}

TypeErrorVariant VariantType::divVariantNumber(int64_t n) {
  if (n == 0) {
    return TypeErrorVariant::ERROR_DIVIDE_BY_ZERO;
  }

  if (type == TypeData::TYPE_IS_NUMBER) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = (double)std::get<int64_t>(vardata) / n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
    }
    auto status = isInfNumber(ret);
    if (!isOkTypeReturnVariant(status)) return status;

    vardata = ret;
    type = TypeData::TYPE_IS_DOUBLE;
    return TypeErrorVariant::NO_ERROR;
  }
  if (type == TypeData::TYPE_IS_BOOL) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = (double)std::get<int64_t>(vardata) / n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
    }
    auto status = isInfNumber(ret);
    if (!isOkTypeReturnVariant(status)) return status;

    vardata = ret;
    type = TypeData::TYPE_IS_DOUBLE;
    return TypeErrorVariant::NO_ERROR;
  }
  if (type == TypeData::TYPE_IS_DOUBLE) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = (double)std::get<double>(vardata) / n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
    }
    auto status = isInfNumber(ret);
    if (!isOkTypeReturnVariant(status)) return status;

    vardata = ret;
    type = TypeData::TYPE_IS_DOUBLE;
    return TypeErrorVariant::NO_ERROR;
  }

  return TypeErrorVariant::ERROR_UNSUPPORTED_DIV_ARITH_TYPES;
}

TypeErrorVariant VariantType::addVariantBool(bool n) {
  if (type == TypeData::TYPE_IS_BOOL) {
    vardata = std::get<int64_t>(vardata) + n;
    type = TypeData::TYPE_IS_NUMBER;
    return TypeErrorVariant::NO_ERROR;
  }
  if (type == TypeData::TYPE_IS_DOUBLE) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = (double)std::get<double>(vardata) + n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
    }
    auto status = isInfNumber(ret);
    if (!isOkTypeReturnVariant(status)) return status;

    vardata = ret;
    type = TypeData::TYPE_IS_DOUBLE;
    return TypeErrorVariant::NO_ERROR;
  }
  if (type == TypeData::TYPE_IS_NUMBER) {
    boost::multiprecision::cpp_int left = std::get<int64_t>(vardata);
    left += n;
    if (left > maxval) {
      return TypeErrorVariant::ERROR_ARITH_OVERFLOW;
    }
    if (left < minval) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
    }
    vardata = left.convert_to<int64_t>();
    type = TypeData::TYPE_IS_NUMBER;
    return TypeErrorVariant::NO_ERROR;
  }

  return TypeErrorVariant::ERROR_UNSUPPORTED_ADD_ARITH_TYPES;
}

TypeErrorVariant VariantType::subVariantBool(bool n) {
  if (type == TypeData::TYPE_IS_BOOL) {
    vardata = std::get<int64_t>(vardata) - n;
    type = TypeData::TYPE_IS_NUMBER;
    return TypeErrorVariant::NO_ERROR;
  }
  if (type == TypeData::TYPE_IS_DOUBLE) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = (double)std::get<double>(vardata) - n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
    }
    auto status = isInfNumber(ret);
    if (!isOkTypeReturnVariant(status)) return status;

    vardata = ret;
    type = TypeData::TYPE_IS_DOUBLE;
    return TypeErrorVariant::NO_ERROR;
  }
  if (type == TypeData::TYPE_IS_NUMBER) {
    boost::multiprecision::cpp_int left = std::get<int64_t>(vardata);
    left -= n;
    if (left > maxval) {
      return TypeErrorVariant::ERROR_ARITH_OVERFLOW;
    }
    if (left < minval) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
    }
    vardata = left.convert_to<int64_t>();
    type = TypeData::TYPE_IS_NUMBER;
    return TypeErrorVariant::NO_ERROR;
  }

  return TypeErrorVariant::ERROR_UNSUPPORTED_SUB_ARITH_TYPES;
}

TypeErrorVariant VariantType::mulVariantBool(bool n) {
  if (type == TypeData::TYPE_IS_BOOL) {
    vardata = (int64_t)std::get<int64_t>(vardata) * n;
    type = TypeData::TYPE_IS_NUMBER;
    return TypeErrorVariant::NO_ERROR;
  }
  if (type == TypeData::TYPE_IS_DOUBLE) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = (double)std::get<double>(vardata) * n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
    }
    auto status = isInfNumber(ret);
    if (!isOkTypeReturnVariant(status)) return status;

    vardata = ret;
    type = TypeData::TYPE_IS_DOUBLE;
    return TypeErrorVariant::NO_ERROR;
  }
  if (type == TypeData::TYPE_IS_NUMBER) {
    boost::multiprecision::cpp_int left = std::get<int64_t>(vardata);
    left *= n;
    if (left > maxval) {
      return TypeErrorVariant::ERROR_ARITH_OVERFLOW;
    }
    if (left < minval) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
    }
    vardata = left.convert_to<int64_t>();
    type = TypeData::TYPE_IS_NUMBER;
    return TypeErrorVariant::NO_ERROR;
  }
  return TypeErrorVariant::ERROR_UNSUPPORTED_MUL_ARITH_TYPES;
}

TypeErrorVariant VariantType::divVariantBool(bool n) {
  if (n == 0) {
    return TypeErrorVariant::ERROR_DIVIDE_BY_ZERO;
  }
  if (type == TypeData::TYPE_IS_BOOL || type == TypeData::TYPE_IS_NUMBER ||
      type == TypeData::TYPE_IS_DOUBLE) {
    return TypeErrorVariant::NO_ERROR;
  }

  return TypeErrorVariant::ERROR_UNSUPPORTED_DIV_ARITH_TYPES;
}

TypeErrorVariant VariantType::addVariantDouble(double n) {
  if (type == TypeData::TYPE_IS_BOOL || type == TypeData::TYPE_IS_NUMBER) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = (double)std::get<int64_t>(vardata) + n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
    }
    auto status = isInfNumber(ret);
    if (!isOkTypeReturnVariant(status)) return status;

    vardata = ret;
    type = TypeData::TYPE_IS_DOUBLE;
    return TypeErrorVariant::NO_ERROR;
  }
  if (type == TypeData::TYPE_IS_DOUBLE) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = std::get<double>(vardata) + n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
    }
    auto status = isInfNumber(ret);
    if (!isOkTypeReturnVariant(status)) return status;

    vardata = ret;
    type = TypeData::TYPE_IS_DOUBLE;
    return TypeErrorVariant::NO_ERROR;
  }
  return TypeErrorVariant::ERROR_UNSUPPORTED_ADD_ARITH_TYPES;
}

TypeErrorVariant VariantType::subVariantDouble(double n) {
  if (type == TypeData::TYPE_IS_BOOL || type == TypeData::TYPE_IS_NUMBER) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = (double)std::get<int64_t>(vardata) - n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
    }
    auto status = isInfNumber(ret);
    if (!isOkTypeReturnVariant(status)) return status;

    type = TypeData::TYPE_IS_DOUBLE;
    vardata = ret;
    return TypeErrorVariant::NO_ERROR;
  }
  if (type == TypeData::TYPE_IS_DOUBLE) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = (double)std::get<double>(vardata) - n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
    }
    auto status = isInfNumber(ret);
    if (!isOkTypeReturnVariant(status)) return status;

    type = TypeData::TYPE_IS_DOUBLE;
    vardata = ret;
    return TypeErrorVariant::NO_ERROR;
  }
  return TypeErrorVariant::ERROR_UNSUPPORTED_SUB_ARITH_TYPES;
}

TypeErrorVariant VariantType::mulVariantDouble(double n) {
  if (type == TypeData::TYPE_IS_BOOL || type == TypeData::TYPE_IS_NUMBER) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = (double)std::get<int64_t>(vardata) * n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
    }
    auto status = isInfNumber(ret);
    if (!isOkTypeReturnVariant(status)) return status;

    type = TypeData::TYPE_IS_NUMBER;
    vardata = ret;
    return TypeErrorVariant::NO_ERROR;
  }
  if (type == TypeData::TYPE_IS_DOUBLE) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = std::get<double>(vardata) * n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
    }
    auto status = isInfNumber(ret);
    if (!isOkTypeReturnVariant(status)) return status;

    type = TypeData::TYPE_IS_DOUBLE;
    vardata = ret;
    return TypeErrorVariant::NO_ERROR;
  }
  return TypeErrorVariant::ERROR_UNSUPPORTED_MUL_ARITH_TYPES;
}

TypeErrorVariant VariantType::divVariantDouble(double n) {
  if (n == 0) {
    return TypeErrorVariant::ERROR_DIVIDE_BY_ZERO;
  }
  if (type == TypeData::TYPE_IS_BOOL || type == TypeData::TYPE_IS_NUMBER) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = (double)std::get<int64_t>(vardata) / n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
    }
    auto status = isInfNumber(ret);
    if (!isOkTypeReturnVariant(status)) return status;

    type = TypeData::TYPE_IS_DOUBLE;
    vardata = ret;
    return TypeErrorVariant::NO_ERROR;
  }
  if (type == TypeData::TYPE_IS_DOUBLE) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = (double)std::get<double>(vardata) / n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
    }
    auto status = isInfNumber(ret);
    if (!isOkTypeReturnVariant(status)) return status;

    type = TypeData::TYPE_IS_DOUBLE;
    vardata = ret;
    return TypeErrorVariant::NO_ERROR;
  }
  return TypeErrorVariant::ERROR_UNSUPPORTED_DIV_ARITH_TYPES;
}

TypeErrorVariant VariantType::addVariantString(const std::string &n) {
  if (type == TypeData::TYPE_IS_STRING) {
    std::string ret = std::get<std::string>(vardata);
    ret += n;
    return TypeErrorVariant::NO_ERROR;
  }
  return TypeErrorVariant::ERROR_UNSUPPORTED_ADD_STRING_TYPES;
}

TypeErrorVariant VariantType::mulVariantNumber(int64_t n) {
  if (type == TypeData::TYPE_IS_NUMBER) {
    boost::multiprecision::cpp_int left = std::get<int64_t>(vardata);
    boost::multiprecision::cpp_int right = n;
    left *= right;
    if (left < minval || left > maxval) {
      return TypeErrorVariant::ERROR_ARITH_OVERFLOW;
    }
    type = TypeData::TYPE_IS_NUMBER;
    vardata = left.convert_to<int64_t>();
    return TypeErrorVariant::NO_ERROR;
  }
  if (type == TypeData::TYPE_IS_DOUBLE) {
    std::feclearexcept(FE_ALL_EXCEPT);
    double ret = (double)std::get<double>(vardata) * n;
    if (std::fetestexcept(FE_OVERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW;
    }
    if (std::fetestexcept(FE_UNDERFLOW)) {
      return TypeErrorVariant::ERROR_ARITH_DOUBLE_UNDERFLOW;
    }
    auto status = isInfNumber(ret);
    if (!isOkTypeReturnVariant(status)) return status;

    vardata = ret;
    type = TypeData::TYPE_IS_DOUBLE;
    return TypeErrorVariant::NO_ERROR;
  }
  if (type == TypeData::TYPE_IS_BOOL) {
    boost::multiprecision::cpp_int left = std::get<int64_t>(vardata);
    boost::multiprecision::cpp_int right = n;
    left *= right;
    if (left < minval || left > maxval) {
      return TypeErrorVariant::ERROR_ARITH_OVERFLOW;
    }
    vardata = left.convert_to<int64_t>();
    type = TypeData::TYPE_IS_NUMBER;
    return TypeErrorVariant::NO_ERROR;
  }

  return TypeErrorVariant::ERROR_UNSUPPORTED_ADD_ARITH_TYPES;
}

VariantType::~VariantType() {}

TypeData VariantType::getType() const { return type; }

Var VariantType::getVar() const { return vardata; }

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

bool VariantType::isNull() const { return type == TypeData::TYPE_IS_UNDEFINED; }

void VariantType::setNull() {
  vardata = std::monostate();
  type = TypeData::TYPE_IS_UNDEFINED;
}

void VariantType::intVar() {
  minval = std::numeric_limits<int64_t>::min();
  maxval = std::numeric_limits<int64_t>::max();
}

void VariantType::doClone(const VariantType &anotherVar) {
  intVar();
  switch (anotherVar.getType()) {
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

TypeErrorVariant VariantType::isInfNumber(double d) {
  return std::isinf(d) ? TypeErrorVariant::ERROR_ARITH_DOUBLE_INFINITY
                       : TypeErrorVariant::NO_ERROR;
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
      if (!ok) break;
    }
  }
  return ok;
}

std::ostream &operator<<(std::ostream &os, const VariantType &var) {
  switch (var.getType()) {
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
      os << " " << std::boolalpha << std::get<int64_t>(var.getVar()) << " ";
      break;
    default:
      os << " [undefined value] ";
      break;
  }
  return os;
}

bool operator==(VariantType &var, bool e) {
  bool ok = false;
  switch (var.getType()) {
    case TypeData::TYPE_IS_BOOL:
      ok = (std::get<int64_t>(var.getVar()) == (int64_t)e);
      break;
    default:
      break;
  }

  return ok;
}

bool operator==(VariantType &var, const std::string &e) {
  bool ok = false;
  switch (var.getType()) {
    case TypeData::TYPE_IS_STRING:
      ok = (std::get<std::string>(var.getVar()) == e);
      break;
    default:
      break;
  }
  return ok;
}

bool operator==(VariantType &var, int64_t e) {
  bool ok = false;
  switch (var.getType()) {
    case TypeData::TYPE_IS_NUMBER:
      ok = (std::get<int64_t>(var.getVar()) == e);
      break;
    default:
      break;
  }
  return ok;
}

bool operator==(VariantType &var, double e) {
  bool ok = false;
  switch (var.getType()) {
    case TypeData::TYPE_IS_DOUBLE:
      ok = (std::get<double>(var.getVar()) == e);
      break;
    default:
      break;
  }
  return ok;
}

bool isOkTypeReturnVariant(const TypeErrorVariant &type) {
  return type == TypeErrorVariant::NO_ERROR;
}

bool operator==(const VariantType &var1, const VariantType &var2) {
  bool ok = var1.getType() == var2.getType();
  if (ok) {
    ok = var1.getVar() == var2.getVar();
  }
  return ok;
}

VariantTypeList::VariantTypeList() {}

VariantTypeList::VariantTypeList(
    const std::initializer_list<VariantType> &vecInit)
    : vectordata(vecInit) {}

VariantTypeList::~VariantTypeList() { clear(); }

VariantTypeList::VariantTypeList(VariantTypeList &&other)
    : VariantTypeList(other) {
  other.clear();
}

VariantTypeList::VariantTypeList(const VariantTypeList &another) {
  for (const auto &val : another.getVector()) {
    vectordata.push_back(val);
  }
}

VariantTypeList &VariantTypeList::operator<<(const VariantType &var) {
  vectordata.push_back(var);
  return *this;
}

size_t VariantTypeList::size() const { return vectordata.size(); }

std::pair<bool, VariantType> VariantTypeList::at(size_t index) const {
  if (exists(index)) {
    return std::make_pair(true, vectordata.at(index));
  }
  return std::make_pair(false, VariantType());
}

bool VariantTypeList::setValue(size_t index, const VariantType &value) {
  if (exists(index)) {
    vectordata[index] = value;
    return true;
  }
  return false;
}

bool VariantTypeList::deleteValue(size_t index) {
  if (exists(index)) {
    vectordata.erase(vectordata.begin() + index);
    return true;
  }
  return false;
}

void VariantTypeList::clear() { vectordata.clear(); }

bool VariantTypeList::exists(size_t index) const {
  return (index < vectordata.size() && index >= 0);
}

std::pair<bool, size_t> VariantTypeList::exists(const VariantType &var) const {
  size_t index = 0;
  for (const auto &val : vectordata) {
    if (val == var) {
      return std::make_pair(true, index);
    }
    index++;
  }
  return std::make_pair(false, index);
}

VectorData VariantTypeList::getVector() const { return vectordata; }

bool operator==(const VariantTypeList &var1, const VariantTypeList &var2) {
  bool ok = var1.size() == var2.size();
  if (!ok) return ok;
  for (size_t x = 0; x < var1.size(); x++) {
    ok = var1.at(x) == var2.at(x);
    if (!ok) break;
  }
  return ok;
}

VariantTypeNDimArray::VariantTypeNDimArray(size_t ndim) : maxdim(ndim) {}

VariantTypeNDimArray::VariantTypeNDimArray(
    const std::initializer_list<VariantTypeList> &vars)
    : maxdim(vars.size()), ndimvector(vars) {}

VariantTypeNDimArray::VariantTypeNDimArray(VariantTypeNDimArray &&other)
    : VariantTypeNDimArray(other) {
  other.clear();
}

VariantTypeNDimArray::VariantTypeNDimArray(
    const VariantTypeNDimArray &anotherVar)
    : maxdim(anotherVar.getNDimVectorVariant().size()),ndimvector(anotherVar.getNDimVectorVariant()) {}

VariantTypeNDimArray::~VariantTypeNDimArray() { ndimvector.clear(); }

VariantTypeNDimArray &VariantTypeNDimArray::operator<<(
    const VariantTypeList &listColumn) {
  if (ndimvector.size() != maxdim) {
    ndimvector.push_back(listColumn);
  }
  return *this;
}

std::pair<bool, VariantType> VariantTypeNDimArray::getValue(size_t col,
                                                            size_t row) const {
  if (isValidColRowInMultiDimArray(col, row)) {
    return std::make_pair(true, ndimvector[col].at(row).second);
  }
  return std::make_pair(false, VariantType());
}

bool VariantTypeNDimArray::setValue(size_t col, size_t row,
                                    const VariantType &value) {
  if (isValidColRowInMultiDimArray(col, row)) {
    ndimvector[col].setValue(row, value);
    return true;
  }
  return false;
}

void VariantTypeNDimArray::clear() { ndimvector.clear(); }

bool VariantTypeNDimArray::deleteValue(size_t col, size_t row) {
  if (isValidColRowInMultiDimArray(col, row)) {
    ndimvector[col].deleteValue(row);
    return true;
  }
  return false;
}

size_t VariantTypeNDimArray::rowSize(size_t col) const {
  size_t num = -1;
  if (col < ndimvector.size() && col >= 0) {
    num = ndimvector.at(col).size();
  }
  return num;
}

size_t VariantTypeNDimArray::colSize() const { return ndimvector.size(); }

NDimVectorVariant VariantTypeNDimArray::getNDimVectorVariant() const {
  return ndimvector;
}

bool VariantTypeNDimArray::isValidColRowInMultiDimArray(size_t col,
                                                        size_t row) const {
  bool ok = col < ndimvector.size() && col >= 0;
  if (ok) {
    ok = row < ndimvector.at(col).size() && row >= 0;
  }
  return ok;
}

bool operator==(const VariantTypeNDimArray &var1,
                const VariantTypeNDimArray &var2) {
  bool ok = var1.colSize() == var2.colSize();
  if (!ok) return ok;

  for (size_t x = 0; x < var1.colSize(); x++) {
    ok = var1.rowSize(x) == var2.rowSize(x);
    if (!ok || var1.rowSize(x) == (size_t)-1 || var2.rowSize(x) == (size_t)-1) {
      ok = false;
      break;
    }
    for (size_t xx = 0; xx < var2.rowSize(x); xx++) {
      ok = var1.getValue(x, xx).second == var2.getValue(x, xx).second;
      if (!ok) return ok;
    }
  }
  return ok;
}

bool operator!=(const VariantTypeNDimArray &var1,
                const VariantTypeNDimArray &var2) {
  return (var1 == var2) == false;
}

bool operator!=(const VariantTypeList &var1, const VariantTypeList &var2) {
  return (var1 == var2) == false;
}

bool operator!=(const VariantType &var1, const VariantType &var2) {
  return (var1 == var2) == false;
}

std::ostream &operator<<(std::ostream &os, const VariantTypeList &var) {
  os << " ";
  for (const auto &item : var.getVector()) {
    os << item << " ";
  }
  os << "\n";
  return os;
}

std::ostream &operator<<(std::ostream &os, const VariantTypeNDimArray &var) {
  os << "\n";
  size_t colnum = 0;
  for (const VariantTypeList &col : var.getNDimVectorVariant()) {
    colnum += 1;
    os << "\n[" << colnum << "] ";
    os << col;
  }
  os << "\n";
  return os;
}

}  // namespace val
