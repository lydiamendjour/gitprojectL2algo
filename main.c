#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
} Node;
Node* head = NULL;
Node* deletedNodes = NULL;

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

void insertNode(int data) {
    Node* newNode = createNode(data);

    if (head == NULL) {
        head = newNode;
    } else {
        Node* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
        newNode->prev = current;
    }
}
void deleteNodeByValue(int value) {
    Node* current = head;

    while (current != NULL) {
        if (current->data == value) {
            if (current->prev != NULL) {
                current->prev->next = current->next;
            } else {
                head = current->next;
            }

            if (current->next != NULL) {
                current->next->prev = current->prev;
            }

            // Add the deleted node to the temporary list
            current->next = deletedNodes;
            deletedNodes = current;

            return;
        }

        current = current->next;
    }

    printf("Node with value %d not found in the list.\n", value);
}
void searchNode(int value) {
    Node* current = head;

    while (current != NULL) {
        if (current->data == value) {
            printf("Node with value %d found in the list.\n", value);
            return;
        }

        current = current->next;
    }

    printf("Node with value %d not found in the list.\n", value);
}
void emptyList() {
    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }
}

void DrawArrow(int startX, int startY, int endX, int endY, int size, int thickness, Color color) {
    DrawLine(startX, startY, endX, endY, color);

    float angle = atan2f(endY - startY, endX - startX);
    float arrow1X = endX - size * cos(angle - PI / 6);
    float arrow1Y = endY - size * sin(angle - PI / 6);
    float arrow2X = endX - size * cos(angle + PI / 6);
    float arrow2Y = endY - size * sin(angle + PI / 6);

    DrawLine(endX, endY, arrow1X, arrow1Y, color);
    DrawLine(endX, endY, arrow2X, arrow2Y, color);
}

void DrawDoubleArrow(int startX1, int startY1, int endX1, int endY1,
                      int startX2, int startY2, int endX2, int endY2,
                      int size, int thickness, Color color) {
    // Draw the Arrow
    DrawArrow(startX1, startY1, endX1, endY1, size, thickness, color);}

void drawList(int valueToSearch) {
    Node* current = head;
    int yPos = GetScreenHeight() / 2;
    int xOffset = GetScreenWidth() / 2;

    while (current != NULL) {
        const char* dataStr = TextFormat("%d", current->data);

        int numDigits = snprintf(NULL, 0, "%d", current->data);
        int radius = 20 + 5 * (numDigits - 1);

        Color circleColor = (valueToSearch != 0 && current->data == valueToSearch) ? RED : GRAY;

        DrawCircle(xOffset, yPos, radius, circleColor);
        DrawText(dataStr, xOffset - radius + 5, yPos - 10, 20, BLACK);

        if (current->next != NULL) {
            int nextXOffset = xOffset + 3 * radius + 40;
            int nextYPos = yPos;

            DrawDoubleArrow(xOffset + radius, yPos, nextXOffset - radius, nextYPos,
                            xOffset - radius, yPos, nextXOffset + radius, nextYPos,
                            10, 20, RED );
        }

        xOffset += 3 * radius + 40;
        current = current->next;
    }

    Node* deletedNode = deletedNodes;
    while (deletedNode != NULL) {
        deletedNode->data += 1;

        const char* dataStr = TextFormat("%d", deletedNode->data);
        int radius = 20;
        DrawCircle(deletedNode->data, GetScreenHeight() / 2, radius, RED);
        DrawText(dataStr, deletedNode->data - radius + 5, GetScreenHeight() / 2 - 10, 20, WHITE);

        deletedNode = deletedNode->next;
    }
}
void triselection() {
    if (head == NULL || head->next == NULL) {
        return; 
    }

    Node *current, *min;
    int temp;

    for (current = head; current != NULL; current = current->next) {
        min = current;
        for (Node* inner = current->next; inner != NULL; inner = inner->next) {
            if (inner->data < min->data) {
                min = inner;
            }
        }
        temp = current->data;
        current->data = min->data;
        min->data = temp;

        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        drawList(0); 
        EndDrawing();

        
        WaitTime(1); 
    }
}
void insertElementAtPosition(int data, int position) {
    Node* newNode = createNode(data);

    if (head == NULL || position <= 1) {
        newNode->next = head;
        if (head != NULL) {
            head->prev = newNode;
        }
        head = newNode;
    } else {
        Node* current = head;
        int currentPosition = 1;

        while (currentPosition < position - 1 && current->next != NULL) {
            current = current->next;
            currentPosition++;
        }

        newNode->next = current->next;
        if (current->next != NULL) {
            current->next->prev = newNode;
        }
        current->next = newNode;
        newNode->prev = current;
    }
}