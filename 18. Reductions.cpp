#include <iostream>
#include <cstdlib>
#include <mpi.h>

int main(int argc, char **argv) {

    // Inicializa o ambiente MPI.
    MPI_Init(&argc, &argv);  

    int size, rank;
    // Obtém o número total de processos.
    MPI_Comm_size(MPI_COMM_WORLD, &size);  
    // Obtém o rank do processo.
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  

    // Inicializa o buffer com zeros.
    constexpr int buffer_count = 5000;
    float buffer[buffer_count];
    memset(buffer, 0, sizeof(buffer));  

    for (int rep = 0; rep < 1000; ++rep) {
        for (int i = 0; i < buffer_count; ++i) {
            // Gera um valor aleatório entre 0 e 1.
            float val = (float)rand() / RAND_MAX;  
            // Adiciona o valor gerado ao buffer.
            buffer[i] += val;  
        }
    }

    // Inicializa o buffer de recepção com zeros.
    float reception[buffer_count];
    memset(reception, 0, sizeof(reception));  

    // Reduz os buffers de todos os processos para o processo 0 somando os valores.
    MPI_Reduce(buffer, reception, buffer_count, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Se for o processo 0 ...
    if (rank == 0) {
        for (int i = 0; i < buffer_count; ++i)
            // Imprime os resultados finais no processo 0.
            std::cout << reception[i] << std::endl;  
    }

    // Finaliza o ambiente MPI.
    MPI_Finalize();  

    return 0;
}
