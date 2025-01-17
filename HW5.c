

#include <stdio.h>
#include <stdlib.h>



int* distanceVectore(int table_size, int *access_vector){
//  int itrNum = sizeof(access_vector) / sizeof(access_vector[0]);
  int itrNum = 24;
  int *distances = (int *)malloc(itrNum * sizeof(int));
  int *myTable = (int *)malloc(sizeof(int));
  int *myTempTable;

  int dis=-1; // INF

  int table_size_local=0;

  for(int i = 0; i < itrNum; i++){
    printf("\nTable size local: %d\n",table_size_local);
    dis = -1; //INF
    for(int j = 0; j < table_size_local; j++){
       if(myTable[j] == access_vector[i]){
          dis=j+1; /// Starts from 0 and not from 1
          printf("element foudnt!! dis= %d\n",dis);
          break;
          }
       }
    distances[i] = dis;
    printf("Curr num: %d\n",access_vector[i]);
    printf("Elements: \n");
    for(int j = 0; j < table_size_local; j++){
        printf("%d, ",myTable[j]);
    }
    printf("dis = %d\n",dis);


    int indxOfMyTable=0;

    // refresh Table DATA
    if(dis == -1){       // add new val to table
        table_size_local = table_size_local + 1;
        myTempTable = malloc(table_size_local * sizeof(int));
        myTempTable[0] = access_vector[i]; // add to head the new val
        for(int j = 1; j < table_size_local; j++){ // add the rest of val
            if(myTable[indxOfMyTable] != access_vector[i]){
                // do not add (again)the exicting val
                myTempTable[j] = myTable[indxOfMyTable];
            }
            else{
              j--;
              }
            indxOfMyTable++;
        }
    }
    else{         // val is already in table
          myTempTable = malloc(table_size_local * sizeof(int));
          myTempTable[0] = access_vector[i];
          for(int j = 1; j < table_size_local; j++){ // add the rest of val
            if(myTable[indxOfMyTable] != access_vector[i]){
              // do not add (again)the exicting val
              myTempTable[j] = myTable[indxOfMyTable];
              }
              else{
                  j--;
              }
          indxOfMyTable++;
          }
    }
    free(myTable);
    myTable = myTempTable;
  }
  free(myTable);
  return distances;
}


int main() {
//    int access_vector[] = {1, 2, 3, 1, 2, 4, 1, 5};
    printf("Hello, access like in Harzaha 11 Memory2:\n");
    int access_vector[] = {0,2,1,3,5,4,6,3,7,4,7,3,3,5,5,3,1,1,1,7,1,3,4,1};
    int table_size = 3;
    int num_elements = sizeof(access_vector) / sizeof(access_vector[0]);

    int *distances = distanceVectore(table_size, access_vector);

    printf("Access:\n");
    for (int i = 0; i < num_elements; i++) {
        printf("%d ", access_vector[i]);
    }
    printf("\n");
    printf("Distances:\n");
    for (int i = 0; i < num_elements; i++) {
        printf("%d ", distances[i]);
    }
    printf("\n");

    free(distances);

    return 0;
}