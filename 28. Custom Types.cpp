#include <iostream>
#include <cstdlib>
#include <cmath>
#include <mpi.h>
#include <cstddef>

// Constante representando o tamanho máximo do array de valores.
constexpr int DOUBLE_MAX = 10;

// Estrutura para armazenar dados personalizados.
struct CustomData {
  int n_values;
  double values[DOUBLE_MAX];
};

int main(int argc, char **argv) {
  // Inicializa o ambiente MPI.
  MPI_Init(&argc, &argv);

  int rank, size;
  // Obtém o rank e o tamanho do comunicador MPI_COMM_WORLD.
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Constante que define o número de estruturas de dados por processo.
  constexpr int n_structure_per_process = 5; // M = 5

  // Inicializa a semente para a geração de números aleatórios com base no rank.
  srand(rank * 10);

  // Array de estruturas CustomData para armazenar dados locais.
  CustomData data[n_structure_per_process];

  // Preenche as estruturas CustomData com dados aleatórios.
  for (int i = 0; i < n_structure_per_process; ++i) {
    data[i].n_values = rand() % DOUBLE_MAX + 1;
    for (int j = 0; j < DOUBLE_MAX; ++j)
      data[i].values[j] = (j < data[i].n_values ? (double)rand() / (double)RAND_MAX : 0.0);
  }

  // Define deslocamentos e tipos para criar um tipo de dado MPI personalizado.
  MPI_Aint displacements[2] = {offsetof(CustomData, n_values), offsetof(CustomData, values)};
  int block_lengths[2] = {1, DOUBLE_MAX};
  MPI_Datatype types[2] = {MPI_INT, MPI_DOUBLE};
  MPI_Datatype custom_dt;

  // Cria o tipo de dado MPI personalizado.
  MPI_Type_create_struct(2, block_lengths, displacements, types, &custom_dt);
  MPI_Type_commit(&custom_dt);

  CustomData *gathered_data = nullptr;

  if (rank == 0)
    // Se o rank for 0, aloca espaço para armazenar os dados coletados.
    gathered_data = new CustomData[n_structure_per_process * size];

  // Coleta os dados personalizados de todos os processos usando MPI_Gather.
  MPI_Gather(data, n_structure_per_process, custom_dt,
             gathered_data, n_structure_per_process, custom_dt, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    // Se o rank for 0, imprime os dados coletados.
    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < n_structure_per_process; ++j) {
        int data_id = i * n_structure_per_process + j; // Índice linear

        std::cout << "Data structure " << data_id << " : [";
        int N = gathered_data[data_id].n_values;

        for (int k = 0; k < N; ++k) {
          std::cout << gathered_data[data_id].values[k] << (k == N - 1 ? "]" : "; ");
        }
        std::cout << std::endl;
      }
    }
  }

  // Libera o tipo de dado MPI personalizado.
  MPI_Type_free(&custom_dt);
  if (rank == 0)
    // Se o rank for 0, libera o espaço alocado para os dados coletados.
    delete[] gathered_data;

  // Finaliza o ambiente MPI.
  MPI_Finalize();

  return 0;
}
