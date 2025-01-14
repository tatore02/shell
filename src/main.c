#include <stdlib.h>
#include "shell.h"

int main() {
  
    // Load config files, if any.

    lsh_loop();

    // Perform any shutdown/cleanup.

    return EXIT_SUCCESS;
}
