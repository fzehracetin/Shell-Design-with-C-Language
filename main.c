#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/wait.h> 
#define MAX 100

char* read_from_console(void) //boyuta gore artirmali yapsan iyi olur bufferi asabilir. . .
{
	char *line = (char*) malloc(MAX*sizeof(char));
	char c;
	int i = 0;
	
	line[0] = '\0';
	
	c = fgetc(stdin);
	while((c != EOF) && (c != '\n')) 
	{
		line[i] = c;
		i++;
		c = fgetc(stdin);
	} 
	line[i] = '\0';
	//printf("%s \n", line);
	return line;
}

char** command_tokenizer(char* str) 
{
	char *token;
	char **tokens = (char**) malloc(MAX*sizeof(char*));
	int i;
	
	for(i = 0; i < MAX; i++)
	{
		tokens[i] = (char*) malloc(MAX*sizeof(char));
	}
	
	i = 0;
	for (token = strtok(str, " "); token != NULL; token = strtok(NULL, " ")) 
	{
		//printf("%s \n", token);
		strcpy(tokens[i], token);
		i++;
	}
	return tokens;
}

int command_comparator(char** tokens) 
{
	char built_in[3][MAX] = { "exit",
							"cd",
							"help"
							};
	int i = 0;
	char str[500] =">> Shell'e hosgeldiniz.\n>> Bu shell asagidakileri yapabilir.\n>> Temel UNIX Shell komutlarini yurutmek.\n>> cd, exit ve help built-in komutlarini calistirmak. \n>> Klavye ust ve alt ok tuslari ile komutlar arasinda gecis yapmak.\n>> Bu shell pipe iceren [command | command] komutlari desteklemez!!\n>> FATMA ZEHRA CETIN 17011068\n";
	
	
	while ((i < 3) && (strcmp(built_in[i], tokens[0]) != 0))
	{
		i++;
	}
	
	if (i < 3) {
		switch(i) {
			case 0:
				printf("Shell exiting. . .\n");
				exit(0);
				break;
			case 1:
				chdir(tokens[1]); 
				break;
			case 2:
				printf("%s", str);
				break;
		}
		return 1;
	}
	return 0;
}

void executor(char** tokens) 
{
	pid_t pid;
	int status;
	pid = fork();
	
	if (pid < 0)
	{
		printf("ERROR: forking child process failed. . .\n");
		return;
	}
	else if (pid == 0) //child succesfully created
	{
		if ( execvp(tokens[0], tokens) < 0)
		{
			printf("ERROR: exec failed \n");
			return;
		}
	}
	 else //parent waits for child                                 
        while (wait(&status) != pid);
}

int main(int argc, char **argv)
{
	int true = 1;
	char *str = (char*) malloc(MAX*sizeof(char));
	char **tokens = (char**) malloc(MAX*sizeof(char*));
	int i;
	
	for(i = 0; i < MAX; i++)
	{
		tokens[i] = (char*) malloc(MAX*sizeof(char));
	}
	
	while (true) 
	{
		str[0] = '\0';
		printf("%s@Shell:~$ ", getenv("USER"));
		str = read_from_console();
		//printf("%s \n", str);
		if (str[0] != '\0') {
			tokens = command_tokenizer(str);
			if (!command_comparator(tokens))
					executor(tokens);
		}
		
	}
	return 0;
}
