#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <math.h>
#define MAX 15
struct user{
	char userName[MAX];
	char name[MAX];
	char surname[MAX];
	char password[MAX];
};
struct score{
	char userName[MAX];
	int score;
};
void freeMatris(char ***matris, int m, int n) {
	int i,j;
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            free(matris[i][j]);
        }
        free(matris[i]);
    }
    free(matris);
}

// Function to read user data from file
int takeUsers(struct user **users, char *filename) {
	
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return ;
    }
	int i=0,userCount=0;

	fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    
	userCount = fileSize / sizeof(struct user);

	struct user *tempUsers = realloc(*users, userCount * sizeof(struct user));

	if (tempUsers != NULL) {
    	*users = tempUsers;
	} 
	else {
    	printf("Error reallocating memory.\n");
    	exit(EXIT_FAILURE); 
	}
	
    for (i = 0; i < userCount; i++) {
        if (fread(*users + i, sizeof(struct user), 1, file) != 1) {
            printf("Error reading data from the file.\n");
            fclose(file);
            return;
        }
    }
	fclose(file);
	return userCount;
	
}
// Function to read scores from file
int takeScores(struct score **scores, char *filename) {
	
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return ;
    }
	int i=0,scoreCount=0;

	fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    
	scoreCount = fileSize / sizeof(struct score);

	*scores = realloc(*scores, scoreCount * sizeof(struct score));
	
    for (i = 0; i < scoreCount; i++) {
        if (fread(*scores + i, sizeof(struct score), 1, file) != 1) {
            printf("Error reading data from the file.\n");
            fclose(file);
            return;
        }
    }
	fclose(file);
	return scoreCount;
	
}

