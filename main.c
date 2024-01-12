#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
} Node;
