#include"BigData.hpp"
1

void TestAtoi()
{
	// "123456789" "+123456890"  "12345asdfg123456"
	// "000001234567"  "      12345678"
	// "aa12345689" "000000000000000000"  "             "
	cout << atoi("") << endl;
	cout << atoi("123456789") << endl;
	cout << atoi("-123456789") << endl;
	cout << atoi("+123456890") << endl;
	cout << atoi("12345asdfg123456") << endl;
	cout << atoi("-000001234567") << endl;
	cout << atoi("      12345678") << endl;
	cout << atoi("  aa12345689") << endl;
	cout << atoi("000000000000000000") << endl;
	cout << atoi("       00      ") << endl;
}

void Test()
{
	BigData b1("");
	BigData b(12345);
	BigData b2("123456789");
	BigData b3("-123456789");
	BigData b4("-000001234567");
	BigData b5("12345asdfg123456");
	BigData b6("aa12345689");

	cout << b1 << endl;
	cout << b2 << endl;
	cout << b3 << endl;
	cout << b4 << endl;
	cout << b5 << endl;
	cout << b6 << endl;
}

void Testadd()
{
	BigData b1("9999999999999999999");
	BigData b2("-9999999999999999999");
	BigData b3("33333333");
	BigData b4("-33333333");
	BigData b5("");
	b5 = b1 + b3;
	cout << b1 + b3 << endl;//正 正  正
	cout << b2 + b4 << endl;//负 负  负

	cout << b1 + b4 << endl;//正 负  正
	cout << b3 + b2 << endl;//正 负  负
	cout << b4 + b1 << endl;//负 正	 正
	cout << b2 + b3 << endl;//负 正  负
	cout << endl;
}

void Testsub()
{
	BigData b1("9999999999999999999");
	BigData b2("-9999999999999999999");
	BigData b3("33333333");
	BigData b4("-33333333");
	
	//同号 调sub，
	cout << b3 - b1 << endl;//正 正	 负 
	cout << b1 - b3 << endl;//正 正  正 
	cout << b2 - b4 << endl;//负 负	 负
	cout << b4 - b2 << endl;//负 负	 正
	//异号调add
	cout << b4 - b1 << endl;//负 正	 负
	cout << b1 - b4 << endl;//正 负	 正
	BigData b5("-555555555555555555555555555555");
	BigData b6("-555555555555555555555555555555");
	cout << b6 - b5 << endl;//借位

}

void TestMul()
{
	BigData b1("-1237");
	BigData b2("456");
	BigData b3("");
	b3 = b1* b2;
	cout << b3 << endl;
}

void TestDiv()
{
	BigData b1("-3333");
	BigData b2("3333");
	BigData b3("");
	b3 = b1 / b2;
	cout << b3 << endl;
}

int main()
{
	//Testadd();
	Testsub();
	system("pause");
	return 0;
}
