#include"BigData.hpp"

INT64 UN_INT = 0xcccccccccccccccc;
const INT64 MaxValue = 9223372036854775807;
const INT64 MinValue = -9223372036854775807;
1233
ostream& operator<<(ostream& out, const BigData& bigdata)
{
	char* pData = (char*)bigdata._strData.c_str();
	if (*pData == '+')
		pData++;
	out << pData;
	return out;
}

BigData::BigData(INT64 data = UN_INT)
:_value(data)
{
	//数字放入字符串中
	_strData = "+";
	INT64 temp = _value;
	if (temp < 0)
	{
		_strData[0] = '-';
		temp = 0 - temp;
	}
	else if (temp == 0)
		_strData = "+0";
	while (temp != 0)
	{
		_strData += temp % 10 + '0';
		temp /= 10;
	}
	reverse(_strData.begin() + 1, _strData.end());
}

//模拟atoi
BigData::BigData(const string& strData)
: _value(0)
, _strData("+0")
{
	// "123456789" "+123456890"  "12345asdfg123456"
	// "000001234567"  "      12345678"
	// "aa12345689" "000000000000000000"  "             "
	if (strData.empty())
		return;

	char* pData = (char*)strData.c_str();
	char symbol = *pData;
	if (*pData == '-' || *pData == '+')
		pData++;	
	
		
	//处理前置空格
	while (isspace(*pData))
		pData++;
	if ('\0' == *pData)
		return;

	//处理 前置0
	while ('0' == *pData)
		pData++;
	if ('\0' == *pData)
		return;
	//处理前置非数字
	if (*pData > '9' || *pData < '0')
		return;

	//处理剩余部分
	size_t Length = strlen(pData);
	_strData.resize(Length + 1);//多加一个符号位
	if (symbol == '-')
		_strData[0] = '-';
	size_t count = 1;
	while (*pData >= '0' && *pData <= '9')
	{
		_value = _value * 10 + *pData - '0';
		_strData[count++] = *pData++;
	}
	_strData.resize(count);

	if (symbol == '-')
		_value = 0 - _value;
}

BigData& BigData::operator=(const BigData& b)
{
	if (this != &b)
	{
		BigData temp(b);
		std::swap(_value, temp._value);
		std::swap(_strData, temp._strData);
	}
	return *this;
}

BigData BigData::operator+(const BigData& b)
{
	if (!IsINT64Overflow() && !b.IsINT64Overflow())
	{
		//没有超出范围
		if (_strData[0] != b._strData[0])//异号
			return BigData(_value + b._value);
		else
		{
			//10 - 2 = 8, 7
			//-10 - -2 = -8,-7
			//同号 但是没有超出范围
			if ((_strData[0] == '+' && MaxValue - _value >= b._value) ||
				(_strData[0] == '-' && MinValue - _value <= b._value))
				return BigData(_value + b._value);
		}
	}
	if (_strData[0] == b._strData[0]) //同号
		return BigData(Add(_strData, b._strData));
	return BigData(Sub(_strData, b._strData));
}
BigData BigData::operator-(const BigData& b)
{
	if (!IsINT64Overflow() && !b.IsINT64Overflow())
	{
		//没有超出范围
		if (_strData[0] == b._strData[0])//同号号
			return BigData(_value - b._value);
		else
		{
			// [10,-10]  2和 -7
			// -10 +2 = -8
			// 10 + -7 = 3
			//异号 但是没有超出范围
			if ((_strData[0] == '+' && MinValue + _value <= b._value) ||
				(_strData[0] == '-' && MaxValue + _value >= b._value))
				return BigData(_value - b._value);
		}
	}
	if (_strData[0] == b._strData[0]) //同号
		return BigData(Sub(_strData, b._strData));
	return BigData(Add(_strData, b._strData));
}

BigData BigData::operator*(const BigData& b)
{
	if (_value == 0 || b._value == 0)
		return BigData(0);
	else if (strcmp(_strData.c_str() + 1, "1") == 0)
		return BigData(b._strData);
	else if (strcmp(b._strData.c_str() + 1, "1") == 0)
		return BigData(_strData);
	return BigData(Mul(_strData, b._strData));
}
BigData BigData::operator/(const BigData& b)
{
	//商值为0,1，-1
	char* left = (char *)_strData.c_str();
	char* right = (char *)b._strData.c_str();
	//除数不能为0
	if (b._value == 0)
	{
		cout << "除数不能为0" << endl;
		return BigData(0);
	}
	//商值为0
	if (_strData.size() < b._strData.size())
		return BigData(0);
	else if (_strData.size() == b._strData.size() && strcmp(_strData.c_str() + 1, b._strData.c_str() + 1) < 0)
		return BigData(0);
	//商值为 1	
    if (strcmp(left, right) == 0)
		return BigData(1);
	//商值为-1
	if (strcmp(_strData.c_str() + 1, b._strData.c_str() + 1) == 0 && *left != *right)
		return BigData(-1);
	else if (b._value == 1) //被除数为1
		return BigData(_strData);
	else if (b._value == -1) //被除数为-1
		return BigData(b._strData);

	return BigData(Div(_strData, b._strData));
}

bool BigData::IsINT64Overflow()const
{
	std::string strTemp("+9223372036854775807");
	if (_strData[0] == '-')
		strTemp = "-9223372036854775808";

	if (_strData.size() < strTemp.size())
		return false;
	else if (_strData.size() == strTemp.size() && _strData < strTemp)
		return false;

	return true;
}

