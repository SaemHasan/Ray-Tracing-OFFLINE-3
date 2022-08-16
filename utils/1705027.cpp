#include "Utils_zBuffer.h"

string defaultScenefile = "input/scene.txt";
string defaultstageFile = "output/stage3.txt";
string defaultconfigFile = "input/config.txt";



int main(int argc,char* argv[]){
    srand(time(NULL));
    //file setup
    FILE *infile = NULL;
    ifstream configfile;
    
    if(argc>2){
        infile = freopen(argv[1],"r",stdin);
        cout<<"opening input file: "<<argv[1]<<endl;
        configfile.open(argv[2]);
        cout<<"Opening config file: "<<argv[2]<<endl;
    } 
    else if(argc==1){
        cout<<"no input file provided"<<endl;
        cout<<"openning default files..."<<endl;
        infile = freopen(defaultScenefile.c_str(),"r",stdin);
        configfile.open(defaultconfigFile.c_str());
    }

    if (infile == NULL || !configfile.is_open()) {
        cout<<"Error opening file"<<endl;
        return 1;
    }

    //create output file
    ofstream outStage1("output/stage1.txt");
    ofstream outStage2("output/stage2.txt");
    ofstream outStage3("output/stage3.txt");
    ofstream zBufferfile("output/z_buffer.txt");


    if(!outStage1.is_open() || !outStage2.is_open() || !outStage3.is_open() || !zBufferfile.is_open()){
        cout<<"Error opening output file"<<endl;
        return 1;
    }


    int numberOfDecimalPlaces = 7;
    outStage1<<fixed<<setprecision(numberOfDecimalPlaces);
    outStage2<<fixed<<setprecision(numberOfDecimalPlaces);
    outStage3<<fixed<<setprecision(numberOfDecimalPlaces);
    zBufferfile<<fixed<<setprecision(numberOfDecimalPlaces-1);

    //Read from file
    Point eye, look, up;
    double fovY, aspectRatio, near, far;

    cin>>eye>>look>>up;
    // cout<<eye<<look<<up;
    cin>>fovY>>aspectRatio>>near>>far;
    // cout<<fovY<<" "<<aspectRatio<<" "<<near<<" "<<far<<endl;

    //create view transformation matrix
    Matrix ViewMatrix = viewTransformationMatrix(eye, look, up);

    //create projection transformation matrix
    Matrix ProjectMatrix = Matrix::projectionMatrix(fovY, aspectRatio, near, far); //fovY, aspectRatio, near, far

    //initialize empty stack S
    stack<Matrix> S;
    S.push(Matrix::identityMatrix());

    //for push and pop
    stack< stack<Matrix> > stackofS;

    while(true){
        string command;
        cin>>command;

        if(command == "triangle"){
            Point p1, p2, p3;
            cin>>p1>>p2>>p3;
            
            Point p[]={p1,p2,p3};
            // outStage1<<"triangle "<<p1<<p2<<p3<<endl;
            for(int i = 0; i < 3; i++){
                Point s1 = transformPoint(S.top(), p[i]);
                outStage1<<s1;
                Point s2 = transformPoint(ViewMatrix, s1);
                outStage2<<s2;
                Point s3 = transformPoint(ProjectMatrix, s2);
                outStage3<<s3;
            }
            outStage1<<endl;
            outStage2<<endl;
            outStage3<<endl;
        }

        else if(command == "translate"){
            Point t;
            cin>>t;
            Matrix T = Matrix::translationMatrix(t);
            // cout<<t;
            // cout<<T;
            S.push(S.top() * T);
        }

        else if(command == "scale"){
            Point s;
            cin>>s;
            Matrix Sc = Matrix::scalingMatrix(s);
            S.push(S.top() * Sc);
        }

        else if(command == "rotate"){
            double angle;
            cin>>angle;
            Point r;
            cin>>r;
            Matrix R = Matrix::rotationMatrix(angle, r);
            S.push(S.top() * R);
        }

        else if(command == "push"){
            stackofS.push(S);
        }

        else if(command == "pop"){
            S = stackofS.top();
            stackofS.pop();
        }
        else if(command == "end"){
            break;
        }
        else{
            cout<<"Invalid command"<<endl;
        }
    }

    // cout<<"done\n";
    // close files
    outStage1.close();
    outStage2.close();
    outStage3.close();

    //read config file
    configfile>>SCREEN_WIDTH>>SCREEN_HEIGHT;
    configfile>>LEFT_LIMIT>>BOTTOM_LIMIT;
    RIGHT_LIMIT = -LEFT_LIMIT;
    TOP_LIMIT = -BOTTOM_LIMIT;
    configfile>>FRONT_Z>>REAR_Z;
    
    //close config file
    configfile.close();

    //open stage 3 file
    ifstream stage3file(defaultstageFile.c_str());
    if(!stage3file.is_open()){
        cout<<"Error : create a output folder named output and put stage3.txt in it"<<endl;
        cout<<"Error opening stage 3 file"<<endl;
        return 1;
    }

    //read stage 3 file
    cout<<"Reading stage 3 file..."<<endl;
    while(!stage3file.eof()){
        Triangle t;
        stage3file>>t;
        if(t.points[0].length()==0 && t.points[1].length()==0 && t.points[2].length()==0){
            break;
        }
        triangles.push_back(t);
        // cout<<t;
    }
    cout<<"stage 3 file read successfully"<<endl;
    // close stage 3 file
    stage3file.close();

    //print read details
    //printDetails();

    //z buffer and image setup
    initializeBuffers();

    //procedure
    zBufferAlgorithm();

    //write to z buffer file
    for(int i = 0; i < SCREEN_HEIGHT; i++){
        for(int j = 0; j < SCREEN_WIDTH; j++){
            if(zBuffer[i][j]!=REAR_Z)
                zBufferfile<<zBuffer[i][j]<<"\t";
        }
        zBufferfile<<endl;
    }
    zBufferfile.close();

    // cout<<"memory clear kora lagbe. vector gula delete korte hobe using vector<tempObject>().swap(tempVector);"<<endl;


    //create image
    image = bitmap_image(SCREEN_WIDTH, SCREEN_HEIGHT);
    for(int i=0; i<SCREEN_HEIGHT; i++){
        for(int j=0; j<SCREEN_WIDTH; j++){
            image.set_pixel(j,i, colorBuffer[i][j].r, colorBuffer[i][j].g, colorBuffer[i][j].b);
        }
    }
    image.save_image("output/out.bmp");

    // clear vector memory
    for(int i = 0; i < SCREEN_HEIGHT; i++){
        vector<Color>().swap(colorBuffer[i]);
        vector<double>().swap(zBuffer[i]);
    }
    vector< vector<Color> >().swap(colorBuffer);
    vector< vector<double> >().swap(zBuffer);
    vector<Triangle>().swap(triangles);

    return 0;
}