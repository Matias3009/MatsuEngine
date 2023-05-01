#pragma once
#include <cmath>
//#include <fstream>


#include <ctime>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Network.hpp>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>



//#include <functional>
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>

#include <stdlib.h>
#include <thread>
#include <algorithm>
#include <future>

#define PI 3.14159265358979323846


using namespace std;

using namespace sf;

Vector2f normalized(Vector2f f) {

	float ang = atan2f(f.y, f.x);
	Vector2f vec(cos(ang), sinf(ang));
	return vec;

}



Vector2f quadrate(Vector2f g) {
	Vector2f n(0, 0);
	n.x = 1 / 2 * (sqrtf(2 + powf(g.x, 2) - powf(g.y, 2) + 2 * g.x * sqrtf(2))) - 1 / 2 * (sqrtf(2 + powf(g.x, 2) - powf(g.y, 2) - 2 * g.x * sqrtf(2) ) );
	n.y = 1 / 2 * (sqrtf(2 - powf(g.x, 2) + powf(g.y, 2) + 2 * g.y * sqrtf(2))) - 1 / 2 * (sqrtf(2 - powf(g.x, 2) + powf(g.y, 2) - 2 * g.y * sqrtf(2)));
	return n;
}

float distanceOf(Vector2f n, Vector2f k) {
	float y = sqrtf(powf(k.x - n.x, 2) + powf(k.y - n.y, 2));
	return y;
}

float ClampD(float v, float minD, float maxD) {
	if (v < minD) {
		v = minD;
	}
	else if (v > maxD) {
		v = maxD;
	}
	return v;
}

Vector2f direcOf(float angle) {
	return Vector2f(cos(angle), sinf(angle));
}
float angleOf(Vector2f f) {
	return atan2f(f.y, f.x);
}
Vector2f LerpVector(Vector2f i, Vector2f h, float t) {
	Vector2f k(0, 0);
	k.x = i.x + (t * (h.x - i.x));
	k.y = i.y + (t * (h.y - i.y));
	return k;
}

float Lrp(float f, float f2, float ax) {
	return f + (ax * (f2 - f));

}
float LrpRad(float f, float f2, float ax) {//NO Funciona jajja
	float x = f + (ax * (f2 - f));
	float f3 = PI - f2;
	return x;

}

float rad2Deg = 180 / PI;
float deg2Rad = PI / 180;

Vector2f multiply(Vector2f vec, float num) {
	Vector2f vec1(vec.x*num, vec.y*num);
	return vec1;
}

Vector2f Suma(Vector2f vec1, Vector2f vec2) {
	Vector2f vec3(vec1.x + vec2.x, vec1.y + vec2.y);
	return vec3;

}
Vector2f Resta(Vector2f vec1, Vector2f vec2) {
	Vector2f vec3(vec1.x - vec2.x, vec1.y - vec2.y);
	return vec3;

}

class CameraD {
public:
	static Vector2f position;

};
Vector2f CameraD::position = Vector2f(0,0);

class TransformD {
public:
	Vector2f position;
	Vector2f pivot = Vector2f(0.5f,0.5f);
	float rotation = 0;
	Vector2f scale;

	/*Vector2f getCenter() {
		Vector2f vec1(position.x + (scale.x / 2), position.y + (scale.y / 2));
		return vec1;
	}
	void translateInCenter(Vector2f f) {
		position = Vector2f(f.x-(scale.x/2), f.y - (scale.y / 2));
	}*/
};





class GameObject {
public:
	class componentD {
	public:
		GameObject* gameobj;
		virtual void OnaddComponent(GameObject* g) {};
		virtual void onBack() {};
	};
	
	int zLayer = 0;
	TransformD* transform;
	string name;
	string tag = "";
	bool active=true;

	

	vector<componentD*> components;
	GameObject(string name = "NN") {
		this->name = name;
		this->transform = new TransformD();
		this->transform->position = Vector2f(0, 0);
		this->transform->rotation = 0;
		this->transform->scale = Vector2f(1, 1);
		
	}
	
	
	void addComponent(GameObject :: componentD* d) {
		d->OnaddComponent(this);
		components.push_back(d);
	}

	void Onback() {
		for (int i = 0; i < components.size(); i++)
		{
			components.at(i)->onBack();
		}
	}
	virtual void EventCall(int id, int arg) {};
	virtual void Update() {};
	virtual void Start() {};
	virtual void Awake() {};
	/// <summary>
	/// se llama a esta funcion cuando el componente
	/// fisico asociado a este gameobject es actaulizado.
	/// </summary>
	/// <param name="contact"> booleano de si toco o no</param>
	/// <param name="contactObj"> el objeto con el cual entro en contacto </param>
	/// <param name="dirContact"> la direccion de contacto </param>
	virtual void PsysicsUpdate(bool contact, GameObject *contactObj, Vector2f dirContact) {};

};

class PoolObject : public GameObject{
public:
	virtual void startPool() {};
	virtual void backPool() {};

	PoolObject(string g) : GameObject(g) {

	}


};


class socketAdmin : public GameObject {
public:

	TcpListener* tcp;
	TcpSocket socket;
	unsigned short port;

	socketAdmin(string g, unsigned short portD) : GameObject(g){
		tcp = new TcpListener();

		this->port = portD;
		if (tcp->listen(portD) != Socket::Done) {
			//Error
		}
		//TcpSocket socket;
		


	}

	void init() {
		if (tcp->accept(socket) != Socket::Done) {
			//Error
		}
		cout << "conectado a " << socket.getRemoteAddress() << endl;
	}
	void send(void* n) {
		
		socket.send(n, sizeof(n));
		cout << "enviado" << endl;
	}

