
#include "Mandel.h"

int main()
{

    __m128 f1 = {1., 2., 3., 4.};
    f1 = _mm_add_ps (f1, f1);

    // mandelDraw ();

    return 0;
}