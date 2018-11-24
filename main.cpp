#include <gtest/gtest.h>
#include <varianttype.h>
#include <iostream>
#include <numeric>

TEST(Undefined1, Undef) {
  using namespace val;
  VariantType var;
  EXPECT_EQ(TypeData::TYPE_IS_UNDEFINED, var.getType());
  EXPECT_EQ((size_t)0, var.getVar().index());
} 
 

TEST(CompareTest, Compare1) {
  using namespace val;
  VariantType var;
  VariantType rval;
  EXPECT_EQ(true, var == rval);
}

TEST(CompareTest, CompareBool1) {
  using namespace val;
  VariantType var(false);
  VariantType rval(true);
  EXPECT_EQ(false, var == rval);
  EXPECT_EQ(val::TypeData::TYPE_IS_BOOL, var.getType());
  EXPECT_EQ(val::TypeData::TYPE_IS_BOOL, rval.getType());
  VariantType vt1(false);
  EXPECT_EQ(true, VariantType(false) == vt1);
}
 

TEST(CompareTest, ComparePlainOldData) {
  using namespace val;
  using namespace std::string_literals;
  VariantType vt1(false);
  VariantType vt2(std::string("foo"));
  EXPECT_EQ(true, vt1 == false);
  EXPECT_EQ(false, vt1 == "foo bar"s);
  EXPECT_EQ(true, vt2 == "foo"s);
  EXPECT_EQ(true, vt2 == std::string("foo"));
  EXPECT_EQ(true, VariantType((int64_t)12321312) == (int64_t)12321312);
  EXPECT_EQ(true, VariantType(31.414) == 31.414);

  EXPECT_EQ(true, VariantTypeList() == VariantTypeList());
  EXPECT_EQ(false, VariantTypeList({VariantType(VariantType::makeNumber(100)),
                                    VariantType(VariantType::makeNumber(23)),
                                    VariantType(std::string("false"))}) ==
                       VariantTypeList({VariantType(std::string("false"))}));
  EXPECT_EQ(true,
            VariantTypeList({VariantType(VariantType::makeNumber(100)),
                             VariantType(VariantType::makeNumber(23)),
                             VariantType(std::string("false"))}) ==
                VariantTypeList({VariantType(VariantType::makeNumber(100)),
                                 VariantType(VariantType::makeNumber(23)),
                                 VariantType(std::string("false"))}));
  EXPECT_EQ(true,
            VariantTypeList({VariantType(VariantType::makeNumber(100)),
                             VariantType(VariantType::makeNumber(23)),
                             VariantType(std::string("false"))}) ==
                VariantTypeList({VariantType(VariantType::makeNumber(100)),
                                 VariantType(VariantType::makeNumber(23)),
                                 VariantType(std::string("false"))}));
  EXPECT_EQ(false,
            VariantTypeList({VariantType(VariantType::makeNumber(100)),
                             VariantType(VariantType::makeNumber(23)),
                             VariantType(std::string("false"))}) ==
                VariantTypeList({VariantType(VariantType::makeNumber(100)),
                                 VariantType(VariantType::makeNumber(23)),
                                 VariantType(std::string("")), VariantType()}));

  auto items = {VariantType(std::string("foo bar")), VariantType(true)};
  VectorData arrayItem(items);
  EXPECT_EQ((decltype(arrayItem.size()))2, arrayItem.size());
  EXPECT_EQ(TypeData::TYPE_IS_STRING, arrayItem.at(0).getType());
  EXPECT_EQ(TypeData::TYPE_IS_BOOL, arrayItem.at(1).getType());
}

TEST(CompareTest, CompareNumber1) {
  using namespace val;
  VariantType var(VariantType::makeNumber(21));
  VariantType rval(VariantType::makeNumber(21));
  EXPECT_EQ(true, var == rval);
  EXPECT_EQ(val::TypeData::TYPE_IS_NUMBER, var.getType());
  EXPECT_EQ(val::TypeData::TYPE_IS_NUMBER, rval.getType());
}

TEST(CompareTest, CompareNumber2) {
  using namespace val;
  int64_t num = (int64_t)std::numeric_limits<int>::max() + 21313;
  VariantType var((int64_t)std::numeric_limits<int>::max() + 21313);
  VariantType rval(num);
  EXPECT_EQ(true, var == rval);
  EXPECT_EQ(val::TypeData::TYPE_IS_NUMBER, var.getType());
  EXPECT_EQ(val::TypeData::TYPE_IS_NUMBER, rval.getType());
}
TEST(CompareTest, CompareNumber3) {
  using namespace val;
  int num = 1231;
  VariantType var(VariantType::makeNumber(1231));
  VariantType rval(VariantType::makeNumber(num));
  EXPECT_EQ(true, var == rval);
  EXPECT_EQ(val::TypeData::TYPE_IS_NUMBER, var.getType());
  EXPECT_EQ(val::TypeData::TYPE_IS_NUMBER, rval.getType());
}

