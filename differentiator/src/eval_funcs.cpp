#include "Differentiator.h"

double func_Add(double value_1, double value_2)
{
    return value_1 + value_2;
}

double func_Sub(double value_1, double value_2)
{
    return value_1 - value_2;
}

double func_Mul(double value_1, double value_2)
{
    return value_1 * value_2;
}

double func_Div(double value_1, double value_2)
{
    if(fabs(value_2) <= EPS)
    {
        return NAN;
    }
    return value_1 / value_2;
}

double func_Cos(double value_1, double value_2)
{
    return cos(value_1 * PI / 180.0);
}

double func_Sin(double value_1, double value_2)
{
    return sin(value_1 * PI / 180.0);
}

double func_Tan(double value_1, double value_2)
{
    return tan(value_1 * PI / 180.0);
}

double func_Cot(double value_1, double value_2)
{
    return cos(value_1 * PI / 180.0) / sin(value_1 * PI / 180.0);
}

double func_Asin(double value_1, double value_2)
{
    if(fabs(value_1) - 1 > 0.0)
    {
        return 0;
    }
    return (asin(value_1) * 180.0) / PI;
}

double func_Acos(double value_1, double value_2)
{
    if(fabs(value_1) - 1 > 0.0)
    {
        return 0;
    }
    return (acos(value_1) * 180.0) / PI;
}

double func_Sqrt(double value_1, double value_2)
{
    return sqrt(value_1);
}

double func_Exp(double value_1, double value_2)
{
    return exp(value_1);
}

double func_Log(double value_1, double value_2)
{
    return log(value_1);
}

// double func_Log10(double value_1, double value_2)
// {
//     return log10(value_1);
// }

double func_Pow(double value_1, double value_2)
{
    return pow(value_1, value_2);
}