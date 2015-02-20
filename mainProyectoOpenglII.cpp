// Cubica

#include <stdlib.h>

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>

// assimp include files. These three are usually needed.
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "glm.h"

using namespace std;

// the global Assimp scene object
const aiScene* scene01 = NULL;
const aiScene* scene02 = NULL;
const aiScene* scene03 = NULL;

GLuint scene_list = 0;
aiVector3D scene_min, scene_max, scene_center;

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

#define DEF_floorGridScale	1.0
#define DEF_floorGridXSteps	10.0
#define DEF_floorGridZSteps	10.0

static GLuint texName;
int iheight, iwidth;
unsigned char* image = NULL;

static GLuint texName2;
int iheight2, iwidth2;
unsigned char* image2 = NULL;


static GLuint texName3;
int iheight3, iwidth3;
unsigned char* image3 = NULL;

static GLuint texName4;
int iheight4, iwidth4;
unsigned char* image4 = NULL;

//------------------------


int iheightPosx, iwidthPosx, iheightPosy, iwidthPosy, iheightPosz, iwidthPosz,
	iheightNegx, iwidthNegx, iheightNegy, iwidthNegy, iheightNegz, iwidthNegz;
unsigned char* imgPositiveX  = NULL;
unsigned char* imgNegativeX = NULL;
unsigned char* imgPositiveY = NULL;
unsigned char* imgNegativeY = NULL;
unsigned char* imgPositiveZ = NULL;
unsigned char* imgNegativeZ = NULL;

/* Cube map Texture ID's
*/
static GLuint texPosY,texNegY,texPosZ,texNegZ,texPosX,texNegX;
/**
* Plane texture ID's
*/
static GLuint planeTexPosX,planeTexPosY,planeTexPosZ,planeTexNegX,planeTexNegY,planeTexNegZ;


bool reflexion = true,
	iluminacion = true;


//----------------------
float cutOff = 50.0f,
	  exponent = 25.0f,
	  compAmbient = 1.0f,
	  spot_light_x = 0.0f,
	  spot_light_z = 0.0f,
	  conejo_CompR = 1.0f,
	  conejo_CompG = 1.0f,
	  conejo_CompB = 1.0f,
	  luz_CompR=1.0f,
	  luz_CompG=1.0f,
	  luz_CompB=1.0f;


  	

void changeViewport(int w, int h) {
	
	float aspectratio;

	if (h==0)
		h=1;

   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(30, (GLfloat) w/(GLfloat) h, 1.0, 3000.0);
   glMatrixMode (GL_MODELVIEW);

}

void init(){

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);

   glEnable(GL_TEXTURE_CUBE_MAP);

   	// Enable texture generation for S,T,R coords
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	// Sets the coordinates to be generated and how
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);
	glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);

	// This glut implementation has an strange issue
	// the Teapot (glutSolidTeapot) is draw using 
	// clock-wise face ordering. Other primitives are
	// draw with Counter clock-wise face ordering...
	// Case this is an isolated issue (which I don't
	// believe) please, revert to GL_CCW
	glFrontFace(GL_CW);


}


void cargar_materiales(int idx) {


	// Material Piso
	if (idx == 0){	
	
   glGenTextures(1, &texName);
   glBindTexture(GL_TEXTURE_2D, texName);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
   image = glmReadPPM("texAO_plano.ppm", &iwidth, &iheight);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidth, iheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
     


	}

	// Material Columna
	if (idx == 1){
	
		glGenTextures(1, &texName2);
		glBindTexture(GL_TEXTURE_2D, texName2);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	
		image2 = glmReadPPM("texAO_columna.ppm", &iwidth2, &iheight2);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidth2, iheight2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
		
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		
	}

	// Material Conejo

	if (idx == 2){
	//float mdiffuse1[] = {1,0,0,1};
	


		/// Componente ambiental de los objetos
		//GLfloat mat_ambient[] = { 0.28, 0.75, 0.82, 1.0 };	
		//GLfloat mat_ambient[] = { 0.0, 1.0, 0.0, 1.0 };	
		//glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	

		glGenTextures(1, &texName4);
		glBindTexture(GL_TEXTURE_2D, texName4);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   		image4 = glmReadPPM("texAO_bunny.ppm", &iwidth4, &iheight4);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidth4, iheight4, 0, GL_RGB, GL_UNSIGNED_BYTE, image4);

   
	//Activamos las texturas
		glEnable(GL_TEXTURE_2D);

		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		
		
	}
		
}

