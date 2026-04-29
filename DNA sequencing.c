#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX 100
#define LEN 100

// ================= INPUT =================
int readFile(char fragments[][LEN]) {
    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        printf("File not found!\n");
        return 0;
    }

    int n = 0;
    while (fscanf(fp, "%s", fragments[n]) != EOF && n < MAX)
        n++;

    fclose(fp);
    return n;
}

int manualInput(char fragments[][LEN]) {
    int n;
    printf("Enter number of fragments: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("Fragment %d: ", i + 1);
        scanf("%s", fragments[i]);
    }
    return n;
}

// ================= PREPROCESS =================
void clean(char s[]) {
    int k = 0;
    for (int i = 0; s[i]; i++) {
        char ch = toupper(s[i]);
        if (ch=='A'||ch=='C'||ch=='T'||ch=='G')
            s[k++] = ch;
    }
    s[k] = '\0';
}

void preprocess(char fragments[][LEN], int n) {
    for (int i = 0; i < n; i++)
        clean(fragments[i]);
}

// ================= OVERLAP =================
int overlap(char a[], char b[]) {
    int max = 0;
    int la = strlen(a), lb = strlen(b);

    for (int k = 1; k <= la && k <= lb; k++) {
        if (strncmp(a + la - k, b, k) == 0)
            max = k;
    }
    return max;
}

// ================= GREEDY =================
void greedy(char fragments[][LEN], int n, char result[]) {
    char temp[MAX][LEN];
    for (int i = 0; i < n; i++)
        strcpy(temp[i], fragments[i]);

    int len = n;

    printf("\n[Step-by-Step Greedy Merging]\n");

    while (len > 1) {
        int max = -1, l = 0, r = 0;
        char merged[LEN * 2];

        for (int i = 0; i < len; i++) {
            for (int j = 0; j < len; j++) {
                if (i == j) continue;

                int ov = overlap(temp[i], temp[j]);
                if (ov > max) {
                    max = ov;
                    l = i;
                    r = j;
                }
            }
        }

        strcpy(merged, temp[l]);
        strcat(merged, temp[r] + max);

        printf("%s + %s → %s\n", temp[l], temp[r], merged);

        strcpy(temp[l], merged);

        for (int i = r; i < len - 1; i++)
            strcpy(temp[i], temp[i + 1]);

        len--;
    }

    strcpy(result, temp[0]);
}

// ================= DE BRUIJN =================
void deBruijn(char genome[]) {
    printf("\n[De Bruijn Graph]\n");

    int L = strlen(genome);
    for (int i = 0; i < L - 2; i++) {
        char p[3], s[3];

        strncpy(p, genome + i, 2); p[2] = '\0';
        strncpy(s, genome + i + 1, 2); s[2] = '\0';

        printf("[%s] ---> [%s]\n", p, s);
    }
}

// ================= TSP =================
void tsp(char fragments[][LEN], int n) {
    int visited[MAX] = {0};
    int current = 0;

    printf("\n[TSP Path]\n");
    printf("%s ", fragments[current]);
    visited[current] = 1;

    for(int step=1; step<n; step++){
        int next = -1, max = -1;

        for(int j=0;j<n;j++){
            if(!visited[j]){
                int ov = overlap(fragments[current], fragments[j]);
                if(ov > max){
                    max = ov;
                    next = j;
                }
            }
        }

        if(next != -1){
            printf("-> %s ", fragments[next]);
            visited[next] = 1;
            current = next;
        }
    }
    printf("\n");
}

// ================= VISUALIZATION =================
void showAssemblyVisualization() {

    printf("\n================ DNA ASSEMBLY VISUALIZATION ================\n");

    printf("\n[Step 1: Read Fragments]\n");
    printf("C C G T G\n");
    printf("C C C G A G C C G\n");
    printf("A A T C C C G\n");

    printf("\n[Step 2: Overlap Step]\n");
    printf("   C C G T G\n");
    printf("   [C C G]\n\n");

    printf("   C C C G A G C C G\n");
    printf("     [C C C G]     [C C G]\n\n");

    printf("   A A T C C C G\n");
    printf("         [C C C G]\n");

    printf("\n[Step 3: Layout Step]\n");
    printf("A A T [C C C G]\n");
    printf("      [C C C G] A G [C C G]\n");
    printf("                [C C G] T G\n");

    printf("\n[Step 4: Consensus Step]\n");
    printf("A A T [C C C G] A G [C C G] T G\n");

    printf("\n===========================================================\n");
}

// ================= MAIN =================
int main() {

    char fragments[MAX][LEN];
    char result[LEN*2] = "";
    int n = 0, choice;

    while (1) {
        printf("\n===== DNA ASSEMBLY SYSTEM =====\n");
        printf("1.Load File\n2.Manual Input\n3.Preprocess\n");
        printf("4.Greedy\n5.DeBruijn\n6.TSP\n");
        printf("7.Visualization\n8.Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if(choice==1) n = readFile(fragments);
        else if(choice==2) n = manualInput(fragments);
        else if(choice==3) preprocess(fragments,n);

        else if(choice==4){
            greedy(fragments,n,result);
            printf("\nFinal Genome: %s\n", result);
        }

        else if(choice==5) deBruijn(result);
        else if(choice==6) tsp(fragments,n);
        else if(choice==7) showAssemblyVisualization();

        else if(choice==8) break;

        else printf("Invalid choice!\n");
    }
}