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

int main(void) {
    InitWindow(1200, 800, " Doubly Linked List algo project ");

    char inputBuffer[16] = "";
    int inputIndex = 0;
    int numElements = 0;
    int elementsEntered = 0;
    int NEntered = 0;
    int valueToDelete = 0;
    int valueToSearch = 0;
    int state = 0;

    int insertPosition = 0;
    int insertValue = 0;
    int insertingElement = 0;

    while (!WindowShouldClose()) {
        while (IsKeyPressed(KEY_BACKSPACE) && inputIndex > 0) {
            inputBuffer[--inputIndex] = '\0';
        }

        if (NEntered == 0 && IsKeyPressed(KEY_ENTER) && inputIndex > 0) {
            numElements = atoi(inputBuffer);
            elementsEntered = 0;
            NEntered = 1;
            inputBuffer[0] = '\0';
            inputIndex = 0;
        }

        if (elementsEntered < numElements && IsKeyPressed(KEY_ENTER) && inputIndex > 0) {
            int element = atoi(inputBuffer);
            insertNode(element);
            elementsEntered++;
            inputBuffer[0] = '\0';
            inputIndex = 0;
        }

        
       

        if (insertingElement) {
            if (insertPosition == 0) {
                DrawText("Enter the position to insert the element:", 50, 160, 20, PURPLE);
                DrawText(inputBuffer, 50, 190, 20, MAROON);

                if (IsKeyPressed(KEY_ENTER) && inputIndex > 0) {
                    insertPosition = atoi(inputBuffer);
                    inputBuffer[0] = '\0';
                    inputIndex = 0;
                }
            } else if (insertValue == 0) {
                DrawText("Enter the value of the element to insert:", 50, 230, 20, PURPLE);
                DrawText(inputBuffer, 50, 260, 20, MAROON);

                if (IsKeyPressed(KEY_ENTER) && inputIndex > 0) {
                    insertValue = atoi(inputBuffer);
                    insertingElement = 0;
                    insertElementAtPosition(insertValue, insertPosition);
                    elementsEntered++;

                    DrawText("Updated List:", 50, 320, 20, BLACK);
                    drawList(valueToSearch);

                    insertPosition = 0;
                    insertValue = 0;
                }
            }
        }
        if (state == 1) {
            DrawText("Enter the value of the node to delete:", 50, 150, 20, GREEN);
            DrawText(inputBuffer, 60, 180, 20, MAROON);

            if (IsKeyPressed(KEY_ENTER) && inputIndex > 0) {
                valueToDelete = atoi(inputBuffer);
                deleteNodeByValue(valueToDelete);
                state = 0;
                inputBuffer[0] = '\0';
                inputIndex = 0;
            }
        }

        
        if (state == 3) {
            DrawText("Enter the value to search:", 50, 150, 20, BLUE);
            DrawText(inputBuffer, 60, 180, 20, MAROON);

            if (IsKeyPressed(KEY_ENTER) && inputIndex > 0) {
                valueToSearch = atoi(inputBuffer);
                searchNode(valueToSearch);
                state = 0;
                inputBuffer[0] = '\0';
                inputIndex = 0;
            }
        }
         Vector2 mousePoint = GetMousePosition();

        if (CheckCollisionPointRec(mousePoint, (Rectangle){50, 10, 100, 30}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            emptyList();
            elementsEntered = 0;
            NEntered = 0;
        }

        if (CheckCollisionPointRec(mousePoint, (Rectangle){160, 10, 100, 30}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            state = 1;
        }

        if (CheckCollisionPointRec(mousePoint, (Rectangle){270, 10, 100, 30}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            state = 3;
        }

        if (CheckCollisionPointRec(mousePoint, (Rectangle){380, 10, 100, 30}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            insertingElement = 1;
            insertPosition = 0;
            insertValue = 0;
        }

        if (CheckCollisionPointRec(mousePoint, (Rectangle){490, 10, 100, 30}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            triselection();
        }

        if (inputIndex < 15) {
            int key = GetKeyPressed();
            if ((key >= '0' && key <= '9') || key == KEY_SPACE) {
                inputBuffer[inputIndex++] = (char)key;
                inputBuffer[inputIndex] = '\0';
            }
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (NEntered == 0) {
            DrawText("Enter the number of elements (N) and press Enter:", 50, 50, 20, BLACK);
        } else {
            DrawText("Enter values:",50, 50, 20, BLACK);
            DrawText(TextFormat("Elements Entered: %d/%d", elementsEntered, numElements), 50, 80, 20, BLACK);
        }

        DrawRectangle(50, 100, 200, 30, LIGHTGRAY);
        DrawText(inputBuffer, 60, 110, 20, MAROON);

        drawList(valueToSearch);

        DrawRectangle(50, 10, 100, 30, YELLOW);
        DrawText("Empty", 65, 15, 20, WHITE);
        DrawRectangle(160, 10, 100, 30, GREEN);
        DrawText("Delete", 175, 15, 20, BLACK);
        DrawRectangle(270, 10, 100, 30, BLUE);
        DrawText("Search", 285, 15, 20, WHITE);
        DrawRectangle(380, 10, 100, 30, PURPLE);
        DrawText("Insert", 395, 15, 20, BLACK);
        DrawRectangle(490, 10, 100, 30, PINK);
        DrawText("Sort", 505, 15, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();

    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }

    // Deallocate memory for the deleted nodes
    while
 (deletedNodes != NULL) {
        Node* temp = deletedNodes;
        deletedNodes = deletedNodes->next;
        free(temp);
    }

    return 0;
    }