void recursive_render (const aiScene *sc, const aiNode* nd)
{
	unsigned int i;
	unsigned int n = 0, t;
	aiMatrix4x4 m = nd->mTransformation;

	// update transform
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);

	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n) {
		
		const aiMesh* mesh = scene01->mMeshes[nd->mMeshes[n]];

		for (t = 0; t < mesh->mNumFaces; ++t) {
			const aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			switch(face->mNumIndices) {
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);

			for(i = 0; i < face->mNumIndices; i++) {
				int index = face->mIndices[i];
				
				if(mesh->mColors[0] != NULL)
					glColor4fv((GLfloat*)&mesh->mColors[0][index]);
				
				if(mesh->mNormals != NULL) 
					glNormal3fv(&mesh->mNormals[index].x);
				
				if (mesh->HasTextureCoords(0)) 
					glTexCoord2f(mesh->mTextureCoords[0][index].x, 1-mesh->mTextureCoords[0][index].y);
				
				glVertex3fv(&mesh->mVertices[index].x);
			}

			glEnd();
		}

	}

	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n) {
		cargar_materiales(n);
		recursive_render(sc, nd->mChildren[n]);
	}

	glPopMatrix();
}



void render(){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glLoadIdentity ();                       
	gluLookAt (0, 80, 250, 0.0, 15.0, 0.0, 0.0, 1.0, 0.0);

	
	float mdiffuse1[] = {conejo_CompR,conejo_CompG,conejo_CompB,1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mdiffuse1);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mdiffuse1);

	GLfloat light1_ambient[] =  {0.0f, 0.0f, 0.0f, 1.0f};
	//GLfloat light1_diffuse[] =  {luz_CompR, luz_CompR, luz_CompR, 1.0f};
	GLfloat light1_diffuse[] =  {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat light1_position[] = {0.0f, 200.0f, 0.0f, 1.0f};
	GLfloat light1_direction[] = {spot_light_x,-1.0f, spot_light_z};
	GLfloat light_specular[] = { 1.0, 1.0, 0.3, 0.0 };


   	glLightfv(GL_LIGHT0, GL_AMBIENT, light1_ambient);
   	glLightfv(GL_LIGHT0, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   	glLightfv(GL_LIGHT0, GL_POSITION, light1_position);
   	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light1_direction);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, cutOff);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, exponent);
	glEnable(GL_LIGHT0);
	

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	// Create and load the 6 textures of the cube map


	
	
	int iheightPosx, iwidthPosx, iheightPosy, iwidthPosy, iheightPosz, iwidthPosz,
	iheightNegx, iwidthNegx, iheightNegy, iwidthNegy, iheightNegz, iwidthNegz;

	imgPositiveX = glmReadPPM("posx.ppm", &iwidthPosx, &iheightPosx);
	imgNegativeX = glmReadPPM("negx.ppm", &iwidthNegx, &iheightNegx);
	imgPositiveY = glmReadPPM("posy.ppm", &iwidthPosy, &iheightPosy);
	imgNegativeY = glmReadPPM("negy.ppm", &iwidthNegy, &iheightNegy);
	imgPositiveZ = glmReadPPM("posz.ppm", &iwidthPosz, &iheightPosz);
	imgNegativeZ = glmReadPPM("negz.ppm", &iwidthNegz, &iheightNegz);




	// Create the cube map textures, positive X
	glGenTextures(1,&texPosX);
	glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X,texPosX);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, iwidthPosx, iheightPosx, 1, GL_RGB, GL_UNSIGNED_BYTE, imgPositiveX);
	// Create the cube map textures, negative X
	glGenTextures(1,&texNegX);
	glBindTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,texNegX);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, iwidthNegx, iheightNegx, 1, GL_RGB, GL_UNSIGNED_BYTE, imgNegativeX);
	// Create the cube map textures, positive Y
	glGenTextures(1,&texPosY);
	glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,texPosY);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, iwidthPosy, iheightPosy, 1, GL_RGB, GL_UNSIGNED_BYTE, imgPositiveY);
	// Create the cube map textures, negative Y
	glGenTextures(1,&texNegY);
	glBindTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,texNegY);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, iwidthNegy, iheightNegy, 1, GL_RGB, GL_UNSIGNED_BYTE, imgNegativeY);
	// Create the cube map textures, positive Z
	glGenTextures(1,&texPosZ);
	glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,texPosZ);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, iwidthPosz, iheightPosz, 1, GL_RGB, GL_UNSIGNED_BYTE, imgPositiveZ);
	// Create the cube map textures, negative Z
	glGenTextures(1,&texNegZ);
	glBindTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,texNegZ);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, iwidthNegz, iheightNegz, 1, GL_RGB, GL_UNSIGNED_BYTE, imgNegativeZ);

	// Sets the texture's behavior for wrapping (optional)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// Sets the texture's max/min filters
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	// Now we re-create the textures to texturize the planes
	


	//-------------------------------------------------------------------------------------
	// Create plane texture, postive X
	glGenTextures(1,&planeTexPosX);
	glBindTexture(GL_TEXTURE_2D,planeTexPosX);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidthPosx, iheightPosx, 0, GL_RGB, GL_UNSIGNED_BYTE, imgPositiveX);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	// Create plane texture, negative X
	glGenTextures(1,&planeTexNegX);
	glBindTexture(GL_TEXTURE_2D,planeTexNegX);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidthNegx, iheightNegx, 0, GL_RGB, GL_UNSIGNED_BYTE, imgNegativeX);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	// Create plane texture, postive X
	glGenTextures(1,&planeTexPosY);
	glBindTexture(GL_TEXTURE_2D,planeTexPosY);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidthPosy, iheightPosy, 0, GL_RGB, GL_UNSIGNED_BYTE, imgPositiveY);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	// Create plane texture, negative X
	glGenTextures(1,&planeTexNegY);
	glBindTexture(GL_TEXTURE_2D,planeTexNegY);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidthNegy, iheightNegy, 0, GL_RGB, GL_UNSIGNED_BYTE, imgNegativeY);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	// Create plane texture, postive X
	glGenTextures(1,&planeTexPosZ);
	glBindTexture(GL_TEXTURE_2D,planeTexPosZ);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidthPosz, iheightPosz, 0, GL_RGB, GL_UNSIGNED_BYTE, imgPositiveZ);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	// Create plane texture, negative X
	glGenTextures(1,&planeTexNegZ);
	glBindTexture(GL_TEXTURE_2D,planeTexNegZ);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidthNegz, iheightNegz, 0, GL_RGB, GL_UNSIGNED_BYTE, imgNegativeZ);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);







	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++




	//Suaviza las lineas
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_LINE_SMOOTH );	


	glPushMatrix();
	glEnable(GL_NORMALIZE);
	if(scene_list == 0) {
	    scene_list = glGenLists(1);
	    glNewList(scene_list, GL_COMPILE);
            // now begin at the root node of the imported data and traverse
            // the scenegraph by multiplying subsequent local transforms
            // together on GL's matrix stack.		
	    recursive_render(scene01, scene01->mRootNode);
	    glEndList();
	}
	glCallList(scene_list);
	
	
	glPopMatrix();
	


	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);
	glutSwapBuffers();
}

