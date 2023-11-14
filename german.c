#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main()
{
	int fd = open("dictionary.txt", O_RDONLY);
	int i = 0;
	int j = 0;
	int letters = 0;
	int i_read = 1;
	int wrong = 0;
	char c = 'a';
	char *str;
	char *keyboard;
	srand(time(NULL));
	int r = 0;
	int newlines = 0;
	while(i < 4)
	{
		newlines = 0;
		r = rand() % 4; 
		while(newlines < r)
		{
			read(fd, &c, 1);
			if (c == '\n')
				newlines++;
		}

		c = 'a';
		while(c != ':')
		{
			read(fd, &c, 1);
			if (c != ':')
				write(1, &c, 1);
		}
		write(1, " in German: \n", 13);

		letters = 0;
		read(fd, &c, 1);
		while(c != '\n')
		{
			read(fd, &c, 1);
			letters++;
		}
		str = malloc(sizeof(char) * (letters + 1));
		lseek(fd, -letters, SEEK_CUR);
		read(fd, str, letters);
		str[letters] = '\0';

		c = 'a';
		j = 0;
		i_read = 1;
		wrong = 0;
		while(i_read > 0 && j < letters)
		{
			i_read = read(0, &c, 1);
			if (c != str[j])
			{
				wrong = 1;
				break;
			}
			j++;
		}
		if (wrong)
			write(1, "Incorrect answer\n", 17);
		else
			write(1, "Correct answer\n", 15);
		lseek(fd, 0, SEEK_SET);
		i++;
	}
	free(str);
	close(fd);
}