TEST(CompareTest, CompareDouble1) {
  using namespace val;
  float num = 1231.3f;
  VariantType var(VariantType::makeDouble(1231.3f));
  VariantType anval(1231.3);
  VariantType rval(VariantType::makeDouble(num));
  EXPECT_EQ(true, var == rval);
  EXPECT_EQ(false, anval == rval);
  EXPECT_EQ(val::TypeData::TYPE_IS_DOUBLE, var.getType());
  EXPECT_EQ(val::TypeData::TYPE_IS_DOUBLE, anval.getType());
  EXPECT_EQ(val::TypeData::TYPE_IS_DOUBLE, rval.getType());

  // anval==rval -> false
  // same as this one
  float numf = 231.2f;
  double dnum = (double)numf;
  double numd = 231.2;
  EXPECT_EQ(false, dnum == numd);
  EXPECT_EQ(true, dnum == VariantType::makeDouble(numf));
}

TEST(String, StringAppend) {
  using namespace val;
  using namespace std::string_literals;
  VariantType str = std::string("foo bar");
  str << std::string("!") << "!!!";
  EXPECT_EQ(true, str == "foo bar!!!!"s);
  VariantType nonStr = (int64_t)1234;
  nonStr << "foo bar"
         << " test";
  EXPECT_EQ(TypeData::TYPE_IS_NUMBER, nonStr.getType());
  EXPECT_EQ(true, nonStr == (int64_t)1234);
}

TEST(AssignVar, Assign) {
  using namespace val;
  using namespace std::string_literals;
  VariantTypeList var(VariantTypeList()
                      << VariantType((bool)false) << VariantType((int64_t)30)
                      << VariantType("from"s) << VariantType(3.43));
  EXPECT_EQ((size_t)4, var.size());
  VariantTypeList lval = var;
  EXPECT_EQ(true, lval == var);
  VariantTypeList vArr;
  vArr << VariantType((int64_t)10) << VariantType("foo"s);
  EXPECT_EQ(true, vArr.size() == 2);
  EXPECT_EQ(true, vArr.at(1).second == "foo"s);
}

TEST(AssignVar, SelfAssign) {
  using namespace val;
  using namespace std::string_literals;
  VariantTypeList var(VariantTypeList()
                      << VariantType((bool)false) << VariantType((int64_t)30)
                      << VariantType("from"s) << VariantType(3.43));
  var = var;
  EXPECT_EQ((size_t)4, var.size());
  EXPECT_EQ(true, var.at(0).second.getType() == TypeData::TYPE_IS_BOOL);
  EXPECT_EQ(true, var.at(1).second == (int64_t)30);
  EXPECT_EQ(true, var.at(2).second == "from"s);
  EXPECT_EQ(true, var.at(3).second == 3.43);
}

TEST(TestAdd, TestAddNumber1) {
  using namespace val;
  VariantType var(VariantType::makeNumber(30));
  VariantType varEnd((int64_t)30 + std::numeric_limits<int>::max());
  TypeErrorVariant e = var.addVariantNumber(
      VariantType::makeNumber(std::numeric_limits<int>::max()));
  EXPECT_EQ(true, isOkTypeReturnVariant(e));
  EXPECT_EQ(true, var == varEnd);
}

TEST(TestAdd, TestAddNumberOverflow) {
  using namespace val;
  VariantType var(-std::numeric_limits<int64_t>::max());
  int64_t asliVar = std::get<int64_t>(var.getVar());
  EXPECT_EQ(true, var == asliVar);

  TypeErrorVariant e =
      var.addVariantNumber(-std::numeric_limits<int64_t>::max());

  asliVar += std::numeric_limits<int64_t>::max();
  EXPECT_EQ(false, isOkTypeReturnVariant(e));
  EXPECT_EQ(false, var == asliVar);
  EXPECT_EQ(true, TypeErrorVariant::ERROR_ARITH_OVERFLOW == e);
}