void animacion(int value) {
	
	glutTimerFunc(10,animacion,1);
    glutPostRedisplay();
	
}
void get_bounding_box_for_node (const aiNode* nd, 
	aiVector3D* min, 
	aiVector3D* max, 
	aiMatrix4x4* trafo
){
	aiMatrix4x4 prev;
	unsigned int n = 0, t;

	
		prev = *trafo;
		aiMultiplyMatrix4(trafo,&nd->mTransformation);

		for (; n < nd->mNumMeshes; ++n) {
			const aiMesh* mesh = scene01->mMeshes[nd->mMeshes[n]];
				for (t = 0; t < mesh->mNumVertices; ++t) {

				aiVector3D tmp = mesh->mVertices[t];
				aiTransformVecByMatrix4(&tmp,trafo);

				min->x = aisgl_min(min->x,tmp.x);
				min->y = aisgl_min(min->y,tmp.y);
				min->z = aisgl_min(min->z,tmp.z);

				max->x = aisgl_max(max->x,tmp.x);
				max->y = aisgl_max(max->y,tmp.y);
				max->z = aisgl_max(max->z,tmp.z);
			}
		}

		for (n = 0; n < nd->mNumChildren; ++n) {
			get_bounding_box_for_node(nd->mChildren[n],min,max,trafo);
		}
		*trafo = prev;
	

}