	template<class d>
	d receive() {
		d* f = new d();
		size_t receiv;
		socket.receive(f, sizeof(f), receiv);
		return f;
	}
	

};

class socketClient : public GameObject {
public:
	IpAddress* add;
	TcpSocket socket;
	unsigned short t;
	socketClient(string g, unsigned short f) : GameObject(g) {
		t = f;
	}

	void setAddress(IpAddress* addd) {
		this->add = addd;
	}
	void createSocket() {
		if (socket.connect(*add, t) != Socket::Done) {
			//Error
		}
		cout << "connected" << endl;
	}
	void send(void* n) {

		if (socket.send(n, sizeof(n)) != Socket::Done) {
			//error
		}
		cout << "enviado" << endl;
	}
};






class Scene {
public:

	static Scene* escenaMain;
	Clock* delta;
	bool run = false;
	int nIndex = 0;
	int IndexMe = 0;
	RenderWindow* window;
	vector<GameObject *> SceneObjects;
	enum Anchors{ArribaIzquierda,AbajoIzquierda,ArribaDerecha,AbajoDerecha,Centro};

	Event eScene;
	//vector<RectangleShape*> rectangles;

	bool inDelete = false;
	//int objetoDel;
	vector<GameObject*> objetoDel;

	bool showFps = false;

	void ChangeScene(int ti) {
		nIndex = ti;
	}

	void Ordenar() {

		sort(SceneObjects.begin(), SceneObjects.end(), [](GameObject* x, GameObject* y) {return x->zLayer < y->zLayer; });
	}

	void Exit() {
		this->window->close();
	}

	Vector2f cameraAnchor() {
		Vector2f j = Vector2f();
		j = window->getView().getCenter();
		j.x -= window->getView().getSize().x / 2;
		j.y -= window->getView().getSize().y / 2;

		return j;
	}

	void TranslateCamera(Vector2f f) {
		View v = window->getView();
		v.move(f);
		window->setView(v);
	}
	void setPositionCam(Vector2f f) {
		View v = window->getView();
		v.setCenter(f);
		window->setView(v);
	}

	void activeShowFps(bool t) {
		showFps = t;
	}
        /**
         * añadir gameobject a escena
         * @param GameObject* Objeto
         */
	void Instantiate(GameObject* Objeto) {
		SceneObjects.push_back(Objeto);
		Ordenar();
		//Objeto->Start();
	}
	void Delete(GameObject* Objeto) {
		
			
		objetoDel.push_back(Objeto);
			//cout << objetoDel.size() << endl;
		inDelete = true;
		
		//objetoDel = this->find(Objeto);
	}

	float getDelta() {
		return delta->getElapsedTime().asSeconds();
	}

	

	Vector2f getAnchor(Anchors anch) {

		switch (anch)
		{
		case Scene::ArribaIzquierda:
			return Vector2f(0, 0);
			break;
		case Scene::AbajoIzquierda:
			return Vector2f(0, window->getSize().y);
			break;
		case Scene::ArribaDerecha:
			return Vector2f(window->getSize().x, 0);
			break;
		case Scene::AbajoDerecha:
			return Vector2f(window->getSize().x, window->getSize().y);
			break;
		case Scene::Centro:
			return Vector2f(window->getSize().x / 2, window->getSize().y / 2);
			break;
		default:
			break;
		}
	}
	
	void addDraw(RectangleShape* d) {
		//rectangles.push_back(d);
	}

	void Ctrea(int n) {
		nIndex = n;
		IndexMe = n;
		escenaMain = this;
		onCreateP(n);
	}

	int find(GameObject* x) {
		for (int i = 0; i < SceneObjects.size(); i++) {
			
			if (SceneObjects.at(i) == x) {
				return i;
			}
		}

		return -1;
	}
	GameObject* findByName(string g) {
		for (int i = 0; i < SceneObjects.size(); i++)
		{
			if (SceneObjects.at(i)->name == g) {
				return SceneObjects.at(i);
			}
		}
		return NULL;
	}
	/// <summary>
	/// Función que se ejecuta al inicio de la escena (un "Start")
	/// </summary>
	/// <param name="num"> El número de la escena dentro de la lista de escenas</param>
	virtual void onCreateP(int num) {};
	void UpdateAll() {

		if (!SceneObjects.empty()) {
			if (inDelete == true) {
				
				for (int h = 0; h < objetoDel.size(); h++)
				{
					//SceneObjects.erase(SceneObjects.begin() + (objetoDel.at(h) - 1));
					objetoDel.at(h)->Onback();
					SceneObjects.erase(remove(SceneObjects.begin(), SceneObjects.end(), objetoDel.at(h)), SceneObjects.end());
					//cout << "borrado - ln 182" << endl;
				}
				objetoDel.clear();
				inDelete = false;
				
			}
			for (int i = 0; i < SceneObjects.size(); i++)
			{
				if (SceneObjects.at(i)->active == true) {
					SceneObjects.at(i)->Update();
				}
			}
		}
		if (showFps == true) {
			cout << 1 / getDelta()<<endl;
		}
		delta->restart();
	}
	void startAll() {
		delta = new Clock();
		if (!SceneObjects.empty()) {
			for (int i = 0; i < SceneObjects.size(); i++)
			{
				if (SceneObjects.at(i)->active == true) {
					SceneObjects.at(i)->Start();
				}
			}
		}
	}

	virtual void Update() {};
	void onBack() {
		
		SceneObjects.clear();

	};

	int setScene() {

		return this->nIndex;

	}

	void inSceneStart(RenderWindow& win) {
		window = &win;
		//startAll();
	}

	
};
Scene* Scene::escenaMain = new Scene();


