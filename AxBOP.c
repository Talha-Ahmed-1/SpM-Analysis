#include <stdio.h>
#include <stdlib.h>

void OuterProduct(const int M, const int K, int cNNZ, float* aD, int* aC, int* aR, float* bD, int* bC, int* bR, float* cD, int* cC, int* cR){
    int estimated_cNNZ = cNNZ;
    float* cD_ = (float *)malloc(estimated_cNNZ*sizeof(float));
    for(int col=0; col<K; col++){
        int start = aC[col];
        int end = aC[col+1];
        for(int row=start; row<end; row++){
            float valA = aD[row];
            float valB = 0.0;
            for(int b_row=bR[col]; b_row<bR[col+1]; b_row++){
                valB = bD[b_row];
                long long int idx = (long long int)aR[row]*(long long int)K + bC[b_row];
                if (idx > estimated_cNNZ){
                    estimated_cNNZ = idx + 1;
                    float* temp_cD = (float *)realloc(cD_, (estimated_cNNZ)*sizeof(float));
                    cD_ = temp_cD;
                }
                cD_[idx] += valA * valB;
            }
        }
    }
    int col_count = 0;
    cC[0] = col_count;
    for(int i=0; i<K; i++){
        for(int j=0; j<M; j++){
            long long int idx = (long long int)j*(long long int)K + i;
            if (idx < estimated_cNNZ){
                if(cD_[idx] != 0){
                    cR[col_count] = j;
                    cD[col_count] = cD_[idx];
                    col_count ++;
                }
            }
        }
        cC[i+1] = col_count;
    }
}


float cNNZ_estimation(int M, int K, int N, int aNNZ, int bNNZ){
    long long MxK = (long long)M*(long long)K;
    float density = (float)(aNNZ)/MxK + (float)(bNNZ)/MxK;
    return density * M * N;
}

int main(){
    FILE *file;
    FILE *file1;
    FILE *file2;
    FILE *file4;
    ////////////
    FILE *file5;
    FILE *file6;
    FILE *file7;

    int* gInfo = (int *)malloc(4*sizeof(int));
    //////////////////////////////////////////
    int number;
    file4 = fopen("MatData/GeneralInfo.txt", "r");
    if (file4 == NULL) {
        perror("Error opening file");
        return -1;
    }
    int idx = 0;
    while (fscanf(file4, "%d", &number) == 1) {
        gInfo[idx] = number;
        idx++;
    }
    fclose(file4);
    //////////////////////////////////////////
    // const int rows  = gInfo[0];
    // const int cols  = gInfo[1];
    // const int NNZ   = gInfo[2];

    const int M    = gInfo[0];
    const int K    = gInfo[1];
    const int N    = gInfo[0];
    const int aNNZ = gInfo[2];
    const int bNNZ = gInfo[2];

    float* aD = (float *)malloc(aNNZ*sizeof(float));
    int*   aC = (int *)malloc(aNNZ*sizeof(int));
    int*   aR = (int *)malloc((M+1)*sizeof(int));

    float* bD = (float *)malloc(bNNZ*sizeof(float));
    int*   bR = (int *)malloc(bNNZ*sizeof(int));
    int*   bC = (int *)malloc((K+1)*sizeof(int));
    //////////////////////////////////////////
    idx = 0;
    file = fopen("MatData/CSR_values.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }
    float val;
    while (fscanf(file, "%f", &val) == 1) {
        aD[idx] = val;
        idx++;
    }

    fclose(file);

    //////////////////////////////////////////
    file1 = fopen("MatData/CSR_colIdx.txt", "r");
    if (file1 == NULL) {
        perror("Error opening file");
        return -1;
    }
    idx = 0;
    while (fscanf(file1, "%d", &number) == 1) {
        aC[idx] = number;
        idx++;
    }
    fclose(file1);
    //////////////////////////////////////////
    file2 = fopen("MatData/CSR_rowPtr.txt", "r");
    if (file2 == NULL) {
        perror("Error opening file");
        return -1;
    }
    idx = 0;
    while (fscanf(file2, "%d", &number) == 1) {
        aR[idx] = number;
        idx++;
    }
    fclose(file2);
    //////////////////////////////////////////
    file5 = fopen("MatData/CSC_rowIdx.txt", "r");
    if (file5 == NULL) {
        perror("Error opening file");
        return -1;
    }
    idx = 0;
    while (fscanf(file5, "%d", &number) == 1) {
        bR[idx] = number;
        idx++;
    }
    fclose(file5);
    //////////////////////////////////////////
    file6 = fopen("MatData/CSC_colPtr.txt", "r");
    if (file6 == NULL) {
        perror("Error opening file");
        return -1;
    }
    idx = 0;
    while (fscanf(file6, "%d", &number) == 1) {
        bC[idx] = number;
        idx++;
    }
    fclose(file6);
    //////////////////////////////////////////
    file7 = fopen("MatData/CSC_values.txt", "r");
    if (file7 == NULL) {
        perror("Error opening file");
        return -1;
    }
    idx = 0;
    while (fscanf(file7, "%f", &val) == 1) {
        bD[idx] = val;
        idx++;
    }
    fclose(file7);
    //////////////////////////////////////////

    int cNNZ = cNNZ_estimation(M,K,N,aNNZ,bNNZ);
    printf("Estimated cNNZ = %d \n", cNNZ);

    float* cD = (float *)malloc(cNNZ*sizeof(float));
    int* cR = (int *)malloc(cNNZ*sizeof(int));
    int* cC = (int *)malloc((M+1)*sizeof(int));

    // float* data_list = (float *)malloc((long long int)M*(long long int)K*sizeof(float));

    OuterProduct(M, K, cNNZ, bD, bC, bR, aD, aC, aR, cD, cC, cR);

    // for(int i = 0; i<cC[M]; i++){
    //     printf("%d %.0f\n", cR[i], cD[i]);
    // }

    // for (int i = 0; i < M+1; i++){
    //     printf("%d\n", cC[i]);
    // }

    // for(int i=0; i<M+1; i++){
    //     for(int j=cC[i]; j<cC[i+1]; j++){
    //         printf("%.f ", cD[j]);
    //     }
    //     printf("\n");
    // }

    printf("Actual cNNZ = %d \n", cC[M]);

    free(aD);
    free(aC);
    free(aR);
    free(bD);
    free(bC);
    free(bR);
    // free(cD);
    // free(cC);
    // free(cR);
    free(gInfo);
    return 0;
}
