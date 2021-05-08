
#include "jit.hpp"

int main() /* R11 !!!*/
{

    JITCompiler compiler;

    compiler.translate ("jit-in", "jit-out");

    return 0;
}