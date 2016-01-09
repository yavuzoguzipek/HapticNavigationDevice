/*
    C socket server example
*/ 
#include "iostream"
#include <irrlicht.h>

#include "driverChoice.h"
#include<stdio.h>
#include<string.h>    //strlen
#include <signal.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<stdlib.h>

/*----------------------------------------------------------------------------- */
#include <vector>
#include <string>
#include <list>

#include <limits> // for numeric_limits

#include <set>
#include <utility> // for pair
#include <algorithm>
#include <iterator>
#include <math.h>

#define FLAG_ENGINE_FORWARD 1
#define FLAG_ENGINE_ROTATE 0
#define FLAG_ENGINE_STOP 2
#define FLAG_ENGINE_WIFI 5

using namespace std;
using namespace irr;
using namespace gui;
using namespace video;
using namespace scene;

typedef int vertex_t;
typedef double weight_t;
//double findYonIcMekan();

const weight_t max_weight = std::numeric_limits<double>::infinity();

struct neighbor {
    vertex_t target;
    weight_t weight;
    neighbor(vertex_t arg_target, weight_t arg_weight)
        : target(arg_target), weight(arg_weight) { }
};

enum
{
    GUI_ID_DIALOG_ROOT_WINDOW = 0x10000,
    GUI_ID_UP_MOVE,
    GUI_ID_DOWN_MOVE,
    GUI_ID_LEFT_MOVE,
    GUI_ID_RIGHT_MOVE,
    GUI_ID_POSITION_TEXT,
    GUI_ID_ANIMATION_INFO,
    GUI_ID_STOP,
    GUI_ID_GO,
    GUI_ID_QUIT
};

struct SAppContext
{
    IrrlichtDevice *device;
    s32             counter;
    IGUIListBox*    listbox1,*listbox2;
    s32 startPos, endPos;
    scene::ISceneNode* anmsRobot;
};


typedef std::vector<std::vector<neighbor> > adjacency_list_t;

int startNode = 0; //giriş
int endNode = -1;
int flag = 0; // node secildi-secilmedi

adjacency_list_t adjacency_list(30);
double coordinates[30][2];

SAppContext context;
scene::ISceneNode* anmsRobot;
core::vector3df dif(0, 0, 90);

IGUIListBox * listbox1,*listbox2;

scene::IAnimatedMeshSceneNode* circle0, *circle1, *circle2, *circle3, *circle4, *circle5,
                             *circle6, *circle7, *circle8, *circle9, *circle10, *circle11,
                             *circle12, *circle13, *circle14, *circle15, *circle16, *circle17, *circle18;
scene::ISceneManager* smgr;
volatile irr::f32 myPos = 0;
scene::ICameraSceneNode* Camera[2] = { 0, 0 };

int returnNode(int posX, int yPos);

void objectNode(int node);
int node = -1;
int PathNodes[16];
int nodeFlag=1;
int flag1=-1,counterFlag=0;
IrrlichtDevice *Device;
IGUIEnvironment* env;
video::IVideoDriver* driver;
IGUIElement* root;
IGUIElement* e;
IGUIWindow* wnd;
IGUITabControl* tab;
IGUITab* t1;
IGUISkin* skin;
IGUIFont* font;

scene::IAnimatedMeshSceneNode* anms[2] ,*indoor, *anmsRobotm1 , *anmsRobotm2 , *anmsRobotm3 , *anmsRobotm4 , *anmsRobotm5,
							*anmsRobotm6, *anmsRobotm7, *anmsRobotm8, *anmsRobotm9, *anmsRobotm10,*anmsRobotm11;
scene::IAnimatedMesh* meshes[2];


class MyEventReceiver : public IEventReceiver
{
public:
    core::position2di cursor;
    MyEventReceiver(SAppContext & context) : Context(context) {
        context.endPos = -1;

    }
    virtual bool OnEvent(const SEvent& event)
    {
        if (event.EventType == EET_GUI_EVENT)
        {
            s32 id = event.GUIEvent.Caller->getID();
            env = Context.device->getGUIEnvironment();
            switch (event.GUIEvent.EventType)
            {
            case EGET_BUTTON_CLICKED:
                switch (id)
                {
                case GUI_ID_GO:
                    Context.startPos = Context.listbox1->getSelected();
                    Context.endPos = Context.listbox2->getSelected();
					flag1++;
                    flag = 1;
					cout << "Startnode: " <<Context.startPos<< endl;
					cout << "Endnode: " <<Context.endPos<< endl;
                    return true;
                    //robot için seçimden sonra yürü hareketi kodlanacak
                case GUI_ID_QUIT:
                    Context.device->closeDevice();
               		exit(0);
                    break;

                default:
                    return false;
                }
                break;
            }
        }
        if (event.EventType == EET_MOUSE_INPUT_EVENT)
        {
            // store mouse X and Y coords
            cursor = core::position2di(event.MouseInput.X, event.MouseInput.Y);
        }
        return false;
    }
private:
    SAppContext & Context;
};

/*-----------------------------------------------------------------------------*/


int socket_desc;
ICameraSceneNode* cam;

