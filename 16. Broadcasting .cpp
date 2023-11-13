#include <iostream>
#include <iomanip>
#include <mpi.h>

int main(int argc, char **argv) {

    // Inicializa o ambiente MPI.
    MPI_Init(&argc, &argv);  

    int rank, size;
    // Obtém o rank do processo.
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  
    // Obtém o número total de processos.
    MPI_Comm_size(MPI_COMM_WORLD, &size); 

    int n_elements;
    double *buffer;

    // Se for o processo 0 ...
    if (rank == 0) {

        // Lê o número de elementos a serem transmitidos.
        std::cin >> n_elements;  
        // Aloca espaço para o buffer no processo.
        buffer = new double[n_elements];  

        for (int i = 0; i < n_elements; ++i)
        // Lê os elementos e os armazena no buffer.
        std::cin >> buffer[i];  
    }

    // Transmite o número de elementos para todos os processos.
    MPI_Bcast(&n_elements, 1, MPI_INT, 0, MPI_COMM_WORLD);  

    // Se for um processo diferente de 0 ...
    if (rank != 0)
        // Aloca espaço para o buffer nos processos.
        buffer = new double[n_elements];  

    // Transmite o buffer para todos os processos.
    MPI_Bcast(buffer, n_elements, MPI_DOUBLE, 0, MPI_COMM_WORLD); 

    double sum = 0.0;

    // Se for o processo 0 ...
    if (rank == 0) {
        for (int i = 0; i < n_elements; ++i)
        // Calcula a soma de todos os elementos no buffer.
        sum += buffer[i];  
    
    // Se for o processo 1 ...
    } else if (rank == 1) {
        for (int i = 0; i < n_elements; ++i)
        if (buffer[i] > 0)
            // Calcula a soma apenas dos elementos positivos no buffer.
            sum += buffer[i];  

    // Se for o processo 2 ...
    } else if (rank == 2) {
        for (int i = 0; i < n_elements; ++i)
        if (buffer[i] < 0)
            // Calcula a soma apenas dos elementos negativos no buffer.
            sum += buffer[i];  
    }

    // Imprime a soma com precisão de 16 dígitos.
    std::cout << std::setprecision(16) << sum << std::endl;  

    
    // Finaliza o ambiente MPI.
    MPI_Finalize();

    // Libera a memória alocada para o buffer.
    delete[] buffer;  

    return 0;
}