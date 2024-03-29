#include "1705027_Floor.h"
#include "bitmap_image.hpp"
#include <sstream>

// input file name
string inputFileName = "scene.txt";
// create a folder to store the output images
// name of the folder ==> Output

//window variables
int windowWidth = 500;
int windowHeight = 500;
double fovY = 80.0;
double moveAmount = 5.0;

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
		}
		glEnd();
	}
}



void capture(){
	// initialize bitmap image
	cout <<"capturing image\n";
	bitmap_image image(pixelsAlongBothAxis,pixelsAlongBothAxis);
	
	for(int i=0;i<pixelsAlongBothAxis;i++){
		for(int j=0;j<pixelsAlongBothAxis;j++){
			image.set_pixel(i,j, 0, 0, 0);
		}
	}

	// plane distance
	double planeDistance = (windowHeight/2.0) / tan(deg2rad(fovY)/2.0);
	// cout << "planeDistance : "<<planeDistance<<endl;
	// top left
	Point topLeft = pos + l * planeDistance - r * (windowWidth/2.0) + u * (windowHeight/2.0);
	// cout << "topLeft : "<<topLeft<<endl;
	// du, dv
	double du = windowWidth / (pixelsAlongBothAxis * 1.0);
	double dv = windowHeight / (pixelsAlongBothAxis * 1.0);

	// choose middle of the grid cell
	topLeft = topLeft + r * (du*0.5) - u * (dv*0.5);
	// cout << "topLeft : "<<topLeft<<endl;

	for (int i=0;i<pixelsAlongBothAxis;i++){

		for(int j=0;j<pixelsAlongBothAxis;j++){

			Point curPixel = topLeft + r * (i*du) - u * (j*dv);
			Ray ray(pos, curPixel - pos);
			Color color = Color(0,0,0);
			int nearest = -1;
			double t, tMin = INF;
			for(int k=0;k<objects.size();k++){
				t = objects[k]->intersect(ray, color, 0);
				if(t>0.0 && t<tMin){
					tMin = t;
					nearest = k;
				}
			}
			if(nearest != -1){
				Color color;
				tMin = objects[nearest]->intersect(ray, color, 1);
				// cout << color;
				image.set_pixel(i,j,(int) round(color.r * 255.0),(int) round(color.g * 255.0), (int)round(color.b * 255.0));
			}
		}
	}
	// to convert int to string
	stringstream stream;
    stream <<  numberOfCapturedImages++;

	// Output_1.bmp, Output_2.bmp, Output_3.bmp, etc.
	string imageName = "Output_" + stream.str() + ".bmp";
	// create a Output folder if not exists
	image.save_image("Output/" + imageName);

	cout<<"saving img: "<<imageName<<"\n";
}



