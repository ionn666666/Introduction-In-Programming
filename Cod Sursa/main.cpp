#include <iostream>
#include <cstring>
#include <graphics.h>
#include <winbgim.h>
#include <stdlib.h>
#include <windows.h>
#include <fstream>

#define MAX_STR 256


using namespace std;



char Operand[]="0123456789x";
char Operator[]="+-/*^sctl";
char operatori[]="+-/*^";
char cifra[]="0123456789";

int i;


struct vect
{

char info[26][26];
int nr; //trebuie sa fie mai mic de 200.
};

struct nod
{
    char info[26];
    nod *urm;
};

struct nodArbore
{
    char info[26];
    int coloana;
    nodArbore *stanga=NULL;
    nodArbore *dreapta=NULL;

};

struct stiva
{
    nod *varf;
    int nrElemente;
};


//PASUL 1 : VERIFICAREA CORECTITUDINII EXPRESIEI ALGEBRICE

int verificareParanteze(char s[101])
{
    //Numărul de paranteze închise este egal cu numărul de paranteze deschise.
    //La o parcurgere a șirului, de la stânga la dreapta, până la orice poziție trebuie ca numărul de paranteze deschise să fie mai mare sau egal cu cel al parantezelor închise

    int parantezeDeschise=0, parantezeInchise=0;
    for(int i=0; i<strlen(s); i++)
    {
        if ( s[i] == '(' )
            parantezeDeschise++;
        if ( s[i] == ')' )
            parantezeInchise++;
        if(parantezeDeschise < parantezeInchise)
            return 0;
    }
    if (parantezeDeschise == parantezeInchise)
        return 1;
    else
        return 0;
}

int verificareFunctie(char s[101], int i)
{
    int ok=0;
    if(s[i]=='s' && s[i+1]=='i' && s[i+2]=='n' && s[i+3]=='(')
        ok=1;
    if(s[i]=='c' && s[i+1]=='o' && s[i+2]=='s' && s[i+3]=='(')
        ok=1;
    if(s[i]=='s' && s[i+1]=='q' && s[i+2]=='r' && s[i+3]=='t' && s[i+4]=='(')
        ok=1;
    if(s[i]=='l' && s[i+1]=='n' && s[i+2]=='(')
        ok=1;
    if(s[i]=='t' && s[i+2]=='g' && s[i+3]=='(')
        ok=1;
    if(s[i]=='c' && s[i+1]=='t' && s[i+2]=='g' && s[i+3]=='(')
        ok=1;


    if(ok==1)
        return 1;
    return 0;
}

int verificareFunctieGeneral(char s[101])
{
   // După un nume de funcție poate urma doar o paranteză deschisă (().
    for(int i=0; i<strlen(s); i++)
    {
        if(s[i]>='a' && s[i]<='z' && s[i]!='x')
        {
            int ok=0;
            if(s[i]=='s' && s[i+1]=='i' && s[i+2]=='n' && s[i+3]=='(' && s[i+4]!=')')
                ok=1, i=i+4;
            if(s[i]=='c' && s[i+1]=='o' && s[i+2]=='s' && s[i+3]=='(' && s[i+4]!=')')
                ok=1, i=i+4;
            if(s[i]=='s' && s[i+1]=='q' && s[i+2]=='r' && s[i+3]=='t' && s[i+4]=='(' && s[i+5]!=')')
                ok=1, i=i+5;
            if(s[i]=='l' && s[i+1]=='n' && s[i+2]=='(' && s[i+3]!=')')
                ok=1, i=i+3;
            if(s[i]=='t' && s[i+1]=='g' && s[i+2]=='(' && s[i+3]!=')')
                ok=1, i=i+3;
            if(s[i]=='c' && s[i+1]=='t' && s[i+2]=='g' && s[i+3]=='(' && s[i+4]!=')')
                ok=1, i=i+4;

            if(ok==0)
                return 0;
        }
    }
    return 1;
}

int verificareOperatori(char s[101])
{
    //După un operator poate urma o variabilă (deci o literă), o funcție (deci sin/cos/ln etc.), o constantă (deci o cifră) sau o paranteză deschisă (().
    for(int i=0; i<strlen(s); i++)
    {
        if(strchr(operatori, s[i])!=0)
        {
            int conditie=0;
            if(s[i+1]!=NULL)
            {
                if(s[i+1]=='x')
                    conditie=1;
                if(strchr(cifra, s[i+1])!=0)
                    conditie=1;
                if(s[i+1]=='(')
                    conditie=1;
                if(verificareFunctie(s, i+1)==1)
                    conditie=1;
            }

            if (conditie==0)
                return 0;


        }
    }
    return 1;
}

int verificareVariabila(char s[101])
{
    //După o variabilă poate urma un operator sau o paranteză închisă ())
    for(int i=0; i<strlen(s)-1; i++)
    {
        if(s[i]=='x')
        {
            int conditie=0;
            if(s[i+1]==')')
                conditie=1;
            if(strchr(operatori, s[i+1])!=0)
                conditie=1;
            if(conditie==0)
                return 0;
        }
    }
    return 1;
}

