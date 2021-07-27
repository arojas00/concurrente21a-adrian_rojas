#define _POSIX_C_SOURCE 199309L
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>

char **create_matrix(int height, int width);
void fillMatrix(int rows, int cols, char **matrix);
void printMatrix(int rows, int cols, char **matrix);
void copyMatrix(int rows, int cols, char **matrix, char **newMatrix);
void changeForest(int rows, int cols, char **matrix, char **newMatrix);
bool checkInundation(int row, int col, char **matrix, int rows, int cols);
bool checkOvercrowding(int row, int col, char **matrix, int rows, int cols);
bool checkDrought(int row, int col, char **matrix, int rows, int cols);
bool checkReforestation(int row, int col, char **matrix, int rows, int cols);
bool checkCell(int row, int col, int rows, int cols);

int main()
{
  int rows, cols, days, num = 0;
  scanf("%i %i\n", &rows, &cols);
  days = -20;
  char **forest = create_matrix(rows, cols);
  fillMatrix(rows, cols, forest);
  char **newForest = create_matrix(rows, cols);
  copyMatrix(rows, cols, forest, newForest);
  
  if (0 < days)
  {
    for (int i = 0; i <= days; i++)
    {
      printf("%i:\n", i);
      // printMatrix(rows, cols, forest);
      changeForest(rows, cols, forest, newForest);
      //copyMatrix(rows, cols, newForest, forest);
      printf("\n");
    }
  }
  else
  {
    num = 0;
    printf("%i:\n", num);
    // printMatrix(rows, cols, forest);
    for (int i = 0; i > days; i--)
    {
      struct timespec start_time;
      clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &start_time);
      changeForest(rows, cols, forest, newForest);
      //copyMatrix(rows, cols, newForest, forest);
      num++;
      struct timespec finish_time;
      clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &finish_time);
      double elapsed = (finish_time.tv_sec - start_time.tv_sec) +
        (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
      printf("execution time: %.9lfs\n", elapsed); //for time measuring
    }
    printf("\n%i:\n", num);
    // printMatrix(rows, cols, forest);
  }
  
  return 0;
}
char **create_matrix(int rows, int cols)
{
  char **matrix = (char **)calloc(rows, sizeof(char *));
  for (int row = 0; row < rows; ++row)
    matrix[row] = (char *)calloc(cols, sizeof(char));
  return matrix;
}
void fillMatrix(int rows, int cols, char **matrix)
{
  char cell;
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      scanf("%c", &cell);
      matrix[i][j] = cell;
    }
    scanf("\n");
  }
}
void copyMatrix(int rows, int cols, char **matrix, char **newMatrix)
{
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      newMatrix[i][j] = matrix[i][j];
    }
  }
}
void printMatrix(int rows, int cols, char **matrix)
{
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      printf("%c", matrix[i][j]);
    }
    printf("\n");
  }
}
void changeForest(int rows, int cols, char **matrix, char **newMatrix)
{
  
  bool checked;
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      checked = checkCell(i,j,rows,cols);
    //   if (matrix[i][j] == 'a')
    //   {
    //     if (checkInundation(i, j, matrix, rows, cols) == true)
    //     {
    //       newMatrix[i][j] = 'l';
    //     }
    //     else
    //     {
    //       if (checkOvercrowding(i, j, matrix, rows, cols) == true)
    //       {
    //         newMatrix[i][j] = '-';
    //       }
    //     }
    //   }
    //   else
    //   {
    //     if (matrix[i][j] == 'l')
    //     {
    //       if (checkDrought(i, j, matrix, rows, cols) == true)
    //       {
    //         newMatrix[i][j] = '-';
    //       }
    //     }
    //     else
    //     {
    //       if (matrix[i][j] == '-')
    //       {
    //         if (checkReforestation(i, j, matrix, rows, cols) == true)
    //         {
    //           newMatrix[i][j] = 'a';
    //         }
    //       }
    //     }
    //   }
    }
  }
  
}
bool checkInundation(int row, int col, char **matrix, int rows, int cols)
{
  int lakes = 0;
  for (int i = row - 1; i <= row + 1; i++)
  {
    for (int j = col - 1; j <= col + 1; j++)
    {
      if (checkCell(i, j, rows, cols) == true)
      {
        if (matrix[i][j] == 'l')
          lakes++;
      }
    }
  }
  if (lakes >= 4)
    return true;
  else
    return false;
}
bool checkOvercrowding(int row, int col, char **matrix, int rows, int cols)
{
  int trees = 0;
  for (int i = row - 1; i <= row + 1; i++)
  {
    for (int j = col - 1; j <= col + 1; j++)
    {
      if (checkCell(i, j, rows, cols) == true)
      {
        if (matrix[i][j] == 'a')
          trees++;
      }
    }
  }
  if (trees - 1 > 4)
    return true;
  else
    return false;
}
bool checkDrought(int row, int col, char **matrix, int rows, int cols)
{
  int lakes = 0;
  for (int i = row - 1; i <= row + 1; i++)
  {
    for (int j = col - 1; j <= col + 1; j++)
    {
      if (checkCell(i, j, rows, cols) == true)
      {
        if (matrix[i][j] == 'l')
          lakes++;
      }
    }
  }
  if (lakes <= 3)
  {
    return true;
  }
  else
    return false;
}
bool checkReforestation(int row, int col, char **matrix, int rows, int cols)
{
  int trees = 0;
  for (int i = row - 1; i <= row + 1; i++)
  {
    for (int j = col - 1; j <= col + 1; j++)
    {
      if (checkCell(i, j, rows, cols) == true)
      {
        if (matrix[i][j] == 'a')
          trees++;
      }
    }
  }
  if (trees >= 3)
    return true;
  else
    return false;
}
bool checkCell(int row, int col, int rows, int cols)
{
  if ((unsigned int)row < (unsigned int)rows &&
   (unsigned int)col < (unsigned int)cols)
    return true;
  else
    return false;
}