void get_bounding_box (aiVector3D* min, aiVector3D* max)
{
	aiMatrix4x4 trafo;

	
		aiIdentityMatrix4(&trafo);
	
		min->x = min->y = min->z =  1e10f;
		max->x = max->y = max->z = -1e10f;
		get_bounding_box_for_node(scene01->mRootNode,min,max,&trafo);


}
int loadasset (const char* path)
{
	// we are taking one of the postprocessing presets to avoid
	// spelling out 20+ single postprocessing flags here.
	
		
		scene01 = aiImportFile(path,aiProcessPreset_TargetRealtime_MaxQuality);

		if (scene01) {
			get_bounding_box(&scene_min,&scene_max);
			scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
			scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
			scene_center.z = (scene_min.z + scene_max.z) / 2.0f;
			return 0;
		}


	return 1;
}
void imprimirEstado() {
	cout << "\n\n\n\n\n\n\n\n";
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	cout << "Estado de las variables\n";
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	cout << "[x++/x--]\n\n";
	cout << "cutOff del Spot Light [q/w]: " << cutOff << "\n";
	cout << "exponente del SPot Light [a/s]: " << exponent << "\n";
	cout << "comonente ambiental de cada modelo [z/x]: " << compAmbient << "\n";
	cout << "direccion en el eje X del Spot Light [e/d]: " << spot_light_x << "\n";
	cout << "direccion en el eje Z del Spot Light [r/f] : " << spot_light_z << "\n";
	cout << "Color Conejo canal R [t/g]: " << conejo_CompR << "\n";
	cout << "Color Conejo canal G [y/h]: " << conejo_CompG << "\n";
	cout << "Color Conejo canal B [u/j]: " << conejo_CompB << "\n";
	cout << " Intensidad de la luz [b/n]: " << "INTENSIDAD DE LA LUZ" << "\n";
	cout << "Color Blanco luz [1]: " << "ALGO" << "\n";
	cout << "Color Rojo luz [2]: " <<  "ALGO" << "\n";
	cout << "Color Verde luz [3]: " << "ALGO" << "\n";
	cout << "Color Azul luz [4]: " <<  "ALGO" << "\n";
	cout << "Reflexion [c]: " << (reflexion ? "On" : "Off") <<"\n";
	cout << "Iluminacion [v]: " << (iluminacion ? "On" : "Off") << "\n";
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
}
void Keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
	case 27:             
		exit (0);
		break;
	case 'q':
		cutOff += 5.0f;
		break;
	case 'w':
		cutOff -= 5.0f;
		break;
	case 'a':
		exponent += 5.0f;
		break;
	case 's':
		exponent -= 5.0f;
		break;
	case 'z':
		compAmbient += 0.1f;
		break;
	case 'x':
		compAmbient -= 0.1f;
		break;
	case 'e':
		spot_light_x += 0.1f;
		break;
	case 'd':
		spot_light_x -= 0.1f;
		break;
	case 'r':
		spot_light_z += 0.1f;
		break;
	case 'f':
		spot_light_z -= 0.1f;
		break;
	case 't':
		conejo_CompR += 0.1f;
		break;
	case 'g':
		conejo_CompR -= 0.1f;
		break;
	case 'y':
		conejo_CompG += 0.1f;
		break;
	case 'h':
		conejo_CompG -= 0.1f;
		break;
	case 'u':
		conejo_CompB += 0.1f;
		break;
	case 'j':
		conejo_CompB -= 0.1f;
		break;
	case 'c':
		if (reflexion){
			glDisable(GL_TEXTURE_CUBE_MAP);
		   	glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
			glDisable(GL_TEXTURE_GEN_R);
			reflexion = false;
			break;
		}else{
			glEnable(GL_TEXTURE_CUBE_MAP);
		   	glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glEnable(GL_TEXTURE_GEN_R);
			reflexion = true;
			break;
		}
		
	case 'v':
		if (iluminacion){
			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);
			iluminacion = false;
			break;
		}else{
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			iluminacion = true;
			break;
		}
	case 'b':
		
		break;
	case 'n':
		
		break;
	case '1':
		luz_CompR=1.0f;
		luz_CompG=1.0f;
		luz_CompB=1.0f;
		break;
	case '2':
		luz_CompR=1.0f;
		luz_CompG=0.0f;
		luz_CompB=0.0f;
		break;
	case '3':
		luz_CompR=0.0f;
		luz_CompG=1.0f;
		luz_CompB=0.0f;
		break;
	case '4':
		luz_CompR=0.0f;
		luz_CompG=0.0f;
		luz_CompB=1.0f;
		break;

  };
  imprimirEstado();
  scene_list = 0;
  glutPostRedisplay();
}
int main (int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(960,540);

	glutCreateWindow("Test Opengl");


	aiLogStream stream;
	// get a handle to the predefined STDOUT log stream and attach
	// it to the logging system. It remains active for all further
	// calls to aiImportFile(Ex) and aiApplyPostProcessing.
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
	aiAttachLogStream(&stream);

	// ... same procedure, but this stream now writes the
	// log messages to assimp_log.txt
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE,"assimp_log.txt");
	aiAttachLogStream(&stream);

	// the model name can be specified on the command line. If none
	// is specified, we try to locate one of the more expressive test 
	// models from the repository (/models-nonbsd may be missing in 
	// some distributions so we need a fallback from /models!).
	//if( 0 != loadasset( argc >= 2 ? argv[1] : "dragon_vrip_res2.ply")) {
	//	if( argc != 1 || (0 != loadasset( "dragon_vrip_res2.ply") && 0 != loadasset( "dragon_vrip_res2.ply"))) { 
	//		return -1;
	//	}
	//}

	if (loadasset( "escenario.obj") != 0) {
		return -1;
	}




	init ();

	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	glutKeyboardFunc (Keyboard);
	


	glutMainLoop();
	return 0;

}
