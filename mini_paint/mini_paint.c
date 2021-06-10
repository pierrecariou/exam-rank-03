#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

typedef struct	drawing {
	int		width;
	int		height;
	char	background_char;
	char	*m;
}				draw;

typedef struct	line {
	char	full;
	float	X;
	float	Y;
	float	radius;
	char	c;
}				l;

/*
int		ft_strlen(char *str)
{
	int i = 0;

	while (str[i])
		i++;
	return (i);
}
*/

void	ft_putstr(char *str)
{
	int i = -1;

	while (str[++i])
		write(1, &str[i], 1);
}

void	ft_draw(draw *draw)
{
	int i = 0;
	int w = 0;

	while (draw->m[i])
	{
		while (w++ < draw->width)
			write(1, &draw->m[i++], 1);	
		write(1, "\n", 1);
		w = 0;
	}
}

int		first_line(FILE *file, draw *draw)
{
	int i = 0;

	if (fscanf(file, "%d %d %c\n", &draw->width, &draw->height, &draw->background_char) != 3)
		return (1);
	if (draw->width <= 0 || draw->width > 300 || draw->height <= 0 || draw->height > 300)
		return (1);
	if (!(draw->m = malloc(sizeof(char) * (draw->width * draw->height + 1))))
		return (1);
	while (i < (draw->width * draw->height))
		draw->m[i++] = draw->background_char;
	draw->m[i] = 0;
	return (0);
}

float	distance(float x, float y, l *l)
{
	return (sqrtf((x - l->X) * (x - l->X) + (y - l->Y) * (y - l->Y)));
}

int		border(float dist, l *l)
{
	float result = dist - l->radius;
	
	if (result < 0.00000000)
		result *= -1;
	if (result < 1.00000000)
		return (1);
	return (0);
}

int		in_rectangle(float x, float y, l *l)
{
	// pixel not in rectangle
	if (distance(x, y, l) > l->radius)
		return (0);
	// border
	if (border(distance(x, y, l), l))
		return (2);
	return (1);
}

int		pixel(draw *draw, l *l, int x, int y)
{
	int	in;

	in = in_rectangle(x, y, l);
	if ((in == 1 && l->full == 'C') || in == 2)
		draw->m[x + y * draw->width] = l->c;
	return (0);
}

int		calculations(draw *draw, l *l)
{
	int x = -1;;
	int y = -1;

	if (l->radius <= 0.00000000 || (l->full != 'c' && l->full != 'C'))
		return (1);
	while (++x < draw->width)
	{
		while (++y < draw->height)
			pixel(draw, l, x, y);
		y = -1;
	}
	return (0);
}

int		other_lines(FILE *file, draw *draw, l *l)
{
	int	ret;
	
	while ((ret = fscanf(file, "%c %f %f %f %c\n", &l->full, &l->X, &l->Y, &l->radius,  &l->c)) == 5)
	{
		if (calculations(draw, l))
			return (1);
	}
	if (ret != -1)
		return (1);
	return (0);
}

int		execute(FILE *file)
{
	draw	draw;
	l		l;

	if (first_line(file, &draw))
		return (1);
	if (other_lines(file, &draw, &l))
		return (1);
	ft_draw(&draw);
	return (0);
}

int		main(int argc, char **argv)
{
	FILE *file;

	if (argc != 2)
	{
		ft_putstr("Error: argument\n");
		return (1);
	}
	if (!(file = fopen(argv[1], "r")))
	{
		ft_putstr("Error: Operation file corrupted\n");
		return (1);
	}
	if (execute(file))
	{
		ft_putstr("Error: Operation file corrupted\n");
		fclose(file);
		return (1);
	}
	fclose(file);
	return (0);
}
