#include"BigData.hpp"

INT64 UN_INT = 0xcccccccccccccccc;
const INT64 MaxValue = 9223372036854775807;
const INT64 MinValue = -9223372036854775807;

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
	//���ַ����ַ�����
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

//ģ��atoi
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
	
		
	//����ǰ�ÿո�
	while (isspace(*pData))
		pData++;
	if ('\0' == *pData)
		return;

	//���� ǰ��0
	while ('0' == *pData)
		pData++;
	if ('\0' == *pData)
		return;
	//����ǰ�÷�����
	if (*pData > '9' || *pData < '0')
		return;

	//����ʣ�ಿ��
	size_t Length = strlen(pData);
	_strData.resize(Length + 1);//���һ������λ
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
		//û�г�����Χ
		if (_strData[0] != b._strData[0])//���
			return BigData(_value + b._value);
		else
		{
			//10 - 2 = 8, 7
			//-10 - -2 = -8,-7
			//ͬ�� ����û�г�����Χ
			if ((_strData[0] == '+' && MaxValue - _value >= b._value) ||
				(_strData[0] == '-' && MinValue - _value <= b._value))
				return BigData(_value + b._value);
		}
	}
	if (_strData[0] == b._strData[0]) //ͬ��
		return BigData(Add(_strData, b._strData));
	return BigData(Sub(_strData, b._strData));
}
BigData BigData::operator-(const BigData& b)
{
	if (!IsINT64Overflow() && !b.IsINT64Overflow())
	{
		//û�г�����Χ
		if (_strData[0] == b._strData[0])//ͬ�ź�
			return BigData(_value - b._value);
		else
		{
			// [10,-10]  2�� -7
			// -10 +2 = -8
			// 10 + -7 = 3
			//��� ����û�г�����Χ
			if ((_strData[0] == '+' && MinValue + _value <= b._value) ||
				(_strData[0] == '-' && MaxValue + _value >= b._value))
				return BigData(_value - b._value);
		}
	}
	if (_strData[0] == b._strData[0]) //ͬ��
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
	//��ֵΪ0,1��-1
	char* left = (char *)_strData.c_str();
	char* right = (char *)b._strData.c_str();
	//��������Ϊ0
	if (b._value == 0)
	{
		cout << "��������Ϊ0" << endl;
		return BigData(0);
	}
	//��ֵΪ0
	if (_strData.size() < b._strData.size())
		return BigData(0);
	else if (_strData.size() == b._strData.size() && strcmp(_strData.c_str() + 1, b._strData.c_str() + 1) < 0)
		return BigData(0);
	//��ֵΪ 1	
    if (strcmp(left, right) == 0)
		return BigData(1);
	//��ֵΪ-1
	if (strcmp(_strData.c_str() + 1, b._strData.c_str() + 1) == 0 && *left != *right)
		return BigData(-1);
	else if (b._value == 1) //������Ϊ1
		return BigData(_strData);
	else if (b._value == -1) //������Ϊ-1
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

