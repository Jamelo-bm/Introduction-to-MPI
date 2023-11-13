void probing_process(int &int_sum, float &float_sum) {

  // Inicializa o MPI_Status para armazenar informações sobre o status da operação.
  MPI_Status status;
  
  // Status MPI para armazenar informações sobre a mensagem sondada
  MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

  // Obtêm a etiqueta (tag) e a origem (source)
  int tag = status.MPI_TAG;
  int source = status.MPI_SOURCE;

  // Imprimir a mensagem recebida
  std::cout << "Received a message from process " << source << " with tag " << tag << std::endl;

  // Se a tag for 0 ...
  if (tag == 0){

    // Recebe a mensagem como um único inteiro
    int received_int;
    MPI_Recv(&received_int, 1, MPI_INT, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    
    // Adiciona a int_sum
    int_sum += received_int;
  }
  // Se a tag for 1 ...
  else if (tag == 1){
    // Recebe a mensagem como um único float
    float received_float;
    MPI_Recv(&received_float, 1, MPI_FLOAT , source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // Adiciona à variável float_sum
    float_sum += received_float;
  }
}