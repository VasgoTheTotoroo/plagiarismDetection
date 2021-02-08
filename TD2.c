#include<stdio.h>
#include<stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>

#define BLANC '_'
#define CARFIN '\n'
#define CHAR_LIGNE 75

struct alignement
{
  char * x;
  char * y;
};

int sub(char a, char b)
{
	return !(a==b);
}

void afficheT(int m, int n, int **T)
{
	int i,j;
	
	for(i=0;i<m+1;i++)
	{
		for(j=0;j<n+1;j++)
		{
			printf("%d ",T[i][j]);
		}
		printf("\n");
	}
}

/* =============================================================== */
char * readtextfile(char * filename)
  /* Retourne le contenu du fichier texte filename */
/* =============================================================== */
{
  struct stat monstat;
  int N;
  char * text = NULL;
  FILE *fd = NULL;

  N = stat(filename, &monstat);
  if (N == -1)
  {
    fprintf(stderr, "error : bad file %s\n", filename);
    exit(0);
  }
  N = monstat.st_size;
  text = (char *)malloc(N+1);
  if (text == NULL)
  {   fprintf(stderr,"readtextfile() : malloc failed for text\n");
      exit(0);
  }
  fd = fopen(filename,"r");
  if (!fd)
  {
    fprintf(stderr, "readtextfile: can't open file %s\n", filename);
    exit(0);
  }
  
  fread(text, sizeof(char), N, fd);
  if((N>0) && (text[N] == '\n') ) text[N] = '\0';
  else text[N] = '\0';
  fclose(fd);
  return text;
}

/* =============================================================== */
int Imax(int a, int b)
/* Retourne  le maximum de a et b                                  */
/* =============================================================== */
{
  if (a < b) return b;
  else return a;	       
}

/* =============================================================== */
int Imin2(int a, int b)
/* Retourne  le minimum de a et b                                  */
/* =============================================================== */
{
  if (a < b) return a;
  else return b;	       
}

/* =============================================================== */
int Imin3(int a, int b, int c)
/* Retourne  le minimum de a, b et c                               */
/* =============================================================== */
{
  return Imin2(Imin2(a,b),c);
}

/* =============================================================== */
void retourne(char *c)
/* Retourner la chaîne de caractère c                              */
/* =============================================================== */
{
  char tmp;
  int m, j, i;
  m = strlen(c);
  j = m/2;
  for(i = 0; i < j; i++ ){
    tmp = c[i];
    c[i] = c[m-i-1];
    c[m-i-1] = tmp;
  }
}

int** lev(char *x, char *y)
{
	int m, n, **T, i, j;
	
	m=strlen(x);
	n=strlen(y);
	T= (int**)malloc((m+1)*sizeof(int*));
	for(i=0;i<m+1;i++)
	{
		T[i]=(int*)malloc((n+1)*sizeof(int));
	}
	
	T[0][0]=0;
	for(i=1;i<m+1;i++)
	{
		T[i][0]=T[i-1][0]+1;
	}
	
	for(j=1;j<n+1;j++)
	{
		T[0][j]=T[0][j-1]+1;
	}
	
	for(i=1;i<m+1;i++)
	{
		for(j=1;j<n+1;j++)
		{
			T[i][j]=Imin3(T[i][j-1]+1, T[i-1][j]+1, T[i-1][j-1]+sub(x[i-1],y[j-1]));
		}
	}
	
	// afficheT(m,n,T);
	return T;
}

struct alignement remonte(char *x, char *y)
{
	int m, n, i, j, k, **T;
	struct alignement res;
	
	T=lev(x,y);
	k=0;
	m=strlen(x);
	n=strlen(y);
	i=m, j=n;
	char *xp, *yp;
	
	xp=(char*) malloc((m+n+1)*sizeof(char));
	yp=(char*) malloc((m+n+1)*sizeof(char));
	
