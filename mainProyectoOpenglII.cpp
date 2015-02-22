// Cubica

#include <stdlib.h>

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include <vector>

// assimp include files. These three are usually needed.
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

using namespace std;

#include "glm.h"

#define NELEMS(x)  (sizeof(x) / sizeof(x[0]))

// --------------- Variables para carga de textura de los objetos: piso, conejo y columnas ------------

static GLuint textPiso;
static GLuint textColumna;
static GLuint textConejo;

int iheightPiso, iwidthPiso,
	iheightColumna, iwidthColumna,
	iheightConejo, iwidthConejo;

unsigned char* imgPiso = NULL;
unsigned char* imgColumna = NULL;
unsigned char* imgConejo = NULL;

// Variables para carga de textura del del Cub Map

/* Cube map Texture ID's*/
static GLuint texPosY,texNegY,texPosZ,
	  texNegZ,texPosX,texNegX;

int iheightPosx, iwidthPosx, iheightPosy, iwidthPosy, iheightPosz, iwidthPosz,
	iheightNegx, iwidthNegx, iheightNegy, iwidthNegy, iheightNegz, iwidthNegz;

unsigned char* imgPositiveX  = NULL;
unsigned char* imgNegativeX = NULL;
unsigned char* imgPositiveY = NULL;
unsigned char* imgNegativeY = NULL;
unsigned char* imgPositiveZ = NULL;
unsigned char* imgNegativeZ = NULL;

//---------- Variables para la carga de textura de los planos en el Cub Map y en el SkyBox ---------------------

/** * Plane texture ID's */
static GLuint planeTexPosX,planeTexPosY,planeTexPosZ,planeTexNegX,planeTexNegY,planeTexNegZ;

// variables para el spotlight
static float spot_light_x = 0.0f;
static float spot_light_z = 0.0f;

//cutOff ,exponent e intensidad del spotlight
static float cutOff;
static float exponent;
static float intensidad;

float compAmbient = 1.0f,
	  conejo_CompR = 0.2f,
	  conejo_CompG = 0.2f,
	  conejo_CompB = 0.2f,
	  ra =0.0f,
	  ga =0.0f,
	  ba=0.0f,
	  ma=0.0f,
	  luz_CompR=1.0f,
	  luz_CompG=1.0f,
	  luz_CompB=1.0f;
		
// Booleanos para la reflexion (cubmapping) e iluminacion
bool reflexion,
	iluminacion;

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

   cutOff = 50.0f;
   exponent = 25.0f;
   intensidad = 1.0f;

   reflexion = false;
   iluminacion = true;

   glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);
   glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);
   glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);


   imgColumna = glmReadPPM("texAO_columna.ppm", &iwidthColumna, &iheightColumna);
   imgPiso = glmReadPPM("texAO_plano.ppm", &iwidthPiso, &iheightPiso);
   imgConejo = glmReadPPM("texAO_bunny.ppm", &iwidthConejo, &iheightConejo);

   	imgPositiveX = glmReadPPM("posx.ppm", &iwidthPosx, &iheightPosx);
	imgNegativeX = glmReadPPM("negx.ppm", &iwidthNegx, &iheightNegx);
	imgPositiveY = glmReadPPM("posy.ppm", &iwidthPosy, &iheightPosy);
	imgNegativeY = glmReadPPM("negy.ppm", &iwidthNegy, &iheightNegy);
	imgPositiveZ = glmReadPPM("posz.ppm", &iwidthPosz, &iheightPosz);
	imgNegativeZ = glmReadPPM("negz.ppm", &iwidthNegz, &iheightNegz);

   glFrontFace(GL_CW);

}

void cargar_materiales(int idx) {

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_ambient[] = { conejo_CompR,conejo_CompG,conejo_CompB,1.0 };

	// Material Piso
	if (idx == 0){	
	
   	glEnable(GL_COLOR_MATERIAL); 
  		
  		glGenTextures(1, &textPiso);
   		glBindTexture(GL_TEXTURE_2D, textPiso);

		glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	  

   		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidthPiso, iheightPiso, 0, GL_RGB, GL_UNSIGNED_BYTE, imgPiso);
     
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_specular);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_specular);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   	glDisable(GL_COLOR_MATERIAL);

	}

	// Material Columna
	if (idx == 1){
		
	glEnable(GL_COLOR_MATERIAL);
		
		glGenTextures(1, &textColumna);
		glBindTexture(GL_TEXTURE_2D, textColumna);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidthColumna, iheightColumna, 0, GL_RGB, GL_UNSIGNED_BYTE, imgColumna);
		

		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_specular);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_specular);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);

	glDisable(GL_COLOR_MATERIAL);
	
	}

	// Material Conejo
	if (idx == 2){

		glGenTextures(1, &textConejo);
		glBindTexture(GL_TEXTURE_2D, textConejo);

		glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidthConejo, iheightConejo, 0, GL_RGB, GL_UNSIGNED_BYTE, imgConejo);
	
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

			
	}
	//Activamos las texturas
	glEnable(GL_TEXTURE_2D);	
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


