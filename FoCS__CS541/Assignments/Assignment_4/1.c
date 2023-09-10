/******************************************************************************
* Executive M.Tech (AI & DSE)
* Subject: CS559 - Computer Systems Lab
* Name: Manjit Singh Duhan
* Addmission No: IITP001316
* email id: duhan.manjit@gmail.com
* Assignement 4
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

/*This structure will hold the respective values.*/
struct bcd_sub{
    int minuend;
    int subtrahend;
    int difference;
    int end_of_carry;
};

static void print_bcd_in_binary(int bcd_value);
static int dec2bcd(int value);
static int bcd2dec(int bcd);
static int do_binary_addition(char a, char b);
static int get_bcd_sub(int a, int b, int *carry_info, int *eoc);
static int get_user_input(int *val1, int* val2);
static int get_1_s_compliment(int value);
static int get_bcd_subtraction(int minuend, int subtrahend, int* carry);

int main()
{
    int count;
    struct bcd_sub *bcd = NULL;

    printf("\nNumber of test cases: ");
    scanf("%d", &count);
    if(count <= 0) {
       printf("ERROR: Number of test case can not be 0 or negative. Exitting...");
       return -1; 
    }

    bcd = (struct bcd_sub *)(calloc(count, sizeof(struct bcd_sub)));
    if(bcd == NULL) {
        printf("ERROR: Not able to allocate memory. Exitting...");
        return -1;
    }
    
    for(int i = 0; i < count; i++) {
        int value1 = 0;
        int value2 = 0;
        
        if (get_user_input(&value1, &value2) != 0) {
            return -1;
        }

        bcd[i].minuend = value1;
        value1 = dec2bcd(value1);
        
        bcd[i].subtrahend = value2;
        value2 = dec2bcd(value2);

        bcd[i].difference = get_bcd_subtraction(value1, value2, &bcd[i].end_of_carry);
    }
    
    for(int i = 0; i < count; i++) {
        int value1 = 0;
        int value2 = 0;

        value1 = dec2bcd(bcd[i].minuend); 
        printf("\n %d (", bcd[i].minuend);
        print_bcd_in_binary(value1);
        printf(")");

        printf(" - ");

        printf("%d (", bcd[i].subtrahend);
        value2 = dec2bcd(bcd[i].subtrahend);
        value2 = get_1_s_compliment(value2);
        print_bcd_in_binary(value2);
        printf(")");

        char sign = bcd[i].end_of_carry ? ' ': '-';
        printf(" = %c%d (%c",  sign, bcd2dec(bcd[i].difference), sign); //Just for printing purpose doing bcd2dec comversion
        print_bcd_in_binary(bcd[i].difference);
        printf(")");
    }

    return 0;
}

/******************************************************************************
* \brief  print the BCD value in the binary string format
*
* \param[in] bcd_value: the value in BCD format
*
* \return : None
* *****************************************************************************/
static void print_bcd_in_binary(int bcd_value)
{
    int reverse = 0;
    int digits = bcd_value ? 0: 4;

    /*For rase of printing, first reverse the digits i.e 123 will become 321 */
    while(bcd_value) {
        reverse = reverse << 4;
        reverse |= (bcd_value&0xF);
        
        bcd_value = bcd_value >> 4;
        
        digits += 4;
    }

    while(1) {
        int temp = reverse&0xF; //Take the LSB side nibble
        
        for(int i = 3; i >=0; i--) {
            printf("%d", (temp & (1 << i))? 1 : 0);
        }
        
        reverse = reverse >> 4;;
        digits-= 4;

        if(digits == 0) {
            break;
        }
        

        printf(" "); //print a blank space after every 4 binary values
    }
}

/******************************************************************************
* \brief  convert the decimal format value in the BCD format
*
* \param[in] value: decimal format value
*
* \return : BCD format
******************************************************************************/
static int dec2bcd(int value)
{
    int retVal = 0;
    int i = 0;
    while(value) {
        int temp = (value%10);
        retVal = retVal | (temp << 4*i);
        value /=10;
        i++;
    }
    
    return retVal;
}

/******************************************************************************
* \brief  convert the BCD format value in the decimal format
*
* \param[in] bcd: BCD format value
*
* \return : decimal format value
******************************************************************************/
static int bcd2dec(int bcd)
{
    int reverse_decimal_val = 0;
    int decimal_val = 0;

    while(bcd) {
        reverse_decimal_val = (reverse_decimal_val*10) + (bcd&0xF);
        bcd = bcd >> 4;
    }
    
    while(reverse_decimal_val) {
        decimal_val = decimal_val*10 + reverse_decimal_val%10;
        reverse_decimal_val /= 10;
    }
    
    return decimal_val;
}