int verificareNumarReal(char s[101])
{
    for (int i=0; i<strlen(s);i++)
    {
        if(s[i]=='.')
        {
            int conditie = 0;
            if(strchr(cifra, s[i+1])!=0)                   //verifica ca dupa punct sa fie o cifra
                conditie = 1;
            if(s[i+1]== NULL)                              //verifica daca dupa punct este ceva sa nu fie situatie: 12454.
                conditie = 0;
            if(strchr(cifra, s[i-1])!=0)
                conditie = 1;
            if(s[i-1]== NULL)                              //verifica daca inaintea punctului este cifra sa nu fie situatie :   .23
                conditie = 0;
            if(strchr(cifra, s[i+1])!=0)
            for (int k=i; k<strlen(s);k++){                //verifica sa nu fie mai multe puncte intr-un numar cum ar fi 12.3.21
                if(s[k+1]=='.')
                    conditie=0;
                if(strchr(cifra, s[k+1])==0)
                    k=strlen(s);
            }

            if (conditie == 0)
                 return 0;
        }

    }
    return 1;
}

int verificareConstanta (char s[101])
{
    //După o constantă poate urma un operator sau o paranteză închisă ())
    for(int i=0; i<strlen(s); i++)
    {
        if(strchr(cifra, s[i])!=0)
        {
            int conditie = 0;
            if(strchr(cifra, s[i+1])!=0)
                conditie = 1;
            if(s[i+1]==')')
                conditie = 1;
            if(strchr(operatori, s[i+1])!=0)
                conditie = 1;
            if(s[i+1]=='.')
                conditie=1;
            if(conditie == 0)
                return 0;
        }
    }
    return 1;
}

int verificareParantezaDeschisa(char s[101])
{
    //După paranteză deschisă (()) poate urma o altă paranteză deschisă, un nume de funcție, o variabilă, o constantă sau chiar operatorul unar + sau -.
    for(int i=0; i<strlen(s); i++)
    {
        if(s[i]=='(')
        {
            int conditie = 0;
            if(s[i+1] == '(')
                conditie = 1;
            if(s[i+1]=='+' || s[i+1]=='-')
                conditie = 1;
            if(s[i+1]=='x')
                conditie=1;
            if(strchr(cifra, s[i+1])!=0)
                conditie=1;
            if(verificareFunctie(s, i+1)==1)
                conditie=1;
            if(conditie==0)
                return 0;
        }
    }
    return 1;
}

int verificareParantezaInchisa(char s[101])
{
    //După paranteză închisă ())) poate urma o altă paranteză închisă sau un operator.
    for(int i=0; i<strlen(s); i++)
    {
        if(s[i]==')')
        {
            int conditie=0;
            if(s[i+1]==')')
                conditie=1;
            if(strchr(operatori, s[i+1])!=0)
                conditie=1;
            if(conditie==0)
                return 0;
        }
    }
    return 1;
}

void verificareExpresie(char s[101])
{
    int y=40;
   int conditie = 1;
    if (verificareParanteze(s)==0)
    {
        outtextxy(170, 200+y,"Expresia contine paranteze gresite" );
        y+=25;
        conditie = 0;
    }
    if(verificareFunctieGeneral(s)==0)
    {
        outtextxy(170, 200+y,"Expresia contine functii gresite" );
        y+=25;
        conditie=0;
    }
    if(verificareOperatori(s)==0)
    {
        outtextxy(170, 200+y,"Expresia contine operatori gresiti" );
        y+=25;
        conditie=0;
    }
    if(verificareVariabila(s)==0)
    {
        outtextxy(170, 200+y,"Expresia contine variabile gresite" );
        y+=25;
        conditie=0;
    }
    if(verificareNumarReal(s)==0)
    {
        outtextxy(170, 200+y,"Expresia contine un numar real gresit");
        y+=25;
        conditie=0;
    }
    if(verificareConstanta(s)==0)
    {
        outtextxy(170, 200+y,"Expresia contine constante gresite");
        y+=25;
        conditie=0;
    }
    if(verificareParantezaDeschisa(s)==0)
    {
        outtextxy(170, 200+y,"Expresia contine paranteze deschise gresite");
        y+=25;
        conditie=0;
    }
    if(verificareParantezaInchisa(s)==0)
    {
        outtextxy(170, 200+y,"Expresia contine paranteze inchise gresite");
        y+=25;
        conditie=0;
    }

   if(conditie==1)
   {
        outtextxy(170, 250, "Expresia introdusa este corecta!");
   }
}




//PASUL 2: VECTORIZAREA EXPRESIEI SI FORMAREA UNUI ARBORE BINAR ASOCIAT ACESTEIA