class PooledObjects {
public:
	map<string, vector<PoolObject*>> lista;
	map<string, int> listaNumero;
	PooledObjects() {

	}
	void addList(string id, PoolObject maked, int num) {
		vector<PoolObject*> vect;
		vect.resize(num);
		for (int i = 0; i < vect.size(); i++)
		{
			vect.at(i) = new PoolObject(maked);
			Scene::escenaMain->Instantiate(vect.at(i));
			vect.at(i)->active = false;
			listaNumero[id] = 0;
			//cout << "a "<<i << endl;
		}
		lista[id] = vect;

	}
	PoolObject* instantiateFromPool(string id) {
		//Scene::escenaMain->Instantiate(lista[id].at(0));//puede no funcionar.
		if (listaNumero[id] >= lista[id].size()) {
			listaNumero[id] = 0;
		}
		lista[id].at(listaNumero[id])->active = true;
		//cout << listaNumero[id] << endl;
		PoolObject* poo = lista[id].at(listaNumero[id]);
		listaNumero[id] += 1;
		
		return poo;
	}
};



class SpriteC : public GameObject::componentD {
public:
	bool mirrorX = false;
	bool active = true;
	int pos = 0;
	string path = "";
	bool smooth = false;
	Vector2f pivot = Vector2f(0.5f, 0.5f);
	//Texture* texture;
	Vector2f off = Vector2f(0,0);
	vector<Texture*> textures;
	RectangleShape* sprite;

	SpriteC(string pathd , bool smoothP = true, Vector2f offset = Vector2f(0,0)) : GameObject::componentD() {
		path = pathd;
		off = offset;
		//texture = new Texture();
		textures.push_back(new Texture());
		textures.at(0)->setSmooth(smoothP);
		
		//texture->setSmooth(smoothP);
		if (textures.at(0)->loadFromFile(path)) {
			//cout << "Bien" << endl;
			
			
			sprite = new RectangleShape();
			//cout << sprite->getTexture()->isSmooth() << endl;
			sprite->setTexture(textures.at(0));


			//sprite->setPosition(gameobj)

		}
		else
		{
			//cout << "malo" << endl;
			active = false;
		}
	}
	void addImage(string pathd) {
		
		textures.push_back(new Texture());
		textures.at(textures.size() - 1)->loadFromFile(pathd);
	}
	void setImage(int i) {
		if (pos != i) {
			if (i < textures.size()) {
				sprite->setTexture(textures.at(i));
				pos = i;
			}
		}
	}
	SpriteC(Texture tec, bool smoothP = true, Vector2f offset = Vector2f(0, 0)) : GameObject::componentD() {
		off = offset;
		textures.push_back(new Texture(tec));
		
			//cout << "Bien" << endl;
		textures.at(0)->setSmooth(smoothP);

		sprite = new RectangleShape();
		sprite->setTexture(textures.at(0));


			//sprite->setPosition(gameobj)

		
		
		
	}
        
    /*void cambiarTextura(Texture tec) {
        texture = new Texture(tec);
        sprite->setTexture(texture);

    }*/

	void OnaddComponent(GameObject* g) {
		gameobj = g;
		//actualiza();
	}
	RectangleShape* get_sprite() {
		actualiza();
		return sprite;
	}

	void ponerSprite() {
		//no
	}
	void actualiza() {
		//cout << sprite->getSize().x << endl;
		if (this->active) {



			sprite->setSize(gameobj->transform->scale);
			sprite->setOrigin(sprite->getSize().x * pivot.x, sprite->getSize().y * pivot.y);
			sprite->setPosition(gameobj->transform->position + off);
			sprite->setRotation(gameobj->transform->rotation);
			Scene::escenaMain->window->draw(*sprite);
		}
		
	}
	/*void setSmooth(bool i) {
		
	}*/
};

class rectangleD : public GameObject::componentD {
public:
	RectangleShape* shape;
	bool shad = false;
	Shader* shader;
	
	rectangleD(Color color) : GameObject::componentD() {
		shape = new RectangleShape();
		shape->setFillColor(color);
	}
	rectangleD(Shader* shader) : GameObject::componentD() {
		shape = new RectangleShape();
		//shader.loadFromFile(shaderPath, Shader::Fragment);
		this->shader = shader;
		shad = true;
	}
	void OnaddComponent(GameObject* g) {
		gameobj = g;
		
	}
	void actualiza() {
		shape->setSize(gameobj->transform->scale);
		shape->setOrigin(shape->getSize().x / 2, shape->getSize().y / 2);
		shape->setPosition(gameobj->transform->position);
		shape->setRotation(gameobj->transform->rotation);
		if (shad == true) {
			Scene::escenaMain->window->draw(*shape, shader);
		}
		else
		{
			Scene::escenaMain->window->draw(*shape);
		}
		
	}
};

class ButtonC : public GameObject::componentD {
public:
	bool clicked = false;
	bool camb_cursor = true;
	bool cur = false;
	//template<class F>
	Cursor* cursorSobre;
	Cursor* cursorpulsa;

	float porcent = 90;
	Vector2f reg;
	Vector2f currentsize;

	bool isEventCall = false;
	function<void()> fun;
	

	ButtonC() : GameObject::componentD() {
		cursorSobre = new Cursor();
		cursorpulsa = new Cursor();
		cursorSobre->loadFromSystem(Cursor::Hand);
		cursorpulsa->loadFromSystem(Cursor::Arrow);
	}
	void OnaddComponent(GameObject* g) {
		gameobj = g;

		reg = g->transform->scale;
		reg = Vector2f(reg.x / 100, reg.y / 100);
		reg = multiply(reg, porcent);
		currentsize = g->transform->scale;
	}
	/// <summary>
	/// add Event, recomended use "bind(&class::func, classInstance)".
	/// </summary>
	/// <param name="fn"></param>
	void addEvent(function<void()> fn){
		//fun = fn;
		fun = fn;
		isEventCall = true;
		
	}
	