	while(i!=0 && j!=0)
	{
		if(T[i][j]==T[i-1][j]+1)
		{
			//on rajoute un blanc à y
			xp[k]=x[i-1];
			yp[k]=BLANC;
			i--;
		}
		else if(T[i][j]==T[i][j-1]+1)
		{
			//on rajoute un blanc à x
			xp[k]=BLANC;
			yp[k]=y[j-1];
			j--;
		}
		else if(T[i][j]==T[i-1][j-1]+sub(x[i-1],y[j-1]))
		{
			//on aligne x[i] avec y[j]
			xp[k]=x[i-1];
			yp[k]=y[j-1];
			i--;
			j--;
		}
		k++;
	}
	
	//i==0 ou j==0, on rajoute les derniers caractères
	if(i==0)
	{
		while(j>0)
		{
			yp[k]=y[j-1];
			xp[k]=BLANC;
			j--;
			k++;
		}
	}
	else
	{
		while(i>0)
		{
			xp[k]=x[i-1];
			yp[k]=BLANC;
			i--;
			k++;
		}
	}
	
	retourne(xp);
	retourne(yp);
	
	res.x=xp;
	res.y=yp;
	
	free(T);
	return res;
}

/* =============================================================== */
void afficheSeparateurHorizontal(int nbcar)
/* =============================================================== */
{
  int i;
  printf("|-");
  for(i=0; i < nbcar; i++)
    printf("-");
  printf("-|-");
  for(i=0; i < nbcar; i++)
    printf("-");
  printf("-|\n");
}


/* =============================================================== */
void affiche(char* texte1, char* texte2, int nbcar)
  /* Affiche simultanément texte1 et texte 2 en positionnnant nbcar  
     caractères sur chaque ligne. */
/* =============================================================== */
{
  int i, l1, l2, l;
  
  char *t1,*t2;
  
  char out[512];
  
  l1 = strlen(texte1);
  l2 = strlen(texte2);

  t1 = (char*) malloc(sizeof(char) * (nbcar + 1));
  t2 = (char*)malloc(sizeof(char) * (nbcar + 1));

  l = Imax(l1, l2);
  afficheSeparateurHorizontal(nbcar);
  for(i = 0; i < l; i+= nbcar){
    if (i < l1) {
      strncpy(t1, &(texte1[i]), nbcar);
      t1[nbcar] = '\0';
    } else t1[0] = '\0';
    if (i < l2) {
      strncpy(t2, &(texte2[i]),nbcar);
      t2[nbcar] = '\0';
    } else t2[0] = '\0';
    
    sprintf(out, "| %c-%ds | %c-%ds |\n",'%', nbcar, '%', nbcar);
    printf(out, t1,t2);
  }
  afficheSeparateurHorizontal(nbcar);
  free(t1);
  free(t2);
}

/* =============================================================== */
void affiche2(char* texte1, char* texte2, int nbcar)
  /* idem affiche, mais avec un formattage différent */
/* =============================================================== */
{

  int i, l1, l2, l;
  
  char *t1,*t2;
  
  char out[512];
  
  l1 = strlen(texte1);
  l2 = strlen(texte2);

  t1 = (char*) malloc(sizeof(char) * (nbcar + 1));
  t2 = (char*)malloc(sizeof(char) * (nbcar + 1));

  l = Imax(l1, l2);

  for(i = 0; i < l; i+= nbcar){
    if (i < l1) {
      strncpy(t1, &(texte1[i]), nbcar);
      t1[nbcar] = '\0';
    } else t1[0] = '\0';
    if (i < l2) {
      strncpy(t2, &(texte2[i]),nbcar);
      t2[nbcar] = '\0';
    } else t2[0] = '\0';
    
    sprintf(out, "x: %c-%ds \ny: %c-%ds\n",'%', nbcar, '%', nbcar);
    printf(out, t1,t2);

  }
  free(t1);
  free(t2);
}

