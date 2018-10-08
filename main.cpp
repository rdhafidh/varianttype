#include <gtest/gtest.h>
#include <varianttype.h>
#include <numeric>
#include <iostream>

TEST(Undefined1, Undef1) {
  using namespace val;
  VariantType var;
  EXPECT_EQ(TypeData::TYPE_IS_UNDEFINED, var.getType());
}
TEST(Undefined1, Undef2) {
  using namespace val;
  VariantType var;
  EXPECT_EQ((size_t)0, var.getVar().index());
}

TEST(Undefined1, Undef3) {
  using namespace val;
  VariantType var;
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
}

TEST(CompareTest, CompareBool2) {
  using namespace val;
  VariantType vt1(false); 
  EXPECT_EQ(true, VariantType(false) == vt1);   
}

TEST(CompareTest, ComparePlainOldData) {
  using namespace val;
  VariantType vt1(false); 
  VariantType vt2(std::string("foo")); 
  EXPECT_EQ(true, vt1==false);  
  EXPECT_EQ(false, vt1=="foo bar");  
  EXPECT_EQ(true, vt2=="foo");  
  EXPECT_EQ(true, vt2==std::string("foo"));  
  EXPECT_EQ(true, VariantType((int64_t)12321312)==(int64_t)12321312);  
  EXPECT_EQ(true, VariantType(31.414)==31.414);  
  
  EXPECT_EQ(true, VariantType(VectorData({}) )==VectorData({}) );  
  EXPECT_EQ(false, VariantType(VectorData({VariantType(VariantType::makeNumber(100)),
	VariantType(VariantType::makeNumber(23)),VariantType(std::string("false"))}) )==VectorData({VariantType(std::string("false"))}) );  
  EXPECT_EQ(true, VariantType(VectorData({VariantType(VariantType::makeNumber(100)),
	VariantType(VariantType::makeNumber(23)),VariantType(std::string("false"))}) )==
	VectorData({VariantType(VariantType::makeNumber(100)),
	VariantType(VariantType::makeNumber(23)),
	VariantType(std::string("false"))})  );  
  
  auto items={ VariantType(std::string("foo bar")), VariantType(true) };
  VectorData arrayItem( items);
  EXPECT_EQ((decltype(arrayItem.size()))2,arrayItem.size());
  EXPECT_EQ(TypeData::TYPE_IS_STRING,arrayItem.at(0).getType());
  EXPECT_EQ(TypeData::TYPE_IS_BOOL,arrayItem.at(1).getType());
   
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
  int64_t num=(int64_t)std::numeric_limits<int>::max ()+21313;
  VariantType var((int64_t)std::numeric_limits<int>::max ()+21313);
  VariantType rval(num);
  EXPECT_EQ(true, var == rval);
  EXPECT_EQ(val::TypeData::TYPE_IS_NUMBER, var.getType());
  EXPECT_EQ(val::TypeData::TYPE_IS_NUMBER, rval.getType());
}
TEST(CompareTest, CompareNumber3) {
  using namespace val;
  int num=1231;
  VariantType var(VariantType::makeNumber(1231));
  VariantType rval(VariantType::makeNumber(num));
  EXPECT_EQ(true, var == rval);
  EXPECT_EQ(val::TypeData::TYPE_IS_NUMBER, var.getType());
  EXPECT_EQ(val::TypeData::TYPE_IS_NUMBER, rval.getType()); 
}
 
TEST(CompareTest, CompareDouble1) {
  using namespace val;
  float num=1231.3f;
  VariantType var(VariantType::makeDouble(1231.3f));
  VariantType anval(1231.3);
  VariantType rval(VariantType::makeDouble(num));
  EXPECT_EQ(true, var == rval);
  EXPECT_EQ(false, anval == rval);
  EXPECT_EQ(val::TypeData::TYPE_IS_DOUBLE, var.getType());
  EXPECT_EQ(val::TypeData::TYPE_IS_DOUBLE, anval.getType());
  EXPECT_EQ(val::TypeData::TYPE_IS_DOUBLE, rval.getType()); 
  
  //anval==rval -> false
  //same as this one
  float numf =231.2f;
  double dnum=(double)numf;
  double numd=231.2;
  EXPECT_EQ(false,dnum==numd);
  EXPECT_EQ(true,dnum==VariantType::makeDouble(numf));
}

TEST(String,StringAppend){
	using namespace val;
	VariantType str=std::string("foo bar");
	str<<std::string("!")<<"!!!";
	EXPECT_EQ(true,str=="foo bar!!!!");
	VariantType nonStr=(int64_t)1234;
	nonStr<<"foo bar"<<" test";
	EXPECT_EQ(TypeData::TYPE_IS_NUMBER,nonStr.getType());
	EXPECT_EQ(true,nonStr==(int64_t)1234);
}	
 
