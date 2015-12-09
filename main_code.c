/* This is the implementation of the Paper titled "Private Distributed Collaborative Filtering Using Estimated Concordance Measures" 
written by Neal Lathia, Stephen Hailes & Licia Capra

The project was done as a part of our Web Intelligence Course Project. We have implemented the algorithm proposed in the above titled paper in C Language. The other two project partners with me were Ruchi and Raghav. 

<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<      This project has been coded by geekyprincess(aka Ashima)     >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<      Other Project partners are Ruchi and Raghav Raina    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

To know more about me, visit my linkedIn profile:       https://in.linkedin.com/in/goelashima
 
*/


#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<assert.h>

int user_with_rnd[2][3]; //to store the C, D and T pairs for user n random user
float actual_value; //user similarity directly
float predict_value; //predicted by lathia

int randint(int n) {
  if ((n - 1) == RAND_MAX) {
    return rand();
  } else {
    // Chop off all of the values that would cause skew...
    long end = RAND_MAX / n; // truncate skew
    assert (end > 0L);
    end *= n;
    int r;
    while ((r = rand()) >= end);

    return r % n;
  }
}

//calculate average rating of a particular user
float cal_average(int A[], int N){
    float avg = 0.0;
    int i;
    for(i=0; i<N; i++){
        avg = avg + A[i];
    }
    avg = avg/(float)N;
   avg = roundf(avg * 100) / 100;
    return avg;
}


//calculate the concordant, discordant and tied pairs among two users with N ratings
void cal_pairs(int user_id,  int *A, int *R, int N){
   int i,num;

    float avgA =0.0, avgR =0.0;
    avgA = cal_average(A, N);
     avgR = cal_average(R,N);
    user_with_rnd[user_id][0] = 0;
     user_with_rnd[user_id][1] = 0;
      user_with_rnd[user_id][2] = 0;

    for(i=0;i<N;i++){
       float a = ((float)A[i]-avgA);
        float b = ((float)R[i]-avgR);
        if( (A[i]==0) || (R[i]==0)||(a == 0.0) ||(b == 0.0)) //tied
            (user_with_rnd[user_id][2])=(user_with_rnd[user_id][2])+1;
        else if((a >0.0 && (b >0.0))||(a <0.0 && (b<0.0) )) //concordant
            (user_with_rnd[user_id][0])= (user_with_rnd[user_id][0])+1;
         else
            (user_with_rnd[user_id][1])=(user_with_rnd[user_id][1])+1;
    }
}

//return max of two numbers
int max(int a, int b){
    if(a>b)
        return a;
    else
        return b;
}

//sum of two numbers
int sum(int a, int b){
    return (a+b);
}

//to calculate the max bound
int maxOverlap(int a, int b){
    if(a<b)
        return a;
    else
        return b;
}

//calculate the min bound
int minOverlap(int a, int b, int N){
    int x = (maxOverlap(a, b)- (N-max(a, b)));
   // x=x/10;

    return x;
}

//somers function
void somers(int *A, int *B, int N ){
    float ans;
   int i,num;
    float avgA =0.0, avgB =0.0;
    avgA = cal_average(A, N);
    avgB = cal_average(B,N);
    float c=0, d=0, t=0;

    for(i=0;i<N;i++){
        float a = ((float)A[i]-avgA);
        float b = ((float)B[i]-avgB);
        a = roundf(a * 100) / 100;
        b = roundf(b * 100) / 100;
        if( (A[i]==0) || (B[i]==0)||(a == 0.0) ||(b == 0.0)) //tied
            t++;
        else if((a >0.0 && (b >0.0))||(a <0.0 && (b<0.0) )) //concordant
            c++;
         else
            d++;
    }
   ans = ((c-d)/(N-t));
   actual_value = ans;
}

void predict(int *A, int *B, int N ){
   float ans;
   int i;
    float t, d, c;
   float tmin, tmax, cmin, cmax, dmin,dmax;

   tmin = (float)max(user_with_rnd[0][2], user_with_rnd[1][2]);
   tmax = (float)sum(user_with_rnd[1][2], user_with_rnd[0][2]);
   cmin = (float)(minOverlap(user_with_rnd[1][0], user_with_rnd[0][0], N) + maxOverlap(user_with_rnd[0][1], user_with_rnd[1][1]));
   cmax = (float)maxOverlap(user_with_rnd[0][0], user_with_rnd[1][0]);
   cmax = roundf(cmax * 100) / 100;
   cmin = roundf(cmin * 100) / 100;
   tmax = roundf(tmax * 100) / 100;
   tmin = roundf(tmin * 100) / 100;

   dmin = (float)((float)N-(cmax+tmax));
   dmax= (float)((float)N-(cmin+tmin));
   dmax = roundf(dmax * 100) / 100;
   dmin = roundf(dmin * 100) / 100;
   t = (tmin+tmax)/2;
   d = (dmax+dmin)/2;
   c = (cmin+cmax)/2;
   ans = ((c-d)/(N-t));
    ans = roundf(ans * 100) / 100;
   predict_value = ans;

}

int main(){
    int i,j, step;
    int N;
    int count = 0;
    float mae[19000];
    for(N=1000;N<20000;N++)
    {
        mae[count] = 0.0;
        float error[100];
        for(step = 0; step<100; step++){
            int user_rating[3][N];
            for(i=0;i<3;i++){
                for(j=0;j<N;j++){
                    user_rating[i][j]=0;
                }
            }

            enum { other=20, neg=1 };
            int hours[2] = {other, neg};

            srand((unsigned)time(NULL));
            int h;

            for(i=0;i<3;i++){
                for(j=0;j<N;j++){

                h = rand() % 2;
                switch(hours[h])
                {
                    case other:
                         user_rating[i][j] = randint(6);
                        break;
                    case neg:
                         user_rating[i][j] = -1;
                }
            }
            }

            for(i=0;i<2;i++){
                cal_pairs(i, &user_rating[i], &user_rating[2], N);
            }
            somers(&user_rating[0], &user_rating[1], N);
            predict(&user_rating[0], &user_rating[1], N);
            error[step] = predict_value - actual_value;
        }
        for(step =0; step<100; step++)
        {
            mae[count] = mae[count]+ error[step];
        }
        mae[count] = mae[count]/100;
        mae[count] = roundf(mae[count] * 100) / 100;
        mae[count] = fabs(mae[count]);
        printf("%f \n", mae[count]);
    }

}
