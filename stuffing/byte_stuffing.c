#include <stdio.h>
#include <string.h>
#define MAX 100
#define FLAG 0x7E
#define ESCAPE 0x7D
void byteStuffing(char input[], char output[]) {
    int j = 0;
    output[j++] = FLAG; 
    for (int i = 0; input[i]; i++) {
        if (input[i] == FLAG || input[i] == ESCAPE) {
            output[j++] = ESCAPE;
        }
        output[j++] = input[i];
    }
     output[j++] = FLAG; 
    output[j] = '\0'; 
}
int main() {
    char input[MAX], output[MAX * 2];
    printf("Enter the input string: ");
    fgets(input, MAX, stdin);
    input[strcspn(input, "\n")] = '\0'; 
    byteStuffing(input, output);
    printf("Byte-stuffed output: ");
    for (int i = 0; i < strlen(output); i++) {
        printf("%02X ", (unsigned char)output[i]);
    }
printf("\n");
return 0;
}