TEST(TestAdd, TestAddNumber1){
	using namespace val;
	VariantType var(VariantType::makeNumber(30));
	VariantType varEnd((int64_t)30+std::numeric_limits<int>::max());
	TypeErrorVariant errFlag =TypeErrorVariant::NO_ERROR;
	bool e=var.addVariantNumber(VariantType::makeNumber(std::numeric_limits<int>::max()),&errFlag);
	EXPECT_EQ(true,e);
	EXPECT_EQ(TypeErrorVariant::NO_ERROR,errFlag);
	EXPECT_EQ(true,var==varEnd); 
}

TEST(TestAdd, TestAddNumberOverflow){
	using namespace val;
	VariantType var(VariantType::makeNumber(30)); 
	TypeErrorVariant errFlag =TypeErrorVariant::NO_ERROR;
	bool e=var.addVariantNumber(std::numeric_limits<int64_t>::max(),&errFlag);
	EXPECT_EQ(false,e);
	EXPECT_EQ(TypeErrorVariant::ERROR_ARITH_OVERFLOW,errFlag);
	EXPECT_EQ(true,var==(int64_t)30); 
}

TEST(TestAdd, TestAddNumberOverflow2){
	using namespace val;
	VariantType var(VariantType::makeNumber(-30)); 
	TypeErrorVariant errFlag =TypeErrorVariant::NO_ERROR;
	bool e=var.addVariantNumber(-std::numeric_limits<int64_t>::max(),&errFlag);
	EXPECT_EQ(false,e);
	EXPECT_EQ(TypeErrorVariant::ERROR_ARITH_OVERFLOW,errFlag);
	EXPECT_EQ(true,var==(int64_t)-30); 
}

TEST(TestAdd, TestAddDoubleOverflow){
	using namespace val;
	VariantType var((double)std::numeric_limits<double>::max()); 
	TypeErrorVariant errFlag =TypeErrorVariant::NO_ERROR;
	bool e=var.addVariantDouble(std::numeric_limits<double>::max(),&errFlag);
	EXPECT_EQ(false,e);
	EXPECT_EQ(TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW,errFlag);
	EXPECT_EQ(true,var==(double)std::numeric_limits<double>::max()); 
} 

TEST(TestAdd, TestAddDoubleOverflow2){
	using namespace val;
	VariantType var((double)-std::numeric_limits<double>::max()); 
	TypeErrorVariant errFlag =TypeErrorVariant::NO_ERROR;
	bool e=var.addVariantDouble(-std::numeric_limits<double>::max(),&errFlag);
	EXPECT_EQ(false,e);
	EXPECT_EQ(TypeErrorVariant::ERROR_ARITH_DOUBLE_OVERFLOW,errFlag);
	EXPECT_EQ(true,var==(double)-std::numeric_limits<double>::max());  
} 

TEST(TestAdd, TestAddDouble){
	using namespace val;
	VariantType var((double)-std::numeric_limits<int>::max()); 
	double asliVar=std::get<double>(var.getVar());
	EXPECT_EQ(true,var==asliVar);  
	
	TypeErrorVariant errFlag =TypeErrorVariant::NO_ERROR;
	bool e=var.addVariantDouble(-std::numeric_limits<double>::max(),&errFlag);
	 
	asliVar += -std::numeric_limits<double>::max();
	EXPECT_EQ(true,e);
	EXPECT_EQ(true,var==asliVar);  
	EXPECT_EQ(TypeErrorVariant::NO_ERROR,errFlag); 
} 
 
TEST(ArrayPrintTest, PrintArray){
    using namespace val;
    VariantType var(VariantType()<<VariantType((bool)false)<<VariantType((int64_t)30)
                    <<VariantType(std::string("from"))<< VariantType(3.43)); 
    EXPECT_EQ((size_t)4,var.getVector ().size ());
    EXPECT_EQ(true,var.getVector ().at (0).getType ()==TypeData::TYPE_IS_BOOL);
    EXPECT_EQ(true,var.getVector ().at (1)==(int64_t)30);
    EXPECT_EQ(true,var.getVector ().at (2)=="from");
    EXPECT_EQ(true,var.getVector ().at (3)==3.43);
   std::cout <<"\nvar:"<<var<<"\n";
    
}

int main(int a, char **b) {
  ::testing::InitGoogleTest(&a, b);
  int ret{RUN_ALL_TESTS()};
  return ret;
}