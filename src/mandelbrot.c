#include "mandelbrot.h"
#include "complex.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

mandelbrot_params mandelbrot_params_new(double x, double y, double zoom)
{
  mandelbrot_params p = {.focal_x = x, .focal_y = y, .zoom = zoom};
  return p;
}

int mandelbrot_params_eq(mandelbrot_params *a, mandelbrot_params *b)
{
  return (a->focal_x == b->focal_x) && (a->focal_y == b->focal_y) &&
         (a->zoom == b->zoom) && (a->max_iterations == b->max_iterations);
}

double run_mandelbrot(complex *c, int max_iterations)
{
  complex z = complex_new(0.0, 0.0);
  int i = 0;
  while (i < max_iterations)
  {
    complex r = complex_multiply(&z, &z);
    z = complex_add(c, &r);

    if (complex_magnitude_lts(&z, 4.0))
    {
      break;
    }

    i++;
  }
  return (double)i / (double)max_iterations;
}

void calc_bounds(complex *min_c, complex *max_c, complex *focal_point,
                 double zoom, double aspect_ratio)
{
  min_c->re = focal_point->re - (aspect_ratio / zoom);
  min_c->im = focal_point->im - (1.0 / zoom);

  max_c->re = focal_point->re + (aspect_ratio / zoom);
  max_c->im = focal_point->im + (1.0 / zoom);
}

typedef struct
{
  complex max_c;
  complex min_c;

  int image_x;
  int image_y;

  int rows;
  int cols;
  int total_rows;

  unsigned int max_iterations;

} mandelbrot_compute_subgrid_params;

void mandelbrot_compute_subgrid(mandelbrot_compute_subgrid_params *subgrid_params, double *data, process_buffer *process)
{
  double x_incr = (subgrid_params->max_c.re - subgrid_params->min_c.re) / ((double)subgrid_params->rows);
  double y_incr = (subgrid_params->max_c.im - subgrid_params->min_c.im) / ((double)subgrid_params->cols);

  int image_x = subgrid_params->image_x;
  int image_y = subgrid_params->image_y;

  int total = subgrid_params->cols * subgrid_params->rows;

  printf("x: %d | y: %d \n\n", image_x, image_y);

  // data[image_x + (image_y*subgrid_params->total_rows)] = 1;
  // return;

  for (double y = subgrid_params->min_c.im; y < subgrid_params->max_c.im; y += y_incr)
  {
    for (double x = subgrid_params->min_c.re; x < subgrid_params->max_c.re; x += x_incr)
    {
      complex current_point = complex_new(x, y);
      double c = run_mandelbrot(&current_point, subgrid_params->max_iterations);
      image_x += 1;
      int index = (image_y * subgrid_params->total_rows) + image_x;
      // int index = image_y + image_x;
      // int index = (600*image_y) + image_x;
      process->progress = (int)((float)100 * index / (float)total);
      data[index] = c;
      // data[index] = image_y;
    }
    if (process->trigger_stop)
    {
      break;
    }
    image_x = 0;
    image_y += 1;
  }
}

void run_mandelbrot_for_subgrid(complex *points, double *data, int idx_start, int idx_end, mandelbrot_params *params, process_buffer *process)
{
  for (int pi = idx_start; pi < idx_end; pi++)
  {
    double d = run_mandelbrot(&points[pi], params->max_iterations);
    data[pi] = d;
    if (process->trigger_stop)
    {
      break;
    }
  }
}

typedef struct
{
  complex *points;
  double *data;
  int idx_start;
  int idx_end;
  mandelbrot_params *params;
  process_buffer *process;
} SubgridComputeParams;

void _thread_mandelbrot_subgrid_compute(SubgridComputeParams *params)
{
  run_mandelbrot_for_subgrid(params->points, params->data, params->idx_start, params->idx_end, params->params, params->process);
}

pthread_t thread_mandelbrot_subgrid_compute(SubgridComputeParams *p)
{
  pthread_t thread_id; // Variable to hold the thread ID
  int result = pthread_create(&thread_id, NULL, _thread_mandelbrot_subgrid_compute, p);
  return thread_id;
}

void mandelbrot_compute(double *data, int rows, int cols,
                        mandelbrot_params *params, process_buffer *process)
{

  process->done = 0;
  double aspect_ratio = (double)rows / cols;
  complex min_c = complex_new(0.0, 0.0);
  complex max_c = complex_new(0.0, 0.0);
  complex focal_point = complex_new(params->focal_x, params->focal_y);

  calc_bounds(&min_c, &max_c, &focal_point, params->zoom, aspect_ratio);

  int total = rows * cols;
  double x_incr = (double)(max_c.re - min_c.re) / (double)cols;
  double y_incr = (double)(max_c.im - min_c.im) / (double)rows;

  complex *points = malloc(total * (sizeof(complex)));

  for (int i = 0; i < total; i++)
  {
    int y_idx = i / cols;
    int x_idx = i % cols;
    double y = min_c.im + y_idx * y_incr;
    double x = min_c.re + x_idx * x_incr;
    points[i] = complex_new(x, y);
  }

  const int threads = 420;
  pthread_t *thread_ids = malloc(sizeof(pthread_t) * threads);

  printf("calculing the subgrid using %d threads;", threads);
  int points_per_thread = total / threads;

  SubgridComputeParams *p;
  for (int pth = 0; pth < threads; pth++)
  {
    p = malloc(sizeof(SubgridComputeParams)); // Dynamically allocate memory for the parameters

    int idx_start = pth * points_per_thread;
    int idx_end = (pth + 1) * points_per_thread;
    if (idx_end >= total)
    {
      idx_end = total - 1;
    }

    *p = (SubgridComputeParams){
        .data = data,
        .idx_start = idx_start,
        .idx_end = idx_end,
        .params = params,
        .points = points,
        .process = process};

    thread_ids[pth] = thread_mandelbrot_subgrid_compute(p);
  }

  for (int pth = 0; pth < threads; pth++)
  {
    pthread_join(thread_ids[pth], NULL);
    process->progress = 100*((pth+1)/(double)threads);
  }

  free(p); // Assuming you store the pointers in an array or similar
  free(thread_ids);
  free(points);
  process->done = 1;
}
