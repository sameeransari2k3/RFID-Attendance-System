#include"header.h"

// Function: ATOF
// Purpose : Custom implementation to convert a string representing a floating-point number 
//           (e.g., "12.34" or "-56.78") into a float value without using the built-in atof() function.

float ATOF(char *str)
{
    int i, index, res = 0, flag = 0;
    float dec = 0;

    if (str[0] == '-')  // Handle negative numbers
    {
        for (i = 1; str[i]; i++)
        {
            if (str[i] == '.')
            {
                index = i;
                flag = 1;
            }
        }

        if (flag) // Decimal point exists
        {
            for (i = 1; i < index; i++)
            {
                res = (res * 10) + (str[i] - '0');
            }
            for (i = strlen(str) - 1; i > index; i--)
            {
                dec = (dec / 10) + (str[i] - '0');
            }
            return -(res + dec / 10);
        }
        else  // No decimal point
        {
            for (i = 1; str[i]; i++)
            {
                res = (res * 10) + (str[i] - '0');
            }
            return -res;
        }
    }
    else // Positive number
    {
        for (i = 0; str[i]; i++)
        {
            if (str[i] == '.')
            {
                index = i;
                flag = 1;
            }
        }

        if (flag) // Decimal point exists
        {
            for (i = 0; i < index; i++)
            {
                res = (res * 10) + (str[i] - '0');
            }
            for (i = strlen(str) - 1; i > index; i--)
            {
                dec = (dec / 10) + (str[i] - '0');
            }
            return res + dec / 10;
        }
        else // No decimal point
        {
            for (i = 0; str[i]; i++)
            {
                res = (res * 10) + (str[i] - '0');
            }
            return res;
        }
    }
}