void vectorizeaza(char s[101], vect & vec)
{

    int i, k=0;
    char copie[101];
    for(i=0; i<strlen(s); i++)
   {
        if(strchr("()+-*/^x0123456789", s[i])!=0)
       {
           k++;
           vec.info[k][0]=s[i];
           vec.info[k][1]=NULL;
       }
        if(s[i]=='s' && s[i+1]=='i' && s[i+2]=='n') //SIN
        {
           k++;
           strcpy(vec.info[k], "sin");
           i=i+2;
        }
        if(s[i]=='c' && s[i+1]=='o' && s[i+2]=='s') //COS
        {
           k++;
           strcpy(vec.info[k], "cos");
           i=i+2;
        }
        if(s[i]=='s' && s[i+1]=='q' && s[i+2]=='r' && s[i+3]=='t') //SQRT
        {
            k++;
            strcpy(vec.info[k], "sqrt");
            i=i+3;
        }
        if(s[i]=='l' && s[i+1]=='n') //LN
        {
            k++;
            strcpy(vec.info[k], "ln");
            i=i+1;
        }
        if(s[i]=='t' && s[i+1]=='g') //TG
        {
            k++;
            strcpy(vec.info[k], "tg");
            i=i+1;
        }
        if(s[i]=='c' && s[i+1]=='t' && s[i+2]=='g') //CTG
        {
            k++;
            strcpy(vec.info[k], "ctg");
            i=i+2;
        }
    vec.nr=k;
   }

}

void initiereStiva(stiva &S)
{
    S.varf=NULL;
    S.nrElemente=0;
}

void push(char x[26], stiva &S) {
    nod *p = new nod;
    strcpy(p->info, x);
    p -> urm = S.varf;
    S.nrElemente++;
    S.varf = p;
}

void pop(stiva &S)
{
    nod *p = new nod;
    p = S.varf;
    S.nrElemente--;
    S.varf = S.varf->urm;
    delete p;
}

void afis(stiva S) {
    nod *p = new nod;
    p = S.varf;
    while(p) {
        cout << p->info << ' ';
        p=p->urm;
    }
}

int prioritate (char c)
{
    int prioritate;
    switch (c) {
        case '+':
            prioritate=1;
            break;
        case '-':
            prioritate=1;
            break;
        case '*':
            prioritate=2;
            break;
        case '/':
            prioritate=2;
            break;
        case '^':
            prioritate=3;
            break;
        case 's':
            prioritate=4;
            break;
        case 'c':
            prioritate=4;
            break;
        case 't':
            prioritate=4;
            break;
        case 'l':
            prioritate=4;
            break;
    }
    return prioritate;
}

void formaPostFixata(vect vin, vect & vout )
{

    stiva s;
    initiereStiva(s);
    int k=0;
    char c;

    for(int i=1; i<=vin.nr; i++)
    {
        c=vin.info[i][0];
        if(strchr(Operand, c)!=0)//daca avem un Operand
        {
            k++;
            vout.info[k][0]=c;
            vout.info[k][1]=NULL;
        }
        else
            if(strchr(Operator, c)!=0)// Daca avem un operator
            {
                if(s.varf==NULL)
                    push(vin.info[i], s);
                else
                    if(s.varf->info[0]=='(')
                       push(vin.info[i], s);
                    else
                        if(prioritate(c) > prioritate(s.varf->info[0]))
                            push(vin.info[i], s);
                        else
                            if(prioritate(c) < prioritate(s.varf->info[0]))
                                {
                                    k++;
                                    strcpy(vout.info[k], s.varf->info);
                                    pop(s);
                                    i--;
                                }
                            else
                                if(prioritate(c)==prioritate(s.varf->info[0]))
                                {
                                    k++;
                                    strcpy(vout.info[k], s.varf->info);
                                    pop(s);

                                    push(vin.info[i], s);
                                }
            }
            else
                if(c=='(')
                   push(vin.info[i], s);
                else
                    if(c==')')
                    {
                        while(s.varf->info[0]!='(')
                        {
                            k++;
                            strcpy(vout.info[k], s.varf->info);
                            pop(s);

                        }
                        if(s.varf->info[0]=='(');
                        {
                            pop(s);
                        }
                    }
    }
    if(s.varf!=NULL)
        while(s.varf!=NULL)
        {
            k++;
            strcpy(vout.info[k], s.varf->info);
            pop(s);
        }
    vout.nr=k;

}




//PASUL 3: REPREZENTAREA GRAFICA A ARBORELUI


int contorColoane=1;
int incrementareColoana;
int incrementareNivel;

void creareArbore(vect v, nodArbore *& arb )
{
    if(i!=0)
    {
            nodArbore * p = new nodArbore;
            strcpy(p->info, v.info[i]);
            p->stanga=NULL;
            p->dreapta=NULL;
            arb=p;

            if(strchr(Operator, v.info[i][0])!=0)
            {
                if(strchr("+-*/^", v.info[i][0])!=0)
                {
                    i--;
                    creareArbore(v, arb->dreapta);
                    i--;
                    creareArbore(v, arb->stanga);
                }
                else
                    if(strchr("sctl", v.info[i][0])!=0)
                    {
                        i--;
                        creareArbore(v, arb->dreapta);
                    }
            }
    }
}

int nrNiveluri(nodArbore * a)
{
    if(a==NULL)
        return 0;
    else
    {
        int n1=nrNiveluri(a->stanga);
        int n2=nrNiveluri(a->dreapta);
        return 1 + max(n1, n2);
    }

}

int nrColoane(nodArbore * a)
{
    if (a==NULL)
        return 0;
    else
    {
        int n1=nrColoane(a->stanga);
        int n2=nrColoane(a->dreapta);
        return 1 + n1 + n2;
    }
}