void keyboardListener(unsigned char key, int x,int y){
	switch(key){
		case '0':
			capture(); // capture image
			break;

		case '1':
			//drawgrid=1-drawgrid;
			r = Point::rotateVector(r, u, deg2rad(moveAmount));
			l = Point::rotateVector(l, u, deg2rad(moveAmount));
			
			break;
		case '2':
			r = Point::rotateVector(r, u, deg2rad(-moveAmount));
			l = Point::rotateVector(l, u, deg2rad(-moveAmount));
			
			break;
		case '3':
			l = Point::rotateVector(l, r, deg2rad(-moveAmount));
			u = Point::rotateVector(u, r, deg2rad(-moveAmount));
			 
			break;
		case '4':
			l = Point::rotateVector(l, r, deg2rad(moveAmount));
			u = Point::rotateVector(u, r, deg2rad(moveAmount));
			 
			break;
		case '5':
			r = Point::rotateVector(r, l, deg2rad(moveAmount));
			u = Point::rotateVector(u, l, deg2rad(moveAmount));
			
			break;
		case '6':
			r = Point::rotateVector(r, l, deg2rad(-moveAmount));
			u = Point::rotateVector(u, l, deg2rad(-moveAmount));
			
			break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			pos = pos - (l * moveAmount);
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			pos = pos + (l * moveAmount);
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			pos = pos - (r * moveAmount);
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			pos = pos + (r * moveAmount);
			break;

		case GLUT_KEY_PAGE_UP:
			pos = pos + (u * moveAmount);
			break;
		case GLUT_KEY_PAGE_DOWN:
			pos = pos - (u * moveAmount);
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
	ifstream inputFile(inputFileName);

	inputFile >> levelsOfRecursion >> pixelsAlongBothAxis;
	inputFile >> numberOfObjects;

	for(int i=0;i<numberOfObjects;i++){
		string objectType;
		inputFile >> objectType;
		// cout<< "Object Type: " << objectType << endl;
		if(objectType == "sphere"){
			Sphere *s = new Sphere();
			inputFile >> (*s);
			// cout<<s;
			objects.push_back(s);
		}
		else if(objectType == "triangle"){
			Triangle *t = new Triangle();
			inputFile >> *t;
			// cout<<t;
			objects.push_back(t);
		}
		else if(objectType == "general"){
			General *g = new General();
			inputFile >> *g;
			// cout<<g;
			objects.push_back(g);
		}
		else{
			cout<<"Invalid Object Type : "<< objectType<<endl;
		}
	}
	
	inputFile >> numberOfPointLights;
	// cout<<"number of point lights :"<<numberOfPointLights<<endl;
	for(int i=0;i<numberOfPointLights;i++){
		PointLight *pl = new PointLight();
		inputFile >> *pl;
		// cout<<pl;
		lights.push_back(pl);
	}

	inputFile >> numberOfSpotLights;
	for(int i=0;i<numberOfSpotLights;i++){
		SpotLight *sl = new SpotLight();
		inputFile >> *sl;
		// cout<<sl;
		lights.push_back(sl);
	}

	inputFile.close();

	// initialize floor
	Object *floor = new Floor(1000, 20);
	floor->setColor(Color(1,1,1)); // set dummy color
	floor->setShine(10);
	floor->setCoEfficients(0.4, 0.2, 0.2, 0.2);
	objects.push_back(floor);
}

void showData(){
	cout << "Levels of Recursion: " << levelsOfRecursion << endl;
	cout << "Pixels along both axis: " << pixelsAlongBothAxis << endl;
	cout << "Number of Objects: " << numberOfObjects << endl;

	for(int i=0;i<objects.size();i++){
		cout << "Object " << i+1 << ": " << endl;
		cout << *objects[i] << endl;
	}

	cout << "Number of Point Lights: " << numberOfPointLights << endl;
	for(int i=0;i<numberOfPointLights;i++){
		cout << "Point Light " << i+1 << ": " << endl;
		cout << *lights[i] << endl;
	}

	cout << "Number of Spot Lights: " << numberOfSpotLights << endl;
	for(int i=numberOfPointLights;i<numberOfPointLights+numberOfSpotLights;i++){
		cout << "Spot Light " << i+1 << ": " << endl;
		cout << *lights[i] << endl;
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

	// draw objects. draw sphere & triangle in opengl
	for(int i=0;i<objects.size();i++){
		objects[i]->draw();
	}
	
	// draw lights. spot lights are drawn as squares. point lights are drawn as spheres
	for(int i=0;i<lights.size();i++){
		lights[i]->draw();
	}
	
	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle += 0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=0;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	double root2 = 1.0/sqrt(2);

	pos = Point(100,100,50);
	u = Point(0,0,1);
	r = Point(-root2,root2,0);
	l = Point(-root2,-root2,0);

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
	gluPerspective(fovY,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	srand(time(NULL));
	glutInit(&argc,argv);
	glutInitWindowSize(windowWidth, windowHeight);
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

	// free vector memory
	vector<Object*>().swap(objects);
	vector<PointLight*>().swap(lights);
	
	return 0;
}