	void onClick() {
		clicked = true;
		if (isEventCall) {
			fun();
		}
		
		
	}
	
	void actualiza() {
		Vector2f vec1(Mouse::getPosition(*Scene::escenaMain->window));
		vec1.x += Scene::escenaMain->cameraAnchor().x;
		vec1.y += Scene::escenaMain->cameraAnchor().y;
		Vector2f vec3(currentsize);
		Vector2f vec2(gameobj->transform->position);
		//cout << gameobj->transform->position.x << endl;
		vec2.x -= vec3.x / 2;
		vec2.y -= vec3.y / 2;
		
		//currentsize = gameobj->transform->scale;
		if (vec1.x < (vec2.x + vec3.x) && vec1.x>(vec2.x)) {
			if (vec1.y < (vec2.y + vec3.y) && vec1.y>(vec2.y)) {
				if (Mouse::isButtonPressed(Mouse::Button::Left)) {
					onClick();
					
				}
				else { clicked = false; }
				gameobj->transform->scale = reg;
				if (camb_cursor && cur == false) {
					Scene::escenaMain->window->setMouseCursor(*cursorSobre);
					cur = true;
				}
			}
			else { clicked = false; gameobj->transform->scale = currentsize; 
			if (camb_cursor && cur == true) {
				Scene::escenaMain->window->setMouseCursor(*cursorpulsa);
				cur = false;
			}
			}
		}
		else { clicked = false;  gameobj->transform->scale = currentsize;
		if (camb_cursor && cur == true) {
			Scene::escenaMain->window->setMouseCursor(*cursorpulsa);
			cur = false;
		}
		}
	}
};

//display


class TextD : public GameObject::componentD {
public:
	string texto;
	Text* textShape;
	int textSize;
	string path;
	Font* font;
	Vector2f sizeG;

	TextD(string g, int textSize, string path, Color col) : GameObject::componentD() {
		texto = g;
		textShape = new Text();
		textShape->setString(g);
		this->textSize = textSize;
		textShape->setCharacterSize(textSize);
		
		this->path = path;
		font = new Font();
		font->loadFromFile(path);
		textShape->setFont(*font);
		textShape->setFillColor(col);
		//cout << font->getGlyph(200, textSize, false).textureRect.width << endl;
		//cout << textShape->getLocalBounds().width << endl;
		sizeG = Vector2f(textShape->getLocalBounds().width, textShape->getLocalBounds().height);
		
	}

	void OnaddComponent(GameObject* g) {
		gameobj = g;

	}

	void actualiza() {
		sizeG = Vector2f(textShape->getLocalBounds().width, textShape->getLocalBounds().height);
		Vector2f vec1(gameobj->transform->position);
		Vector2f vec2(gameobj->transform->scale);
		textShape->setPosition(Vector2f(vec1.x-(sizeG.x/2), vec1.y-(sizeG.y/1.5)));
		textShape->setCharacterSize(this->textSize);
		textShape->setString(this->texto);
		textShape->setFont(*font);
		Scene::escenaMain->window->draw(*textShape);
	}
};

//physics(por ahora):

class PhysicComponent {
public:
	enum typo{
		circle,
		rectangle,
		shape
	};
	typo tipo;
	bool solid = false;

	Vector2f trans = Vector2f(0,0);
	float radius = 0;
	Vector2f tamano;
	GameObject* go;
	GameObject* gE;
	Vector2f normalC;
	PhysicComponent* contacto;

	bool contact = false;
	Vector2f vectorContact;
	bool* active;
	bool ondelete = false;
	Vector2f offset;

	void translate(Vector2f vec) {
		trans = vec;
	}
};




class physicsD {
public:
	static vector<PhysicComponent*> components;
	static vector<PhysicComponent*> ondelete;
	static bool ondeleteThis;
	static Clock cloc;
	static int num;
	bool run = true;
	static physicsD physi;
	static int idn;

	static void crear(void) {
		idn = 1;
		//cout << idn;
	}

	physicsD() {
		
	}
	void operator()() {
		
		UpdatePhysics();
	}
	void start() {
		//F es functor
		
		thread tre(*this);
		tre.detach();
		
		return;
	}

	void moveI(int i, int f) {
		if (components.at(f)->solid == true) {
			if (components.at(i)->solid == false) {
				components.at(i)->go->transform->position -= components.at(i)->trans*0.1f;
				// 
				
				Vector2f com1 = components.at(i)->go->transform->position;
				com1 = components.at(i)->offset;
				Vector2f com2 = components.at(f)->go->transform->position;
				com2 = components.at(f)->offset;
				Vector2f res1 = normalized(com2 - com1);
				Vector2f res2 = normalized(com1 - com2);

				Vector2f res3 = quadrate(res1);
				res3 = Vector2f(res3.x * components.at(i)->tamano.x, res3.y * components.at(i)->tamano.y);
				Vector2f res4 = quadrate(res2);
				res4 = Vector2f(res4.x * components.at(f)->tamano.x, res4.y * components.at(f)->tamano.y);

				float d1 = distanceOf(com1, res3);
				float d2 = distanceOf(com2, res4);
				float h1 = d1 + d2;
				float d3 = distanceOf(com1, com2);
				//cout << com1.x << endl;
				if (d3 < h1) {
					cout << "pan" << endl;
				}

				//cout << components.at(i)->trans.x << endl;
				components.at(i)->trans = Vector2f(0, 0);
			}
			
		}
		
		//components.at(i)->trans = Vector2f(0, 0);
	}
	
	
	static float getDelta() {
		return (cloc.getElapsedTime().asSeconds());
	}

