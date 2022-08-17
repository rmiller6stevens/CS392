#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
This program reads from an input ASCII file with 4 columns of long integers

It first opens a file named according to the second argument and writes
the column of the input specified by column_index to that file.
Only one number is written per line in ASCII format

Then, it counts the number of lines in the input file and writes the last 
third of the input file to the second output file, also in ASCII format

USAGE: lab4_files input_file output_file1 output_file2 column_index
*/

void usage(char* name){
	printf("%s input_file output_file1 outpute_file2 column_index\n", name);
}

int main (int argc, char **argv) {
	if (argc != 5) {
		fprintf(stderr, "Wrong number of command-line arguments\n");
		usage(argv[0]);
		return -1;
	}
	
	
	FILE* in = fopen(argv[1], "r");
	if (in == NULL) {
		printf("%s", "The input file cannot be opened!");
		exit(-1);
	}

	FILE* out = fopen(argv[2], "r+");
	if (out == NULL) {
		printf("%s", "The output file cannot be written to/opened!");
		exit(-1);
	}
	
	// TODO: read all lines from the file, write the column the user 
	// selected to the first output file
	char line[200] = "";
	int linecounter = 0;
	while(fgets(line, 200, in)){ 
		fputs(line[atoi(argv[4])], out);
		linecounter++;
	}
	fclose(out);

	// third of the file to the second output file starting from index
	// num_lines*2/3 to the end
	rewind(in);
	FILE* out2 = fopen(argv[3], "r+");
	if (out2 == NULL) {
		printf("%s", "The output file cannot be written to/opened!");
		exit(-1);
	}

	int i = 0;
	while(fgets(line, 200, in)){
		if(i >= linecounter*2/3){
			fputs(line, out2);
		}
	i++;
	}

	fclose(in);
	fclose(out2);

	return 0;
}