void initialization(){

    //girisBina
    adjacency_list[0].push_back(neighbor(6, 41.00));
	adjacency_list[0].push_back(neighbor(10,46.00)); 			//kisayol
	adjacency_list[0].push_back(neighbor(17,6.43));

    //otopark
    //adjacency_list_dis[1].push_back(neighbor(0, 1.7973247));//distance = 1.7973247
    adjacency_list[1].push_back(neighbor(6, 42.00));//distance = 0.0019053608582103
    adjacency_list[1].push_back(neighbor(9, 41.00));//distance = 0.0019053608582103
	adjacency_list[1].push_back(neighbor(13, 80.00));

    //köprü
    adjacency_list[2].push_back(neighbor(4, 50.00));//distance = 0.0383625878469325
    adjacency_list[2].push_back(neighbor(9, 119.00));//distance = 0.0014761693669766
   
    //kütüphaneKose
    adjacency_list[3].push_back(neighbor(5, 31.32));//distance = 2,77274232484737 e üstlü
    adjacency_list[3].push_back(neighbor(4, 60.00));//distance = 0.007169135024534
    //adjacency_list_dis[3].push_back(neighbor(13, 23.07));										// bu gitcek
	adjacency_list[3].push_back(neighbor(11, 90.00 ));						//istasyona giderken kutuphaneye donus
	
    // kütüphaneCardak
    adjacency_list[4].push_back(neighbor(3, 60.00));//distance = 0.007169135024534
    adjacency_list[4].push_back(neighbor(2, 50.00));//distance = 2,19510819778889 e üssü
	adjacency_list[4].push_back(neighbor(12, 103.43));

    //kütüphane
    adjacency_list[5].push_back(neighbor(3, 31.32));
	adjacency_list[5].push_back(neighbor(15, 60.00));		//kimya kose
	adjacency_list[5].push_back(neighbor(4, 10.00));

    //bina köse(köprüye dogru) 

    adjacency_list[6].push_back(neighbor(0, 41.00));//distance = 0.0169115352644282
    adjacency_list[6].push_back(neighbor(1, 42.00));//distance = 4,950080807421228 e üssü
    adjacency_list[6].push_back(neighbor(9, 30.00));//distance = 0.0014761693669766

    //bina kose(alt gecite dogru) 
    //adjacency_list_dis[7].push_back(neighbor(14, 41.03));//distance = 2.658514622867439 e üstlü
    //adjacency_list_dis[7].push_back(neighbor(11, 69.88));//distance = 0,0042451383958594
	adjacency_list[7].push_back(neighbor(13, 69.88));//distance = 0,0042451383958594
	
	
	//alt gecit 
    adjacency_list[8].push_back(neighbor(10, 40.54 ));			//kisayol
	adjacency_list[8].push_back(neighbor(12, 60.00 ));

    //elektronik bolumune dogru olan kose
    adjacency_list[9].push_back(neighbor(6, 30.00));//distance = 0,0021422476514166
    adjacency_list[9].push_back(neighbor(2, 119.00));//distance
    adjacency_list[9].push_back(neighbor(1, 41.00));//distance
   
    // Alt gecit köse
    //adjacency_list_dis[10].push_back(neighbor(11, 88.83 ));//distance = 0,0021422476514166
    //adjacency_list_dis[10].push_back(neighbor(12, 181.21));//distance
    adjacency_list[10].push_back(neighbor(8, 40.54));//distance
	adjacency_list[10].push_back(neighbor(0, 46.00));//distance
   
    //Bölüm çardak köse
    //adjacency_list_dis[11].push_back(neighbor(0, 45.55));
    //adjacency_list_dis[11].push_back(neighbor(10, 88.83));
	adjacency_list[11].push_back(neighbor(8, 45.55));
	adjacency_list[11].push_back(neighbor(3, 90.00));
   
    //Tren istasyonuna giderken , kütüphaneye dönen köse
    //adjacency_list_dis[12].push_back(neighbor(13, 103.43));
    //adjacency_list_dis[12].push_back(neighbor(10, 181.21));
	adjacency_list[12].push_back(neighbor(4, 103.43));
    adjacency_list[12].push_back(neighbor(8, 60.00 ));

   
    //istasyondan döndükten sonra Kütüphneye gelmek için (12 den sonraki yer)
    //adjacency_list_dis[13].push_back(neighbor(3, 23.07));
    //adjacency_list_dis[13].push_back(neighbor(12, 103.43));
	adjacency_list[13].push_back(neighbor(7, 69.88));
	adjacency_list[13].push_back(neighbor(1, 80.00));
   
    
	//adjacency_list_dis[14].push_back(neighbor(7, 41.03));

	// genetik kose
	adjacency_list[15].push_back(neighbor(5, 60.00));
	adjacency_list[15].push_back(neighbor(16, 20.00));

	// genetik onu
	adjacency_list[16].push_back(neighbor(15, 20.00));
   
	// Bina ici giris
	adjacency_list[17].push_back(neighbor(0, 6.43));
	adjacency_list[17].push_back(neighbor(18, 11.52));
	adjacency_list[17].push_back(neighbor(27, 11.65));

	// Kantin
	adjacency_list[18].push_back(neighbor(17, 11.52));
	adjacency_list[18].push_back(neighbor(19, 8.17));

	// lab - erkek wc
	adjacency_list[19].push_back(neighbor(18, 8.17));
	adjacency_list[19].push_back(neighbor(20, 12.38));

	// z11
	adjacency_list[20].push_back(neighbor(19, 12.38));
	adjacency_list[20].push_back(neighbor(21, 11.28));
	adjacency_list[20].push_back(neighbor(22, 11.32));

	// z06 - robotik
	adjacency_list[21].push_back(neighbor(20, 11.28));
	adjacency_list[21].push_back(neighbor(22, 13.23));

	// z02
	adjacency_list[22].push_back(neighbor(21, 13.23));
	adjacency_list[22].push_back(neighbor(20, 11.32));
	adjacency_list[22].push_back(neighbor(23, 10.85));

	// z04
	adjacency_list[23].push_back(neighbor(22, 10.85));
	adjacency_list[23].push_back(neighbor(24, 11.64));
	adjacency_list[23].push_back(neighbor(25, 11.63));

	// z05 - z10
	adjacency_list[24].push_back(neighbor(23, 11.64));
	adjacency_list[24].push_back(neighbor(25, 10.86));

	// mikro lab
	adjacency_list[25].push_back(neighbor(23, 11.63));
	adjacency_list[25].push_back(neighbor(25, 10.86));
	adjacency_list[25].push_back(neighbor(26, 10.96));

	// devre lab - bayan wc
	adjacency_list[26].push_back(neighbor(25, 10.96));
	adjacency_list[26].push_back(neighbor(27, 11.03));

	// z23
	adjacency_list[27].push_back(neighbor(26, 11.03));
	adjacency_list[27].push_back(neighbor(17, 12.19));



    //giris bina
    coordinates[0][0] = 40.807996;
    coordinates[0][1] = 29.356393;
    //otopark
    coordinates[1][0] = 40.807594;
    coordinates[1][1] = 29.355901;
    //köprü
    coordinates[2][0] = 40.807103;
    coordinates[2][1] = 29.357749;
    //kütüphane Köþe
    coordinates[3][0] = 40.806824;
    coordinates[3][1] = 29.359194;
   
    //kütüphane Çardak
    coordinates[4][0] = 40.806996;
    coordinates[4][1] = 29.358506;
   
    //kütüphane
    coordinates[5][0] = 40.806565;
    coordinates[5][1] = 29.359095;
    //bina kose(köprüye doðru)
    coordinates[6][0] = 40.807626;
    coordinates[6][1] = 29.356423;
    //bina kose(alt gecite dogru)
    coordinates[7][0] = 40.808461; //40.808164
    coordinates[7][1] = 29.355814; //29.356568
   
    //alt gecit
    coordinates[8][0] = 40.808172;
    coordinates[8][1] = 29.357658;
   
    //elektronik bolumune dogru olan kose
    coordinates[9][0] = 40.807389;
    coordinates[9][1] = 29.356345;
   
    // Bölüe gidiş kısa yol
    coordinates[10][0] = 40.808215;
    coordinates[10][1] = 29.356934;
   
    // İstasyona giderken kütüphaneye dönüş
    coordinates[11][0] = 40.807717;
    coordinates[11][1] = 29.359746;
   
    // Tren istasyonuna giderken , kütüphaneye dönen köse
    coordinates[12][0] = 40.806807;
    coordinates[12][1] = 29.359467;
   
    // Bina Arkası
    coordinates[13][0] = 40.808128;
    coordinates[13][1] = 29.355605;   
   
    //Bina arkası köşe
    coordinates[14][0] = 40.808469;
    coordinates[14][1] = 29.355723;

    //Genetik köşe   
    coordinates[15][0] = 40.805911;
    coordinates[15][1] = 29.359178;

    //Genetik önü
    coordinates[16][0] = 40.805570;
    coordinates[16][1] = 29.359070;

	// IC MEKAN //

	// Bina ici giris
    coordinates[17][0] = 40.808008;
    coordinates[17][1] = 29.356321;

	// Kantin
    coordinates[18][0] = 40.807919;
    coordinates[18][1] = 29.356246;

	// Lab - Erkek wc
    coordinates[19][0] = 40.807936;
    coordinates[19][1] = 29.356147;

	// z11
    coordinates[20][0] = 40.807976;
    coordinates[20][1] = 29.355925;

	// z06-robotik
    coordinates[21][0] = 40.807925;
    coordinates[21][1] = 29.355870;

	// z02
    coordinates[22][0] = 40.808044;
    coordinates[22][1] = 29.355910;

	// z04
    coordinates[23][0] = 40.808136;
    coordinates[23][1] = 29.355939;

	// z05-z10
    coordinates[24][0] = 40.808235;
    coordinates[24][1] = 29.355969;

	// mikro lab
    coordinates[25][0] = 40.808171;
    coordinates[25][1] = 29.356068;

	// bayan wc - devre lab
    coordinates[26][0] = 40.808149;
    coordinates[26][1] = 29.356193;

	// z23
    coordinates[27][0] = 40.808119;
    coordinates[27][1] = 29.356318;
}

