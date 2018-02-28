#include <iostream>
#include <fstream>
#include <string>

const size_t MOVE_SCHEMA[8] = {2, 3, 4, 6, 7, 0, 1, 5};

void MoveBits(char &c)
{
	char result = 0;
	for (size_t index = 0; index < CHAR_BIT; index++) 
	{
		if (c & (1u << index)) 
		{
			result |= 1 << MOVE_SCHEMA[index];
		}
	}
	c = result;
}

void UndoMoveBits(char &c)
{
	char result = 0;
	for (size_t index = 0; index < CHAR_BIT; index++)
	{
		if (c & (1 << MOVE_SCHEMA[index]))
		{
			result |= 1 << index;
		}
	}
	c = result;
}

void Crypt(std::istream &input, std::ostream &output, char key)
{
	char c;
	while (input >> c)
	{
		c ^= key;
		MoveBits(c);
		output << c;
	}
}

void Decrypt(std::istream &input, std::ostream &output, char key)
{
	char c;
	while (input >> c)
	{
		UndoMoveBits(c);
		c ^= key;
		output << c;
	}
}

long StrToLong(char *str, bool &wasErr)
{
	char *pEnd = NULL;
	long value = strtol(str, &pEnd, 10);
	wasErr = ((*str == '\0') || (*pEnd != '\0'));
	return value;
}

void ShowUsage()
{
	std::cout << "Usage: crypt.exe crypt|decrypt <input file> <output file> <key=0..255>\n";
}

const int ARGS_COUNT = 4;

int main(int argc, char* argv[])
{
	if (argc != ARGS_COUNT + 1)
	{
		std::cout << "Invalid arguments count\n";
		ShowUsage();
		return 1;
	}

	std::ifstream inputFile(argv[2]);
	if (!inputFile.is_open())
	{
		std::cout << "Failed to open input file: " << argv[2] << '\n';
		ShowUsage();
		return 1;
	}

	std::ofstream outputFile(argv[3]);
	if (!outputFile.is_open())
	{
		std::cout << "Failed to open output file: " << argv[3] << '\n';
		ShowUsage();
		return 1;
	}

	std::string command = argv[1];

	bool wasErr = false;
	long key = StrToLong(argv[4], wasErr);
	if ((key < 0) || (key > 255) || wasErr)
	{
		std::cout << "Wrong key: " << argv[4] << '\n';
		ShowUsage();
	}

	if (command == "crypt")
	{
		Crypt(inputFile, outputFile, (char)key);
	}
	else if (command == "decrypt")
	{
		Decrypt(inputFile, outputFile, (char)key);
	}
	else
	{
		std::cout << "Wrong command\n";
		ShowUsage();
		return 1;
	}

	return 0;
}