	static bool isIn(Vector2f point, Vector2f rectPos, Vector2f rectSize) {
		

		if (point.x >= rectPos.x && point.x <= (rectPos.x + rectSize.x)) {
			if (point.y >= rectPos.y && point.y <= (rectPos.y + rectSize.y)) {
				return true;
			}
		}
		return false;
	}

	void isCollCircle(int i, int f) {
		float ancho1 = components.at(i)->radius;
		float ancho2 = components.at(f)->radius;
		float ancho3 = ancho1 + ancho2;

		Vector2f vec1 = components.at(i)->go->transform->position;
		Vector2f vec2 = components.at(f)->go->transform->position;
		vec1 += components.at(i)->offset;
		vec2 += components.at(f)->offset;

		float distance = sqrtf((powf(vec2.x - vec1.x, 2)) + (powf(vec2.y - vec1.y, 2)));
		if (distance <= ancho3) {
			components.at(i)->contact = true;
			components.at(i)->contacto = components.at(f);
			components.at(i)->vectorContact = normalized(Vector2f(vec2.x - vec1.x, vec2.y - vec1.y));
			components.at(i)->gE = components.at(f)->go;
			moveI(i, f);
			components.at(i)->go->PsysicsUpdate(true, components.at(f)->go, components.at(i)->vectorContact);
			//cout << "b";
		}
		else {
			components.at(i)->contact = false;
			components.at(i)->go->PsysicsUpdate(false, components.at(f)->go, components.at(i)->vectorContact);

			//components.at(i)->trans = Vector2f(0, 0);
		}
	}

	void isCollCirRect(int i, int f) {
		float anchoC = components.at(i)->radius;
		Vector2f vec1 = components.at(f)->tamano;

		Vector2f vec2 = components.at(i)->go->transform->position;
		Vector2f vec3 = components.at(f)->go->transform->position;
		vec2 += components.at(i)->offset;
		vec3 += components.at(f)->offset;

		if (vec3.x-vec1.x<=vec2.x+anchoC
			&& vec3.y-vec1.y<=vec2.y+anchoC) {
			if (vec3.x+vec1.x>=vec2.x-anchoC
				&& vec3.y+vec1.y >= vec2.y-anchoC) {
				//si
				components.at(i)->contact = true;
				components.at(i)->gE = components.at(f)->go;
				components.at(i)->contacto = components.at(f);
				components.at(i)->vectorContact = normalized(Vector2f(vec3.x - vec2.x, vec3.y - vec2.y));
				moveI(i, f);
				components.at(i)->go->PsysicsUpdate(true, components.at(f)->go, components.at(i)->vectorContact);
				//cout << "a";
				return;
			}
		}

		components.at(i)->contact = false;
		components.at(i)->go->PsysicsUpdate(false, components.at(f)->go, components.at(i)->vectorContact);

		//components.at(i)->trans = Vector2f(0, 0);

	}
	void isCollRect(int i, int f) {
		Vector2f vecpos1 = components.at(i)->go->transform->position;
		Vector2f vecpos2 = components.at(f)->go->transform->position;
		vecpos1 += components.at(i)->offset;
		vecpos2 += components.at(f)->offset;

		Vector2f vec1 = components.at(i)->tamano;
		Vector2f vec2 = components.at(f)->tamano;

		if (vecpos1.x + vec1.x > vecpos2.x - vec2.x){//he sacado los <= y los >=, por si acaso;
			components.at(i)->normalC = Vector2f(1, 0);
			if (vecpos1.y + vec1.y > vecpos2.y - vec2.y) {
				components.at(i)->normalC = Vector2f(0, 1);
				if (vecpos1.x - vec1.x < vecpos2.x + vec2.x) {
					components.at(i)->normalC = Vector2f(-1, 0);
					if (vecpos1.y - vec1.y < vecpos2.y + vec2.y) {
						components.at(i)->normalC = Vector2f(0, -1);
						//si
						components.at(i)->contact = true;
						components.at(i)->gE = components.at(f)->go;
						components.at(i)->contacto = components.at(f);
						components.at(i)->vectorContact = normalized(Vector2f(vecpos2.x - vecpos1.x, vecpos2.y - vecpos1.y));
						moveI(i, f);
						//cout << components.at(i)->solid<<endl;

						components.at(i)->go->PsysicsUpdate(true, components.at(f)->go, components.at(i)->vectorContact);
						return;
					}
				}
				
			}
			
		}
		components.at(i)->contact = false;
		components.at(i)->go->PsysicsUpdate(false, components.at(f)->go, components.at(i)->vectorContact);

		//components.at(i)->trans = Vector2f(0, 0);
	}
	
