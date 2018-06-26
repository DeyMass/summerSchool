#include <math.h>

char description[30] = "Find logarithm";
char funcName[10] = "op_log";

float op_log(float base, float val)
{
    return (float)((float)log(base) / (float)log(val));
}
