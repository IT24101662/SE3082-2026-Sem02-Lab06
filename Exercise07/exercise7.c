#include <stdio.h>
#include <omp.h>

#define WIDTH 1000
#define HEIGHT 1000
#define MAX_ITER 2000

int main() {
    int count = 0;

    double xmin = -2.0;
    double xmax = 1.0;
    double ymin = 0.0;
    double ymax = 1.5;

    double start = omp_get_wtime();

#pragma omp parallel
    {
        int local_count = 0;

#pragma omp for
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {

                double cr = xmin + (xmax - xmin) * x / (WIDTH - 1);
                double ci = ymin + (ymax - ymin) * y / (HEIGHT - 1);

                double zr = cr;
                double zi = ci;

                int inside = 1;

                for (int i = 0; i < MAX_ITER; i++) {
                    double zr_new = zr * zr - zi * zi + cr;
                    double zi_new = 2.0 * zr * zi + ci;

                    zr = zr_new;
                    zi = zi_new;

                    if (zr * zr + zi * zi > 4.0) {
                        inside = 0;
                        break;
                    }
                }

                if (inside)
                    local_count++;
            }
        }

#pragma omp atomic
        count += local_count;
    }

    double end = omp_get_wtime();

    double width = xmax - xmin;
    double height = ymax - ymin;

    double area = ((double)count / (WIDTH * HEIGHT)) * width * height * 2.0;

    printf("Points inside: %d\n", count);
    printf("Estimated area: %.10f\n", area);
    printf("Time: %.6f seconds\n", end - start);

    return 0;
}
