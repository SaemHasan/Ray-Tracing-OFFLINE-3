#include "1705027_Header.h"


double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
Point pos, u, r, l;

//vectors

vector<Object> objects;
vector<PointLight> lights;


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
}




void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			//drawgrid=1-drawgrid;
			r = Point::rotateVector(r, u, deg2rad(3.0));
			l = Point::rotateVector(l, u, deg2rad(3.0));
			
			break;
		case '2':
			r = Point::rotateVector(r, u, deg2rad(-3.0));
			l = Point::rotateVector(l, u, deg2rad(-3.0));
			
			break;
		case '3':
			l = Point::rotateVector(l, r, deg2rad(-3.0));
			u = Point::rotateVector(u, r, deg2rad(-3.0));
			 
			break;
		case '4':
			l = Point::rotateVector(l, r, deg2rad(3.0));
			u = Point::rotateVector(u, r, deg2rad(3.0));
			 
			break;
		case '5':
			r = Point::rotateVector(r, l, deg2rad(3.0));
			u = Point::rotateVector(u, l, deg2rad(3.0));
			
			break;
		case '6':
			r = Point::rotateVector(r, l, deg2rad(-3.0));
			u = Point::rotateVector(u, l, deg2rad(-3.0));
			
			break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			pos = pos - (l * 3.0);
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			pos = pos + (l * 3.0);
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			pos = pos - (r * 3.0);
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			pos = pos + (r * 3.0);
			break;

		case GLUT_KEY_PAGE_UP:
			pos = pos + (u * 3.0);
			break;
		case GLUT_KEY_PAGE_DOWN:
			pos = pos - (u * 3.0);
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			
			break;
		case GLUT_KEY_END:
			
            break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}


void loadData(){
	ifstream inputFile("scene_test.txt");
	inputFile >> levelsOfRecursion >> pixelsAlongBothAxis;
	inputFile >> numberOfObjects;

	for(int i=0;i<numberOfObjects;i++){
		string objectType;
		inputFile >> objectType;
		cout<< "Object Type: " << objectType << endl;
		if(objectType == "sphere"){
			Sphere s;
			inputFile >> s;
			// cout<<s;
			objects.push_back(s);
		}
		else if(objectType == "triangle"){
			Triangle t;
			inputFile >> t;
			// cout<<t;
			objects.push_back(t);
		}
		else if(objectType == "general"){
			General g;
			inputFile >> g;
			// cout<<g;
			objects.push_back(g);
		}
		else{
			cout<<"Invalid Object Type : "<< objectType<<endl;
		}
	}
	
	inputFile >> numberOfPointLights;
	cout<<"number of point lights :"<<numberOfPointLights<<endl;
	for(int i=0;i<numberOfPointLights;i++){
		PointLight pl;
		inputFile >> pl;
		// cout<<pl;
		lights.push_back(pl);
	}

	inputFile >> numberOfSpotLights;
	for(int i=0;i<numberOfSpotLights;i++){
		SpotLight sl;
		inputFile >> sl;
		// cout<<sl;
		lights.push_back(sl);
	}
}

void showData(){
	cout << "Levels of Recursion: " << levelsOfRecursion << endl;
	cout << "Pixels along both axis: " << pixelsAlongBothAxis << endl;
	cout << "Number of Objects: " << numberOfObjects << endl;

	for(int i=0;i<numberOfObjects;i++){
		cout << "Object " << i+1 << ": " << endl;
		cout << objects[i] << endl;
	}

	cout << "Number of Point Lights: " << numberOfPointLights << endl;
	for(int i=0;i<numberOfPointLights;i++){
		cout << "Point Light " << i+1 << ": " << endl;
		cout << lights[i] << endl;
	}

	cout << "Number of Spot Lights: " << numberOfSpotLights << endl;
	for(int i=numberOfPointLights;i<numberOfPointLights+numberOfSpotLights;i++){
		cout << "Spot Light " << i+1 << ": " << endl;
		cout << lights[i] << endl;
	}
}


void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?
	gluLookAt(pos.x,pos.y,pos.z, pos.x+l.x,pos.y+l.y,pos.z+l.z, u.x,u.y,u.z);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	pos = Point(0,0,-190);
	u = Point(0,1,0);
	r = Point(1,0,0);
	l = Point(0,0,1);

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	srand(time(NULL));
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Ray Tracing");

	init();
	loadData();
	// showData();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}