void spot_light(){
	
	//GLfloat light_ambient[] =  {0.0f, 0.0f, 0.0f, 0.0f};
	GLfloat light_diffuse[] =  {intensidad, intensidad, intensidad, intensidad/intensidad};
	//GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat light_position[] = {0.0f, 200.0f, 0.0f, 1.0f};
	GLfloat light_direction[] = {spot_light_x,-1.0f, spot_light_z};

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 }; ///////////////
    GLfloat mat_shininess[] = { 50.0 };	////////////////////////////  esto da la sensacion de brillo

	
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); ////////////
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);//////////////

	//glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

	//glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, cutOff);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, exponent);
	glEnable(GL_LIGHT0);
	
}
void cube_map() {

	// Create and load the 6 textures of the cube map
	
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
	
	glPushMatrix();
	glRotated(180,0.0,0.0,1.0);
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
	glPopMatrix();
	
	}

void Draw_Skybox(float x, float y, float z, float width, float height, float length)
{
	glEnable(GL_COLOR_MATERIAL);
	// Center the Skybox around the given x,y,z position
	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;

// Draw Back side ------------------------------------FRENTE-----------------------------------------------
	glGenTextures(1,&planeTexPosZ);
	glBindTexture(GL_TEXTURE_2D,planeTexPosZ);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidthPosz, iheightPosz, 0, GL_RGB, GL_UNSIGNED_BYTE, imgPositiveZ);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height, z); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
	glEnd();


// Draw Left side --------------------------------------LADO DERECHO---------------------------------
	glGenTextures(1,&planeTexNegX);
	glBindTexture(GL_TEXTURE_2D,planeTexNegX);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidthNegx, iheightNegx, 0, GL_RGB, GL_UNSIGNED_BYTE, imgNegativeX);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);



	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z+length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z);		
	glEnd();


// Draw Right side -----------------------------------LADO IZQUIERDO------------------------------
	glGenTextures(1,&planeTexPosX);
	glBindTexture(GL_TEXTURE_2D,planeTexPosX);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidthPosx, iheightPosx, 0, GL_RGB, GL_UNSIGNED_BYTE, imgPositiveX);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height,	z+length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height,	z);
	glEnd();

	
  glDisable(GL_COLOR_MATERIAL); 
}


void render(){

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glLoadIdentity ();                       
	gluLookAt (0, 80, 250, 0.0, 15.0, 0.0, 0.0, 1.0, 0.0);

	
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
	

	if (iluminacion){
		spot_light();
	}
	 
	if (reflexion){
	glPushMatrix();
	glRotated(180,0.0,0.0,1.0);
	//glRotated(180,0.0,1.0,0.0);
		glDisable(GL_TEXTURE_CUBE_MAP);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_R);
	Draw_Skybox(0,-20,0,500,500,800);	// Draw the Skybox
		glEnable(GL_TEXTURE_CUBE_MAP);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_GEN_R);
	glPopMatrix();
	}
	cube_map();
	


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
	cout << " Intensidad de la luz [b/n]: " << intensidad << "\n";
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
		cutOff += 2.0f;
		break;
	case 'w':
		cutOff -= 2.0f;
		break;
	case 'a':
		exponent += 2.0f;
		break;
	case 's':
		exponent -= 2.0f;
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
		conejo_CompR += 0.5f;
		break;
	case 'g':
		conejo_CompR -= 0.5f;
		break;
	case 'y':
		conejo_CompG += 0.5f;
		break;
	case 'h':
		conejo_CompG -= 0.5f;
		break;
	case 'u':
		conejo_CompB += 0.5f;
		break;
	case 'j':
		conejo_CompB -= 0.5f;
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
		intensidad += 1.0f;
		break;
	case 'n':
		intensidad -= 1.0f;
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
  glDeleteLists(scene_list,1);
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
