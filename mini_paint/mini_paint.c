#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct s_paint
{
    int width;
    int height;
    char bg;
    char matrix[300][300];
} t_paint;

typedef struct s_shape
{
    char type;
    float x;
    float y;
    float radius;
    char c;

} t_shape;

t_paint p;
t_shape s;

FILE *f;

void print_error(char *str)
{
    int i = 0;
    while (str[i] != '\0')
        i++;
    write(1, str, i);
}

void fill_background()
{
	for (int j = 0; j < p.height; j++)
	{
		for (int i = 0; i < p.width; i++)
		{
			p.matrix[i][j] = p.bg;
		}
	}
}

void draw_shape()
{
	float dist;

	for (int j = 0; j < p.height; j++)
	{
		for (int i = 0; i < p.width; i++)
		{
			dist = sqrtf( (i - s.x) * (i - s.x) + (j - s.y) * (j - s.y) );
			if (dist <= s.radius)
				p.matrix[i][j] = s.c;
		}
	}
}

void draw_outline()
{
	float dist;

	for (int j = 0; j < p.height; j++)
	{
		for (int i = 0; i < p.width; i++)
		{
			dist = sqrtf( (i - s.x) * (i - s.x) + (j - s.y) * (j - s.y) );
			if (dist >= s.radius - 1 && dist < s.radius)
				p.matrix[i][j] = s.c;
		}
	}
}

void draw_matrix()
{
	for (int j = 0; j < p.height; j++)
	{
		for (int i = 0; i < p.width; i++)
		{
			write(1, &p.matrix[i][j], 1);
		}
		write(1, "\n", 1);
	}
}

int main(int argc, char **argv)
{
    int r;
    if (argc != 2)
    {
        print_error("Error: argument\n");
        return (1);
    }
	f = fopen(argv[1], "r");
    if (f == NULL)
    {
        print_error("Error: Operation file corrupted\n");
        return (1);
    }
    r = fscanf(f, "%d %d %c\n", &p.width, &p.height, &p.bg);
    if (r != 3 || p.width > 300 || p.height > 300 || p.width <= 0 || p.height <= 0)
    {
        print_error("Error: Operation file corrupted\n");
        return (1);
    }
	fill_background();
	while ((r = fscanf(f, "%c %f %f %f %c\n", &s.type, &s.x, &s.y, &s.radius, &s.c)) == 5)
	{
		if (s.type == 'C')
		{
			draw_shape();
		}
		if (s.type == 'c')
		{
			draw_outline();
		}
		if ((s.type != 'C' && s.type != 'c') || s.radius <= 0)
		{
		    print_error("Error: Operation file corrupted\n");
        	return (1);
		}
	}
	if (r != -1)
	{
		print_error("Error: Operation file corrupted\n");
        return (1);
	}
	draw_matrix();
    return (0);
}