	void UpdatePhysics() {

		cloc.restart();
		
		
		
		

		while (run)
		{
			
			if (ondeleteThis == true) {
				for (int k = 0; k < ondelete.size(); k++)
				{

					components.erase(remove(components.begin(), components.end(), ondelete.at(k)), components.end());

					
				}
				ondelete.clear();
			}

			if (components.empty()==false) {
				for (int i = 0; i < components.size(); i++)
				{
					if (*components.at(i)->active == true) {
						for (int f = 0; f < components.size(); f++)
						{
							if (*components.at(f)->active == true) {
								if (components.at(i) != components.at(f)) {


									


									if (components.at(f)->tipo == PhysicComponent::circle
										&& components.at(i)->tipo == PhysicComponent::circle) {

										isCollCircle(i, f);

									}
									else if (components.at(i)->tipo == PhysicComponent::circle
										&& components.at(f)->tipo == PhysicComponent::rectangle) {
											isCollCirRect(i, f);
									
									}
									else if (components.at(i)->tipo == PhysicComponent::rectangle
										&& components.at(f)->tipo == PhysicComponent::circle) {
											isCollCirRect(f, i);
								
									}
									else if (components.at(f)->tipo == PhysicComponent::rectangle
										&& components.at(i)->tipo == PhysicComponent::rectangle) {
										//rect x rect
										
										isCollRect(i, f);
										
									}


									
									//cout << to_string(components.at(f)->tipo) << endl;

									
									/*float ancho1 = components.at(i)->radius;
									float ancho2 = components.at(f)->radius;
									float ancho3 = ancho1 + ancho2;
									Vector2f vec1 = components.at(i)->go->transform->position;
									Vector2f vec2 = components.at(f)->go->transform->position;
									float distance = sqrtf((powf(vec2.x - vec1.x, 2)) + (powf(vec2.y - vec1.y, 2)));
									if (distance <= ancho3) {
										components.at(i)->contact = true;
										components.at(i)->vectorContact = normalized(Vector2f(vec2.x - vec1.x, vec2.y - vec1.y));
										components.at(i)->go->PsysicsUpdate(true, components.at(f)->go, components.at(i)->vectorContact);
									}
									else {
										components.at(i)->contact = false;
										components.at(i)->go->PsysicsUpdate(false, components.at(f)->go, components.at(i)->vectorContact);
									}*/
								}
								
							}
							/*if (f == components.size() && components.at(i)->solid == false) {
								//components.at(i)->trans = Vector2f(0, 0);
							}*/
						}
						if (components.at(i)->solid == false) {
							components.at(i)->go->transform->position += components.at(i)->trans;
						}
						components.at(i)->trans = Vector2f(0, 0);
					}
				}
			}
			cloc.restart();
			this_thread::sleep_for(chrono::milliseconds(1000 / num));

		}
	}
	void Borrar() {
		components.clear();
	}

	void add(PhysicComponent* g) {
		physicsD::components.push_back(g);
	}
	void Borrar(PhysicComponent* g) {
		physicsD::ondelete.push_back(g);
		physicsD::ondeleteThis = true;
	}
};

int physicsD::idn = 0;
physicsD physicsD::physi = physicsD();
vector<PhysicComponent*> physicsD::components = vector<PhysicComponent*>();
vector<PhysicComponent*> physicsD::ondelete = vector<PhysicComponent*>();
bool physicsD::ondeleteThis = false;
Clock physicsD::cloc = Clock();
int physicsD::num = 60;

class BodyPhys2D : public GameObject::componentD {
public:

	Vector2f* posgame;
	Vector2f* velocity;
	PhysicComponent* comp;

	void OnaddComponent(GameObject * g) {
		gameobj = g;

	}
	BodyPhys2D(Vector2f* posGameObj, PhysicComponent* com) : GameObject::componentD() {
		posgame = posGameObj;
		velocity = new Vector2f(0, 0);
		comp = com;
	}
	void actualiza() {
		
		if (comp->active) {
			cout << comp << endl;
			if (comp->contact == true) {
				
				int t = 0;
				int h = 0;
				Vector2f direc(0, 0);

				switch (comp->tipo)
				{
				default:
					break;
				case PhysicComponent::circle:
					direc = comp->vectorContact;
					break;
				case PhysicComponent::rectangle:
					t = 1;
					
					break;
				}
				Vector2f vec1 = comp->gE->transform->position;
				switch (comp->contacto->tipo)
				{
				default:
					break;
				case PhysicComponent::rectangle:
					h = 1;
					break;
				case PhysicComponent::circle:
					h = 0;
					break;
				}

				if (t == 0 && h == 0) {
					float m = distanceOf(comp->go->transform->position + comp->offset, comp->contacto->go->transform->position + comp->contacto->offset);
					m -= (comp->radius + comp->contacto->radius);
					gameobj->transform->position = Vector2f(gameobj->transform->position - multiply(direc, -1*m));
				}
				else if (t == 0 && h == 1) {
					float radj = (comp->contacto->tamano.x + comp->contacto->tamano.y) / 2;
					float m = distanceOf(comp->go->transform->position + comp->offset, comp->contacto->go->transform->position + comp->contacto->offset);
					m -= (comp->radius + radj);
					gameobj->transform->position = Vector2f(gameobj->transform->position - multiply(direc, -1 * m));
				}
				else if (t == 1 && h == 0) {
					float radj = (comp->tamano.x + comp->tamano.y) / 2;
					float m = distanceOf(comp->go->transform->position + comp->offset, comp->contacto->go->transform->position + comp->contacto->offset);
					m -= (radj + comp->contacto->radius);
					//posgame = new Vector2f(*posgame - multiply(direc, -1 * m));
					gameobj->transform->position = Vector2f(gameobj->transform->position - multiply(direc, -1 * m));
				}
				else if (t == 1 && h == 1) {
					float t1 = distanceOf(gameobj->transform->position, comp->gE->transform->position);
					if (comp->normalC == Vector2f(1, 0)) {
						t1 = (comp->gE->transform->position.x - gameobj->transform->position.x);
						float m = t1 - (gameobj->transform->position.x + comp->gE->transform->position.x);
						gameobj->transform->position.x -= m;
					}
					else if (comp->normalC == Vector2f(-1, 0)) {
						t1 = (comp->gE->transform->position.x - gameobj->transform->position.x);
						float m = t1 - (gameobj->transform->position.x + comp->gE->transform->position.x);
						gameobj->transform->position.x -= m;
					}
					else if (comp->normalC == Vector2f(0, 1)) {
						t1 = (comp->gE->transform->position.y - gameobj->transform->position.y);
						float m = t1 - (gameobj->transform->position.y + comp->gE->transform->position.y);
						gameobj->transform->position.y -= m;
						
					}
					else if (comp->normalC == Vector2f(0, -1)) {
						t1 = (comp->gE->transform->position.y - gameobj->transform->position.y);
						float m = t1 - (gameobj->transform->position.y + comp->gE->transform->position.y);
						gameobj->transform->position.y -= m;
					}
					
					/*Vector2f v1(0, 0);
					Vector2f v2(0, 0);
					v1 = quadrate(comp->vectorContact);
					v2 = quadrate(multiply(comp->vectorContact, -1));
					v1.x *= comp->tamano.x;
					v1.y *= comp->tamano.y;
					v2.x *= comp->contacto->tamano.x;
					v2.y *= comp->contacto->tamano.y;
					float d1 = distanceOf(Vector2f(0, 0), v1);
					float d2 = distanceOf(Vector2f(0, 0), v2);
					float d3 = distanceOf(comp->o->transform->position + comp->offset, comp->gE->transform->position + comp->contacto->offset);
					float d4 = d3 - (d1 + d2);*/
					//gameobj->transform->position = Vector2f(gameobj->transform->position - multiply(quadrate(comp->vectorContact), d4));
				}
				
				velocity = new Vector2f(0, 0);

			}
			else
			{
				gameobj->transform->position = Vector2f(Suma(gameobj->transform->position, *velocity));
				//cout << "h" << endl;
			}
		}
	}
};