/******************************** HAZEL'İN FONKSİYONU **********************************/

void objectNode(int node) {

//giriş
    if (node == 0) {  
        circle0 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/circleObj.3ds"));
        circle0->setPosition(core::vector3df(-188.5, 0.0, -13.0));
       

    }
//otopark

    else if (node == 1) {
        circle1 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/circleObj.3ds"));
        circle1->setPosition(core::vector3df(-167.0, 0.0, -32.5));
       
       
    }

// köprü
    else if (node == 2) {

        circle2 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/circleObj.3ds"));
        circle2->setPosition(core::vector3df(-127.8, 0.0, 25.0));
       
       

    }
//kütüphane köşe
    else if (node == 3) {

        circle3 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/circleObj.3ds"));
        circle3->setPosition(core::vector3df(-107.2, 0.0, 102.0));
       

    }

//  kütüphane çardak
    else if (node == 4) {

        circle4 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/circleObj.3ds"));
        circle4->setPosition(core::vector3df(-105.2, 0.0, 85.0));

    }

//kütüphane önü
    else if (node == 5) {

        circle5 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/circleObj.3ds"));
        circle5->setPosition(core::vector3df(-75.5, 0.0, 100.0));
       

       

    }

//bölüm çardak önü
    else if (node == 6) {

        circle6 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/circleObj.3ds"));
        circle6->setPosition(core::vector3df(-160.5, 0.0, -18.88));
       
       

    }

//alt geçide doğru bina köşe
    else if (node == 7) {

        circle7 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/circleObj.3ds"));
        circle7->setPosition(core::vector3df(-225.5, 0.0, -10.0));
       


    }
//alt geçit giriş
    else if (node == 8) {

        circle8 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/circleObj.3ds"));
        circle8->setPosition(core::vector3df(-230.5, 0.0, 50.0));
       


    }
// köprüye dönüş köşe
    else if (node == 9) {

        circle9 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/circleObj.3ds"));
        circle9->setPosition(core::vector3df(-132.0, 0.0, -22.38));
       

    }


// bölüme gidiş kısa yol
    else if (node == 10) {

        circle10 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/circleObj.3ds"));
        circle10->setPosition(core::vector3df(-223.5, 0.0, 13.0));
       


    }
// istasyona giderken kütüphaneye dönüş
    else if (node == 11) {

        circle11 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/circleObj.3ds"));
        circle11->setPosition(core::vector3df(-210.5, 0.0, 122.0));
       


    }

//
    else if (node == 12) {

        circle12 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/circleObj.3ds"));
        circle12->setPosition(core::vector3df(-115.5, 0.0, 106.0));
       

    }
//bina arkası
    else if (node == 13) {

        circle13 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/circleObj.3ds"));
        circle13->setPosition(core::vector3df(-200.5, 0.0, -55.0));
       

    }
//bina arkası köşe
    else if (node == 14) {

        circle14 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/circleObj.3ds"));
        circle14->setPosition(core::vector3df(-230.5, 0.0, -30.0));
       

    }
// genetik köşe
    else if (node == 15) {

        circle15 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/circleObj.3ds"));
        circle15->setPosition(core::vector3df(-36.5, 0.0, 90.0));
       
    }
// genetik önü
    else if (node == 16) {

        circle16 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/circleObj.3ds"));
        circle16->setPosition(core::vector3df(-10.5, 0.0, 90.0));
    }

	// bölüm giriş
    else if(node == 17) {
      
      anmsRobotm1 = smgr -> addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/square.3ds"));
      anmsRobotm1->setPosition(core::vector3df(0.5, 5.5, -28.0));
      anmsRobotm1->setMaterialTexture(0 , driver->getTexture("../../media/red.jpg")); 
    
    }

// kantin    
     else if(node == 18) {
      
      anmsRobotm2 = smgr -> addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/square.3ds"));
      anmsRobotm2->setPosition(core::vector3df(-9.8, 5.5, -23.0));
      anmsRobotm2->setMaterialTexture(0 , driver->getTexture("../../media/red.jpg")); 
    
    }
// lab -erkek wc    
   else if(node == 19) {
      
      anmsRobotm3 = smgr -> addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/square.3ds"));
      anmsRobotm3->setPosition(core::vector3df(-9.8, 5.5, -8.0));
      anmsRobotm3->setMaterialTexture(0 , driver->getTexture("../../media/red.jpg")); 
    
    }  
    
// z11    
   else if(node == 20) {
      
      anmsRobotm4 = smgr -> addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/square.3ds"));
      anmsRobotm4->setPosition(core::vector3df(-9.8, 5.5, 6.0));
      anmsRobotm4->setMaterialTexture(0 , driver->getTexture("../../media/red.jpg")); 
    
    } 
// z06-robotik   
   else if(node == 21) {
      
      anmsRobotm5 = smgr -> addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/square.3ds"));
      anmsRobotm5->setPosition(core::vector3df(-13, 5.5, 12.0));
      anmsRobotm5->setMaterialTexture(0 , driver->getTexture("../../media/red.jpg")); 
    
    }
// z02   
   else if(node == 22) {
      
      anmsRobotm6 = smgr -> addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/square.3ds"));
      anmsRobotm6->setPosition(core::vector3df(-3.0, 5.5, 13.0));
      anmsRobotm6->setMaterialTexture(0 , driver->getTexture("../../media/red.jpg")); 
    
    }
    
// z04   
   else if(node == 23) {
      
      anmsRobotm7 = smgr -> addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/square.3ds"));
      anmsRobotm7->setPosition(core::vector3df(5.0, 5.5, 13.0));
      anmsRobotm7->setMaterialTexture(0 , driver->getTexture("../../media/red.jpg")); 
    
    }
        
// z05 - z10   
   else if(node == 24) {
      
      anmsRobotm8 = smgr -> addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/square.3ds"));
      anmsRobotm8->setPosition(core::vector3df(14.0, 5.5, 12.0));
      anmsRobotm8->setMaterialTexture(0 , driver->getTexture("../../media/red.jpg")); 
    
    }
// mikro lab  
   else if(node == 25) {
      
      anmsRobotm9 = smgr -> addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/square.3ds"));
      anmsRobotm9->setPosition(core::vector3df(11.0, 5.5, 6.5));
      anmsRobotm9->setMaterialTexture(0 , driver->getTexture("../../media/red.jpg")); 
    
    } 
 // bayan wc-devre lab 
   else if(node == 26) {
      
      anmsRobotm10 = smgr -> addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/square.3ds"));
      anmsRobotm10->setPosition(core::vector3df(11.0, 5.5, -9.0));
      anmsRobotm10->setMaterialTexture(0 , driver->getTexture("../../media/red.jpg")); 
    
    } 
    
 // bayan wc-devre lab 
   else if(node == 27) {
      
      anmsRobotm11 = smgr -> addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/square.3ds"));
      anmsRobotm11->setPosition(core::vector3df(11.0, 5.5, -22.5));
      anmsRobotm11->setMaterialTexture(0 , driver->getTexture("../../media/red.jpg")); 
    
    }      
   
}
void setActiveCamera(scene::ICameraSceneNode* newActive, SAppContext context)
{
    if (0 == context.device)
        return;
    scene::ICameraSceneNode * active = context.device->getSceneManager()->getActiveCamera();
    active->setInputReceiverEnabled(false);
    newActive->setInputReceiverEnabled(true);
    context.device->getSceneManager()->setActiveCamera(newActive);
}
/*
Set the skin transparency by changing the alpha values of all skin-colors
*/
void setSkinTransparency(s32 alpha, irr::gui::IGUISkin * skin)
{
    for (s32 i = 0; i<irr::gui::EGDC_COUNT; ++i)
    {
        video::SColor col = skin->getColor((EGUI_DEFAULT_COLOR)i);
        col.setAlpha(alpha);
        skin->setColor((EGUI_DEFAULT_COLOR)i, col);
    }
}

