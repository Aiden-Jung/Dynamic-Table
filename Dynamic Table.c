// C libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Struct for a table
struct table_struct{
  int* table;
  int num;
  int size;
};

/* The functions for this algorithm
 * The descriptions for these are below the main function
 */
void table_insert(struct table_struct *T, int x);

void table_delete(struct table_struct *T, int x);

int upper_bound_binary_search(int arr[], int num, int x);

void table_print(struct table_struct *T);


//The main function
int main () {

  /* Declare and initialize a table struct T
   * Since the initial table has no element, its table points a null pointer and its num and size are 0.
   */
  struct table_struct T = {NULL, 0, 0};

  //Since some tests use rand function, srand sets its seed value.
  srand(time(NULL));

  //A variable for random values
  int random;
  
  //Test 1: Insert one integer and delete it.
  printf("Test 1: Insert one integer and delete it.\n");
  printf("Inital ");
  table_print(&T);
  printf("After inserting 1, ");
  table_insert(&T, 1);
  table_print(&T);
   printf("After deleting 1, ");
  table_delete(&T, 1);
  table_print(&T);

  printf("\n");

  //Test 2: Insert several integers.
  printf("Test 2: Insert several integers.\n");
  printf("Inital ");
  table_print(&T);
  
  for(int i=0;i<10;i++){
    random = rand()%11-5;
    printf("After inserting %d, ", random);
    table_insert(&T, random);
    table_print(&T);
  }

  printf("\n");

  //Test 3: Delete an integer not in the table.
  printf("Test 3: Delete an integer not in the table.\n");
  printf("Inital ");
  table_print(&T);

  printf("When trying to delete 100, ", 100);
  table_delete(&T, 100);
  printf("After the try, ");
  table_print(&T);

  printf("\n");

  //Test 4: Delete several integers.
  printf("Test 4: Delete several integers.\n");
  printf("Inital ");
  table_print(&T);

  for(int i=0;i<10;i++){
    random = rand()%T.num;
    printf("After deleting %d, ", T.table[random]);
    table_delete(&T, T.table[random]);
    table_print(&T);
  }

  printf("\n");

  //Test 5: Delete an integer when the table is empty.
  printf("Test 5: Delete an integer when the table is empty.\n");
  printf("Inital ");
  table_print(&T);
    
  printf("When trying to delete 1, ");
  table_delete(&T, 1);
  printf("After the try, ");
  table_print(&T);

  printf("\n");
  
  //End the program
  return 0;
  
}

/* @Name   table_insert
 * @brief  Insert an integer into the table
 * @param  *T, a pointer for the table struct
 *         x, an integer that will be inserted
 * @pre    x
 * @post   The integers in the table are in monotonically nondescending order
 */
void table_insert(struct table_struct *T, int x){

  //If the table size is 0, an array of length 1 is allocated and x is inserted. 
  if(T->size == 0){
    
    T->table = (int (*))malloc(sizeof(int));
    T->size = 1;
    T->table[0]=x;

  }else{

    /* If the number of element in the table is equal to the number of slots in the table,
     * the table is reallocated with twice the size  
     */
    if(T->num == T->size){

      T->table = (int (*))realloc(T->table, 2*T->size*sizeof(int));

      //If realloc fails, the program will terminate.
      if(T->table == NULL){

        printf("Error: realloc failed.\n");
        exit(0);

      }
      
      T->size *= 2;

    }

    //Find the upper bound for x in the table to insert x in the correct index
    int index = upper_bound_binary_search(T->table, T->num, x);

    //Shift all the elements after index in the table to the right by 1
    for(int i=T->num;i>index;i--){

      T->table[i]=T->table[i-1];

    }
    
    //Insert x into the table
    T->table[index]=x;

  }

  //After inserting x, num is incremented.
  T->num++;

}

/* @Name   table_delete
 * @brief  Delete an integer in the table
 * @param  *T, a point for the table struct
 *         x, an integer that will be deleted
 * @post   The integers in the table are in monotonically nondescending order
 */
void table_delete(struct table_struct *T, int x){

  //If the table is empty, an error message will be printed.
  if(T->num == 0){

    printf("Error: The table is empty.\n");

    /* If the table has only one element and it is the same value with x,
     * the table pointer will point a null pointer and its size and num will be set to 0.
     * If the table has only one elemnt but it is not the same value with x,
     * an error message will be printed.
     */
  }else if(T->num == 1){
    
    if(T->table[0] == x){

      free(T->table);
      T->num--;
      T->size=0;
      T->table=NULL;
      
    }else{
      
      printf("Error: The given value is not in the table.\n");
      
    }
    
  }else{

    //Find the upper bound for x to delete x
    int index=upper_bound_binary_search(T->table, T->num, x);

    /* If the returned index is not zero and the element on index-1 is x,
     * it will mean that x is in the table.
     * If not, it will mean that x is not in the table and print an error message.
     */
    if(index != 0 && T->table[index-1] == x){

      //When a load factor drops below 1/4, the table will be reallocated with half the size.
      if(4*(T->num) == T->size){

      T->table = (int (*))realloc(T->table, T->size*sizeof(int)/2);

      //If realloc fails, the program will terminate.
      if(T->table == NULL){

        printf("Error: realloc failed.\n");
        exit(0);

      }
      
      T->size /= 2;

      }

      //Shift all the elements after index in the table to left by 1
      for(int i=index;i<T->num;i++){

        T->table[i-1]=T->table[i];

      }

      //After deleting x, num is decremented.
      T->num--; 

    }else{

      printf("Error: The given value is not in the table.\n");

    }
  }
}

/* @Name   binary_search
 * @brief  Find the upper bound for the given value in the given array
 * @param  arr, an array of integers
 *         num, an integer that indicates the number of elements in arr
 *           x, the given value whose upper bound will be returned
 * @return an index where the first integer that is greater than x appears
 * @post   If x is smaller than all the elements in arr, 0 is returned
 *         If x is greater than all the elements in arr, the index where the last element appears plus one is returned
 */
int upper_bound_binary_search(int arr[], int num, int x){

  //Variables to track the range of arr that will be searched
  int low = 0;
  int high = num;
  int mid;

  //Find the upper bound for x in arr using binary search
  while(low < high){

    mid = (low + high)/2;

    if(arr[mid] <= x){

      low = mid+1;

    }else{

      high = mid;

    }
  }

  return high;

}

/* @Name   table_print
 * @brief  Print all the elements in the table
 * @param  *T, a pointer for the table struct
 */
void table_print(struct table_struct *T){

  printf("Table:");

  for(int i=0;i<T->num;i++){

    printf(" %d", T->table[i]);

  }
  
  printf(" (num: %d, size: %d)", T->num, T->size);  
  printf("\n");

}
