//Name: Vince Vennedick Banks
//ID: 1001702150
/* Coding Assignment 4 */
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>
#include <time.h>


void swap(int *SwapA, int *SwapB);
int partition(int A[], int low, int high);

typedef struct node
{
	int number;
	struct node *next_ptr;
}
NODE;

typedef struct list
{
    NODE *head;
    int count;
    int sum;
}
LIST;

void AddNodeToLL(int Number, LIST *LinkedList)
{
    NODE *newNode = (NODE *)malloc(sizeof(NODE));
    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    newNode->number = Number;
    newNode->next_ptr = NULL;

    if (LinkedList->head == NULL)
    {
        LinkedList->head = newNode;
    }
    else
    {
        NODE *current = LinkedList->head;
        while (current->next_ptr != NULL)
        {
            current = current->next_ptr;
        }

        current->next_ptr = newNode;
    }

    LinkedList->count++;
    LinkedList->sum += Number;
}



void QuickSort(int A[], int low, int high)
{
    if (low < high)
    {
        int ndx = partition(A, low, high);
        QuickSort(A, low, ndx - 1);
        QuickSort(A, ndx + 1, high);
    }
}

int partition (int A[], int low, int high)
{
    int i, j = 0;

    #if QSM
        int middle = (high+low)/2;
        swap(&A[middle], &A[high]);
    #elif QSRND
        int random = (rand() % (high-low+1)) + low;
        swap(&A[random], &A[high]);
    #endif

    int pivot = A[high];
    i = (low - 1);
    
    for (j = low; j < high; j++)
    {
        if (A[j] < pivot)
        {
            i++;
            swap(&A[i], &A[j]);
        }
    }
    swap(&A[i + 1], &A[high]);
    return (i + 1);
}

void swap(int *SwapA, int *SwapB)
{
    int temp = *SwapA;
    *SwapA = *SwapB;
    *SwapB = temp;
}


void PrintArray(int ArrayToPrint[], int SizeAP) 
{
    for (int i = 0; i < SizeAP; i++) 
    {
        printf("%d\n", ArrayToPrint[i]);
        
    }
    printf("\n\n");
}

void ReadFileIntoLL(int argc, char* argv[], LIST* LLH, int** array, int* line_count) 
{
    if (argc < 2) 
    {
        printf("File must be provided on the command line...exiting\n");
        exit(EXIT_FAILURE);
    }

    char* filename = argv[1];
    FILE* file = fopen(filename, "r");
    if (file == NULL) 
    {
        printf("File must be provided on command line...exiting\n");
        exit(EXIT_FAILURE);
    }

    char buffer[100];
    *line_count = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL) 
    {
        (*line_count)++;
    }
    
    *array = (int*)malloc(*line_count * sizeof(int));
    if (*array == NULL) 
    {
        printf("Memory allocation failed...exiting\n");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_SET);

    int i = 0;
    
    while (fgets(buffer, sizeof(buffer), file) != NULL && i < *line_count) 
    {
        buffer[strcspn(buffer, "\n")] = '\0';
        (*array)[i] = atoi(buffer);
        i++;
    }

    fclose(file);
}



void FreeLL(LIST *LLH) 
{ 
    if (LLH->head == NULL)
    {
        printf("Linked list is empty.\n");
        return;
    }

    NODE *current = LLH->head;
    int count = 0;
    int sum = 0;
   
    while (current != NULL)
    {
        NODE *temp = current;
        sum += current->number;
    
        #ifdef PRINT
            printf("\nFreeing %p %d %p\n", temp, temp->number, temp->next_ptr);
        #endif

        current = current->next_ptr;
        free(temp);
        count++;
    }

    LLH->head = NULL; 

    printf("\n%d nodes were deleted with a total sum of %d\n", count, sum);
}


int main(int argc, char *argv[])
{
    LIST LLH = {0};
    int* array = NULL;  
    int line_count = 0; 
    clock_t quickSortStart, quickSortEnd, total_tics = 0;
    
    int num_runs = 10; 

    if (argc >= 3) 
    {
        num_runs = atoi(argv[2]); 
    } 
    else 
    {
        printf("Number of runs not specified on command line...defaulting to 10\n");
    }


    for (int i = 0; i < num_runs; i++)
    {

        ReadFileIntoLL(argc, argv, &LLH, &array, &line_count);
        #ifdef PRINTARRAY
            PrintArray(array, line_count);  
        #endif

        quickSortStart = clock();
        QuickSort(array, 0, line_count-1);
        quickSortEnd = clock();

        printf("Run %d complete : %ld tics\n",i+1,quickSortEnd-quickSortStart);
        #ifdef PRINTARRAY
            PrintArray(array, line_count);
        #endif

        total_tics += (quickSortEnd-quickSortStart);
    
    }
    
    printf("The average run time for %d runs is %ld\n\n", num_runs, total_tics/num_runs);

    printf("\nProcessed %d records\n", line_count);

    free(array); 


    return 0; 
}   