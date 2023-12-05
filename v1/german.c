// Añadir mas diccionarios, uno por semana y uno con las palabras mas dificiles que quiero repasar siempre (elegir por teclado)
// Añadir gestion de errores y protecciones opne, read, malloc... 
// Guardar numero respuestas correctas cada palabra (separar con otro ":"?) para preguntar más por las que no me se!!
	// Aumento el contador aun cuando hay mas palabras con menos iteraciones. Es necesario corregirlo o seguir asi??
	// Corregir 100 char *str = (char *)malloc(100 * sizeof(char)); en change_number()
	// calloc mejor?
// Probar si la última palabra del diccionario se está extrayendo también
// Necesito un último salto de linea para detectar fin de la ultima palabra

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

static void	part1(int *minus, int *tam, int *n, int *aux)
{
	*minus = 0;
	*tam = 0;
	if (*n == 0)
		*tam = 1;
	if (*n < 0)
	{
		*minus = 1;
		*n = *n * (-1);
	}
	*aux = *n;
	while (*aux >= 1)
	{
		*aux = *aux / 10;
		*tam = *tam + 1;
	}
}

static void	part2(int *pot, int *tam, int *aux)
{
	*aux = 10;
	*pot = 1;
	while (*tam > 1)
	{
		*pot = *pot * *aux;
		*tam = *tam - 1;
	}
	*aux = 0;
}

static char	*special_int(char *rtrn)
{
	if (!rtrn)
		return (NULL);
	rtrn[0] = '-';
	rtrn[1] = '2';
	rtrn[2] = '1';
	rtrn[3] = '4';
	rtrn[4] = '7';
	rtrn[5] = '4';
	rtrn[6] = '8';
	rtrn[7] = '3';
	rtrn[8] = '6';
	rtrn[9] = '4';
	rtrn[10] = '8';
	rtrn[11] = '\0';
	return (rtrn);
}

char	*ft_itoa(int n)
{
	int		aux;
	int		tam;
	int		pot;
	int		minus;
	char	*rtrn;

	if (n < -2147483647)
		return (special_int((char *)malloc(12)));
	part1(&minus, &tam, &n, &aux);
	rtrn = (char *)malloc(tam + 1 + minus);
	if (!rtrn)
		return (NULL);
	part2(&pot, &tam, &aux);
	if (minus)
		rtrn[aux++] = '-';
	while (pot >= 1)
	{
		rtrn[aux++] = '0' + (n / pot);
		n = n - (n / pot) * pot;
		pot = pot / 10;
	}
	rtrn[aux] = '\0';
	return (rtrn);
}

void reset()
{
  printf("\x1B[37m");
}

void red()
{
  printf("\033[0;31m");
}

void green()
{
  printf("\033[0;32m");
}

void move_fd_to_start(int fd)
{
	lseek(fd, 0, SEEK_SET);
}

void go_to_line(int fd, int *r)
{
	int newlines = 0;
	char c = 'a';

	move_fd_to_start(fd);
	while(newlines < *r) // Mover cursor al comienzo de la fila deseada
	{
		read(fd, &c, 1);
		if (c == '\n')
			newlines++;
	}
}

void change_number(int fd, int *r, int times_correct)
{
	char c = '1';
	char *str = (char *)malloc(100 * sizeof(char));
	int i = 0;

	go_to_line(fd, r);
	while (c >= '0' && c <= '9')
		read(fd, &c, 1);
	while (c != '\n')
	{
		str[i] = c;
		read(fd, &c, 1);
		i++;
	}
	str[i] = '\n';
	str[i + 1] = '\0';

	//printf("\n\nLinea guardada:%s\n\n", str);
	go_to_line(fd, r);
	write(fd, ft_itoa(times_correct + 1), ft_strlen(ft_itoa(times_correct + 1)));
	write(fd, str, ft_strlen(str));
	free(str);
}

int extract_info(int fd)
{
	int least_correct = 0;
	char c = 'a';

	move_fd_to_start(fd);
	while(c != '\n') // Extraer numero inicial
	{
		read(fd, &c, 1);
		if (c != '\n')
			least_correct = least_correct * 10 + (c - '0'); 
	}
	return least_correct;
}

int search(int fd, int dict_size, int *r)
{
	char c = 'a';
	int times_correct = 0;

	*r = rand() % dict_size + 1; // Elegir fila aleatoria (no quiero la del 0, info)
	go_to_line(fd, r);
	while(c != ':') // Extraer numero a la izquiera de los ":"
	{
		read(fd, &c, 1);
		if (c != ':')
			times_correct = times_correct * 10 + (c - '0'); 
	}
	return times_correct;
}

int main()
{
	// int fd = open("dictionary_a1_1_w1.txt", O_RDWR);
	int fd = open("test_dic.txt", O_RDWR);
	int i = 0;
	int j = 0;
	int letters = 0;
	int i_read = 1;
	int wrong = 0;
	char c = 'a';
	char *str;
	srand(time(NULL));
	int r = 0;
	int newlines = 0;
	int points = 0;
	int total_q = 0;
	int dict_size;
	int times_correct = 0;
	int least_correct = 0;

	least_correct = extract_info(fd);

	while(i_read) // Contar cuantas filas hay en el diccionario
	{
		i_read = read(fd, &c, 1);
		if (c == '\n')
			newlines++;
	}
	dict_size = newlines;
	printf("\nDictionary size: %d\n\n", dict_size);

	// printf("How many questions do you want?\n");
	// scanf("%d", &total_q);
	// printf("\n");
	total_q = 10;

	while(i < total_q) // Extraer preguntas
	{
		least_correct = extract_info(fd);
		times_correct = search(fd, dict_size, &r);
		while (times_correct > least_correct)
			times_correct = search(fd, dict_size, &r);

		// printf("%d\n", times_correct);

		read(fd, &c, 1);
		printf("\"");
		c = 'a';
		while(c != ':') // Extraer palabra a la izquiera de los ":"
		{
			read(fd, &c, 1);
			if (c != ':')
				printf("%c", c);
		}
		printf("\" in German: \n");

		c = 'a';
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
			if (c == '\n')
				break;
			j++;
		}
		while(i_read > 0 && c != '\n')
			i_read = read(0, &c, 1);			
		if (wrong)
		{
			red();
			printf("x \u2192 %s\n\n", str);
			reset();
		}
		else
		{
			// Primero guardar linea sin numero por prroblema 9 -> 10!!
			change_number(fd, &r, times_correct);
			green();
			printf("\xE2\x9C\x93\n\n");
			reset();
			points++;
			move_fd_to_start(fd);
			write(fd, ft_itoa(times_correct + 1), ft_strlen(ft_itoa(times_correct + 1)));
			write(fd, "\n", 1);
		}
		move_fd_to_start(fd);
		i++;
	}
	printf("\nSCORE: %d / %d\n", points, i);
	free(str);
	close(fd);
}