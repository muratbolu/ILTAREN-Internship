#include <cstdio>

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		int rc = fputs(
			"Provide exactly one argument, which should be the input file.\n", stderr);
		if (rc == EOF)
		{
			perror("Invalid argument number");
		}
		return 1; // EXIT_FAILURE
	}

	FILE* fp = fopen(argv[1], "r");
	if (fp == nullptr)
	{
		perror("Could not open file");
		return 1;
	}

	// TODO: get chars

	fclose(fp);
	return 0;
}