/******************************************************************************
* \brief  Do the sum operation on the binary values
*
* \param[in] a: first BCD format nibble
* \param[in] b: second BCD format nibble
*
* \return : sum of a and b
******************************************************************************/
static int do_binary_addition(char a, char b)
{
    char carry = 0;
    
    while(b) {
       //shift it left and thake the carry
        carry = (a & b) << 1;

        //do the addition operation
        a ^= b;
        b = carry;
    }
    
    return a;
}

/******************************************************************************
* \brief  Do the sum operation on the BCD values
*
* \param[in] a: first BCD format value
* \param[in] b: second BCD format vale
*
* \return : sum of a and b
******************************************************************************/
static int get_bcd_sub(int a, int b, int *carry_info, int *eoc)
{
    int sum = 0;
    int carry = 0;
    int i = 0;

    while ( a || b) {
        
        //To handle the case where both a & b are not of same size
        if(a && !b) b = 0xF;
        if(!a && b) a = 0xF;
        
        int temp = do_binary_addition(a&0xF, b&0xF);
        
        /*If we have carry from last operation then add it*/
        temp += carry;
        if(temp &0x10) {
            /*If there result is > 9 then add 6 to it*/
            // temp = do_binary_addition(temp&0xF, 6);
            temp &= 0xF;
            carry = 1;
        }
        else {
            carry = 0;
        }

        sum = sum | (temp << (4*i));
        *carry_info = *carry_info | (carry << (4*i));

        i++;
        a = a >> 4;
        b = b >> 4;
    }

    *eoc = carry;
    
    return sum;
}

/******************************************************************************
* \brief  Take the input from user
*
* \param[out] first number
* \param[out] second number
*
* \return : 0 if input by user is valid otherwise -1
******************************************************************************/
static int get_user_input(int *val1, int* val2)
{
    int temp = 0;
    printf("\nFirst Number : ");
    scanf("%d", &temp);

    if(temp < 0) {
        printf("Wrong input entered by user. Exitting...");
        return -1;
    }

    *val1 = temp;

    printf("Second Number: ");
    scanf("%d", &temp);

    if(temp < 0) {
        printf("Wrong input entered by user. Exitting...");
        return -1;
    }

    *val2 = temp;
    
    return 0;
}

/******************************************************************************
* \brief  calculate & return the 1's complement of the given number
*
* \param[in] value: the value for which 1's compliment is required
*
* \return : 1's complement of the given value
******************************************************************************/
static int get_1_s_compliment(int value)
{
    int retVal = 0;
    int i = 0;

    while((value != 0) || (i % 4 != 0)) 
    {
        retVal |= ((value & 1) ? 0 : 1) << i ;
        
        value = value>>1;
        i++;
    }

    return retVal;
}

/******************************************************************************
* \brief  Do the subtraction operation on the BCD values
*
* \param[in] minuend: first BCD format value i.e. minuend
* \param[in] subtrahend: second BCD format vale i.e. subtrahend
* \param[out] end_of_carry : this variable will return the EOC information to inform
*              the caller that the result is in negative or positive.
*              EOC: 0 means the result is negative, any other value  means result = positive
*
* \return : subtraction of minuend and subtrahend i.e. minuend- subtrahend
******************************************************************************/
static int get_bcd_subtraction(int minuend, int subtrahend, int *end_of_carry)
{
    int adder1 = 0;
    int result = 0;
    int i = 0;

    int temp_carry = 0;

    int a = minuend;
    int b = get_1_s_compliment(subtrahend);

    adder1 = get_bcd_sub(a, b, &temp_carry, end_of_carry);
    
    //If there is EOC then add 1 to LSB nibble
    adder1 += *end_of_carry ? 1 : 0;

    //If the EOC is 0 then we need to take the 1s compliment otherwise no need
    a = *end_of_carry ? adder1 : get_1_s_compliment(adder1);

    while (a) {
        // if Carry =1 & EOC = 0 then add 0xA to that nibble
        // if Carry =1 & EOC = 1 then add 0x0 to that nibble i.e. nothing needs to be done
        // if Carry =0 & EOC = 0 then add 0x0 to that nibble i.e nothing needs to be done
        // if Carry =0 & EOC = 1 then add 0xA to that nibble
        // This requirement matches the XOR truth table so using it
        if( (temp_carry & 1) ^ (*end_of_carry & 1)) {
            //According to rule of EOC, do addion of 0xA
            b = do_binary_addition(a&0xF, 0xA);
            b = b&0xF;
        } else {
            b = a&0xF;
        }

        result = result | (b << (4 * i));

        i++;
        a = a >> 4;
        temp_carry = temp_carry >> 4;
    }

    return result;
}