/**********************************************************************************************/
void signal_callback_handler(int signum)
{
   cout << "\n\tCaught signal " << signum << " !\n";
   // Cleanup and close up stuff here
   // Terminate program
   close(socket_desc);
   exit(signum);
}

void DijkstraComputePaths(vertex_t source,
    const adjacency_list_t &adjacency_list,
    std::vector<weight_t> &min_distance,
    std::vector<vertex_t> &previous)
{
    int n = adjacency_list.size();
    min_distance.clear();
    min_distance.resize(n, max_weight);
    min_distance[source] = 0;
    previous.clear();
    previous.resize(n, -1);
    std::set<std::pair<weight_t, vertex_t> > vertex_queue;
    vertex_queue.insert(std::make_pair(min_distance[source], source));

    while (!vertex_queue.empty())
    {
        weight_t dist = vertex_queue.begin()->first;
        vertex_t u = vertex_queue.begin()->second;
        vertex_queue.erase(vertex_queue.begin());

        // Visit each edge exiting u
        const std::vector<neighbor> &neighbors = adjacency_list[u];
        for (std::vector<neighbor>::const_iterator neighbor_iter = neighbors.begin();
            neighbor_iter != neighbors.end();
            neighbor_iter++)
        {
            vertex_t v = neighbor_iter->target;
            weight_t weight = neighbor_iter->weight;
            weight_t distance_through_u = dist + weight;
            if (distance_through_u < min_distance[v]) {
                vertex_queue.erase(std::make_pair(min_distance[v], v));

                min_distance[v] = distance_through_u;
                previous[v] = u;
                vertex_queue.insert(std::make_pair(min_distance[v], v));

            }
        }
    }
}