int checkUser(char *userName,struct user *users,int userCount){
	int i=0;
	while(i<userCount && strcmp(userName,users[i].userName) != 0){
		i++;
	}
	if(i<userCount){
		return 1;
	}
	return 0;
}
int checkPassword(char *password,char* userName,struct user *users,int userCount){
	int i=0;
	while(i<userCount && strcmp(userName,users[i].userName) != 0){
		i++;
	}
	if(i<userCount && strcmp(password,users[i].password) == 0){
		return 1;
	}
	return 0;
}
int login(int userCount, struct user **users,struct user* currentUser){
	char dummyName[MAX];
	char dummyPas[MAX];
	printf("---Login---\n");
    printf("Username:\n");
    scanf("%s", dummyName);
    printf("Password:\n");
    scanf("%s", dummyPas);
    while((checkUser(dummyName,*users,userCount)==0 || checkPassword(dummyPas,dummyName,*users,userCount)==0 )&& dummyName[0]!='0'){
    	printf("password or username is wrong. (press 0 to quit)\n");
    	printf("Username:\n");
    	scanf("%s", dummyName);
    	printf("Password:\n");
    	scanf("%s", dummyPas);
	}
	if(dummyName[0]=='0')// User chose to quit
		return 0;
	strcpy((*currentUser).userName,dummyName);
	return 1;
	
}
void createUser(int userCount, struct user **users) {
    struct user *tempUsers = realloc(*users, (userCount + 1) * sizeof(struct user));

    if (tempUsers != NULL) {
        *users = tempUsers;
    } else {
        printf("Error reallocating memory.\n");
        free(*users);
    	exit(EXIT_FAILURE); 
    }

    struct user *u = &((*users)[userCount]);

    FILE *file = fopen("users.txt", "ab");
    if (file == NULL) {
        printf("Error opening the file.\n");
    }

    printf("---New User---\n");
    printf("name:\n");
    scanf("%s", u->name);

    printf("surname:\n");
    scanf("%s", u->surname);
    printf("username:\n");
    scanf("%s", u->userName);
	while(checkUser(u->userName,*users,userCount)==1){
		printf("(this username is taken!) username:\n");
    	scanf("%s", u->userName);
	}


    printf("password:\n");
    scanf("%s", u->password);
	
    fwrite(u,sizeof(struct user),1,file);


    fclose(file);
}
void createScore(int scoreCount, struct score **scores, int score, struct user currentUser) {
	int tmp,i,j;
	struct score *tempScores = (struct score*)realloc(*scores, (scoreCount + 1) * sizeof(struct score));
	scoreCount++;
    if (tempScores != NULL) {
        *scores = tempScores;
    } else {
        printf("Error reallocating memory.\n");
    	exit(EXIT_FAILURE); 
    }
    struct score *s = &((*scores)[scoreCount-1]);
    (*s).score= score;
    strcpy((*s).userName,currentUser.userName);
    FILE *file = fopen("scores.txt", "wb");
    if (file == NULL) {
        printf("Error opening the file.\n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < scoreCount-1; i++) {
        for (j = 0; j < scoreCount-i-1; j++) {
            if ((*scores)[j].score < (*scores)[j+1].score) {
                tmp = (*scores)[j].score;
                (*scores)[j].score = (*scores)[j+1].score;
                (*scores)[j+1].score = tmp;
            }
        }
	}
	if(scoreCount>5){
		struct score *newScores = malloc(5 * sizeof(struct score));
        for (i = 0; i < 5; i++) {
            newScores[i].score = (*scores)[i].score;
            strcpy(newScores[i].userName, (*scores)[i].userName);
        }
        free(*scores);
        *scores = newScores;
        scoreCount = 5; 
	}
	
	for(i=0;i<scoreCount;i++){
		fwrite(&(*scores)[i],sizeof(struct score),1,file);
		
	}
	sleep(2); system("cls");
	fclose(file);
}
char inGameMenu(){
	char in;
	printf("---------MENU---------\n\n");
	printf("1- Start Game\n");
	printf("2- Show TOP 5\n");
	printf("3- How To Play\n");
	in= getch();
	return in;
	
}
void howToPlay(){
	sleep(3/4); system("cls");
	int in;
	printf("\nP+ : P\t\t   e- : e\t\tP- : p\t\t   e+ : E\nKara Delik : K     Kullanici : X\tCikis : C\t\t\nYol : 0\t\t   Duvar : 1\t\tGiris : G\n");
	printf("\nKurallar:\nKarsit hidrojen uretebilmek icin labirentin cikisinda elinizde sadece P- ve e+ parcaciklari bulunmalidir.\n");
	printf("Bir parcacikla o parcacigin zit isaretli karsit parcacigi bir araya gelirse birbirini yok eder.\n");
	printf("Karadelikler K ile gosterilmistir. Karadeliklerin bulundugu hucrelerden gecildigi takdirde oyun sonlanir.\n");
	printf("Belirli bir oyun suresi icinde kullanici cýkisa ulasamazsa oyun sonlanir.\n");
	printf("\nMenuye donmek icin  klavyeden bir tusa basiniz.");
	in = getch();
	sleep(3/4); system("cls");
}
// Function to create and initialize the game matrix from a file

char*** matrisOlustur(char*** matris,char* filename, int*m, int*n){
	FILE *file = fopen(filename, "r");
	int c,i,j;
	*m=0;*n=0;
    if (file == NULL) {
        printf("Error opening the file.\n");
        exit(EXIT_FAILURE);
    }
    // finding row and column counts
    while((c= fgetc(file))!= EOF){
    	if(c==' '){
    		(*n)++;
		}
		if(c=='\n'){
			(*m)++;
		}
	}
	printf("a");
	(*m)++;
	(*n)= ((*n)/(*m))+1;
	
	matris = (char ***)malloc((*m) * sizeof(char **));
	printf(" m %d n %d ",*m,*n);
    for (i = 0; i < (*m); i++) {
        matris[i] = (char **)malloc((*n) * sizeof(char *));
    }
    for (i = 0; i < (*m); i++) {
        for (j = 0; j < (*n); j++) {
            matris[i][j] = (char *)malloc((MAX) * sizeof(char));
        }
    }
    rewind(file);
    //placing the strings (elements) to the matrix
	for (i = 0; i < (*m); i++) {
        char line[100];  
        fgets(line, sizeof(line), file);

        char *token = strtok(line, " ");
        int j = 0;
        while (token != NULL && j < MAX) {
            strcpy(matris[i][j], token);
            token = strtok(NULL, " ");
            j++;
        }
    }
    
    printf("Harita:\n");
    for (i = 0; i < (*m); i++) {
        for (j = 0; j < (*n); j++) {
            printf("%-5s ", matris[i][j]);
        }
        printf("\n");
    }
    fclose(file);
    return matris;
}
void yenileYazdir(char*** matris,int m,int n){
	int i,j;
	sleep(3/4); system("cls");
	printf("\nHarita:\n");
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            printf("%-5s ", matris[i][j]);
        }
        printf("\n");
    }
}
int play(char*** matris,int m,int n){
	int i,j,x,y,cik=0,top=1,earti=0,eeksi=0,parti=0,peksi=0,score,ch;
	char **toplanan;
	toplanan= (char**)malloc(sizeof(char*));
	toplanan[0] = (char*)malloc(sizeof(char)*MAX);
	for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            if(strcmp(matris[i][j],"G")==0 || strcmp(matris[i][j],"G\n")==0){
            	x=i;y=j;
            	strcpy(matris[x][y],"X");
			}
        }
    }
    do{
    	ch=getch(); // gets the arrow keys
    	if(y==(n-1)){
    		strcpy(matris[x][y],"0\n");
		}
		else{
			strcpy(matris[x][y],"0");
		}
    	
		
    	
    	if((ch== 72) && x!=0 && matris[x-1][y][0]!= '1'){
    		x--;
		}
		else if((ch==75) && y!=0 && matris[x][y-1][0]!= '1'){
    		y--;
		}
		else if((ch== 77) && y!=n-1 && matris[x][y+1][0]!= '1'){
    		y++;
		}
		else if((ch== 80) && x!=m-1 && matris[x+1][y][0]!= '1'){
    		x++;
		}
		if(strcmp(matris[x][y],"C")==0 || strcmp(matris[x][y],"K")==0 || strcmp(matris[x][y],"C\n")==0 || strcmp(matris[x][y],"K\n")==0){
			cik=1;
		}
		
		else if(strcmp(matris[x][y],"0")!=0 && strcmp(matris[x][y],"X")!=0 && strcmp(matris[x][y],"0\n")!=0 && strcmp(matris[x][y],"X\n")!=0){
			printf(" %d ",strcmp(matris[x][y],"0"));
			strcpy(toplanan[top-1],matris[x][y]);
			top++;
			toplanan= realloc(toplanan,top*sizeof(char*));
			toplanan[top-1] = (char*)malloc(sizeof(char)*MAX);
			
		}
		if(y==(n-1)){
    		strcpy(matris[x][y],"X\n");
		}
		else{
			strcpy(matris[x][y],"X");
		}
    	yenileYazdir(matris,m,n);
		printf("\nToplananlar: ");
    	for(i=0;i<top-1;i++){
    		printf(" %s ",toplanan[i]);
		}

	}while(cik==0);
	for (i = 0; i < top-1; i++) {
        if(strcmp(toplanan[i],"P+")==0 || strcmp(toplanan[i],"P+\n")==0){
        	parti++;
		}
		if(strcmp(toplanan[i],"P-")==0 || strcmp(toplanan[i],"P-\n")==0){
        	peksi++;
		}
		if(strcmp(toplanan[i],"e+")==0 || strcmp(toplanan[i],"e+\n")==0){
        	earti++;
		}
		if(strcmp(toplanan[i],"e-")==0 || strcmp(toplanan[i],"e-\n")==0){
        	eeksi++;
		}
    }
    //artilar eksileri goturur
    if(parti>peksi){
    	parti-=peksi;
    	peksi=0;
	}
	else{
		peksi-=parti;
		parti=0;
	}
	if(earti>eeksi){
    	earti-=eeksi;
    	eeksi=0;
	}
	else{
		eeksi-=earti;
		earti=0;
	}
	if(peksi!=0 && peksi>=earti){
		score=earti;
	}
	else if(eeksi!=0 && eeksi>=parti){
		score=parti;
	}
    printf("\n%d adet karsit madde uretildi.\n",score);
    return score;
}
void showTop(struct score *scores,int scoreCount){
	sleep(3/4); system("cls");
	int i,in;
	printf("------TOP 5------\n\n");
	for(i=0;i<scoreCount;i++){
		printf("%d. %-5s %-5d\n",i+1,scores[i].userName,scores[i].score);
	}
	printf("\nMenuye donmek icin  klavyeden bir tusa basiniz.");
	in = getch();
	sleep(3/4); system("cls");
}