void desenareNod(nodArbore * a, int nivel, int coloana)
{
    char s[100];
    strcpy(s, a->info);
    outtextxy(coloana * incrementareColoana, nivel * incrementareNivel, s);
    int x=max(textheight(s), textwidth(s));
    circle(coloana * incrementareColoana + x/2, nivel * incrementareNivel + x/2, x+12);
}

void desenareLinie (nodArbore * a1, nodArbore * a2, int nivel)
{
    char s[100], p[100];
    strcpy(s, a1->info);
    strcpy(p, a2->info);
    int x=max(textheight(s), textwidth(s));
    int y=max(textheight(p), textwidth(p));
    int x1=a1->coloana * incrementareColoana + x/2;
    int y1=nivel * incrementareNivel + x/2 + x +12;
    int x2=a2->coloana * incrementareColoana + y/2;
    int y2=(nivel+1) * incrementareNivel + y/2 - y - 12;
    line(x1, y1, x2, y2);
}

void distribuireNoduri(nodArbore *a, int nivel)
{

    if(a!=NULL)
    {
        distribuireNoduri(a->stanga, nivel+1);
        desenareNod(a, nivel, contorColoane);
        a->coloana=contorColoane;

        if(a->stanga!=NULL)
            desenareLinie(a, a->stanga, nivel);

        contorColoane++;

        distribuireNoduri(a->dreapta, nivel+1);

        if(a->dreapta!=NULL)
            desenareLinie(a, a->dreapta, nivel);
    }
}

void desenareButonBack(int window)
{
    int x,y;
    outtextxy(55,50,"<---   Back");
    while(1)
    {
        Sleep(100);
            if(ismouseclick(WM_LBUTTONDOWN))
            {
            clearmouseclick(WM_LBUTTONDOWN);
            rectangle(50,25,150,85);
            x=mousex();
            y=mousey();
            if(x>50 && x<150 && y>25 && y<85)
            {
              break;
            }


            }
    }
    closegraph(CURRENT_WINDOW);
    setcurrentwindow(window);
}

void desenareArbore(nodArbore * varf)
{
    int window =  getcurrentwindow();
    contorColoane=1;
    incrementareNivel=720/nrNiveluri(varf);
    incrementareColoana=1280/nrColoane(varf);

    if(nrNiveluri(varf)==1 && nrColoane(varf)==1)
    {
        initwindow(1280, 720, "Desenare Arbore");
        incrementareNivel/=2;
        incrementareColoana/=2;
    }
    else
        initwindow(1280+incrementareColoana, 720+incrementareNivel, "Desenare Arbore");

    distribuireNoduri(varf, 1);

    desenareButonBack(window);
}

void afisareArbore(nodArbore * a)
{
    if(a!=NULL)
    {

        afisareArbore(a->stanga);
        cout<<a->info<<" ";
        afisareArbore(a->dreapta);

    }

}
void creareSir(nodArbore * a, char t[101])
{
    if(a!=NULL)
    {
        creareSir(a->stanga, t);
        strcat(t, a->info);
        strcat(t, " ");
        creareSir(a->dreapta, t);
    }
}




//PASUL 4: DERIVAREA SI SIMPLIFICAREA ARBORELUI


void simplificareAdunare(nodArbore * & arbin)
{
    if(arbin->stanga->info[0]!=0 && arbin->dreapta->info[0]=='0') // f + 0 = f
        arbin=arbin->stanga;
    else
        if(arbin->stanga->info[0]=='0' && arbin->dreapta->info[0]!='0') // 0 + f = f
            arbin=arbin->dreapta;
        else
            if(arbin->stanga->info[0]=='0' && arbin->dreapta->info[0]=='0') // 0 + 0 = 0
            {
                arbin->info[0]='0';
                arbin->info[0]=NULL;
                arbin->stanga=NULL;
                arbin->dreapta=NULL;
            }
            else
                if(strchr("123456789", arbin->stanga->info[0])!=0 && strchr("123456789", arbin->dreapta->info[0])!=0) // cifra1 + cifra2 = suma
                {
                    int x=arbin->stanga->info[0] - '0';
                    int y=arbin->dreapta->info[0] - '0';
                    int z=x+y;
                    if(z<=9)
                    {
                        arbin->info[0]=z + '0';
                        arbin->info[1]=NULL;
                        arbin->stanga=NULL;
                        arbin->dreapta=NULL;
                    }
                }
}

void simplificareScadere(nodArbore * & arbin)
{

        if(arbin->stanga->info[0]!=0 && arbin->dreapta->info[0]=='0') // f - 0 = f
            arbin=arbin->stanga;
        else
            if(arbin->stanga->info[0]=='0' && arbin->dreapta->info[0]!='0') // 0 - f = -f
                arbin->stanga=NULL;
            else
                if(arbin->stanga->info[0]=='0' && arbin->dreapta->info[0]=='0') // 0 - 0 = 0
                {
                    arbin->info[0]='0';
                    arbin->info[0]=NULL;
                    arbin->stanga=NULL;
                    arbin->dreapta=NULL;
                }
                else
                    if(strchr("123456789", arbin->stanga->info[0])!=0 && strchr("123456789", arbin->dreapta->info[0])!=0) // cifra1 - cifra2 = diserenta
                    {
                        int x=arbin->stanga->info[0] - '0';
                        int y=arbin->dreapta->info[0] - '0';
                        int z=x-y;
                        if(z>=0)
                        {
                            arbin->info[0]=z + '0';
                            arbin->info[1]=NULL;
                            arbin->stanga=NULL;
                            arbin->dreapta=NULL;
                        }
                    }
}

