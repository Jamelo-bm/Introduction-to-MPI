void compute(int total_count, int my_count, float my_points[][3]) {
  
    // Inicializa um vetor local para armazenar a soma das coordenadas x, y e z.
    float local_sum[3] = {0.0f, 0.0f, 0.0f};

    // Calcula a soma local das coordenadas x, y e z nos pontos pertencentes a este processo.
    for (int i = 0; i < my_count; ++i) {
        local_sum[0] += my_points[i][0];
        local_sum[1] += my_points[i][1];
        local_sum[2] += my_points[i][2];
    }

    // Inicializa um vetor para armazenar o baricentro global.
    float barycentre[3];

    // Reduz todas as somas locais para obter o baricentro global usando MPI_Allreduce.
    MPI_Allreduce(local_sum, barycentre, 3, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

    // Calcula a média para obter o baricentro global.
    barycentre[0] /= total_count;
    barycentre[1] /= total_count;
    barycentre[2] /= total_count;

    // Calcula as distâncias entre cada ponto local e o baricentro global.
    for (int i = 0; i < my_count; ++i) {
        float dist = 0.0f;

        // Calcula a distância euclidiana entre o ponto local e o baricentro global.
        dist = sqrt((my_points[i][0] - barycentre[0]) * (my_points[i][0] - barycentre[0]) +
                    (my_points[i][1] - barycentre[1]) * (my_points[i][1] - barycentre[1]) +
                    (my_points[i][2] - barycentre[2]) * (my_points[i][2] - barycentre[2]));

        // Imprime o rank do processo e a distância calculada.
        std::cout << rank << " " << dist << std::endl;
    }
}
