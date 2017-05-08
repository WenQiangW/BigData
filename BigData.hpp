#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
using namespace std;

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
	bool IsINT64Overflow()const;

	//Add  ��Щ�����Ĳ��� �϶��������ţ�
	string Add(string left, string right)
	{
		size_t Lsize = left.size();
		size_t Rsize = right.size();
		string res;
		char symbol = 0;
		//��������add��Ϊͬ�ţ�
		//��Ϊ-ʱ����ŵ���Add,λ����ͬʱ������λΪleft[0],λ����ͬʱ������λ����֮ǰ��left[0]
		if (Lsize < Rsize) //ʹleft��λ����
		{
			if (left[0] != right[0]) //���
				symbol = left[0];
			std::swap(Lsize, Rsize);
			std::swap(left, right);
		}	
		res.resize(Lsize + 1); //������������λ
		if (symbol != 0)
			res[0] = symbol;
		else 
			res[0] = left[0];
		char takeover = 0;//��λ
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
			if (left[0] == right[0] && left[0] == '+')//С�ļ�ȥ��ģ���ͬ�ŵĻ�������left = 5��right = 10��
				symbol = '-';
			
			if (left[0] == right[0] && left[0] == '-')//����left = -5��right = -10��
				symbol = '+';
		}
		string res;
		res.resize(Lsize);// ��ȥ�����ܲ�����λ
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
				int step = 1;//��ǰstepλ��λ
				while ((Lsize >(idx + step)) && left[Lsize - idx - step] == 0)
				{
					left[Lsize - idx - step] = '9';
					step++;
				}
				left[Lsize - idx - step]--; //��Ϊ0ʱ Ҫ-1
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
		if (LSize > RSize) //λ��С�ķŵ����ѭ��
		{
			std::swap(LSize, RSize);
			std::swap(left, right);
		}
		char takeover = 0; //��λ

		size_t resSize = LSize + RSize - 1;
		string res(resSize, '0');//ȫ����ʼ��'0';
		size_t offset = 0;//�����λ
		for (size_t i = 1; i < LSize; ++i)
		{
			char cleft = left[LSize - i] - '0';// ���ѭ����λ��ֵ
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
		//���һ�ν�λû��д�뵽�����
		res[1] = takeover + '0';
		//����
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

		while (*(pleft + len - 1) != '\0') //�о������Խ��
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

	//�жϱ��������ڳ���
	bool isLeftBig(char* left, size_t Lsize, char* right, size_t Rsize)//leftǰ�治Ϊ0
	{
		if (Lsize == Rsize && strncmp(left, right, Rsize) >= 0)
			return true;
		else if (Lsize > Rsize)
			return true;

		return false;
	} 
	//������ֵ�����ҷ�������
	char loopsub(char*& left, size_t& Lsize, char*& right, size_t Rsize)
	{
		char count = '0';//�൱����ֵ	
		while (isLeftBig(left, Lsize, right, Rsize))
		{
			for (size_t i = 0; i < Lsize; ++i)
			{
				char temp = left[Lsize - 1 - i] - '0';
				if (i < Rsize)
					temp -= (right[Rsize - 1 - i] - '0');
				if (temp < 0)
				{
					//��ǰ��λ
					size_t step = 1;//��Ĳ���
					while ((1 + i + step < Lsize) && left[Lsize - 1 - i - step] == 0) //�϶����Խ赽����Ϊ��ߴ����ұ�
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
			while (Lsize > 0 && *left == '0') //ȥ��ǰ���0
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