TEST(TestAdd, TestAddNumberOverflow2) {
  using namespace val;
  VariantType var(VariantType::makeNumber(-30));
  TypeErrorVariant e =
      var.addVariantNumber(-std::numeric_limits<int64_t>::max());
  EXPECT_EQ(false, isOkTypeReturnVariant(e));
  EXPECT_EQ(true, TypeErrorVariant::ERROR_ARITH_OVERFLOW == e);
  EXPECT_EQ(true, var == (int64_t)-30);
}

TEST(TestAdd, TestAddDoubleOverflow) {
  using namespace val;
  VariantType var((double)std::numeric_limits<double>::max());
  TypeErrorVariant e = var.addVariantDouble(std::numeric_limits<double>::max());
  EXPECT_EQ(false, isOkTypeReturnVariant(e));
  EXPECT_EQ(true, TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW == e);
  EXPECT_EQ(true, var == (double)std::numeric_limits<double>::max());
}

TEST(TestAdd, TestAddDoubleOverflow2) {
  using namespace val;
  VariantType var((double)-std::numeric_limits<double>::max());
  TypeErrorVariant e =
      var.addVariantDouble(-std::numeric_limits<double>::max());
  EXPECT_EQ(false, isOkTypeReturnVariant(e));
  EXPECT_EQ(true, TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW == e);
  EXPECT_EQ(true, var == (double)-std::numeric_limits<double>::max());
}

TEST(TestAdd, TestAddDouble) {
  using namespace val;
  VariantType var((double)-std::numeric_limits<int>::max());
  double asliVar = std::get<double>(var.getVar());
  EXPECT_EQ(true, var == asliVar);

  TypeErrorVariant e =
      var.addVariantDouble(-std::numeric_limits<double>::max());

  asliVar += -std::numeric_limits<double>::max();
  EXPECT_EQ(true, isOkTypeReturnVariant(e));
  EXPECT_EQ(true, var == asliVar);
  EXPECT_EQ(true, TypeErrorVariant::NO_ERROR == e);
}

TEST(TestSub, TestSubNumber) {
  using namespace val;
  VariantType var((int64_t)std::numeric_limits<int64_t>::max());

  TypeErrorVariant e =
      var.subVariantNumber(-std::numeric_limits<int64_t>::max());

  EXPECT_EQ(false, isOkTypeReturnVariant(e));
  EXPECT_EQ(true, TypeErrorVariant::ERROR_ARITH_OVERFLOW == e);
  e = var.subVariantNumber(std::numeric_limits<int>::max());
  EXPECT_EQ(true, TypeErrorVariant::NO_ERROR == e);
  EXPECT_EQ(true, isOkTypeReturnVariant(e));
  EXPECT_EQ(true, var == (int64_t)(std::numeric_limits<int64_t>::max() -
                                   std::numeric_limits<int>::max()));
  e=var.subVariantDouble (std::numeric_limits<double>::infinity ());
  EXPECT_EQ(true, e==TypeErrorVariant::ERROR_ARITH_DOUBLE_INFINITY);
}

TEST(TestSub, TestSubBoolean) {
  using namespace val;
  VariantType var((bool)true);

  TypeErrorVariant e =
      var.subVariantNumber(std::numeric_limits<int64_t>::min());

  EXPECT_EQ(false, isOkTypeReturnVariant(e));
  EXPECT_EQ(true, TypeErrorVariant::ERROR_ARITH_OVERFLOW == e);
  e = var.subVariantNumber(std::numeric_limits<int>::min());
  EXPECT_EQ(true, TypeErrorVariant::NO_ERROR == e);
  EXPECT_EQ(true, isOkTypeReturnVariant(e));
  int64_t res = std::numeric_limits<int>::max();
  res += 2;
  EXPECT_EQ(true, var == res);
}

TEST(TestSub, TestSubDouble) {
  using namespace val;
  VariantType var(3.14);

  TypeErrorVariant e = var.subVariantDouble(std::numeric_limits<double>::min());

  EXPECT_EQ(true, isOkTypeReturnVariant(e));
  EXPECT_EQ(true, TypeErrorVariant::NO_ERROR == e);
  double res = std::get<double>(var.getVar());
  e = var.subVariantNumber(std::numeric_limits<int>::min());
  EXPECT_EQ(true, TypeErrorVariant::NO_ERROR == e);
  EXPECT_EQ(true, isOkTypeReturnVariant(e));
  res -= std::numeric_limits<int>::min();
  EXPECT_EQ(true, var == res); 
  e = var.subVariantDouble(std::numeric_limits<double>::max ()); 
  EXPECT_EQ(true, isOkTypeReturnVariant (e));
  e=var.subVariantDouble (std::numeric_limits<double>::max());
  EXPECT_EQ(true,e==TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW);
  res -=std::numeric_limits<double>::max ();
  EXPECT_EQ(true,var==res);
}

