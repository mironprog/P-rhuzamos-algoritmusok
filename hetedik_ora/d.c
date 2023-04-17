#include <stdio.h>

//int isIdentity(int matrix[10][20], int n, int m);
void displayMatrix(int matrix[10][20]);


int main(void)
{
    
    int i, j, m, n;
    int matrix[10][20];

    printf("Enter the number of rows:\n");
    scanf("%d", &m);
    printf("Enter the number of columns:\n");
    scanf("%d", &n);

    for(i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            printf("Enter data in [%d][%d]: ", i,j);
            scanf("%d", &matrix[i][j]);
        }
        
    }

    

    

    //printf("%d", isIdentity(matrix[10][20], n, m));
    
    
    return 0;
}

void displayMatrix(int matrix[10][20])
{
     for(i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            printf("%d\t", matrix[i][j]);
        }
        
        printf("\n");
    }

}


/*
int isIdentity(int matrix[10][20], int n, int m)
{
    int flag;
    int i, j;

    for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			if (i == j && matrix[i][j] != 1)
			{
				flag = 1;
				break;
			}
			else if (i != j && matrix[i][j] != 0)
			{
				flag = 0;
				break;
			}
		}
	}


    return flag; 
}
*/