/* =============================================================== */
char ** text2tabseq(char *text, char fin, int *nbseq)
/* =============================================================== */
/* 
   Parcourt "text" qui contient des fragments termines par le caractere fin.
   Les fragments sont ranges (sans le caractere fin) dans un tableau
     de chaines de caracteres (char **), qui est alloue et retourne 
     par text2tabseq.
   En sortie, *nbseq contient le nombre de fragments trouves.
*/
{
  register int i, j, n, ts;
  char ** tabseq = NULL;
  char *p;

  /* compte le nb de sequences - ie - de caracteres fin */
  *nbseq = 0;
  ts = strlen(text);
  for (i = 0; i < ts; i++) 
  {
    if (text[i] == fin) 
      (*nbseq)++;
  }

  /* alloue le tableau de pointeurs tabseq */
  tabseq = (char **)malloc(*nbseq * sizeof(char *));
  if (tabseq == NULL)
  {   fprintf(stderr,"text2tabseq() : malloc failed for tabseq\n");
      exit(0);
  }

  /* transfere les chaines dans tabseq */
  j = i = 0;
  for (n = 0; 
       n < *nbseq; 
       n++, i = j + 1, j = i)
  {
    while (text[j] != fin) j++;
    p = (char *)malloc((j - i + 1) * sizeof(char));
    if (!p) { fprintf(stderr, "error : malloc failed\n"); exit(0); }
    strncpy(p, text+i, j-i);
    p[j-i] = '\0';
    tabseq[n] = p;
  }
  return tabseq;
} /* text2tabseq() */

/* =============================================================== */
void freetabseq(int nbseq, char **tabseq)
/* =============================================================== */
{
  int i;

  for (i = 0; i < nbseq; i++)
    if (tabseq[i] != NULL) free(tabseq[i]);
  free(tabseq);
} /* freetabseq() */

void remonteLignes(char *x, char *y)
{
	int taille1, taille2, min, max, **T, score, i, j;
	char **text1, **text2;
	struct alignement res;
	
	text1 = text2tabseq(x, CARFIN, &taille1);
	text2 = text2tabseq(y, CARFIN, &taille2);
	min=Imin2(taille1,taille2);
	max=Imax(taille1,taille2);
	int indices[min];
	int indicesMax[max];
	int scores[min];
	
	for(i=0;i<min;i++)
	{
		indices[i]=0;
		scores[i]=INT_MAX;
	}
	
	//trouver la meilleure correspondance entre les chaines.
	for(i=0;i<min;i++)
	{
		score=INT_MAX;
		
		for(j=0;j<max;j++)
		{
			if(min==taille1)
			{
				T=lev(text1[i],text2[j]);
				score=T[strlen(text1[i])][strlen(text2[j])];
				// printf("score pour %d, %d: %d\n",i,j,score);
			}
			else
			{
				T=lev(text1[j],text2[i]);
				score=T[strlen(text1[j])][strlen(text2[i])];
			}
			if(score<scores[i])
			{
				scores[i]=score;
				indices[i]=j;
			}
		}
	}
	free(T);
	
	//on modifie le tableau des indices pour qu'il corresponde au texte le plus long.
	for(i=0;i<max;i++)
	{
		indicesMax[i]=-1;
	}
	for(i=0;i<min;i++)
	{
		indicesMax[indices[i]]=i;
	}
	
	//afficher les chaines dans le bon ordre et avec les blancs sur certains paragraphes
	for(i=0;i<max;i++)
	{
		if(indicesMax[i]==-1)
		{
			if(min==taille1)
			{
				affiche("", text2[i], CHAR_LIGNE);
			}
			else
			{
				affiche(text1[i], "", CHAR_LIGNE);
			}
		}
		else
		{
			if(min==taille1)
			{
				res=remonte(text1[indicesMax[i]],text2[i]);
				affiche(res.x, res.y, CHAR_LIGNE);
			}
			else
			{
				res=remonte(text1[i],text2[indicesMax[i]]);
				affiche(res.x, res.y, CHAR_LIGNE);
			}
		}
	}
	
	freetabseq(taille1, text1);
	freetabseq(taille2, text2);
}

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  char *x, *y;
  
  if(argc != 3){
    printf("usage: %s text1 text2\n", argv[0]);
    exit(0);
  }  

  x = readtextfile(argv[1]);
  y = readtextfile(argv[2]);
  remonteLignes(x,y);
  
  free(x);
  free(y);
}