TEST(TestMul, TestMulDouble) { 
    using namespace val; 
    VariantType var(25.05);
    double dvar=std::get<double>(var.getVar ());
    TypeErrorVariant e=var.mulVariantNumber (3);
    EXPECT_EQ(true, e==TypeErrorVariant::NO_ERROR);
    dvar *=3;
    EXPECT_EQ(true,var==dvar); 
    e=var.mulVariantDouble (std::numeric_limits<double>::infinity ());
    EXPECT_EQ(true,var==dvar);
    EXPECT_EQ(true, e==TypeErrorVariant::ERROR_ARITH_DOUBLE_INFINITY);
    e=var.mulVariantDouble (std::numeric_limits<double>::max ());
    EXPECT_EQ(true,e==TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW);
    EXPECT_EQ(true,var==dvar);
}

TEST(ArrayPrintTest, PrintArray) {
  using namespace val;
  using namespace std::string_literals;
  VariantTypeList var(VariantTypeList()
                      << (bool)false << (int64_t)30
                      << "from"s << VariantType(3.43));
  EXPECT_EQ((size_t)4, var.size());
  EXPECT_EQ(true, var.at(0).second.getType() == TypeData::TYPE_IS_BOOL);
  EXPECT_EQ(true, var.at(1).second == (int64_t)30);
  EXPECT_EQ(true, var.at(2).second == "from"s);
  EXPECT_EQ(true, var.at(3).second == 3.43); 
  VariantTypeNDimArray dim(3);
  dim << var
      << (VariantTypeList() << "foo"s
                            << "bar"s << VariantType())
      << VariantTypeList() << (VariantTypeList() << "nothings"s);

  std::cout << "\ndim:" << dim;
  EXPECT_EQ(true, dim.colSize() == 3);
  EXPECT_EQ(true, dim.rowSize(2) == 0);
}


TEST(NDimArrayTest, test) {
  using namespace val;
  using namespace std::string_literals;
  VariantTypeNDimArray dim1(3);
  VariantTypeList var;
  var << "foo"s
      << "food"s
      << "feeding"s << (bool)true;
  dim1 << var
       << (VariantTypeList() << "foo"s
                             << "bar"s << VariantType())
       << VariantTypeList() << (VariantTypeList() << "nothings"s);
  VariantTypeNDimArray dim2(3);
  dim2 << var
       << (VariantTypeList() << "foo"s
                             << "bar"s << VariantType())
       << VariantTypeList() << (VariantTypeList() << "nothings"s);
  EXPECT_EQ(true, dim1 == dim2);
  VariantTypeNDimArray dim3 = dim2;
  EXPECT_EQ(true, dim3 == dim2);
  EXPECT_EQ(true, dim3.colSize() == 3);
  EXPECT_EQ(true, dim3.setValue(0, 1, "eat"s));
  EXPECT_EQ(true, dim3.getValue(0, 1).second == "eat"s);
  EXPECT_EQ(true, dim3 != dim2);
  EXPECT_EQ(true, dim3 != dim1);
  EXPECT_EQ(true, dim3.deleteValue(0, 1));
  EXPECT_EQ(true, dim3.getValue(0, 1).second == "feeding"s);
  VariantTypeNDimArray ndiminit({ (VariantTypeList()<<"foo"s<<"bar"s<<3.12),(VariantTypeList()<<"foo"s<<"bar"s<<3.33)});
  EXPECT_EQ(true, ndiminit.colSize ()==2);
  EXPECT_EQ(true, ndiminit.getValue (1,2).second==3.33);
  EXPECT_EQ(true, ndiminit.getValue (0,2).second==3.12);
  ndiminit<< (VariantTypeList()<<"nothings"s<<"nothings"s);
  EXPECT_EQ(true, ndiminit.colSize ()==2);  
  auto ndim2=std::move(ndiminit);
  EXPECT_EQ(true, ndim2 !=ndiminit);
  EXPECT_EQ(true, ndiminit ==VariantTypeNDimArray(0));
  EXPECT_EQ(true, ndim2.colSize ()==2);
  EXPECT_EQ(true, ndim2.getValue (1,2).second==3.33);
  EXPECT_EQ(true, ndim2.getValue (0,2).second==3.12);
}

int main(int a, char **b) {
  ::testing::InitGoogleTest(&a, b);
  int ret{RUN_ALL_TESTS()};
  return ret;
}