void simplificareInmultire(nodArbore * & arbin)
{

    if(arbin->stanga->info[0]!=0 && arbin->dreapta->info[0]=='0') // f * 0 = 0
    {
        arbin->info[0]='0';
        arbin->info[1]=NULL;
        arbin->stanga=NULL;
        arbin->dreapta=NULL;
    }
    else
        if(arbin->stanga->info[0]=='0' && arbin->dreapta->info[0]!='0') // 0 * f = 0
        {
            arbin->info[0]='0';
            arbin->info[1]=NULL;
            arbin->stanga=NULL;
            arbin->dreapta=NULL;
        }
        else
            if (arbin->stanga->info[0]!='0' && arbin->dreapta->info[0]=='1') // f * 1 = f
                arbin=arbin->stanga;
            else
                if(arbin->stanga->info[0]=='1' && arbin->dreapta->info[0]!='0') // 1 * f = f
                    arbin=arbin->dreapta;
                else
                    if(strchr("123456789", arbin->stanga->info[0])!=0 && strchr("123456789", arbin->dreapta->info[0])!=0) // cifra1 * cifra2 = suma
                    {
                        int x=arbin->stanga->info[0] - '0';
                        int y=arbin->dreapta->info[0] - '0';
                        int z=x*y;
                        if(z<=9)
                        {
                            arbin->info[0]=z + '0';
                            arbin->info[1]=NULL;
                            arbin->stanga=NULL;
                            arbin->dreapta=NULL;
                        }
                    }


}

void simplificareImpartire(nodArbore * & arbin)
{

    if(arbin->stanga->info[0]!='0' && arbin->dreapta->info[0]=='1') // f / 1 = f
        arbin=arbin->stanga;
    else
        if(arbin->stanga->info[0]=='0' && arbin->dreapta->info[0]!='0') // 0 / f = 0
        {
            arbin->info[0]='0';
            arbin->info[1]=NULL;
            arbin->stanga=NULL;
            arbin->dreapta=NULL;
        }

}

void simplificarePutere(nodArbore * & arbin)
{

    if(arbin->stanga->info[0]!='0' && arbin->dreapta->info[0]=='0') // f ^ 0 = 1
    {
        arbin->info[0]='1';
        arbin->info[1]=NULL;
        arbin->stanga=NULL;
        arbin->dreapta=NULL;
    }
    else
        if(arbin->stanga->info[0]=='0' && arbin->dreapta->info[0]!='0') // 0 ^ f = 0
        {
            arbin->info[0]='0';
            arbin->info[1]=NULL;
            arbin->stanga=NULL;
            arbin->dreapta=NULL;
        }
        else
            if(arbin->stanga->info[0]!='0' && arbin->dreapta->info[0]=='1') // f ^ 1 = f
                arbin=arbin->stanga;
            else
                if(arbin->stanga->info[0]=='1' && arbin->dreapta->info[0]!='0') // 1 ^ f = 1
                {
                    arbin->info[0]='1';
                    arbin->info[1]=NULL;
                    arbin->stanga=NULL;
                    arbin->dreapta=NULL;
                }
}


