#include <SFML/Graphics.hpp>
#include <SFML/audio.hpp>

#include <iostream>
#include <sstream>
#include <typeinfo>

using namespace std;
/*
 #include "Game.hpp"



 int main2() {

 Game * game = Game::getInstance();
 game->start();
 game->run();
 game->stop();

 return 0;
 }


 */

class Movel {
public:
	Movel(int x, int y, int direcaoX, int direcaoY, string textura,
			sf::RenderWindow *screen) {

		this->window = screen;

		int vel = rand() % 500 + 100; // de 100 ~499.
		velocidadeX = vel * direcaoX;
		vel = rand() % 500 + 100; // de 100 ~499.
		velocidadeY = vel * direcaoY;

		texture = new sf::Texture();

		if (!texture->loadFromFile(textura)) {
			cout << "Erro ao carregar a imagem " << endl;
		}
		sprite.setTexture(*texture);
		sprite.setScale(0.2, 0.2);
		sprite.setOrigin(sprite.getLocalBounds().width/2,sprite.getLocalBounds().height/2 );

		//sf::Vector2f tamanho(90, 39);
		//sprite.setScale(tamanho.x / sprite.getLocalBounds().width,
		//		tamanho.y / sprite.getLocalBounds().height);

		sprite.setPosition(x, y);

	}
	sf::FloatRect getMyBounds(){
		return sprite.getLocalBounds();

	}

	virtual void move(sf::Time dt) {
		testaColisaoComParede();
		this->sprite.move(velocidadeX * dt.asSeconds(),
				velocidadeY * dt.asSeconds());
	}

	string toString() {
		stringstream ss;
		sf::FloatRect retangulo = sprite.getGlobalBounds();
		ss << "X,Y[" << retangulo.left << "," << retangulo.top << "],"
				<< " Vx,Vy[" << velocidadeX << "," << velocidadeY << "],"
				<< "W,H[" << retangulo.width << "," << retangulo.height << "]";
		return ss.str();
	}

	virtual void mostra() {
		window->draw(sprite);
		//cout << toString() << endl;
	}
	virtual ~Movel() {
		delete (texture);
	}

protected:
	int velocidadeX = 100;
	int velocidadeY = 100;
	sf::Texture *texture;
	sf::RenderWindow *window;
	sf::Sprite sprite;

	 void testaColisaoComParede() {

		int w = window->getSize().x;
		int h = window->getSize().y;
		sf::Vector2f posicao = sprite.getPosition();

		if (posicao.x + sprite.getGlobalBounds().width/2 > w) {
			velocidadeX = -velocidadeX;


		}
		if (posicao.x - sprite.getGlobalBounds().width/2 < 0) {
			velocidadeX = -velocidadeX;

		}

		if (posicao.y + sprite.getGlobalBounds().height/2 > h) {
			velocidadeY = -velocidadeY;

		}
		if (posicao.y - sprite.getGlobalBounds().height/2 < 0) {
			velocidadeY = -velocidadeY;

		}

	}
};

class Bola: public Movel {
public:
	Bola(int x, int y, int direcaoX, int direcaoY, string textura,
			sf::RenderWindow *screen) :
			Movel(x, y, direcaoX, direcaoY, textura, screen) {

		//Define origem no meio da bola.
		sf::FloatRect rect = sprite.getLocalBounds();
		sprite.setOrigin(rect.width / 2, rect.height / 2);

		//Centraliza imagem e aureola.
		const sf::Vector2f *posicao = &sprite.getPosition();
		circle.setRadius( sprite.getLocalBounds().width / 10);
		circle.setOrigin(circle.getLocalBounds().width / 2,
				circle.getLocalBounds().height / 2);
		circle.setPosition(sprite.getPosition());

		//Cor da aureola conforme posi��o e velocidade.
		sf::Uint8 r = int(posicao->x) % 255; //sf::Uint8 == short int.
		sf::Uint8 g = int(posicao->y) % 255;
		sf::Uint8 b = velocidadeY % 255;
		circle.setFillColor(sf::Color { r, g, b });


	}
	void move(sf::Time dt) {
		testaColisaoComParede();
		this->sprite.move(0, velocidadeY * dt.asSeconds());
		circle.setPosition(sprite.getPosition()); // Move aureola junto.
	}

	void mostra() {
		//Pinta circulo e aureola.
		window->draw(circle);
		Movel::mostra();

	}
	virtual ~Bola() {
	}
protected:
	sf::CircleShape circle;
};

