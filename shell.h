
//function to do cd
int cd(char* string);

//function to parse commands
int parse(char* array[], int argc);

//function to handle children (in background)
void background(int signal);

//sigint handler
void sigint(int sign);
