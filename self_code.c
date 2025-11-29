#include<stdio.h>
#include<string.h>
#include<stdlib.h>


 //STRUCTURE DEFINATION


typedef struct {
    char name[50];
    double watt;
    double hrs;
    int on;

}thing;


// LOADING CSV 

thing* loading(const char *file, size_t *n){
    FILE *f=fopen(file,"r");
    if(!f) return NULL;
    thing t,*array = NULL;
    *n=0;

    while (fscanf(f,"%49[^,],%lf,%lf,%d\n",t.name,&t.watt,&t.hrs,&t.on)==4){
        array =realloc(array,(*n+1)*sizeof(thing));
        array[*n]=t;
        (*n)++;
    }
    fclose(f);
    return array;
}

//SAVE CSV

void store(const char *file,thing *array,size_t n){
    FILE *f=fopen(file,"w");
    if(!f) return;
    for(size_t i=0;i<n;i++)
    fprintf(f,"%s,%.2f,%.2f,%d\n", array[i].name, array[i].watt, array[i].hrs, array[i].on);
    fclose(f);
}

//KWH

double kilowatthrs(thing t){
    return t.on ? (t.watt * t.hrs)/1000.0 : 0.0;
}

//ADD DEVICE 
thing* adding(thing *array, size_t *n){
    thing t;
    printf("NAME: ");
    scanf(" %[^\n]",t.name);
    printf("WATT: ");
    scanf("%lf",&t.watt);
    printf("HOURS: ");
    scanf("%lf",&t.hrs);
    t.on = 1;

    array = realloc(array, (*n+1)*sizeof(thing));
    array[*n] = t;
    (*n)++;
    return array;
}

//REMOVE DEVICE 

thing* remov(thing *array,size_t *n,size_t i){
    if(i>= *n)
        return array;
    for(size_t j = i;j+1< *n;j++)
        array[j]= array[j+1];
    (*n)--;
    return realloc(array,*n * sizeof(thing));
}

//TOGGLE ON/OFF--

void toggle(thing *array,size_t i){
    array[i].on = !array[i].on;
}

void displayreport(thing *array,size_t n, double rate){
    double total = 0;


    printf("\n==== ENERGY REPORT ====\n");
    printf("%-6s %-8s %-8s %-8s %-8s %-10s\n",
    "IDX","NAME","WATT","HRS/DAY","STATUS","DAILY_KWH");


    for(size_t i=0;i<n;i++){
        double t = kilowatthrs(array[i]);
        total = total+t;

        printf("%-6zu %-8s %-8.1f %-8.1f %-8s %-10.3f\n",
        i,
        array[i].name,
        array[i].watt,
        array[i].hrs,
        array[i].on ? "ON" :"OFF",
        t);
    
        }

    printf("\nTotal Daily kWh: %.3f kWh\n",total);
    printf("Daily Cost: %.2f\n",total*rate);
    printf("Weekly Cost: %.2f\n",total*rate*7);
    printf("Monthly Cost: %.2f\n",total*rate*30);
}


// MAIN PROGRAM


int main() {
    size_t n =0;
    double rate = 0.12;
    thing *array = loading("data/appliances.csv",&n);

    int target;
    while(1){
        printf("\n1) REPORT\n2) TOGGLE\n3) ADD\n4) REMOVE\n5) TARIFF\n6) SAVE&EXIT\n> ");
    scanf("%d",&target);


    if(target==1){
        displayreport(array ,n,  rate);
    }
    else if(target==2){
        size_t i;
        printf("Index: ");
        scanf("%zu",&i);
        toggle(array,i);
    }
    else if(target==3){
        array = adding(array,&n);
    }
    else if(target==4){
        size_t i;
        printf("Index: ");
        scanf("%zu",&i);
        array = remov(array,&n,i);
    }
    else if(target ==5){
        printf("New Tariff: ");
        scanf("%lf",&rate);
    }
    else if(target=6){
        store("data/appliances.csv",array,n);
        printf("Saved.\n");
        break;
    }
    else{
        printf("Invalid!\n");
    }
}
    free(array);
    return 0;
}