void Deriveaza(nodArbore * arbin, nodArbore *& arbout)
{
    char c;
    c=arbin->info[0];

    if(strchr("0123456789", c)!=0)
    {
        strcpy(arbout->info, "0");
    }

    if(c=='x')
    {
        strcpy(arbout->info, "1");
    }

    if(c=='+')
    {
        nodArbore * arb1 = new nodArbore;
        nodArbore * arb2 = new nodArbore;

        Deriveaza(arbin->stanga, arb1);
        Deriveaza(arbin->dreapta, arb2);

        arbout->stanga=arb1;
        arbout->dreapta=arb2;
        strcpy(arbout->info, "+");

        simplificareAdunare(arbout);


    }
    if(c=='-')
    {
        nodArbore * arb1 = new nodArbore;
        nodArbore * arb2 = new nodArbore;

        Deriveaza(arbin->stanga, arb1);
        Deriveaza(arbin->dreapta, arb2);

        arbout->stanga=arb1;
        arbout->dreapta=arb2;
        strcpy(arbout->info, "-");

        simplificareScadere(arbout);
    }

    if(c=='*')
    {
        nodArbore * arb1 = new nodArbore;
        nodArbore * arb2 = new nodArbore;
        nodArbore * arb3 = new nodArbore;
        nodArbore * arb4 = new nodArbore;

        Deriveaza(arbin->stanga, arb1);
        arb3->stanga=arb1;
        arb3->dreapta=arbin->dreapta;
        strcpy(arb3->info, "*");
        simplificareInmultire(arb3);

        Deriveaza(arbin->dreapta, arb2);
        arb4->stanga=arbin->stanga;
        arb4->dreapta=arb2;
        strcpy(arb4->info, "*");
        simplificareInmultire(arb4);

        arbout->stanga=arb3;
        arbout->dreapta=arb4;
        strcpy(arbout->info, "+");
        simplificareAdunare(arbout);
    }
    if(c=='/')
    {
        nodArbore * arb1 = new nodArbore;
        nodArbore * arb2 = new nodArbore;
        nodArbore * arb3 = new nodArbore;
        nodArbore * arb4 = new nodArbore;
        nodArbore * arb5 = new nodArbore;
        nodArbore * arb6 = new nodArbore;
        nodArbore * arb7 = new nodArbore;

        Deriveaza(arbin->stanga, arb1);
        arb3->stanga=arb1;
        arb3->dreapta=arbin->dreapta;
        strcpy(arb3->info, "*");
        simplificareInmultire(arb3);

        Deriveaza(arbin->dreapta, arb2);
        arb4->stanga=arbin->stanga;
        arb4->dreapta=arb2;
        strcpy(arb4->info, "*");
        simplificareInmultire(arb4);

        arb6->stanga=arb3;
        arb6->dreapta=arb4;
        strcpy(arb6->info, "-");
        simplificareScadere(arb6);

        strcpy(arb5->info, "2");
        arb5->stanga=NULL;
        arb5->dreapta=NULL;
        arb7->stanga=arbin->dreapta;
        arb7->dreapta=arb5;
        strcpy(arb7->info, "^");
        simplificarePutere(arb7);

        arbout->stanga=arb6;
        arbout->dreapta=arb7;
        strcpy(arbout->info, "/");
        simplificareImpartire(arbout);

    }
    if(c=='^')
    {
        nodArbore * arb1 = new nodArbore;
        nodArbore * arb2 = new nodArbore;
        nodArbore * arb3 = new nodArbore;
        nodArbore * arb4 = new nodArbore;

        strcpy(arb1->info, "1");

        arb2->stanga=arbin->dreapta;
        arb2->dreapta=arb1;
        strcpy(arb2->info, "-");
        simplificareScadere(arb2);

        arb3->stanga=arbin->stanga;
        arb3->dreapta=arb2;
        strcpy(arb3->info, "^");
        simplificarePutere(arb3);


        arbout->stanga=arbin->dreapta;
        arbout->dreapta=arb3;
        strcpy(arbout->info, "*");
        simplificareInmultire(arbout);


    }
    if(strcmp(arbin->info, "sin")==0)
    {
        nodArbore * arb1 = new nodArbore;
        nodArbore * arb2 = new nodArbore;

        arb1->stanga=NULL;
        arb1->dreapta=arbin->dreapta;
        strcpy(arb1->info, "cos");

        Deriveaza(arbin->dreapta, arb2);

        arbout->stanga=arb1;
        arbout->dreapta=arb2;
        strcpy(arbout->info, "*");
        simplificareInmultire(arbout);
    }
    if(c=='c')
    {
        nodArbore * arb1 = new nodArbore;
        nodArbore * arb2 = new nodArbore;
        nodArbore * arb3 = new nodArbore;

        arb1->stanga=NULL;
        arb1->dreapta=arbin->dreapta;
        strcpy(arb1->info, "sin");

        arb2->stanga=NULL;
        arb2->dreapta=arb1;
        strcpy(arb2->info, "-");

        Deriveaza(arbin->dreapta, arb3);

        arbout->stanga=arb2;
        arbout->dreapta=arb3;
        strcpy(arbout->info, "*");
        simplificareInmultire(arbout);
    }
    if(strcmp(arbin->info, "sqrt")==0)
    {
        nodArbore * arb1 = new nodArbore;
        nodArbore * arb2 = new nodArbore;
        nodArbore * arb3 = new nodArbore;
        nodArbore * arb4 = new nodArbore;
        nodArbore * arb5 = new nodArbore;
        nodArbore * arb6 = new nodArbore;

        strcpy(arb1->info, "2");

        arb2->stanga=NULL;
        arb2->dreapta=arbin->dreapta;
        strcpy(arb2->info, "sqrt");

        arb3->stanga=arb1;
        arb3->dreapta=arb2;
        strcpy(arb3->info, "*");

        strcpy(arb4->info, "1");

        arb5->stanga=arb4;
        arb5->dreapta=arb3;
        strcpy(arb5->info, "/");

        Deriveaza(arbin->dreapta, arb6);

        arbout->stanga=arb5;
        arbout->dreapta=arb6;
        strcpy(arbout->info, "*");
        simplificareInmultire(arbout);

    }
    if(c=='l')
    {
        nodArbore * arb1 = new nodArbore;
        nodArbore * arb2 = new nodArbore;
        nodArbore * arb3 = new nodArbore;

        strcpy(arb1->info, "1");

        arb2->stanga=arb1;
        arb2->dreapta=arbin->dreapta;
        strcpy(arb2->info, "/");

        Deriveaza(arbin->dreapta, arb3);

        arbout->stanga=arb2;
        arbout->dreapta=arb3;
        strcpy(arbout->info, "*");
        simplificareInmultire(arbout);
    }



}