int main(){
	int userCount=0,m,n,scoreCount=0,i,sc;
	char ***matris,haritaName[MAX*3],ch,choice,inGameChoice;
	struct score* scores = (struct score*)malloc(sizeof(struct score));
	struct user* users= (struct user*)malloc(sizeof(struct user));
	struct user* currentUser = (struct user*)malloc(sizeof(struct user));
	userCount = takeUsers(&users,"users.txt");
	scoreCount = takeScores(&scores,"scores.txt");
	
	do{
		
		printf("----------------------------------------------\n");
		printf("OYUNA HOSGELDIN\n 1-Login  2- Hesap Olustur\n");
		choice = getch();
		if(choice=='2'){
			createUser(userCount,&users);
			userCount++;
		}
		else if(choice=='1'){
			if(login(userCount,&users,currentUser)==1){
				printf("logged in:)");
				sleep(3/4); system("cls");
				do{	
					inGameChoice = inGameMenu();
					if(inGameChoice=='3'){
						howToPlay();
					}
					if(inGameChoice=='2'){
						showTop(scores,scoreCount);
					}
				}while(inGameChoice!='1');
				printf("\nOynamak istediginiz haritanin ismini sonunda .txt olacak sekilde giriniz.\n1- harita1\n2- harita2\n");
				scanf("%s",haritaName);
				sleep(3/4); system("cls");
				matris = matrisOlustur(matris,haritaName,&m,&n);
				sc = play(matris,m,n);
				createScore(scoreCount, &scores,sc, *currentUser);
			}
		}
	}while(choice!=3);
	free(users);
	free(scores);
	freeMatris(matris,m,n);
	return 0;
	
}
