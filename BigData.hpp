#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
using namespace std;

123
typedef long long INT64;

class BigData
{
public:

	BigData(INT64 data);
	BigData(const string& str);
	BigData operator+(const BigData& b);
	BigData operator-(const BigData& b);
	BigData operator*(const BigData& b);
	BigData operator/(const BigData& b);
	BigData& operator=(const BigData& b);
	friend ostream& operator<<(ostream& out, const BigData& bigdata);
private:
	bool IsINT64Overflow()const;//判断是否超过long long范围

	//Add  这些函数的参数 肯定都带符号，
	string Add(string left, string right) //加函数，被重载函数调用
	{
		size_t Lsize = left.size();
		size_t Rsize = right.size();
		string res;
		char symbol = 0;
		//正常调用add的为同号，
		//当为-时候，异号调用Add,位数相同时，符号位为left[0],位数不同时，符号位交换之前的left[0]
		if (Lsize < Rsize) //使left的位数大
		{
			if (left[0] != right[0]) //异号
				symbol = left[0];
			std::swap(Lsize, Rsize);
			std::swap(left, right);
		}	
		res.resize(Lsize + 1); //可能最后产生进位
		if (symbol != 0)
			res[0] = symbol;
		else 
			res[0] = left[0];
		char takeover = 0;//进位
		for (size_t idx = 1; idx < Lsize; ++idx)
		{
			char temp = left[Lsize - idx] - '0' + takeover;
			if (Rsize > idx)
				temp += right[Rsize - idx] - '0';
			takeover = temp / 10;
			res[Lsize + 1 - idx] = temp % 10 + '0';
		}
		res[1] = takeover + '0';
		return res;
	}
	
	string Sub(string left, string right)
	{
		size_t Lsize = left.size();
		size_t Rsize = right.size();		
		char symbol  = 0;
		if (Lsize < Rsize || (Lsize == Rsize && strcmp(left.c_str() + 1, right.c_str() + 1) < 0))
		{
			std::swap(Lsize, Rsize);
			std::swap(left, right);
			if (left[0] == right[0] && left[0] == '+')//小的减去大的，且同号的话，比如left = 5，right = 10，
				symbol = '-';
			
			if (left[0] == right[0] && left[0] == '-')//比如left = -5，right = -10，
				symbol = '+';
		}
		string res;
		res.resize(Lsize);// 减去不可能产生进位
		if (symbol != 0)
			res[0] = symbol;
		else
			res[0] = left[0];
		
		for (size_t idx = 1; idx < Lsize; ++idx)
		{
			char temp = left[Lsize - idx] - '0';
			if (Rsize > idx)
				temp -= right[Rsize - idx] - '0';
			if (temp < 0)
			{
				int step = 1;//向前step位借位
				while ((Lsize >(idx + step)) && left[Lsize - idx - step] == 0)
				{
					left[Lsize - idx - step] = '9';
					step++;
				}
				left[Lsize - idx - step]--; //不为0时 要-1
				temp += 10;
			}
			res[Lsize - idx] = temp % 10 + '0';
		}
		return res;
	}
	string Mul(string left, string right)
	{
		size_t LSize = left.size();
		size_t RSize = right.size();
		if (LSize > RSize) //位数小的放到外层循环
		{
			std::swap(LSize, RSize);
			std::swap(left, right);
		}
		char takeover = 0; //进位

		size_t resSize = LSize + RSize - 1;
		string res(resSize, '0');//全都初始化'0';
		size_t offset = 0;//结果移位
		for (size_t i = 1; i < LSize; ++i)
		{
			char cleft = left[LSize - i] - '0';// 外层循各个位的值
			takeover = 0;
			for (size_t j = 1; j < RSize; ++j)
			{
				char resBit = res[resSize - offset - j] - '0';
				char mulBit = (right[RSize - j] - '0') * cleft + takeover + resBit;
				takeover = mulBit / 10;
				res[resSize - offset - j] = mulBit % 10 + '0';
			}
			offset++;
		}
		//最后一次进位没有写入到结果中
		res[1] = takeover + '0';
		//符号
		if (left[0] == right[0])
			res[0] = '+';
		else
			res[0] = '-';
		return res;
	}
	string Div(string left, string right)
	{
		char symbol = '+';
		if (left[0] != right[0])
			symbol = '-';
		string res;
		res.append(1, symbol);
		char* pleft = (char*)left.c_str() + 1;
		char* pright = (char*)right.c_str() + 1;
		size_t Lsize = left.size() - 1;
		size_t Rsize = right.size() - 1;
		size_t len = Rsize;

		while (*(pleft + len - 1) != '\0') //感觉这可能越界
		{
			if (!isLeftBig(pleft, len, pright, Rsize))
			{
				len++;
				res.append(1, '0');
				continue;
			}
			else
			{
				res.append(1, loopsub(pleft, len, pright, Rsize));
				len++;
			}
		}
		return res;
	}

	//判断被除数大于除数
	bool isLeftBig(char* left, size_t Lsize, char* right, size_t Rsize)//left前面不为0
	{
		if (Lsize == Rsize && strncmp(left, right, Rsize) >= 0)
			return true;
		else if (Lsize > Rsize)
			return true;

		return false;
	} 
	//计算商值，并且返回余数
	char loopsub(char*& left, size_t& Lsize, char*& right, size_t Rsize)
	{
		char count = '0';//相当于商值	
		while (isLeftBig(left, Lsize, right, Rsize))
		{
			for (size_t i = 0; i < Lsize; ++i)
			{
				char temp = left[Lsize - 1 - i] - '0';
				if (i < Rsize)
					temp -= (right[Rsize - 1 - i] - '0');
				if (temp < 0)
				{
					//向前借位
					size_t step = 1;//借的步数
					while ((1 + i + step < Lsize) && left[Lsize - 1 - i - step] == 0) //肯定可以借到，因为左边大于右边
					{
						left[Lsize - 1 - i - step] = '9';
						step++;
					}
					left[Lsize - 1 - i - step]--;
					temp += 10;
				}
				left[Lsize - 1 - i] = temp + '0';
			}
			count++;
			while (Lsize > 0 && *left == '0') //去除前面的0
			{
				left++;
				Lsize--;
			}
		}
		return count;
	}
private:
	INT64 _value;
	string _strData;
};
