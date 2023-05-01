#include "Differentiator.h"

double func_Add(double value_1, double value_2) // ok
{
    return value_1 + value_2;
}

double func_Sub(double value_1, double value_2) // ok
{
    return value_1 - value_2;
}

double func_Mul(double value_1, double value_2) // ok
{
    return value_1 * value_2;
}

double func_Div(double value_1, double value_2) // ok
{
    if(fabs(value_2) <= EPS)
    {
        return NAN;
    }

    return value_1 / value_2;
}

double func_Cos(double value_1, double value_2) // ok
{
    return cos(value_1 * PI / 180.0);
}

double func_Sin(double value_1, double value_2) // ok
{
    return sin(value_1 * PI / 180.0);
}

double func_Tan(double value_1, double value_2) // ok
{
    double cos_val = func_Cos(value_1);
    if(check_is_positive(cos_val) == IS_ZERO)
    {
        ERROR_MESSAGE(stderr, ERR_INVALID_ARGUMENT)
        return NAN;
    }

    return sin(value_1 * PI / 180.0) / cos_val;
}

double func_Cot(double value_1, double value_2) // ok
{
    double sin_val = func_Sin(value_1);
    if(check_is_positive(sin_val) == IS_ZERO)
    {
        ERROR_MESSAGE(stderr, ERR_INVALID_ARGUMENT)
        return NAN;
    }

    return cos(value_1 * PI / 180.0) / sin_val;
}

double func_Asin(double value_1, double value_2) // ok
{
    if(fabs(value_1) - 1 > EPS)
    {
        ERROR_MESSAGE(stderr, ERR_INVALID_ARGUMENT)
        return NAN;
    }

    return (asin(value_1) * 180.0) / PI;
}

double func_Acos(double value_1, double value_2) // ok
{
    if(fabs(value_1) - 1 > EPS)
    {
        ERROR_MESSAGE(stderr, ERR_INVALID_ARGUMENT)
        return NAN;
    }

    return (acos(value_1) * 180.0) / PI;
}

double func_Sqrt(double value_1, double value_2) // ok
{
    if(check_is_positive(value_1) == IS_POSITIVE)
    {
        return sqrt(value_1);
    }
    else if(check_is_positive(value_1) == IS_ZERO)
    {
        return 0;
    }
    
    ERROR_MESSAGE(stderr, ERR_INVALID_ARGUMENT)
    return NAN;
}

double func_Exp(double value_1, double value_2) // ok
{
    return exp(value_1);
}

double func_Log(double value_1, double value_2) // ok
{
    if(check_is_positive(value_1) == IS_POSITIVE)
    {
        return log(value_1);
    }

    ERROR_MESSAGE(stderr, ERR_INVALID_ARGUMENT)
    return NAN;
}

double func_Pow(double value_1, double value_2) // ok
{
    return pow(value_1, value_2);
}

int check_is_positive(double value) // ok
{
    if((fabs(value - fabs(value)) < EPS) && (fabs(value) > EPS))
    {
        return IS_POSITIVE;
    }
    else if(fabs(value) <= EPS)
    {
        return IS_ZERO;
    }
    
    return IS_NEGATIVE;
}