class Barra: public Movel {
public:
	Barra(int x, int y, int direcaoX, int direcaoY, string textura,
			sf::RenderWindow *screen) :
			Movel(x, y, direcaoX, direcaoY, textura, screen) {

		//Define origem no meio da bola.
		sf::FloatRect rect = sprite.getLocalBounds();
		sprite.setOrigin(rect.width / 2, rect.height / 2);


	}
	void move(sf::Time dt) {
		testaColisaoComParede();
		this->sprite.move( velocidadeX * dt.asSeconds(),0);
	}

	void mostra() {
		//Pinta circulo e aureola.
		window->draw(sprite);

	}
	virtual ~Barra() {
	}
};

class Mageste: public Movel {
public:
	Mageste(int x, int y, int direcaoX, int direcaoY, string textura,
				sf::RenderWindow *screen) :
				Movel(x, y, direcaoX, direcaoY, textura, screen) {

			//Define origem no meio da bola.
			sf::FloatRect rect = sprite.getLocalBounds();
			sprite.setOrigin(rect.width / 2, rect.height / 2);

		}
	void testaColisaoComParede() {
			int w = window->getSize().x;
			int h = window->getSize().y;
			float rotacao = 90;
			sf::Vector2f posicao = sprite.getPosition();

			rotacao+=90;
			if (posicao.x +sprite.getGlobalBounds().width/2> w) {
				velocidadeX = -velocidadeX;
				posicao.x = w - 3;
				this->sprite.setRotation(rotacao);
			}
			else if (posicao.x -sprite.getGlobalBounds().width/2< 0) {
				velocidadeX = -velocidadeX;
				posicao.x = 3;
				this->sprite.setRotation(rotacao);
			}
			rotacao+=90;
			if (posicao.y +sprite.getGlobalBounds().height/2> h) {
				velocidadeY = -velocidadeY;
				posicao.y = h - 3;
				this->sprite.setRotation(rotacao);

			}else if (posicao.y - sprite.getGlobalBounds().height/2< 0) {
				velocidadeY = -velocidadeY;
				posicao.y = 3;
				this->sprite.setRotation(rotacao);
			}
			rotacao+=90;

		}
		void move(sf::Time dt) {
		//testaColisaoComParede();


			testaColisaoComParede() ;
			this->sprite.move(velocidadeX * dt.asSeconds(), velocidadeY * dt.asSeconds());

		}

		void mostra() {
			//Pinta circulo e aureola.
			window->draw(sprite);

		}
		virtual ~Mageste() {
		}

};

class Heart: public Movel {
public:
	 Heart(int x, int y, int direcaoX, int direcaoY, string textura,
			sf::RenderWindow *screen) :
			Movel(x, y, direcaoX, direcaoY, textura, screen) {

		//Define origem no meio da bola.
		sf::FloatRect rect = sprite.getLocalBounds();
		sprite.setOrigin(rect.width / 2, rect.height / 2);


	}
	void move(sf::Time dt) {
		testaColisaoComParede();
	static	int  count = 0, Vx = 0;

		if(count >= 0 and count < 10){
			this->sprite.move(velocidadeX * dt.asSeconds(),0);
			if(Vx == -velocidadeX){
				count++;
			}
			Vx = velocidadeX;
		}

		if(count >= 10 and count < 20){
			this->sprite.move(velocidadeX * dt.asSeconds(), velocidadeY * dt.asSeconds());
			if(Vx == -velocidadeX){
				count++;
			}
			Vx = velocidadeX;
		}

		if(count == 20){
			count =0;
		}

	}

	void mostra() {
		//Pinta circulo
		window->draw(sprite);

	}
	virtual ~ Heart() {
	}
};
/**
 * Enumera��o utilizada como constantes para determinar os tipos de objetos que podem ser gerados
 * Basicamente uma lista com Movel e cada um de seus filhos.
 * Aleat�rio � para gara��o 'aleat�ria', mas tambem marca o final do enum
 * e serve para determinar quantos elementos h� no enum.
 *
 */
enum OBJETO {
	MOVEL, BARRA, BOLA, MAGESTE, HEART, ALEATORIO //ALEATORIO � sempre o �ltimo e reprensenta o tamanho da enum
};

class Game {
public:
	Game() {

		window = new sf::RenderWindow(sf::VideoMode(1280, 768), "Tia Betes Game ");
		image.loadFromFile("assets/coracao.png");
		window->setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());



		window->setFramerateLimit(60);

		textureFundo.loadFromFile("assets/fundo.png");
		fundoImage.setTexture(textureFundo);

