void play_non_blocking_scenario() {

	// Inicializa o MPI_Request para rastrear a operação não-bloqueante.
  MPI_Request request;

	// Inicializa o MPI_Status para armazenar informações sobre o status da operação.
  MPI_Status  status;
	
	// Variável indicando se a operação não-bloqueante foi concluída.
  int request_finished = 0;  

  // Inicializando o buffer com valores diferentes para o processo 0 e 1:
  for (int i=0; i < buffer_count; ++i)
    buffer[i] = (rank == 0 ? i*2 : 0);  

	// Garante que todos os processos alcancem este ponto antes de prosseguir.
  MPI_Barrier(MPI_COMM_WORLD);  
  
  // Iniciando o cronômetro
  double time = -MPI_Wtime();  
  
	// Se for o processo 0 ...
  if (rank == 0) {

		// Aguarda 3 segundos
    sleep(3);

    // Inicializa o envio não-bloqueante para o processo 1
      // Args.1 (buffer): Indica o endereço do dado a ser enviado;
      // Args.2 (buffer_count): Número de dados a serem enviados;
      // Args.3 (MPI_INT): Tipo de dado a ser enviado;
      // Args.4 (1): Rank de Destino;
      // Args.5 (0): Tag;
      // Args.6 (MPI_COMM_WORLD): Nome do Comunicador.
      // Args.7 (&request): Ponteiro para uma estrutura MPI_Request, que será usada para verificar ou esperar pela conclusão da operação.
		MPI_Isend(buffer, buffer_count, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
	
		// Inicializa a variável de controle de tempo restante
    double time_left = 6000.0;

		// Enquanto houver tempo restante para executar ...
    while (time_left > 0.0) {
			// Aguarda 1 milissegundo para simular o trabalho
      usleep(1000); 
      
      // 2. Testa se a requisição foi concluída (somente se ainda não foi concluída)
      MPI_Test(&request, &request_finished, &status);
      
      // Reduz o tempo restante em 1 segundo
      time_left -= 1000.0;
    }

    // Se a requisição não foi concluída ... 
    if (!request_finished)
				// Aguarde até que ela seja completada.
        MPI_Wait(&request, &status);

    // Modificando o buffer para a segunda etapa
    for (int i=0; i < buffer_count; ++i)
      buffer[i] = -i;

    // Prepara outra requisição para o processo 1 com uma tag diferente
    MPI_Isend(buffer, buffer_count, MPI_INT, 1, 1, MPI_COMM_WORLD, &request);
    
		// Define um valor para a variável de controle de tempo restante
    time_left = 3000.0;
		
		// Enquanto houver tempo restante para executar ...
    while (time_left > 0.0) {
			// Aguarda 1 milissegundo para simular o trabalho
      usleep(1000);
      
      // Testa se a requisição foi concluída (somente se ainda não foi concluída)
      MPI_Test(&request, &request_finished, &status);

      // Reduz o tempo restante em 1 segundo
      time_left -= 1000.0;
    }

		// Se a requisição não foi concluída ... 
    if (!request_finished)
				// Aguarde até que ela seja completada.
        MPI_Wait(&request, &status);
  }

	// Se for algum processo diferente de 0 ...
  else {
    
		// Trabalha por 5 segundos
    sleep(5);

    // Inicializa a recepção não-bloqueante do processo 0
    MPI_Irecv(buffer, buffer_count, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);

		// Espera aqui pela conclusão da requisição
    MPI_Wait(&request, &status);

		// Função para imprimir o conteúdo do buffer
    print_buffer();  
    
    // Trabalha por 3 segundos
    sleep(3);

    // Inicializa outra recepção não-bloqueante
    MPI_Irecv(buffer, buffer_count, MPI_INT, 0, 1, MPI_COMM_WORLD, &request);
    
    // Espera que seja concluída
    MPI_Wait(&request, &status);

    print_buffer();  // Imprime novamente o conteúdo do buffer
  }

  // Parando o cronômetro
  time += MPI_Wtime();

  // Esta linha nos dá o tempo máximo decorrido em cada processo.
  double final_time;
  MPI_Reduce(&time, &final_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  
  // Se for o processo 0 ...
  if (rank == 0)
    std::cout << "Tempo total para o cenário não-bloqueante: " << final_time << "s" << std::endl;
}