std::list<vertex_t> DijkstraGetShortestPathTo(
    vertex_t vertex, const std::vector<vertex_t> &previous)
{
    std::list<vertex_t> path;
    for (; vertex != -1; vertex = previous[vertex])
        path.push_back(vertex);
    return path;
}

double radians(double degree){
    return degree * M_PI / 180.0;
}

double degrees(double radian){
    return radian * 180.0 / M_PI;
}

double angleFromCoordinate(double lat1, double long1, double lat2,
        double long2) {

    double startLat = radians(lat1);
    double startLong = radians(long1);

    double endLat = radians(lat2);
    double endLong = radians(long2);

    double dLong = endLong - startLong;

    double dPhi = log(tan(endLat/2.0+M_PI/4.0) / tan(startLat/2.0+ M_PI/4.0));

    if (fabs(dLong) > M_PI){
         if (dLong > 0.0)
             dLong = -(2.0 * M_PI - dLong);
         else
             dLong = (2.0 * M_PI + dLong);
    }

    double bearing = fmodf((degrees(atan2(dLong, dPhi)) + 360.0) , 360.0);

    return bearing;
}

bool isInSpot(double clientPosX, double clientPosY, double destX, double destY){
   
    cout.setf(ios::fixed,ios::floatfield);
    cout.precision(6);
    cout << "clientPosX: " << clientPosX << endl;
    cout << "clientPosY: " <<  clientPosY << endl;
    cout << "destX: " << destX << endl;
    cout << "destY: " << destY << endl;

	if((clientPosX <= destX + 0.000120 && clientPosX >= destX - 0.000120) && (clientPosY <= destY + 0.000120 && clientPosY >= destY - 0.000120))
		return true;
	return false;	
}
void cameraInitialization(){
	Device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(1280, 800), 16);
    Device->setResizable(true);
    driver = Device->getVideoDriver();
    env = Device->getGUIEnvironment();
    smgr = Device->getSceneManager();
    driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);

    video::SColorf color(0.9f, 0.9f, 0.9f, 0.9f);
    //video::SColorf color2(0,0,0);
    float light = 300.f;
    float lightDegree = 300;
    smgr->addLightSceneNode(0, core::vector3df(lightDegree, 90, lightDegree), color, light, -1);
    smgr->addLightSceneNode(0, core::vector3df(-lightDegree, 90, lightDegree), color, light, -1);
    smgr->addLightSceneNode(0, core::vector3df(-lightDegree, 90, -lightDegree), color, light, -1);
    smgr->addLightSceneNode(0, core::vector3df(lightDegree, 90, -lightDegree), color, light, -1);
    // font büyüklüğünü ayarlıyor
    skin = env->getSkin();
    font = env->getFont("../../media/fontlucida.png");
    if (font)
        skin->setFont(font);

    for (s32 i = 0; i<gui::EGDC_COUNT; ++i)
    {
        video::SColor col = env->getSkin()->getColor((gui::EGUI_DEFAULT_COLOR)i);
        col.setAlpha(255);
        env->getSkin()->setColor((gui::EGUI_DEFAULT_COLOR)i, col);
    }



    root = env->getRootGUIElement();
    e = root->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
    if (e)
        e->remove();
    // create the toolbox window
   wnd = env->addWindow(core::rect<s32>(700, 45, 1100, 700),
		false, L"MENU", 0, GUI_ID_DIALOG_ROOT_WINDOW);
	// create tab control and tabs
	tab = env->addTabControl(
		core::rect<s32>(0, 20, 900 - 502, 700 - 7), wnd, true, true);
	t1 = tab->addTab(L"           ~~~~~~  SELECT A REGITION  ~~~~~~                       ");

	//env->addButton(core::rect<s32>(65, 80, 115, 130), t1, GUI_ID_STOP, L"STOP");
	env->addButton(core::rect<s32>(100, 460, 250, 500), t1, GUI_ID_GO, L"GO");
	
	env->addButton(core::rect<s32>(100, 505, 250, 545), t1, GUI_ID_QUIT, L"QUIT");

	env->addStaticText(L"START POSITION", core::rect<s32>(15, 70, 150, 220), false, true, t1, true);
	env->addStaticText(L"END POSITION", core::rect<s32>(200, 70, 350, 220), false, true, t1, true);

	listbox1 = env->addListBox(core::rect<s32>(0, 110, 150, 400), t1); // start position
	listbox2 = env->addListBox(core::rect<s32>(200, 110, 350, 400), t1); // end position
	
	listbox1->addItem(L"Bolum Cikis");
	listbox1->addItem(L"Otopark");
	listbox1->addItem(L"Kopru");
	listbox1->addItem(L"Kutuphane Kose");
	listbox1->addItem(L"Kutuphane Cardak");
	listbox1->addItem(L"Kutuphane Onu");
	listbox1->addItem(L"Bina kose(köprüye dogru)"); // bölüm cardak önu
	listbox1->addItem(L"Bina kose(alt gecite dogru)");
	listbox1->addItem(L"Alt gecit");
	listbox1->addItem(L"Kopru Kose");
	listbox1->addItem(L"Bolum kisayol");
	listbox1->addItem(L"Kutuphaneye donus");
	listbox1->addItem(L"Tren istasyonundan kutuphane");
	listbox1->addItem(L"Bina Arkasi");
	listbox1->addItem(L"Bina Arka Kose");
	listbox1->addItem(L"Genetik Kose");
	listbox1->addItem(L"Genetik Onu");
	listbox1->addItem(L"Bolum Giris");
	listbox1->addItem(L"Kantin");
	listbox1->addItem(L"lab-erkek wc");
	listbox1->addItem(L"z11");
	listbox1->addItem(L"z06-robotik");
	listbox1->addItem(L"z02");
	listbox1->addItem(L"z04");
	listbox1->addItem(L"z05-z10");
	listbox1->addItem(L"micro lab");
	listbox1->addItem(L"devrelab-bayan wc");
	listbox1->addItem(L"z23");
	
	
	
	listbox2->addItem(L"Bolum Cikis");
	listbox2->addItem(L"Otopark");
	listbox2->addItem(L"Kopru");
	listbox2->addItem(L"Kutuphane Kose");
	listbox2->addItem(L"Kutuphane Cardak");
	listbox2->addItem(L"Kutuphane Onu");
	listbox2->addItem(L"Bina kose(köprüye dogru)"); // bölüm cardak önu
	listbox2->addItem(L"Bina kose(alt gecite dogru)");
	listbox2->addItem(L"Alt gecit");
	listbox2->addItem(L"Kopru Kose");
	listbox2->addItem(L"Bolum kisayol");
	listbox2->addItem(L"Kutuphaneye donus");
	listbox2->addItem(L"Tren istasyonundan kutuphane");
	listbox2->addItem(L"Bina Arkasi");
	listbox2->addItem(L"Bina Arka Kose");
	listbox2->addItem(L"Genetik Kose");
	listbox2->addItem(L"Genetik Onu");
	listbox2->addItem(L"Bolum Giris");
	listbox2->addItem(L"Kantin");
	listbox2->addItem(L"lab-erkek wc");
	listbox2->addItem(L"z11");
	listbox2->addItem(L"z06-robotik");
	listbox2->addItem(L"z02");
	listbox2->addItem(L"z04");
	listbox2->addItem(L"z05-z10");
	listbox2->addItem(L"micro lab");
	listbox2->addItem(L"devrelab-bayan wc");
	listbox2->addItem(L"z23");
   
    meshes[0] = smgr->getMesh("../../media/model/Kat0/zemin_asama_1.obj");
    meshes[1] = smgr->getMesh("../../media/model/model.3ds");
    anms[0] = smgr->addAnimatedMeshSceneNode(meshes[0]);
    anms[1] = smgr->addAnimatedMeshSceneNode(meshes[1]);
    
    anms[0]->setVisible(false);
    anms[1]->setVisible(false);  
	    

    indoor= smgr->addAnimatedMeshSceneNode(smgr->getMesh("../../media/model/Kat0/zemin.3ds"));
    anmsRobot = smgr->addEmptySceneNode();
    
	indoor -> setVisible(false);
    indoor->setParent(anmsRobot);
    indoor->setPosition(core::vector3df(-196.5, 0.0, -35.0));
   
    float ax = 30, ay = 30, u = 5;
    int lastFPS = -1;
    //getPath();
    int k = 0;
    float dx = cos(ay)*cos(ax)*u;
    float dy = sin(ay)*u;
    float dz = cos(ay)*sin(ax)*u;
    


    context.device = Device;
    context.counter = 0;
    context.listbox1 = listbox1;
    context.listbox2 = listbox2;
    context.anmsRobot = anmsRobot;
   

    cam = smgr->addCameraSceneNodeMaya(0, -1500.0F,-1000.0F, -200.0F, -1, 50.0F);
   // cam = smgr->addCameraSceneNode(0 , core::vector3df(0,10,-10),  core::vector3df(0,-10,10));
} 

