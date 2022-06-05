#include <stdlib.h>			
#include <stdio.h>			
#include <cmath>
#include <iostream>

#include <cstdlib>
#include <math.h>			

#  include <GL/glut.h>

#define PI 3.14159265

GLint windowHeight, windowWidth;
#define GL_SILENCE_DEPRECATION

using namespace std;





typedef struct { 
    int isAlive;
	float X;
	float Y;
	float VelX; 
	float VelY;  
} alien;
typedef struct { 
    int isActive;
	int level;
	float X;
	float Y; 
	int time=0;
} bomb;

/// variables
alien* levels[5];
alien level0[5];
alien level1[5];
alien level2[5];
alien level3[5];
alien level4[5];

bomb bombs[50];
int bombIndex=0;
float colors[5]={0.6,0.5,0.40,0.6,0.4};
bool isPaused=false;
int score=0; 

static GLsizei width, height; // OpenGL window size.
int getRandomInt(int limit){
	return rand()%limit;
}
float getRandomFloat(int max){ /// random generates for max 100 return 0-1.00

	return ( (float)( getRandomInt(max)))/100;
}
bool checkGameEnd(){ /// check whether alien is dead or not
	for(int k=3;k<5;k++){
		for(int i=0;i<4;i++){
			if(levels[k][i].isAlive==0)
				return false;
		}
	}
	return true;
}

void hitControl(int bombIndex,int levelindex,int alienindex){ /// checks whether bombs has collide with alien at certain index
	int bombcenterX=(int)((bombs[bombIndex].X+0.02)*100);
	int bombcenterY=(int)((bombs[bombIndex].Y+0.02)*100);
	int aliencenterX=(int)((levels[levelindex][alienindex].X+0.05)*100);
	int aliencenterY=(int)((levels[levelindex][alienindex].Y+0.05)*100);
	float distance=sqrt(pow((bombcenterX-aliencenterX),2) + pow((bombcenterY-aliencenterY),2));
	
	if(  distance < 7  ) {
		
		levels[levelindex][alienindex].isAlive=1; /// set aliend sate to dying
		if(levelindex == 0) score-=4000;
		else if (levelindex == 1) score-=2000;
		else if (levelindex == 2) score-=1000;
		else if (levelindex == 3) score+=1000;
		else if (levelindex == 4) score+=2000;
	
	}
}

void moveAliens(bool isSingleStep = false){ /// moves aliens
	if(!isPaused || isSingleStep){
		for(int k=0;k<5;k++){ /// levels
			for(int i=0;i<4;i++){ ////  aliens in the level k
				if(levels[k][i].isAlive==0){
					levels[k][i].X=levels[k][i].X+levels[k][i].VelX/10;
					levels[k][i].Y=levels[k][i].Y+levels[k][i].VelY/10;
					/// BURADA UZAYLILAR SINIRDAN CIKMASIN ESNEK CARPISMA YAPMASI ICIN KONTROL YAPILIYOR
					if(levels[k][i].X<0.0){
						levels[k][i].VelX=-1*levels[k][i].VelX;
						levels[k][i].X=0.0;
					}
					else if(levels[k][i].X>0.90){
						levels[k][i].VelX=-1*levels[k][i].VelX;
						levels[k][i].X=0.90;
					}

					if(levels[k][i].Y<0.0){
						levels[k][i].VelY=-1*levels[k][i].VelY;
						levels[k][i].Y=0.0;
					}
					else if(levels[k][i].Y>0.90){
						levels[k][i].VelY=-1*levels[k][i].VelY;
						levels[k][i].Y=0.90;
					}
					for(int bombIndex=0;bombIndex<50;bombIndex++){ /// checks whether bomb hits alien at index i on level k
						if(bombs[bombIndex].isActive && bombs[bombIndex].level==k ){
							hitControl(bombIndex,k,i);
						}
					}
				}
				else if (levels[k][i].isAlive==1){
					levels[k][i].isAlive=2;
				}
				else{

				}
			}
		}
	}
	else{
	}
}
void moveBombs(bool isSingleStep = false){ /// move bombs
	if(!isPaused || isSingleStep){
		for(int i=0;i<50;i++){ /// BOMBANIN TIMe DEGERI 10 OLUNCA 1 ALT LEVELE INIYOR
				if(bombs[i].isActive){
					if(bombs[i].time==9){
						bombs[i].time=0;
						bombs[i].level+=1;
						if(bombs[i].level==5){
							bombs[i].isActive=false;
						}
					}
					else{
						bombs[i].time++;
					}
				}
			}
	}
	else{

	}

	
}

void initAliens(){ //// initilizaion
	for(int k=0;k<5;k++){ /// levels
		for(int i=0;i<4;i++){
			levels[k][i].X=getRandomFloat(90);
			levels[k][i].Y=getRandomFloat(90);;
			levels[k][i].VelX=getRandomFloat(50)-0.25; // return between -0.25-0.25
			levels[k][i].VelY=getRandomFloat(50)-0.25;

		}
	}
}

void Debug(){ //// single step mode
	for(int k=0;k<5;k++){
		cout<< "level: " << k <<"   ";
		for(int i=0;i<4;i++){
			cout<< "alien:" << i<<" X:"  << (int)(levels[k][i].X*windowWidth)<<" Y:" <<  (int)(levels[k][i].Y*windowHeight)<< " ";
			if(levels[k][i].isAlive==0) cout<< " alive--";
			else   cout<< " dead--";
		}
		cout<<  endl;
	}
	for(int k=0;k<50;k++){
		if(bombs[k].isActive){
			cout<< "bomb:" << k<<" at level "  << bombs[k].level <<endl;
		}
	}
		
}

