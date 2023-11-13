#include <iostream> // Inclui a biblioteca para entrada e saída de dados.
#include <mpi.h> // Inclui a biblioteca MPI.

int main(int argc, char** argv) {

	 // Inicia o ambiente MPI.
	MPI_Init(&argc, &argv); 
	
	// Declaração de variáveis para armazenar o tamanho e rank do comunicador.
	int size, rank;  

	// Obtém o número de processos no comunicador global, armazenando-o em size.
	MPI_Comm_size(MPI_COMM_WORLD, &size);  

	// Obtém o rank do processo no comunicador global, armazenando-o em rank.
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
	
	// Imprime uma mensagem com o rank do processo.
	std::cout << "Hello world, from process #" << rank << std::endl;  
	
	// Finaliza o ambiente MPI.
	MPI_Finalize();

	return 0; 
}