bool isInOutdoor(int node){
	if(node < 17)
		return 1;
	return 0;
}

int main(int argc , char *argv[])
{
    int  client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[250];
  	initialization();
	cameraInitialization();

     // Register signal and signal handler
    signal(SIGINT, signal_callback_handler);
 
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    puts("Socket created");
    
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8001);
    
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
        perror("bind failed. Error");
    
    puts("bind done");
    
    //Listen
    listen(socket_desc , 3);
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    MyEventReceiver receiver(context);
    Device->setEventReceiver(&receiver);
	cout << "flag1" << flag1 << endl;

    while(1){
		cout << "Ilk " << endl;
		while(Device->run()  && driver && flag1<1){

				bool movedWithJoystick = false;

				cam->setTarget(anmsRobot->getPosition());

				cam->setPosition(core::vector3df(0.827389 , 153.21 , -101.157));
	
				driver->beginScene(true, true, video::SColor(150, 50, 50, 50));
				 
				if (context.startPos > 16) {
					anms[0]->setVisible(true);
					anms[1]->setVisible(false);
					indoor->setVisible(false);
				}
				else if(context.startPos>-1 && context.startPos < 17) {
					anms[1]->setVisible(true);
					anms[0]->setVisible(false);
					indoor->setVisible(true);
				}

				smgr->drawAll(); // draw the 3d scene
				env->drawAll();
				driver->endScene();
	
		}

		flag1=1;
		cout<<"whiledan cıktım"<<"\n";
		cout<<"node : "<<context.endPos<<"\n";

		std::vector<weight_t> min_distance;
		std::vector<vertex_t> previous;
		DijkstraComputePaths(context.endPos, adjacency_list, min_distance, previous); // Varis noktasi
		std::cout << "Distance : " << min_distance[context.endPos] << std::endl;
		std::list<vertex_t> path = DijkstraGetShortestPathTo(context.startPos, previous); // Baslangic noktasi
		std::cout << "Path : ";
		std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
		std::cout << std::endl;

		//accept connection from an incoming client
		if(counterFlag==0)
		{	
			client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);

		if (client_sock < 0)
		{
			perror("accept failed");
			return 1;
		}
		puts("Connection accepted");
			
		}


		/*------------------------------------------------------------------------*/
			vertex_t current, dest=0;
			int counter=0, forwardFlag =0;
			double angle, givenAngle, diffAngle = 0 ,clientPosX, clientPosY;
		   
			std::list<vertex_t>::iterator it = path.begin();
			//std::advance( it, n );
		/*------------------------------------------------------------------------*/
		//Receive a message from client
	   
		char buffer [33];
		char flag[2];
		char chPDifAngle[5];
		char* split;
		int rotated = 0;
		int isForwarded = 0;

		int arr[100];
		for(int k=0;k<100;k++)
		{
			arr[k]=-1;
		}
		for(std::list<int>::iterator it1=path.begin();it1!=path.end();++it1)
		{
			arr[counter] = *it1;
			counter++;
		}
	   
		int indoor_location;
	   
		for(int i = 0; i < counter; i++)
			cout << "arr: " << i << " " << arr[i] << endl;

		counter = 0;

		objectNode(arr[0]);
	
			  /****************************** HAZELİN WHILE'I ********************************/

		if(counterFlag == 0)
			recv(client_sock , client_message , 250 , 0);

		

		int turningFlag=0;
		if(counterFlag == 0){
			if(isInOutdoor(arr[1])){	// Dis mekanda ise
				snprintf(flag, sizeof(flag), "%d", FLAG_ENGINE_ROTATE);
				cout << "flag = " << flag << endl;
				strcat(buffer, flag);
				cout << "buffer: " << buffer << endl;
			 	cout <<"TO CLIENT ROTATE INF : " << write(client_sock , buffer, strlen(buffer)) << " \n";
				cout << "dis mekan\n";
			}else{
				snprintf(flag, sizeof(flag), "%d", FLAG_ENGINE_WIFI);
				cout << "flag = " << flag << endl;
				strcat(buffer, flag);
				cout << "buffer: " << buffer << endl;
			 	cout <<"TO CLIENT ROTATE INF : " << write(client_sock , buffer, strlen(buffer)) << " \n";
				cout << "ic mekan\n";
			}
			sleep(2);
		}

		counterFlag++;		
	
		while (Device->run()  && driver)
		{
			cout<<"2.while girdim \n";
			cam->setTarget(anmsRobot->getPosition());
			cam->setRotation(dif);
			  driver->beginScene(true, true, video::SColor(150, 50, 50, 50));
			
			cout << "Device is active" << endl;
			if(nodeFlag == 1){	// Vardiysa yak
				cout << "Node counter: " << counter<< endl;
				objectNode(arr[counter]);
				if(arr[counter] > 16){
					anms[0] -> setVisible(true);
					anms[1] -> setVisible(false);
					indoor -> setVisible(false);
				}
				else{
					anms[0] -> setVisible(false);
					anms[1] -> setVisible(true);
					indoor -> setVisible(true);
				}
			}       

			// Turn lighting on and off depending on whether the left mouse button is down.
			// node->setMaterialFlag(video::EMF_LIGHTING, receiver.GetMouseState().LeftButtonDown);
			core::position2d<s32> m = Device->getCursorControl()->getPosition();
			driver->draw2DRectangle(video::SColor(100,255,255,255),
			core::rect<s32>(m.X-20, m.Y-20, m.X+20, m.Y+20));
			driver->beginScene(true, true, video::SColor(255, 113, 113, 133));
			smgr->drawAll(); // draw the 3d scene
			driver->endScene();

			if(dest == -1){
				cout << "Son node" << endl;
				flag1 = -1;
				break;
			}

			current = arr[counter]; 
			dest    = arr[counter+1];
			cout << "current : ";
			cout << current << endl;
			cout << "dest : ";
			cout << dest << endl;

			buffer[0] = '\0';
			if((read_size = recv(client_sock , client_message , 250 , 0)) > 0){
				if(isInOutdoor(dest)){	// dis mekan bilgileri 
					cout <<"Client message disari: " << client_message << endl;
					split=strtok(client_message," ");
					cout << "Enlem: " << split << endl;       
					clientPosX = atof(split);
					split=strtok(NULL," ");
					cout << "Boylam: " << split << endl;
					clientPosY = atof(split);
					split=strtok(NULL," ");
					cout << "Aci: " <<  split << endl;
					givenAngle = atof(split);
				}else{	// ic mekan bilgileri
					cout <<"Client message iceri: " << client_message << endl;
					split=strtok(client_message," ");
					indoor_location = atof(split);	// Bulundugu bolge
					cout << "Iceri bulundugu bolge: " << indoor_location << endl;
					split=strtok(NULL," ");
					cout << "Aci: " <<  split << endl;
					givenAngle = atof(split);	// aci
				}
		   
			}else{
				cout <<"Server couldn't read from Client\n";
			}

			if(forwardFlag == 0){cout << "\nforwardFlag 0 \n";
				angle = angleFromCoordinate(coordinates[current][0], coordinates[current][1],
					                        coordinates[dest][0], coordinates[dest][1]);
				cout << "-------------------------" << endl;
				cout << "angle : " << angle << endl;
				cout << "givenAngle : " << givenAngle << endl;
				if(!((angle < givenAngle + 15) && (angle > givenAngle - 15) )){
					if(diffAngle == 0){
						turningFlag = 1;
					    diffAngle = givenAngle - angle;
					    if(diffAngle > 180){ // tumleyenini alip eksilisini gonder
					        diffAngle = 360 - diffAngle;
					        diffAngle = 0 - diffAngle;
					    }
					    if(diffAngle < -180){ // tumleyenini alip eksilisini gonder
					        diffAngle = -360 - diffAngle;
					        diffAngle = 0 - diffAngle;
					    }

						if(!isInOutdoor(dest)){ // ic mekanda ise
							snprintf(flag, sizeof(flag), "%d", 5);
							strcat(buffer, flag);
							strcat(buffer, " ");
						}

					    //Client a FLAG_ENGINE_ROTATE i gonder
					    snprintf(flag, sizeof(flag), "%d", FLAG_ENGINE_ROTATE);
					    cout << "flag" << flag << endl;
					    snprintf(chPDifAngle, sizeof(chPDifAngle), "%f", diffAngle);
					    strcat(buffer, flag);
					    strcat(buffer, " ");
					    strcat(buffer, chPDifAngle);
					    cout << "buffer: " << buffer << endl;
					    cout <<"TO CLIENT ROTATE INF 0 : " << write(client_sock , buffer, strlen(buffer)) << " \n";
						// mesaj: 0 "donmesi gereken aci miktari"
					}
					else{
						if(!isInOutdoor(dest)){ // ic mekanda ise
							snprintf(flag, sizeof(flag), "%d", 5);
							strcat(buffer, flag);
							strcat(buffer, " ");
						}
						snprintf(flag, sizeof(flag), "%d", FLAG_ENGINE_ROTATE);
						strcat(buffer, flag);
						cout <<"TO CLIENT ROTATE INF 0_2 : " << write(client_sock , buffer, strlen(buffer)) << " \n";
					}
				}
				else{
					forwardFlag = 1;	
				}
			}
			if(forwardFlag == 1){cout << "\nforwardFlag 1 \n";
				buffer[0] = '\0';
				int stillMoving=1;
				if(isInOutdoor(dest)){
					if(isInSpot(clientPosX, clientPosY, coordinates[dest][0], coordinates[dest][1]))
						stillMoving = 0;
				}else{
					if(indoor_location == dest && isForwarded == 1){
						stillMoving = 0;
					}
				}
				
				if(stillMoving){  
					cout << "still moving\n";
					if(!isInOutdoor(dest)){ // ic mekanda ise
						snprintf(flag, sizeof(flag), "%d", 5);
						strcat(buffer, flag);
						strcat(buffer, " ");
					}

					snprintf(flag, sizeof(flag), "%d", FLAG_ENGINE_FORWARD);
					strcat(buffer, flag);
					if(turningFlag == 1){	// Aciyi eski haline getir ve ileri git
						// Client a FLAG_ENGINE_FORWARD i gonder
						diffAngle = 0 - diffAngle;
						snprintf(chPDifAngle, sizeof(chPDifAngle), "%f", diffAngle);
						strcat(buffer, " ");
						strcat(buffer, chPDifAngle);
						cout <<"TO CLIENT ROTATE INF 1 ACI: " << write(client_sock , buffer, strlen(buffer)) << " \n"; // mesaj : 1 "aci"
						isForwarded = 1;
						diffAngle = 0;
						turningFlag=0;
					}
					else if(isForwarded == 0){	// Sadece ileri hareketi
						// Client a FLAG_ENGINE_FORWARD i gonder
						snprintf(chPDifAngle, sizeof(chPDifAngle), "%f", 1.0);
						strcat(buffer, " ");
						strcat(buffer, chPDifAngle); // 1 gonderilecek 
						cout <<"TO CLIENT ROTATE INF 1 1: " << write(client_sock , buffer, strlen(buffer)) << " \n"; // mesaj: 1 1
						isForwarded = 1;
					}
					else{	// Hic bisey yapma
						cout <<"TO CLIENT ROTATE INF 1: " << write(client_sock , buffer, strlen(buffer)) << " \n"; // mesaj: 1
					}
					   
				}
				else{
					cout << "REACHED DEST!" << endl;
					forwardFlag =2;
				}
			}  
			if(forwardFlag == 2){cout << "\nforwardFlag 2 \n";
				buffer[0] = '\0';
				nodeFlag=1;
				counter += 1;

				current = arr[counter];
				dest    = arr[counter+1];
			   
				angle = angleFromCoordinate(coordinates[current][0], coordinates[current][1],
					                        coordinates[dest][0], coordinates[dest][1]);
				cout << "-------------------------" << endl;
				cout << "angle 2: " << angle << endl;
				cout << "givenAngle 2: " << givenAngle << endl;
				if(!((angle < givenAngle + 15) && (angle > givenAngle - 15) )){	// Vardiktan sonra acisi bir sonraki 
																			    // node a gore duzgun degil ise
					if(!isInOutdoor(dest)){ // ic mekanda ise
						snprintf(flag, sizeof(flag), "%d", 5);
						strcat(buffer, flag);
						strcat(buffer, " ");
					}

					snprintf(flag, sizeof(flag), "%d", FLAG_ENGINE_STOP);
					strcat(buffer,flag);
					strcat(buffer, " ");
					snprintf(chPDifAngle, sizeof(chPDifAngle), "%f", 2.0);
					strcat(buffer, chPDifAngle); 
					cout <<"TO CLIENT ROTATE INF 2-2: " << write(client_sock , buffer, strlen(buffer)) << " \n";	// mesaj: 2 2
					isForwarded = 0;
					forwardFlag = 0;
					sleep(2);
				}else{
					forwardFlag = 1; // Acisi duzgun ileri sinyali gondermeye devam et
					snprintf(flag, sizeof(flag), "%d", FLAG_ENGINE_STOP);
					strcat(buffer,flag);
					cout <<"TO CLIENT ROTATE INF 2: " << write(client_sock , buffer, strlen(buffer)) << " \n";	// mesaj: 2
					sleep(2);
				}
			}
			sleep(2);
			
	    }
		cout <<"\n Server read "<< read_size << " character \n";
		
	}
}

