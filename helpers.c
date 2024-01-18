#include "helpers.h"
#include<math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int average;
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            average=round((image[i][j].rgbtBlue+image[i][j].rgbtGreen+image[i][j].rgbtRed)/3.0);
            image[i][j].rgbtBlue=average;
            image[i][j].rgbtGreen=average;
            image[i][j].rgbtRed=average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int z;
    RGBTRIPLE temp[1][1];
    for (int i=0; i<height;i++)
    {
        z=width-1;
        for (int j=0;j<=(width/2)-1;j++)
        {
            temp[0][0]=image[i][z];
            image[i][z]=image[i][j];
            image[i][j]=temp[0][0];
            z--;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            copy[i][j].rgbtBlue=image[i][j].rgbtBlue;
            copy[i][j].rgbtGreen=image[i][j].rgbtGreen;
            copy[i][j].rgbtRed=image[i][j].rgbtRed;
        }
    }
    float count=0;
    int red=0;
    int green=0;
    int blue=0;
    int avg_red, avg_blue, avg_green;
    for(int row=0;row<height;row++)
    {
        for(int col=0;col<width;col++)
        {
            //neighbouring pixels//
            for(int x=row-1;x<=row+1;x++)
            {
                for(int y=col-1;y<=col+1;y++)
                {
                    if(x>=0 && x<height && y>=0 && y<width)
                    {
                        red=red+copy[x][y].rgbtRed;
                        green=green+copy[x][y].rgbtGreen;
                        blue=blue+copy[x][y].rgbtBlue;
                        count++;
                    }
                }
            }
            avg_red=round(red/count);
            avg_green=round(green/count);
            avg_blue=round(blue/count);
            image[row][col].rgbtRed=avg_red;
            image[row][col].rgbtGreen=avg_green;
            image[row][col].rgbtBlue=avg_blue;
            count=0;
            red=0;
            green=0;
            blue=0;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            copy[i][j].rgbtBlue=image[i][j].rgbtBlue;
            copy[i][j].rgbtGreen=image[i][j].rgbtGreen;
            copy[i][j].rgbtRed=image[i][j].rgbtRed;
        }
    }
    int values_x[3][3] = {{-1, 0, 1},{-2, 0, 2},{-1, 0, 1}};
    int values_y[3][3] = {{-1, -2, -1},{0,0,0},{1,2,1}};

    //pixel beyond boundary we should treat as 0 - black//
    int p=0;
    int q=0;
    int Gx_r=0;
    int Gx_g=0;
    int Gx_b=0;
    int Gy_r=0;
    int Gy_g=0;
    int Gy_b=0;
    int new_val_r;
    int new_val_g;
    int new_val_b;
    for(int row=0;row<height;row++)
    {
        for(int col=0; col<width; col++)
        {
            //neighbouring pixels//
            for(int x=row-1;x<=row+1;x++)
            {
                q=0;
                for(int y=col-1;y<=col+1;y++)
                {
                    if(x>=0 && x<height && y>=0 && y<width)
                    {
                        Gx_r+=(values_x[p][q])*(copy[x][y].rgbtRed);
                        Gx_g+=(values_x[p][q])*(copy[x][y].rgbtGreen);
                        Gx_b+=(values_x[p][q])*(copy[x][y].rgbtBlue);
                        Gy_r+=(values_y[p][q])*(copy[x][y].rgbtRed);
                        Gy_g+=(values_y[p][q])*(copy[x][y].rgbtGreen);
                        Gy_b+=(values_y[p][q])*(copy[x][y].rgbtBlue);
                        
                    }
                    else
                    {
                        Gx_r+=values_x[p][q]*0;
                        Gx_g+=values_x[p][q]*0;
                        Gx_b+=values_x[p][q]*0;
                        Gy_r+=values_y[p][q]*0;
                        Gy_g+=values_y[p][q]*0;
                        Gy_b+=values_y[p][q]*0;
                        
                    }
                    q++;
                }
                p++;
            }
            p=0;
            new_val_r=round(sqrt((Gx_r*Gx_r)+(Gy_r*Gy_r)));
            if(new_val_r>=0 && new_val_r<=255)
            {
                image[row][col].rgbtRed=new_val_r;
            }
            else{
                image[row][col].rgbtRed=255;
            }

            new_val_g=round(sqrt((Gx_g*Gx_g)+(Gy_g*Gy_g)));
            if(new_val_g>=0 && new_val_g<=255)
            {
                image[row][col].rgbtGreen=new_val_g;
            }
            else{
                image[row][col].rgbtGreen=255;
            }
        
            new_val_b=round(sqrt((Gx_b*Gx_b)+(Gy_b*Gy_b)));
            if(new_val_b>=0 && new_val_b<=255)
            {
                image[row][col].rgbtBlue=new_val_b;
            }
            else{
                image[row][col].rgbtBlue=255;
            }
            Gx_r=0;
            Gx_g=0;
            Gx_b=0;
            Gy_r=0;
            Gy_g=0;
            Gy_b=0;
        }
    }
    return;
}
