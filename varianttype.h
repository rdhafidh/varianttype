#ifndef VARIANTTYPE_H
#define VARIANTTYPE_H

#include <boost/multiprecision/cpp_int.hpp>
#include <exception>
#include <initializer_list>
#include <iostream>
#include <numeric>
#include <variant>
#include <vector>

namespace val {

typedef std::variant<std::monostate, int64_t, std::string, double> Var;

enum class TypeData {
  TYPE_IS_UNDEFINED,
  TYPE_IS_STRING,
  TYPE_IS_NUMBER,
  TYPE_IS_BOOL,
  TYPE_IS_DOUBLE
};

enum class TypeErrorVariant {
  ERROR_UNDEFINED,
  ERROR_ARITH_OVERFLOW,
  ERROR_UNSUPPORTED_ADD_ARITH_TYPES,
  ERROR_UNSUPPORTED_SUB_ARITH_TYPES,
  ERROR_UNSUPPORTED_MUL_ARITH_TYPES,
  ERROR_UNSUPPORTED_DIV_ARITH_TYPES,
  ERROR_UNSUPPORTED_ADD_STRING_TYPES,
  ERROR_DIVIDE_BY_ZERO,
  ERROR_ARITH_DOUBLE_OVERFLOW,
  ERROR_ARITH_DOUBLE_UNDERFLOW,
  ERROR_ARITH_DOUBLE_INFINITY,
  NO_ERROR
};

bool isOkTypeReturnVariant(const TypeErrorVariant &type);

class VariantType;

typedef std::vector<VariantType> VectorData;

bool operator==(const Var &f1, const Var &f2);
bool operator==(const VectorData &vec1, const VectorData &vec2);
bool operator==(const VariantType &var1, const VariantType &var2);
bool operator!=(const VariantType &var1, const VariantType &var2);

class VariantType {
 public:
  VariantType();

  VariantType(bool e);
  VariantType(int64_t e);
  VariantType(const std::string &e);
  VariantType(const VariantType &anotherVar);
  VariantType(double n);
  VariantType &operator=(const VariantType &anotherVar);
  bool operator==(const VariantType &anotherVar);
  bool operator!=(const VariantType &anotherVar);
  VariantType &operator<<(const std::string &str);
  VariantType &operator<<(const char *buf);

  TypeErrorVariant addVariantNumber(int64_t n);
  TypeErrorVariant subVariantNumber(int64_t n);
  TypeErrorVariant mulVariantNumber(int64_t n);
  TypeErrorVariant divVariantNumber(int64_t n);

  TypeErrorVariant addVariantBool(bool n);
  TypeErrorVariant subVariantBool(bool n);
  TypeErrorVariant mulVariantBool(bool n);
  TypeErrorVariant divVariantBool(bool n);

  TypeErrorVariant addVariantDouble(double n);
  TypeErrorVariant subVariantDouble(double n);
  TypeErrorVariant mulVariantDouble(double n);
  TypeErrorVariant divVariantDouble(double n);

  TypeErrorVariant addVariantString(const std::string &n);

  ~VariantType();
  TypeData getType() const;
  Var getVar() const;
  bool isNull() const;
  void setNull();

  static int64_t makeNumber(int n);
  static double makeDouble(float n);

 private:
  void intVar();
  void doClone(const VariantType &anotherVar);
  TypeErrorVariant isInfNumber(double d);

  Var vardata;
  boost::multiprecision::cpp_int minval;
  boost::multiprecision::cpp_int maxval;
  TypeData type;
};

std::ostream &operator<<(std::ostream &os, const VariantType &var);

bool operator==(VariantType &var, bool e);
bool operator==(VariantType &var, const std::string &e);
bool operator==(VariantType &var, int64_t e);
bool operator==(VariantType &var, double e);
bool operator==(VariantType &var, const VectorData &vec);

class VariantTypeList {
 public:
  VariantTypeList();
  VariantTypeList(const std::initializer_list<VariantType> &vecInit);
  ~VariantTypeList();
  VariantTypeList(VariantTypeList &&other);
  VariantTypeList(const VariantTypeList &another);
  VariantTypeList &operator=(const VariantTypeList &) = default;
  VariantTypeList &operator<<(const VariantType &var);

  size_t size() const;
  std::pair<bool, VariantType> at(size_t index) const;
  bool setValue(size_t index, const VariantType &value);
  bool deleteValue(size_t index);
  void clear();
  bool exists(size_t index) const;
  std::pair<bool, size_t> exists(const val::VariantType &var) const;
  VectorData getVector() const;

 private:
  VectorData vectordata;
};

bool operator==(const VariantTypeList &var1, const VariantTypeList &var2);
bool operator!=(const VariantTypeList &var1, const VariantTypeList &var2);

typedef std::vector<VariantTypeList> NDimVectorVariant;

class VariantTypeNDimArray {
 public:
  VariantTypeNDimArray(size_t ndim);
  VariantTypeNDimArray(const std::initializer_list<VariantTypeList> &vars);
  VariantTypeNDimArray(VariantTypeNDimArray &&other);
  VariantTypeNDimArray &operator=(const VariantTypeNDimArray &) = default;
  VariantTypeNDimArray(const VariantTypeNDimArray &anotherVar);
  ~VariantTypeNDimArray();
  VariantTypeNDimArray &operator<<(const VariantTypeList &listColumn);
  std::pair<bool, VariantType> getValue(size_t col, size_t row) const;
  bool setValue(size_t col, size_t row, const VariantType &value);
  void clear();
  bool deleteValue(size_t col, size_t row);
  size_t rowSize(size_t col) const;
  size_t colSize() const;

  NDimVectorVariant getNDimVectorVariant() const;

 private:
  bool isValidColRowInMultiDimArray(size_t col, size_t row) const;
  size_t maxdim = 0;
  NDimVectorVariant ndimvector;
};
bool operator==(const VariantTypeNDimArray &var1,
                const VariantTypeNDimArray &var2);
bool operator!=(const VariantTypeNDimArray &var1,
                const VariantTypeNDimArray &var2);

std::ostream &operator<<(std::ostream &os, const VariantTypeList &var);
std::ostream &operator<<(std::ostream &os, const VariantTypeNDimArray &var);

}  // namespace val
#endif  // VARIANTTYPE_H
