#pragma once

#include "Globals.h"

#include <string>

namespace Binary
{
	template<class Data>
	char* GetBinaryStream(Data data)
	{
		char* output = new char[sizeof(Data)];

		unsigned char* stream = reinterpret_cast<unsigned char*>(&data);
		memcpy(output, stream, sizeof(Data));

		return output;
	}

	template<class Data>
	Data GetDataFromStream(char* stream)
	{
		Data* ptr = reinterpret_cast<Data*>(stream);

		Data output = *ptr;
		return output;
	}

	template<class data>
	void PrintBinary(data a)
	{
		int amount = 0;
		unsigned int size = sizeof(data) * 8;
		std::string n;
		for (int i = size - 1; i >= 0; i--)
		{
			if (a & (1 << i))
				n.push_back('1');
			else
				n.push_back('0');
			amount++;
		}

		LOG("%s", n.c_str());
	}

	template<class Data>
	void PrintBinaryStream(char* stream)
	{
		LOG("Binary:")
		for (int i = 0; i < sizeof(Data); i++)
		{
			PrintBinary<unsigned char>(stream[i]);
		}
		LOG("---")
	}
}