		font.loadFromFile("assets/Kawaii Stitch.ttf");
		txt.setFont(font);
		txt.setFillColor(sf::Color::Blue);
		txt.setCharacterSize(56);
		txt.setString("Clique na tela para criar!");
		sf::FloatRect bounds(txt.getLocalBounds());
		txt.setPosition(
				window->getSize().x / 2 - (bounds.left + bounds.width / 2),
				window->getSize().y / 2 - (bounds.top + bounds.height / 2));

		/**** LOOP DO JOGO ****/
		while (window->isOpen()) {
			//Controle de tempo de dura��o do loop.
			sf::Time duration = globalClock.restart();

			sf::Event event;
			// Loop de processamento de eventos
			while (window->pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window->close();
				else if (event.type == sf::Event::MouseButtonReleased) {
					if (event.mouseButton.button == sf::Mouse::Left) {
						//Cria objeto no local do click.
						criaMovel(event.mouseButton.x, event.mouseButton.y,
								OBJETO::ALEATORIO);
						txt.setFillColor(sf::Color::Transparent);
					}

				}
			}

			/**** Movimenta os objetos ****/
			for (unsigned int i = 0; i < moveis.size(); ++i) {
				moveis.at(i)->move(duration);
			}

			/**** DESENHA A TELA E OS OBJETOS ****/
			//colide();
			window->clear(sf::Color::White);
			window->draw(fundoImage);
			window->draw(txt);
			for (unsigned int i = 0; i < moveis.size(); ++i) {
				moveis.at(i)->mostra();
			}

			window->display();
			//Aguarda x milissegundos
			//sf::sleep(sf::milliseconds(100));

		}
		/**** FIM DO LOOP PRINCIPAL DO JOGO ****/

	}
private:
	sf::RenderWindow *window;
	vector<Movel*> moveis;
	sf::Clock globalClock;
	sf::Texture textureFundo;
	sf::Sprite fundoImage;
	sf::Font font;
	sf::Text txt;
	sf::Image image; // Icone!


	void criaMovel(int x, int y, OBJETO objeto);
	void colide() {
	    for (unsigned int i = 0; i < moveis.size(); i++) {
	        for (unsigned int j = i + 1; j < moveis.size(); j++) {
	            if (typeid(*moveis.at(i)) == typeid(Bola) && typeid(*moveis.at(j)) == typeid(Bola)) {
	                if (moveis.at(i)->getMyBounds().intersects(moveis.at(j)->getMyBounds())) {
	                    moveis.erase(moveis.begin() + i);
	                    moveis.erase(moveis.begin() + j - 1);
	                }
	            }
	        }
	    }
	}

};

void Game::criaMovel(int x, int y, OBJETO objeto = OBJETO::ALEATORIO) {

	int direcaoX, direcaoY;
	Movel *meuMovel = nullptr;
	string imagem;

	//Numero aleatorio 0 ou 1
	int aleatorio = rand() % 2;
	direcaoX = (aleatorio == 0 ? 1 : -1);
	aleatorio = rand() % 2;
	direcaoY = (aleatorio == 0 ? 1 : -1);

	if (objeto == OBJETO::ALEATORIO) {
		aleatorio = rand() % OBJETO::ALEATORIO;	//Aleat�rio � o tamanho do enum(� o ultimo).
		objeto = static_cast<OBJETO>(aleatorio);
	}

	//Seleciona o tipo de objeto a ser criado com base
	//na ordem do pr�ximo objeto
	switch (objeto) {
	case OBJETO::MOVEL:
		imagem = "assets/sanduiche.png";
		meuMovel = new Movel(x, y, direcaoX, direcaoY, imagem, window);
		break;
	case OBJETO::BOLA:
		imagem = "assets/donut.png";
		meuMovel = new Bola(x, y, direcaoX, direcaoY, imagem, window);
		break;
	case OBJETO::BARRA:
			imagem = "assets/marshmallow.png";
			meuMovel = new Barra(x, y, direcaoX, direcaoY, imagem, window);
			break;
	case OBJETO::MAGESTE:
				imagem = "assets/sorvete.png";
				meuMovel = new Mageste(x, y, direcaoX, direcaoY, imagem, window);
				break;
	case OBJETO::HEART:
					imagem = "assets/morango.png";
					meuMovel = new Heart(x, y, direcaoX, direcaoY, imagem, window);
					break;
	default:
		imagem = "assets/bola.png";
		meuMovel = new Bola(x, y, direcaoX, direcaoY, imagem, window);
		break;
	}

	if (meuMovel != nullptr)
		moveis.push_back(meuMovel);

}

int main(int argc, char **argv) {
	srand(time(NULL));

	Game game;

	return (0);
}

