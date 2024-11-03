#include <stdio.h>
#include <stdlib.h>

int main() {
    int rows, cols;
    int **matrix1, **matrix2, **result;

    printf("행렬의 행 수를 입력하세요: ");
    scanf("%d", &rows);
    printf("행렬의 열 수를 입력하세요: ");
    scanf("%d", &cols);
    getchar();

    matrix1 = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
        matrix1[i] = (int *)malloc(cols * sizeof(int));

    matrix2 = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
        matrix2[i] = (int *)malloc(cols * sizeof(int));

    result = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
        result[i] = (int *)malloc(cols * sizeof(int));

    printf("첫 번째 행렬의 값을 입력하세요: \n");
    for (int i = 0; i < rows; i++) {
        printf("matrix1[%d] = ", i);
        for (int j = 0; j < cols; j++) {
            scanf("%d", &matrix1[i][j]);
        }
    }
    getchar();

    printf("두 번째 행렬의 값을 입력하세요: \n");
    for (int i = 0; i < rows; i++) {
        printf("matrix2[%d] = ", i);
        for (int j = 0; j < cols; j++) {
            scanf("%d", &matrix2[i][j]);
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }

    printf("두 행렬의 합:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < rows; i++) {
        free(matrix1[i]);
        free(matrix2[i]);
        free(result[i]);
    }
    free(matrix1);
    free(matrix2);
    free(result);
    return 0;
}