void myInit()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);		// background color
    glShadeModel(GL_SMOOTH);		
	srand(time(NULL));  //// randomly locate aliens 
	levels[0]=level0; levels[1]=level1; levels[2]=level2; levels[3]=level3; levels[4]=level4;
	initAliens();
	
}

void myReshape(int winWidth, int winHeight) 
{
	windowHeight = winHeight-20; //// -20 value is given to locate score 
	windowWidth = winWidth;
	glViewport (0, 0, winWidth,  winHeight-20);  
	glMatrixMode(GL_PROJECTION);              
	glLoadIdentity();               
	gluOrtho2D(0.0, 1.0, 0.0, 1.0);           
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();                      
}

void myDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);       // clear the window

	////// DRAW ALIENS
	for(int k=4;k>-1;k--){ /// levels
		for(int i=0;i<4;i++){ /// aliend
			// cout << level0[i].X << endl;
			float angle;
			if(levels[k][i].isAlive==0){
				(k<3?glColor3f(1.0, 1.0, colors[k]):glColor3f(1.0,  colors[k],1.0));
				//  glColor3f(0.0, 0.0, colors[k]);   
				glBegin(GL_TRIANGLE_FAN);
					for(int j= 0; j < 50; ++j) /// circle
						{
							angle = 2*PI*j/50;
							glVertex2f(levels[k][i].X+0.05 + cos(angle) * 0.05, levels[k][i].Y +0.05+ sin(angle) * 0.05);
						}
				glEnd();
			}
			else if (levels[k][i].isAlive==1){
				glBegin(GL_TRIANGLE_FAN);
				glColor3f(0.0, 0.0, 0.0);   
				for(int j= 0; j < 50; ++j) /// circle
					{
						angle = 2*PI*j/50;
						glVertex2f(levels[k][i].X+0.05 + cos(angle) * 0.05, levels[k][i].Y +0.05+ sin(angle) * 0.05);
					}
				glEnd();
			}
			else{

			}
			
		}
	}
	// DRAW BOMBS
	for(int i=0;i<50;i++){
		if(bombs[i].isActive){
		// glColor3f(colors[bombs[i].level], 0.0,0.0 );    
		(bombs[i].level<3?glColor3f(1.0, 1.0, colors[bombs[i].level]):glColor3f(1.0,  colors[bombs[i].level],1.0));
		glBegin(GL_POLYGON);
		    glVertex2f(bombs[i].X , bombs[i].Y);
			glVertex2f(bombs[i].X +0.04 , bombs[i].Y);
			glVertex2f(bombs[i].X +0.04 , bombs[i].Y+0.04);
			glVertex2f(bombs[i].X  , bombs[i].Y+0.04);
	    glEnd();
		}
	}
	char *c;
	char const *s =("score "+to_string(score)).c_str();
	glColor3f (0.0, 0.0, 0.0);
	glRasterPos2f(0.0,1);
	
	/// DRAW SCORE
	for (c=(char*)s; *c != '\0'; c++) {
		
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
    glFlush();				
    glutSwapBuffers();			
}

void myKeyboard(unsigned char c, int x, int y)
{
    switch (c)
    {
		case 'q':
			cout << "score: " << score <<endl;
			exit(0);	
			break;
		case 'p':
			isPaused=!isPaused;
			break;
		case 's':
			if(isPaused){
				moveAliens(true);
				moveBombs(true);
				Debug();
				glutPostRedisplay();
				break;
			}	
			else{
				isPaused=!isPaused;
				break;
			}					
	}
}


void myMouse(int b, int s, int x, int y) {
	switch  ( b ) {    // b indicates the button
		case GLUT_LEFT_BUTTON:
			if (s == GLUT_DOWN){
				break;
			}     
				
			else if (s == GLUT_UP){
				bombs[bombIndex].isActive=true; /// createBombs
				bombs[bombIndex].level=0;
				bombs[bombIndex].time=0;
				bombs[bombIndex].X=(((float)x/(float)windowWidth))-0.02;
				bombs[bombIndex].Y=(((float)(20+windowHeight-y)/(float)windowHeight))-0.02;
				bombIndex=(bombIndex+1)%50;

			break; 			//... other button events may follow

			}  
			
	}
}

void myTimeOut(int id) {
	// cout << "saniye"<< std::flush;
	moveAliens();
	moveBombs();
	if(checkGameEnd()){ //// checks whether game is ended{}
		cout << "score: " << score <<endl;
		exit(0);
	}
	glutPostRedisplay();			 
	glutTimerFunc(100, myTimeOut, 0);  
}


int main(int argc, char **argv)
{
    				
    glutInit(&argc, argv);  
    glutInitDisplayMode(GLUT_DOUBLE |	GLUT_RGB );		    

    glutInitWindowSize(800, 820);   
    glutInitWindowPosition(0,0);  
    glutCreateWindow("hw1"); 	

	///register callbacks
    glutDisplayFunc(myDisplay);		
    glutKeyboardFunc(myKeyboard);   
    glutReshapeFunc(myReshape);     
    glutMouseFunc(myMouse);        
    glutTimerFunc(1000, myTimeOut, 0);

    myInit();				

    glutMainLoop();			
    return 0;            
}