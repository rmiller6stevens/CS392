#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//credit to lab on wednesday, works well as a template
void usage(char* name){
	printf("%s <input filename> <output filename>\n", name);
}

int main(int argc, char** argv){
if (argc != 3) {
    fprintf(stderr, "Wrong number of command-line arguments\n");
    usage(argv[0]);
    return 1;
}

FILE* in = fopen(argv[1], "r");
if (in == NULL) {
    printf("%s", "The input file cannot be opened!");
    return 1;
}
//ask prof if I should use w or r+
FILE* out = fopen(argv[2], "wb");
if (out == NULL) {
    printf("%s", "The output file cannot be written to/opened!");
    return 1;
}
char line[50] = "";
long* arr[4];
while(fgets(line, 50, in)){ 
    sscanf(line, "%ld, %ld, %ld, %ld", arr[0], arr[1], arr[2], arr[3]);
    fwrite(arr, sizeof(long), sizeof(arr), out);
}

fclose(in);
fclose(out);
/*max chars is = to the size of max int of each byte size
this would be 3 + 5 + 10 + 20 and then another 3 for each \t, so it is 41
I am making it 50 since fgets stop when it reaches newline, I just wanted
to use the smallest number possible here
*/
return 0;
}