//PASUL 5 : INTERFATA GRAFICA A PROGRAMULUI

void puneImagine(){
setbkcolor(BLACK);
readimagefile("math.jpg",0,0,1200,800);
}

void verifica(){
    setfillstyle(SOLID_FILL,BLACK);
    setcolor(CYAN);
    rectangle(150,625,300,675);
    floodfill(202,627,CYAN);
    setcolor(LIGHTGREEN);
    settextstyle(3,0, 3);
    outtextxy(155,635,"Verifica");
}

void arboreInitial(){
    setfillstyle(SOLID_FILL,BLACK);
    setcolor(CYAN);
    rectangle(400,625,550,675);
    floodfill(405,650,CYAN);
    setcolor(LIGHTGREEN);
    settextstyle(3,0, 3);
    outtextxy(405,635,"Arbore Initial");
}

void info(){
    setfillstyle(SOLID_FILL,BLACK);
    setcolor(CYAN);
    rectangle(150,700,550,750);
    floodfill(300,725,CYAN);
    setcolor(LIGHTGREEN);
    settextstyle(3,0, 3);
    outtextxy(325,715,"INFO");
}

void infotext(){
    outtextxy(155,300,"Programul calculeaza derivata formala de gradul I si de gradul II a unei expresii algebrice introduse.");
    outtextxy(155,325,"Programul foloseste operatorii: + , - , * , / , ^ , sin , cos, sqrt , ln");
    outtextxy(155,350,"Programul foloseste ca variabila x si poate lucra doar cu cifre pe post de constante,");
    outtextxy(155,375,"Proiectul a fost realizat de Matasaru Tavi-Cristian si Zmeu Ion.");
    outtextxy(155,400,"Profesor coordonator: Bogdan Patrut");

}

void derivataGrad2(){
    setfillstyle(SOLID_FILL,BLACK);
    setcolor(CYAN);
    rectangle(650,700,800,750);
    floodfill(700,725,CYAN);
    setcolor(LIGHTGREEN);
    settextstyle(3,0, 3);
    outtextxy(655,715,"Derivata II");
}

void arboreGrad2(){
    setfillstyle(SOLID_FILL,BLACK);
    setcolor(CYAN);
    rectangle(900,700,1050,750);
    floodfill(1000,725,CYAN);
    setcolor(LIGHTGREEN);
    settextstyle(3,0, 3);
    outtextxy(905,715,"Arbore II");
}

void butonDeriveaza(){
    setfillstyle(SOLID_FILL,BLACK);
    setcolor(CYAN);
    rectangle(650,625,800,675);
    floodfill(700,627,CYAN);
    setcolor(LIGHTGREEN);
    settextstyle(3,0, 3);
    outtextxy(655,635,"Derivata I");
}

void arboreDerivat(){
    setfillstyle(SOLID_FILL,BLACK);
    setcolor(CYAN);
    rectangle(900,625,1050,675);
    floodfill(950,627,CYAN);
    setcolor(LIGHTGREEN);
    settextstyle(3,0, 3);
    outtextxy(905,635,"Arbore I");
}

void introduceManual(){
    setfillstyle(SOLID_FILL,BLACK);
    setcolor(CYAN);
    rectangle(150,50,550,100);
    floodfill(200,75,CYAN);
    setcolor(LIGHTGREEN);
    settextstyle(3,0, 3);
    outtextxy(200,65,"Introdu Expresia De La Tastatura");
}

void introduceDinFisier(){

    setfillstyle(SOLID_FILL,BLACK);
    setcolor(CYAN);
    rectangle(650,50,1050,100);
    floodfill(700,75,CYAN);
    setcolor(LIGHTGREEN);
    settextstyle(3,0, 3);
    outtextxy(730,65,"Introdu Expresia Din Fisier");
}

void desenareInterfata(){

    puneImagine();

    setfillstyle(SOLID_FILL,BLACK);
    setcolor(CYAN);
    rectangle(150,150,1050,600);
    floodfill(151,151,CYAN);

    butonDeriveaza();
    arboreInitial();
    verifica();
    arboreDerivat();
    introduceManual();
    introduceDinFisier();
    info();
    derivataGrad2();
    arboreGrad2();

}

void verificaCod(){
   outtextxy(155,155,"am aperlat verifica cod");
   getch();
   cleardevice();
   desenareInterfata();

}

