// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

#define BITS_IN_ONE_MEM (sizeof(TELEM) * 8)

TBitField::TBitField(int len)
{
  if (len < 0)
    throw "negative length";
  BitLen = len;
  MemLen = BitLen / BITS_IN_ONE_MEM + 1;
  pMem = new TELEM[MemLen];
  for (int i = 0; i < MemLen; i++)
	  pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = BitLen / BITS_IN_ONE_MEM + 1;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
  return n / BITS_IN_ONE_MEM;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
  return 1 << (n % BITS_IN_ONE_MEM);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n > -1) && (n < BitLen))
		pMem[GetMemIndex(n)] |= GetMemMask(n); 
	else throw "Negative bit";
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n> -1) && (n<BitLen))
		pMem[GetMemIndex(n)] &= ~GetMemMask(n); 
	else throw "Negative bit";
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n> -1) && (n<BitLen))
		return (((pMem[GetMemIndex(n)] & GetMemMask(n)) != 0)? 1 : 0);
	else throw "Negative bit";
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (BitLen == bf.BitLen)
	{
		for (int i = 0; i < MemLen; i++)
			pMem[i] = bf.pMem[i];
	}
	else
	{
		delete[] pMem;
		BitLen = bf.BitLen;
		MemLen = BitLen / BITS_IN_ONE_MEM + 1;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++)
			pMem[i] = bf.pMem[i];
	}
  return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen)
		return 0;
	else
	{
		int flag = 1;
		for (int i = 0; (i < bf.MemLen) && (flag); i++)
			if (pMem[i] != bf.pMem[i])
				flag = 0;

		return flag;
	}
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	if (BitLen > bf.BitLen)
	{
		TBitField temp(BitLen);
		for (int i = 0; i < bf.MemLen; i++)
			temp.pMem[i] = pMem[i] | bf.pMem[i];
		for (int i = bf.MemLen; i < temp.MemLen; i++)
			temp.pMem[i] = pMem[i];
		return temp;
	}
	else
	{
		TBitField temp(bf.BitLen);
		for (int i = 0; i < MemLen; i++)
			temp.pMem[i] = pMem[i] | bf.pMem[i];
		for (int i = MemLen; i < temp.MemLen; i++)
			temp.pMem[i] = bf.pMem[i];
		return temp;
	}
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	if (BitLen > bf.BitLen)
	{
		TBitField temp(BitLen);
		for (int i = 0; i < bf.MemLen; i++)
			temp.pMem[i] = pMem[i] & bf.pMem[i];
		for (int i = bf.MemLen; i < temp.MemLen; i++)
			temp.pMem[i] = 0;
		return temp;
	}
	else
	{
		TBitField temp(bf.BitLen);
		for (int i = 0; i < MemLen; i++)
			temp.pMem[i] = pMem[i] & bf.pMem[i];
		for (int i = MemLen; i < temp.MemLen; i++)
			temp.pMem[i] = 0;
		return temp;
	}
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField temp(BitLen);
	for (int i = 0; i < MemLen - 1; i++)
		temp.pMem[i] = ~pMem[i];
	for (int i = ((MemLen - 1) * BITS_IN_ONE_MEM); i < BitLen; i++)
		if (GetBit(i) == 0)
			temp.SetBit(i);
		else
			temp.ClrBit(i);
	return temp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	/*cout << "BitLen ?" << endl;
	int len = 0;
	istr >> len;
	for (int i = 0; i < len; i++)
	{
		int flag = 0;
		istr >> flag;
		if (flag == 1)
			bf.SetBit(i);
	}
	return istr;*/
	int i = 0;
	char ch;
			do {
			istr >> ch;
		} while (ch != ' ');
		while (1) {
			istr >> ch;
			if (ch == '0') bf.ClrBit(i++);
			else if (ch == '1') bf.SetBit(i++);
			else break;
		}
		return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	ostr << "{ ";
	for (int i = 0; i < bf.BitLen; i++)
		ostr << bf.GetBit(i);
	ostr << "}\n";
	return ostr;
}
