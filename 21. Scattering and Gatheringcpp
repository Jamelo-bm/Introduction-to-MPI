#include <iostream>
#include <mpi.h>
#include <cmath>

// Declaração de variáveis globais para rank e size.
int rank, size;

// Constante para o número de pontos por lado na grade.
constexpr int p_count = 512;

// Variáveis globais para os parâmetros do conjunto de Mandelbrot.
int cutoff;
double min_x, max_x, min_y, max_y, dx, dy;

// Função para calcular o módulo de um número complexo.
double modulus(double x, double y) {
  return sqrt(x * x + y * y);
}

// Função para multiplicar dois números complexos.
void self_mul(double &x, double &y) {
  double ox = x * x - y * y;
  double oy = x * y + y * x;
  x = ox;
  y = oy;
}

// Função para calcular o conjunto de Mandelbrot para um conjunto de pontos.
void compute_mandelbrot(double *points, int npts, int mset[]) {
  for (int i = 0; i < npts; ++i) {
    double px, py;
    px = points[i * 2];
    py = points[i * 2 + 1];

    int iteration = 0;
    double zx = 0;
    double zy = 0;

    while (iteration < cutoff) {
      self_mul(zx, zy);
      zx += px;
      zy += py;
      double mod = modulus(zx, zy);

      if (mod > 2.0f)
        break;

      iteration++;
    }

    if (iteration == cutoff)
      mset[i] = -1;
    else
      mset[i] = iteration;
  }
}

int main(int argc, char **argv) {
  // Inicializa o ambiente MPI.
  MPI_Init(&argc, &argv);

  // Obtém os parâmetros da linha de comando.
  min_x = std::stod(argv[1]);
  max_x = std::stod(argv[2]);
  min_y = std::stod(argv[3]);
  max_y = std::stod(argv[4]);
  dx = max_x - min_x;
  dy = max_y - min_y;
  cutoff = std::stoi(argv[5]);

  // Obtém o rank e o tamanho do comunicador MPI_COMM_WORLD.
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // Calcula o número total de pontos na grade.
  int total_npts = p_count * p_count;

  // Calcula o número de pontos locais para cada processo.
  int local_npts = total_npts / size;

  // Aloca memória para armazenar os pontos locais.
  double *local_points = new double[local_npts * 2];

  if (rank == 0) {
    // Se o rank for 0, gera todos os pontos e distribui para os outros processos.
    double *points = new double[total_npts * 2];
    for (int yp = 0; yp < p_count; ++yp) {
      double py = min_y + dy * yp / p_count;
      for (int xp = 0; xp < p_count; ++xp) {
        double px = min_x + dx * xp / p_count;

        int lid = yp * p_count * 2 + xp * 2;
        points[lid] = px;
        points[lid + 1] = py;
      }
    }

    // Distribui os pontos para os outros processos usando MPI_Scatter.
    MPI_Scatter(points, local_npts * 2, MPI_DOUBLE,
                local_points, local_npts * 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    delete[] points;
  } else {
    // Recebe os pontos distribuídos usando MPI_Scatter.
    MPI_Scatter(nullptr, local_npts * 2, MPI_DOUBLE,
                local_points, local_npts * 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  // Atualiza local_npts para o último processo, que pode ter mais pontos.
  local_npts = total_npts - (size - 1) * local_npts;

  // Aloca memória para armazenar o conjunto de Mandelbrot local.
  int *local_mset = new int[local_npts];

  // Calcula o conjunto de Mandelbrot para os pontos locais.
  compute_mandelbrot(local_points, local_npts, local_mset);

  // Aloca memória para armazenar o conjunto de Mandelbrot global.
  int *global_mset = nullptr;

  if (rank == 0) {
    global_mset = new int[total_npts];
  }

  // Coleta os conjuntos de Mandelbrot locais usando MPI_Gather.
  MPI_Gather(local_mset, local_npts, MPI_INT,
             global_mset, local_npts, MPI_INT, 0, MPI_COMM_WORLD);

  // Imprime o conjunto de Mandelbrot global se o rank for 0.
  if (rank == 0) {
    for (int yp = 0; yp < p_count; ++yp) {
      for (int xp = 0; xp < p_count; ++xp)
        std::cout << global_mset[yp * p_count + xp] << " ";
      std::cout << std::endl;
    }
  }

  // Libera a memória alocada.
  delete[] local_points;
  delete[] local_mset;

  if (rank == 0) {
    delete[] global_mset;
  }

  // Finaliza o ambiente MPI.
  MPI_Finalize();
  return 0;
}
