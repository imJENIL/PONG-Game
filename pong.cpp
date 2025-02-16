#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cstring>

class Ball {
public:
	float x, y, radius;
	int speed_x, speed_y, width, height;

	// Ball initialization
	sf::CircleShape ball;
	Ball(float radius) : radius(radius) {
		ball.setRadius(radius);
		ball.setFillColor(sf::Color::Yellow);
		ball.setOrigin(radius, radius);
	}

	// Ball bounce back from the boundary
	void update() {
		x = speed_x + x;
		y = speed_y + y;

		if (x - radius <= 0 ) {
			x = width / 2;
			y = height / 2;
			speed_x = 6;
			speed_y = 6;
		}

		if (x + radius >= width) {
			x = width / 2;
			y = height / 2;
			speed_x = -6;
			speed_y = 6;
		}

		if (y + radius >= height || y - radius <= 0) {
			speed_y *= -1;
		}

	 // Set ball position
		ball.setPosition(x, y);
	}

	// Declaration of ball
	sf::CircleShape& get_ball() {
		return ball;
	}

};

class Player {

protected:
	// Paddle don't go outside of the screen
	void paddle_bounce() {
		if (y <= 0) {
			y = 0;
		}

		if (y + height >= 800) {
			y = 800 - height;
		}
	}

public:
	int width, height;
	float x, y;
	int speed;

	// Paddle initialization
	sf::RectangleShape paddle;
	Player(int width = 25, int height = 120) : width(width), height(height) {
		paddle.setSize(sf::Vector2f(width, height));
	}

	// Paddle movement
	void update() {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			y = y - speed;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			y = y + speed;
		}

		paddle_bounce();

	 // set paddle position

		paddle.setPosition(x, y);
	}

	// Paddle Declaration
	sf::RectangleShape& get_paddle() {
		return paddle;
	}

};

class AI : public Player {
public:
	// AI Alogorithm if ball center is greater than or equal to paddle center
	void update(float ball_y) {
		if (y + height / 2 >= ball_y) {
			y = y - speed;
		}

		if (y + height / 2 <= ball_y) {
			y = y + speed ;
		}

		paddle_bounce();

		paddle.setPosition(x, y);
	}
};

class Button {
	sf::CircleShape button;
	sf::Text text;
	sf::Font font;
	bool clicked = false;
public:
	Button(float x, float y, float radius, const std::string& label) {
		button.setRadius(radius);
		button.setOrigin(radius, radius);
		button.setPosition(x, y);
		button.setFillColor(sf::Color::Cyan);
		
		if (!font.loadFromFile("C:/Users/jenil/AppData/Local/Microsoft/Windows/Fonts/fox_version_5_by_mickeyfan123_daxvfx5.ttf")) {
			std::cerr << "Error File not Found!" << std::endl;
		}
		text.setFont(font);
		text.setString(label);
		text.setFillColor(sf::Color::Red);
		text.setCharacterSize(40);
		text.setPosition(x, y);
		text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
	}

	bool isclicked(const sf::Vector2i& mousepos) {
		return button.getGlobalBounds().contains(sf::Vector2f(mousepos)) && sf::Mouse::isButtonPressed(sf::Mouse::Left);
	}

	void draw(sf::RenderWindow& window) {
		if (!clicked) {
			window.clear(sf::Color::Yellow);
			window.draw(button);
			window.draw(text);
		}
		else {
			clicked = true;
			window.close();
		}
	}
};

int main() {

	// Initialization
	int width = 1200;
	int height = 800;
	int radius = 20;
	int score_player = 0;
	int score_cpu = 0;

	sf::Font font;
	if (!font.loadFromFile("C:/Users/jenil/AppData/Local/Microsoft/Windows/Fonts/fox_version_5_by_mickeyfan123_daxvfx5.ttf")) {
		std::cerr << "Error File not Found!" << std::endl;
	}

	// Window
	sf::RenderWindow window(sf::VideoMode(width, height), "PONG");
	window.setFramerateLimit(120);

	// Drawing
	sf::RectangleShape line(sf::Vector2f(5, 800));
	sf::CircleShape circle(100);
	sf::RectangleShape rec(sf::Vector2f(width / 2, 800));

	line.setPosition(width / 2, 0);

	circle.setPosition(width / 2, height / 2);
	circle.setOutlineThickness(5);
	circle.setFillColor(sf::Color(179, 255, 236));
	circle.setOrigin(100, 100);

	rec.setPosition(width / 2, 0);
	rec.setFillColor(sf::Color(0, 153, 115));

	// Ball
	Ball ball(radius);
	ball.width = width;
	ball.height = height;
	ball.x = width / 2;
	ball.y = height / 2;
	ball.speed_x = 6;
	ball.speed_y = 6;
	ball.ball.setPosition(ball.x, ball.y);

	// Player Paddle
	Player player;
	player.speed = 6;
	player.x = 0;
	player.y = height / 2 - 120 / 2;
	player.paddle.setPosition(player.x, player.y);

	// AI paddle
	AI cpu;
	cpu.speed = 6;
	cpu.x = width - 25;
	cpu.y = height / 2 - 120 / 2;
	cpu.paddle.setPosition(cpu.x, cpu.y);

	// Player score
	sf::Text score_p;
	score_p.setFont(font);
	score_p.setCharacterSize(40);
	score_p.setPosition(width / 4, height / 8);

	// CPU score
	sf::Text score_c;
	score_c.setFont(font);
	score_c.setCharacterSize(40);
	score_c.setPosition(width - width / 4, height / 8);

	// Game state
	bool game_state = false;

	// Play button
	Button button(width / 2 , height / 2 , 80, "PLAY");

	while (window.isOpen()) {
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				window.close();
			}
		// Start the game when pressed the space button
			sf::Vector2i mousepos = sf::Mouse::getPosition(window);
			if (button.isclicked(mousepos)) {
				game_state = true;
			}
			
		}
		
		if (game_state) {
			// Update
			ball.update();
			player.update();
			cpu.update(ball.y);
			score_p.setString(std::to_string(score_player));
			score_c.setString(std::to_string(score_cpu));

			// Collisions

			if (ball.get_ball().getGlobalBounds().intersects(player.get_paddle().getGlobalBounds())) {
				ball.speed_x *= -1;
				score_player++;
			}

			if (ball.get_ball().getGlobalBounds().intersects(cpu.get_paddle().getGlobalBounds())) {
				ball.speed_x *= -1;
				score_cpu++;
			}
		}
		// Show
		window.clear(sf::Color(26, 255, 198));
		window.draw(rec);
		window.draw(circle);
		window.draw(line);
		window.draw(score_p);
		window.draw(score_c);
		window.draw(ball.get_ball());
		window.draw(player.get_paddle());
		window.draw(cpu.get_paddle());

		if (!game_state) {
			button.draw(window);
		}
		window.display();

	}

	return 0;

}