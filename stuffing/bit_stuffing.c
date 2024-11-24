#include <stdio.h>
#include <string.h>
#define MAX 100
void bitStuffing(char input[], char output[]) {
    int i, j = 0;
    int count = 0;
    int length = strlen(input);
      for (i = 0; i < length; i++) {
        if (input[i] == '1') {
            count++;
        } else {
            count = 0;
        }
           output[j++] = input[i];
       // If five consecutive '1' are found, stuff a '0'
        if (count == 5) {
            output[j++] = '0';
            count = 0;
        }
    }
 output[j] = '\0'; // Null-terminate the output string
}
int main() {
    char input[MAX], output[MAX * 2]; // Output may need to be twice the size of input due to stuffing
     printf("Enter the input bit string: ");
    scanf("%s", input);
     bitStuffing(input, output);
     printf("Bit-stuffed output: %s\n", output);
     return 0;
}