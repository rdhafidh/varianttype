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
  TYPE_IS_ARRAY,
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
  NO_ERROR
};

bool isOkTypeReturnVariant(const TypeErrorVariant &type);

class VariantType;

typedef std::vector<VariantType> VectorData;

bool operator==(const Var &f1, const Var &f2);
bool operator==(const VectorData &vec1, const VectorData &vec2);
bool operator==(const VariantType &var1, const VariantType &var2);

class VariantType {
 public:
  VariantType();

  VariantType(bool e);
  VariantType(int64_t e);
  VariantType(const std::string &e);
  VariantType(const VectorData &vec);
  VariantType(const VariantType &anotherVar);
  VariantType(double n);
  VariantType &operator=(const VariantType &anotherVar);
  bool operator==(const VariantType &anotherVar);
  bool operator!=(const VariantType &anotherVar);
  VariantType &operator<<(const std::string &str);
  VariantType &operator<<(const char *buf);
  VariantType &operator<<(const VariantType &anotherVar);

  bool addVariantNumber(int64_t n, TypeErrorVariant *errorFlag);
  bool subVariantNumber(int64_t n, TypeErrorVariant *errorFlag);
  bool mulVariantNumber(int64_t n, TypeErrorVariant *errorFlag);
  bool divVariantNumber(int64_t n, TypeErrorVariant *errorFlag);

  bool addVariantBool(bool n, TypeErrorVariant *errorFlag);
  bool subVariantBool(bool n, TypeErrorVariant *errorFlag);
  bool mulVariantBool(bool n, TypeErrorVariant *errorFlag);
  bool divVariantBool(bool n, TypeErrorVariant *errorFlag);

  bool addVariantDouble(double n, TypeErrorVariant *errorFlag);
  bool subVariantDouble(double n, TypeErrorVariant *errorFlag);
  bool mulVariantDouble(double n, TypeErrorVariant *errorFlag);
  bool divVariantDouble(double n, TypeErrorVariant *errorFlag);

  bool addVariantString(const std::string &n, TypeErrorVariant *errorFlag);

  ~VariantType();

  TypeData getType() const;

  Var getVar() const;

  VectorData getVector() const;

  bool isNull() const;
  void setNull();

  static int64_t makeNumber(int n);
  static double makeDouble(float n);

 private:
  void intVar();

  Var vardata;
  VectorData vectordata;
  boost::multiprecision::cpp_int minval;
  boost::multiprecision::cpp_int maxval;
  TypeData type;
};

std::ostream &operator<<(std::ostream &os, const VariantType &var);

bool operator==(VariantType &var, bool e);
bool operator==(VariantType &var, const std::string &e);
bool operator==(VariantType &var, const char *e);
bool operator==(VariantType &var, int64_t e);
bool operator==(VariantType &var, double e);
bool operator==(VariantType &var, const VectorData &vec);

}  // namespace val
#endif  // VARIANTTYPE_H