class CircleColl : public GameObject::componentD, public PhysicComponent {
public:
	//float radius;
	void OnaddComponent(GameObject* g) {
		gameobj = g;
		go = g;
		active = &go->active;
		physicsD::physi.add(this);
		this->tipo = PhysicComponent::circle;
		//cout << "yes";
		physicsD::idn += 1;
	}
	CircleColl(float radius, Vector2f offsett = Vector2f(0,0) ) : GameObject::componentD(), PhysicComponent() {
		this->radius = radius;
		this->offset = offsett;
	}
	void onBack() {
		physicsD::physi.Borrar(this);
	}
};

class RectColl : public GameObject::componentD, public PhysicComponent {
public:
	//float radius;
	void OnaddComponent(GameObject* g) {
		gameobj = g;
		go = g;
		active = &go->active;
		physicsD::physi.add(this);
		this->tipo = PhysicComponent::rectangle;
		//cout << "yes";
		physicsD::idn += 1;
		
	}
	RectColl(Vector2f f,bool solid=false, Vector2f offsett = Vector2f(0, 0)) : GameObject::componentD(), PhysicComponent() {
		this->tamano = f;
		this->offset = offsett;
		this->solid = solid;
	}
	void onBack() {
		physicsD::physi.Borrar(this);
	}
};
class Tile {
public:
	RectangleShape* rec;
	Texture* tex;
	bool phys = false;
	Tile(string path, bool p = false) {
		rec = new RectangleShape();
		tex = new Texture();
		tex->loadFromFile(path);
		tex->setSmooth(false);
		rec->setTexture(tex);
		phys = p;
	}
};

class Map : public GameObject {
public: 
	RenderTexture* mapa;
	RectangleShape* rectang;
	vector<Tile*> tiles;
	int* mapping;
	int Width;
	int Height;
	int tama;
	
	Map(int width, int height,int tamano, string g = "map") : GameObject(g) {
		this->Width = width;
		this->Height = height;
		mapping = new int[width*height];
		mapa = new RenderTexture();
		mapa->create(width * tamano, height * tamano);
		//mapa->
		this->tama = tamano;
		rectang = new RectangleShape(Vector2f(width * tamano, height * tamano));
		for (int i = 0; i < width*height; i++)
		{
			mapping[i] = 0;
		}
	}
	void SetPos(int x, int y, int value) {
		y = y * Width;
		int n = y + x;
		//cout << n << endl;
		//cout << "este: " << rectang->getRotation() << endl;
		mapping[n] = value;
	}
	Vector2f getPositionT(Vector2f g) {
		Vector2f j(0, 0);
		g -= this->transform->position;
		j.x = floorf( g.x / tama);
		j.y = floorf( g.y / tama);
		return j;
	}
	Vector2f getPosOf(Vector2f k) {
		Vector2f l(0, 0);
		l.x = (k.x * tama);
		l.y = (k.y * tama);
		l.x += tama / 2;
		l.y += tama / 2;

		l.x += this->transform->position.x;
		l.y += this->transform->position.y;
		return l;
	}
	Tile* getInfo(Vector2f j) {
		int h = (j.y * Width) + j.x;
		return tiles.at(mapping[h]);
	}
	/*Vector2f getPos(int g) {
		int x = floorf(g/Width);
		int y = floorf(g-(x*Height));


	}*/
	void setTile(int n, string pathImage, bool ph = false, Vector2i size = Vector2i(1, 1)) {
		Tile* tt = new Tile(pathImage, ph);
		tt->rec->setSize(Vector2f(tama, tama));
		tiles.push_back(tt);
	}
	void addRectPhys() {

	}
	void StartPhys() {
		for (int g = 0; g < Width * Height; g++) {
			if (tiles.at(mapping[g])->phys == true) {
				RectColl* colis;
				Vector2f off(convertTo(g));
				off.x = off.x * tama;
				off.y = off.y * tama;
				off.x += tama / 2;
				off.y += tama / 2;
				colis = new RectColl(Vector2f(tama/2, tama/2),false, off);
				addComponent(colis);
			}
		}
	}
	void setMapOfFile(string path) {
		//const char* c = path.c_str();
		ifstream archivo(path.c_str());
		string h = "";
		getline(archivo, h);
		char delimeter = ',';
		//cout << h[0] << endl;
		
		//int x = 0;
		size_t n = 0;
		//bool xd = true;
		//cout << h << endl;
		string salida = "";
		int* cf;
		cf = new int[Width*Height];
		int hd = 0;

		while ((n = h.find(delimeter)) != std::string::npos)
		{
			salida = h.substr(0, n);
			if (hd < Width * Height) {
				//mapping[hd] = stoi(salida);
				//cout << hd << endl;
				setT(hd, stoi(salida));
			}
			h.erase(0, n + 1);

			hd++;
		}
		


		//cout << h.size() << endl;


	}
	Vector2f convertTo(int h) {
		int yd = h / Width;

		int xd = h - (yd * Width);

		Vector2f vec1(xd, yd);
		
		return vec1;
	}
	
