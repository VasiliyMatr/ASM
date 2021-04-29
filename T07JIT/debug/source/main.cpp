
#include "jit.hpp"

int main()
{

    JITCompiler compiler;

    compiler.translate ("jit-in", "jit-out");

    return 0;
}