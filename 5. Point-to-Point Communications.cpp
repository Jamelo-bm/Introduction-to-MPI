#include <iostream> // Inclui a biblioteca para entrada e saída padrão
#include <mpi.h> // Inclui a biblioteca MPI
#include <cstdlib> // Inclui a biblioteca para funções de conversão

int main(int argc, char **argv) {

	// Inicializa o ambiente MPI
  MPI_Init(&argc, &argv);  

	// Obtém o ranque do processo
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);  

  // local_value irá conter um inteiro específico para o processo 0 e outro para o processo 1.
  int local_value;

	// Converte o argumento da linha de comando para um inteiro
  local_value = atoi(argv[1]);

  int other_value;

	// Se for o processo 0 ...
  if (rank == 0) {

    // Envia o valor para o processo 1
      // Args.1 (&local_value): Indica o endereço do dado a ser enviado;
      // Args.2 (1): Número de dados a serem enviados;
      // Args.3 (MPI_INT): Tipo de dado a ser enviado;
      // Args.4 (1): Rank de Destino;
      // Args.5 (0): Tag;
      // Args.6 (MPI_COMM_WORLD): Nome do Comunicador.
    MPI_Send(&local_value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

    // Recebe o valor do processo 1 (em other_value)
      // Args.1 (&other_value): Indica o endereço que deve armazenar o dado recebido;
		  // Args.2 (1): Número de dados a serem recebidos;
		  // Args.3 (MPI_INT): Tipo de dado a ser recebido;
		  // Args.4 (1): Rank de Origem;
		  // Args.5 (0): Tag;
		  // Args.6 (MPI_COMM_WORLD): Nome do Comunicador.
      // Args.7 (MPI_STATUS_IGNORE): Retorna informações sobre o recebimento.
    MPI_Recv(&other_value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // Imprime a soma dos dois valores no stdout
    std::cout << local_value + other_value << std::endl;
  }

	// Se for o processo 1 ...
  else {

    // Recebe o valor do processo 0 (em other_value)
    MPI_Recv(&other_value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    
    // Envia o valor para o processo 0
    MPI_Send(&local_value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

    // Imprime o produto dos dois valores no stdout
    std::cout << local_value * other_value << std::endl;
  }
  
	// Finaliza o ambiente MPI
  MPI_Finalize();  
  
  return 0;
}