	void setT(int g, int value) {
		mapping[g] = value;
	}
	void actualizeMe() {
		mapa->display();
		mapa->clear(Color::Transparent);

		//RectangleShape h()

		for (int g = 0; g < Width*Height; g++)
		{
			int h = mapping[g];
			if (h == -1) {



				continue;
			}
			Vector2f vec1 = convertTo(g);
			vec1 = Vector2f(vec1.x * tama, vec1.y * tama);
			tiles.at(h)->rec->setPosition(vec1);

			mapa->draw(*tiles.at(h)->rec);
			
		}
		
		rectang->setTexture(&mapa->getTexture());
	}

	void DrawMe() {
		Scene::escenaMain->window->draw(*rectang);
	}

};





class SceneManager {
	public:

		Scene* escena;
		vector<Scene*> escenas;
		
		int num = 0;
		void sig_Scene() {
			//this->escenas.at(num) = scens.at(num);
			num += 1;
			this->escena = escenas.at(num);
		}
		void ir_Scene(int i) {
			num = i;
			this->escena = escenas.at(num);
		}
		

};




class GameManager {
public:
	RenderWindow* window{};
	
	SceneManager* scenes;

	static int limitFps;
	//physicsD* phisc{};
	int stylew;

	
	int width;
	int height;
	string name;
	/// <summary>
	/// Para crear el administrador de todo el juego.
	/// </summary>
	/// <param name="scenanueva">El puntero a la primera escena</param>
	/// <param name="width">Ancho de ventana</param>
	/// <param name="height">Alto de ventana</param>
	/// <param name="name">Titulo de ventana</param>
	/// <example>
	/// Ejemplo de código:
	/// <code>
	/// SceneOne scene1 = new SceneOne();
	/// 
	/// int main(){
	///		
	///		scene1 = new SceneOne();
	///		GameManager game(*scene1, 700, 600, "Game");
	///		game.setStyleWindow(5);
	///		
	///		thread t(game);
	///		t.join();
	/// 
	/// }
	/// </code>
	/// </example>
	GameManager(Scene &scenanueva, int width, int height, string name) {
		//this->escena = &scenanueva;
		//this->escenas.push_back(&scenanueva);
		this->scenes = new SceneManager();
		//this->phisc = new physicsD();

		this->scenes->escena = &scenanueva;
		this->scenes->escenas.push_back(&scenanueva);
		//this->scenes->ir_Scene(0);

		this->width = width;
		this->height = height;
		this->name = name;
		
	}
	void operator()() {
		UpdateGame();
	}

	void addScene(Scene& sce) {
		this->scenes->escenas.push_back(&sce);
	}
        /**
         * ste the style for the new window:
         * 0 = none,
         * 1 = titlebar,
         * 2 = resize,
         * 3 = close,
         * 4 = fullscreen,
         * 5 = default.
         * @param int g
         */
	void setStyleWindow(int g) {
		this->stylew = g;
                
	}

	void UpdateGame() {
		//RenderWindow d(VideoMode(this->width, this->height), this->name);
		
		if (stylew == Style::Fullscreen) {
			this->window = new RenderWindow(VideoMode::getFullscreenModes().at(0), this->name, stylew, ContextSettings(0, 0, 2));
		}
		else {
			this->window = new RenderWindow(VideoMode(this->width, this->height), this->name, stylew, ContextSettings(0, 0, 2));
		}
		
		
		//Style::Fullscreen;
		this->window->setActive(true);
		this->window->setFramerateLimit(limitFps);
		this->window->setVerticalSyncEnabled(false);
		
		

		/*thread tred(*phisc);
		tred.join();*/
		
		physicsD::physi.start();
		
		
		//cout << "sali" << endl;

		while (this->window->isOpen()) {

			UpdateEvent();
			this->window->clear(Color::Black);

			if (this->scenes->escena->run == false) {
				this->scenes->escena->run = true;
				this->scenes->escena->inSceneStart(*this->window);
				this->scenes->escena->Ctrea(this->scenes->num);
				
				this->scenes->escena->startAll();//Porque start 2 veces?!!!
			}

			//this->escena->UpdateAll();
			this->scenes->escena->Update();
			this->scenes->escena->UpdateAll();

			if (this->scenes->escena->setScene() != this->scenes->num) {
				this->scenes->escena->onBack();
				//this->phisc->Borrar();
				Scene* scen;
				scen = this->scenes->escena;
				physicsD::physi.Borrar();
				this->scenes->ir_Scene(this->scenes->escena->setScene());
				//scen->run == false;
				scen->nIndex = scen->IndexMe;
				this->scenes->escena->run = false;

			}
			//RectangleShape rec(Vector2f(10, 10));
			//rec.setPosition(Vector2f(0, 0));
			//rec.setFillColor(Color::White);
			//this->window->draw(rec);
			this->window->display();
		}

		//salida:
		//this->phisc->run = false;
		physicsD::physi.run = false;
	}

	void UpdateEvent() {
		Event e;
		while (this->window->pollEvent(e)) {
			if (e.type == Event::Closed) {
				this->window->close();
				
			}
			this->scenes->escena->eScene = e;
		}
	}
};

int GameManager::limitFps = 60;
