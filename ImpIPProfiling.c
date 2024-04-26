#include <stdio.h>
#include <stdlib.h>

void ImprovedInnerProduct(const int rows, const int cols, const int rcols, float* aD, int* aC, int* aC_, int* aR, float* data_list){
    for(int row=0; row<rows; row++){
        int start = aR[row];
        int end = aR[row+1];
        for(int b_col=0; b_col<rcols; b_col++){
            for(int col=start; col<end; col++){
                float valA = 0;
                float valB = aD[col];
                int r = aC[col];
                float sum = 0;
                int idx = 0;
                for(int i=aR[r]; i<aR[r+1]; i++){
                    if(aC_[i] == b_col){
                        valA = aD[i];
                        sum += valA * valB; 
                        idx = i;
                        break;
                    }
                }
                if(sum != 0){
                    data_list[(long long int)row*(long long int)cols + aC[idx]] += sum;
                }
            }
        }
    }
}

int main(){
    FILE *file;
    FILE *file1;
    FILE *file2;
    FILE *file3;
    FILE *file4;

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
    const int rows  = gInfo[0];
    const int cols  = gInfo[1];
    const int NNZ   = gInfo[2];
    const int rcols  = gInfo[3];

    float* aD = (float *)malloc(NNZ*sizeof(float));
    int*   aC = (int *)malloc(NNZ*sizeof(int));
    int*   aR = (int *)malloc((rows+1)*sizeof(int));
    int*   aC_ = (int *)malloc(NNZ*sizeof(int));

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
    file3 = fopen("MatData/CSR_redColIdx.txt", "r");
    if (file3 == NULL) {
        perror("Error opening file");
        return -1;
    }
    idx = 0;
    while (fscanf(file3, "%d", &number) == 1) {
        aC_[idx] = number;
        idx++;
    }
    fclose(file3);
    //////////////////////////////////////////

    float* data_list = (float *)malloc((long long int)rows*(long long int)cols*sizeof(float));

    ImprovedInnerProduct(rows, cols, rcols, aD, aC, aC_, aR, data_list);

    // for(int i=0; i<rows; i++){
    //     for(int j=0; j<cols; j++){
    //         printf("%.0f ", data_list[i*cols + j]);
    //     }
    //     printf("\n");
    // }

    free(aD);
    free(aC);
    free(aR);
    free(aC_);
    free(data_list);

    return 0;
}
