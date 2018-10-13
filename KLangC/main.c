#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "chunk.h"
#include "debug.h";
#include "vm.h"

// Single command line
static void repl()
{
	char line[1024];
	for (;;)
	{
		printf(">");

		if (!fgets(line, sizeof(line), stdin))
		{
			printf("\n");
			break;
		}

		interpret(line);
	}
}

// Dynamically reads through the entire source file
static char* readFile(const char* path)
{
	FILE* file;
	// Invalid file
	if ((fopen_s(&file, path, "r")) == NULL)
	{
		fprintf(stderr, "Could not open file \"%s\".\n", path);
		exit(74);
	}

	fseek(file, 0L, SEEK_END);
	size_t fileSize = ftell(file);
	rewind(file);

	char* buffer = (char*)malloc(fileSize + 1);
	// Not enough memory to allocate
	if (buffer == NULL)
	{
		fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
		exit(74);
	}
	size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);

	// Fail to read
	if (bytesRead < fileSize)
	{
		fprintf(stderr, "Could not read file \"%s\".\n", path);
		exit(74);
	}

	fclose(file);
	return buffer;
}

static void runFile(const char* path)
{
	char* source = readFile(path);
	InterpretResult result = interpret(source);
	free(source);

	if (result == INTERPRET_COMPILE_ERROR) exit(65);
	if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}

int main(int argc, const char* argv[])
{
	initVM();

	if (argc == 1)
	{
		repl();
	}
	else if (argc == 2)
	{
		runFile(argv[1]);
	}
	else
	{
		fprintf(stderr, "Usage: klang [path]\n");
		exit(64);
	}

	freeVM();

	// TEMP, ONLY USE FOR CONSOLE DEBUG
	getchar();
	return 0;
}