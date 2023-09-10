/******************************************************************************
* Executive M.Tech (AI & DSE)
* Subject: CS559 - Computer Systems Lab
* Name: Manjit Singh Duhan
* Addmission No: IITP001316
* email id: duhan.manjit@gmail.com
*
* *******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>


void print_array(const int *arr, int array_size)
{
    if(arr == NULL) {
        printf("[%s:%d] Invalid arg received", __func__, __LINE__);
        return;
    }

    printf("[");
    for(int index = 0; index < array_size; index++) {
        printf("%d", arr[index]);
		if(index < array_size-1) {
			printf(", ");
		}
    }
    printf("]\n");
}

/******************************************************************************
* \brief  Validate the user input. ONly valid input are >= 0 & < INT_MAX
* 
* \param[out] input: the input entered by user
* 
* \return : true if input entered by user is valid otherwise false
* *******************************************************************************/
bool validate_user_input(const int input){

    if(input >= 0 && input < INT_MAX) {
        return true;
    }

    printf("Invalid user Input. It should be > 0 & < %d\n", INT_MAX);
    return false;
}

/******************************************************************************
* \brief  get the size of array from user & Validate it. It should be > 0 &
*           < INT_MAX because we are taking the size as int.
* 
* \param[in] *array_size: Address on which the size entered by user will be written
* \param[out] array_size: size of array entered by user
* 
* \return : true if size is valid otherwise false
* *******************************************************************************/
bool get_user_input_array_size(int *array_size)
{
    if(array_size == NULL){
        printf("[%s:%d] Invalid arg received", __func__, __LINE__);
        return false;
    }

    printf("Please enter the size of array: ");
    scanf("%d", array_size);

    //validate the size of input array entered by user
    if( (false == validate_user_input(*array_size)) || (0 == *array_size)) {
        printf("An array can not be of size: %d\n", *array_size);
        return false;
    }

    return true;
}

/******************************************************************************
* \brief  get the elements of array from user & Validate then. Each element should 
*           be > 0 & < INT_MAX. Anything, except int will be discared and will
*           lead to exit of program.
* 
* \param[in] *arr: Address to the memory on which the elements entered by user will be written
* \param[out] array_size: size of array entered by user
* 
* \return : true if elements entered are user are valid otherwise false
*******************************************************************************/
bool get_user_input_array_values(int *arr, int array_size)
{
    if(arr == NULL){
        printf("[%s:%d] Invalid arg received", __func__, __LINE__);
        return false;
    }

    for(int index = 0; index < array_size; index++) {

        // Get the values of each element in a temp variable to avoid entering invalid entery in array
        int tempVal = 0;
        printf("array[%d]=", index);
        scanf("%d", &tempVal);

        //validate the input given by user for arr[index]
        if(false == validate_user_input(tempVal)) {
            printf("Invalid user input give for arr[%d].", index);
            return false;
        }

        arr[index] = tempVal;
    }

    return true;
}

/******************************************************************************
* \brief  sort the array using selection sort
* 
* \param[in] *arr: Address to the array
* \param[out] array_size: size of array entered by user
*******************************************************************************/
void sort_array(int *arr, int array_size)
{
    if(arr == NULL){
        printf("[%s:%d] Invalid arg received", __func__, __LINE__);
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < array_size - 1; i++) {

        int minVal = arr[i];
        int minValIndex = i;

        for(int j = i+1; j < array_size; j++) {
            // check if arr[j] is less than minVal. If yes then update minVal
            if(arr[j] < minVal) {
                minValIndex = j;
                minVal = arr[j];
            }
        }

        // swap the arr[i] with the minimum value found
        if(minVal != arr[i]) {
            minVal = arr[i];
            arr[i] = arr[minValIndex];
            arr[minValIndex] = minVal;        
        }
    }

    printf("Sorted Array is : ");
    print_array(arr, array_size);
}

/******************************************************************************
* \brief  Get the largest difference between 2 adjesent indexes of an array
* 
* \param[in] *arr: Address to the array
* \param[in] array_size: size of array entered by user
* 
* \return : return the largest difference between 2 adjesent indexes of an array
*******************************************************************************/
int get_largest_difference_in_array(const int *arr, int array_size)
{
    if(arr == NULL) {
        printf("[%s:%d] Invalid arg received", __func__, __LINE__);
        exit(EXIT_FAILURE);
    }

    // Return 0 if the array size is < 2
    if(array_size < 2){
        return 0;
    }

    int maxDifference = 0;

    for(int i = 0; i < array_size - 1; i++) {
        if(arr[i+1] - arr[i] > maxDifference) {
            maxDifference = arr[i+1] - arr[i];
        }
    }

    return maxDifference;
}


int main()
{
    int *arr = NULL;
    int array_size = 0;
    int largest_difference = 0;

    // Get the size of array from the user.
    if(false == get_user_input_array_size(&array_size)) {
        printf("User input is not in proper format. Please try again.\n");
        return EXIT_FAILURE;
    }

    // Assign the memory of the size entered by user.
    arr = (int *)(calloc(array_size, sizeof(int)));
    if(arr == NULL) {
        printf("Unable to allocate memory to array. Exitting...");
        exit(EXIT_FAILURE);
    }

    // Get the elements of array from user.
    if(false == get_user_input_array_values(arr, array_size)) {
        printf("User inputData is not in proper format. Please try again.\n");

        free(arr);
        return -1;
    }

    printf("\nEntered array [A] = ");
    print_array(arr, array_size);

    sort_array(arr, array_size);

    largest_difference = get_largest_difference_in_array(arr, array_size);
    printf("Largest Difference between two subsequent entries is: %d\n", largest_difference);

    free(arr);
    return 0;
}