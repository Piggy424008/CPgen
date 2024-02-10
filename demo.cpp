#include "generator.h"

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);
    Geometry<double> geo; geo.make_raw_convex_shell(100000);
}