void CitesteSir(int x1, int y1, char mesaj[MAX_STR], char S[MAX_STR] ){
    char Enter = 13;
    char BackSpace = 8;
    char Escape = 27;
    char s2[2]; s2[1]='\0';
    char MultimeDeCaractereAcceptabile[MAX_STR]="0123456789abcdefghijklmnopqrstuvwxyz~!`@#$%+-^*&/\\()_=[],;.?<>:;{} ";
    char S_initial[MAX_STR];
    char tasta;
    char S_[MAX_STR];
    char mesaj_[MAX_STR];
    strcpy(mesaj_,mesaj);
    strcpy(S_initial,S);
    settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
    setcolor(WHITE);
    outtextxy(x1,y1,mesaj_);
    x1=x1+textwidth(mesaj);
    strcpy(S,"");
    strcpy(S_,S);
     strcat(S_,"_");
    setcolor(BLACK);
     outtextxy(x1,y1,S_);
    s2[0]=tasta;
     strcat(S,s2);
     strcpy(S_,S);
      strcat(S_,"_");
    setcolor(WHITE);
    outtextxy(x1,y1,S_);

    do {
        tasta=getch();

        if (tasta==0)
            {
                tasta=getch();
                Beep(1000,500);
            }
        else
            if (strchr(MultimeDeCaractereAcceptabile, tasta))
            {
                // stergem fostul sir
                strcpy(S_,S);
                strcat(S_,"_");
                setcolor(BLACK);
                outtextxy(x1,y1,S_);
                s2[0]=tasta;
                 strcat(S,s2);
                 strcpy(S_,S);
                 strcat(S_,"_");
                setcolor(WHITE);
                outtextxy(x1,y1,S_);
            }
            if (tasta==BackSpace)
                    if (!(strcmp(S,""))) Beep(500,100);
                    else
                    {
                        setcolor(BLACK);
                        outtextxy(x1,y1,S_);
                        setcolor(WHITE);
                        S[strlen(S)-1]='\0';
                        strcpy(S_,S);
                        strcat(S_,"_") ;
                        outtextxy(x1,y1,S_);
                        Beep(200,20);
                    }
            else
                if (tasta!=Enter && tasta!=Escape)
                {
                    Beep(200,20);
                }
    } while (tasta!=Enter && tasta!=Escape);
    if (tasta == Escape)
        strcpy(S,S_initial);

    setcolor(BLACK);
    outtextxy(x1,y1,S_);
    setcolor(WHITE);
     outtextxy(x1,y1,S);

}

void grafica()
{
    nodArbore * varf = new nodArbore;
    nodArbore * arboreDerivat = new nodArbore;
    nodArbore * arboreDerivat2 = new nodArbore;

    char  s[101];
    int x,y, window;
    initwindow(1200, 800);
    window =  getcurrentwindow();
    desenareInterfata();
 while(true)
 {
     if(ismouseclick(WM_LBUTTONDOWN))
    {
        x=mousex();
        y=mousey();
        clearmouseclick(WM_LBUTTONDOWN);
        if(x>150 && x<550 && y>50 && y<100)
        { //buton Introdu manual expresia
            clearviewport();
            desenareInterfata();
            char mesaj[256];
            mesaj[0]='f';
            mesaj[1]='(';
            mesaj[2]='x';
            mesaj[3]=')';
            mesaj[4]='=';
            mesaj[5]=NULL;

        CitesteSir(160,175,mesaj,s);
        }

        if(x>150 && x<300 && y>625 && y<675)
        { //buton verifica
            verificareExpresie(s);

        }

        if(x>650 && x<1050 && y>50 && y<100)
        {
            clearviewport();
            desenareInterfata();
            ifstream fin("expresie.txt");
            fin>>s;
            outtextxy(170, 180, "Expresia a fost citita din fisier cu succes!");
            outtextxy(170, 210, s);
        }

         if(x>400 && x<550 && y>625 && y<675)
        {  // buton arbore initial
            int k=1;
            if(k==1)
            {
                vect v1;
                vect v2;
                vectorizeaza(s, v1);
                formaPostFixata(v1, v2);
                i=v2.nr;
                creareArbore(v2, varf);
                desenareArbore(varf);
            }
            else
                desenareArbore(varf);
        }
         if(x>650 && x<800 && y>625 && y<675)
        {   //buton derivare

            vect v1;
            vect v2;
            vectorizeaza(s, v1);
            formaPostFixata(v1, v2);
            i=v2.nr;
            creareArbore(v2, varf);
            Deriveaza(varf, arboreDerivat);
            outtextxy(170, 350, "Derivata de gradul I este:");
            char t[101]={NULL};
            creareSir(arboreDerivat, t);
            outtextxy(170, 380, t);
        }
        if(x>900 && x<1050 && y>625 && y<675)
        {   //buton arbore derivat
            int k=1;

            if (k==1)
            {
                vect v1;
                vect v2;
                nodArbore * varf = new nodArbore;
                vectorizeaza(s, v1);
                formaPostFixata(v1, v2);
                i=v2.nr;
                creareArbore(v2, varf);
                Deriveaza(varf, arboreDerivat);
                desenareArbore(arboreDerivat);
            }
            else
                desenareArbore(arboreDerivat);


        }
         if(x>150 && x<300 && y>625 && y<675)
        {

        }
         if(x>150 && x<550 && y>700 && y<750)
        { //buton info
            clearviewport();
            desenareInterfata();
            infotext();
        }
        if(x>900 && x<1050 && y>700 && y<750)
        {
            desenareArbore(arboreDerivat2);
        }
        if(x>650 && x<800 && y>700 && y<750)
        {
            //buton derivataGrad2
            Deriveaza(arboreDerivat, arboreDerivat2);
            outtextxy(170, 430, "Derivata de gradul II este:");
            char t[101]={NULL};
            creareSir(arboreDerivat2, t);
            outtextxy(170, 470, t);
        }
    }
    Sleep(100);
 }
}


int main()
{
    grafica();

    return 0;
}
