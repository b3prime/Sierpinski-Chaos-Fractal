#include <SFML/Graphics.hpp>
#include <iostream>

bool isInTriangle(sf::Vector2f point, sf::CircleShape &triangle)
{
	float yShift = triangle.getPosition().y;

	//Triangle points, as vectors
	sf::Vector2f A, B, C;
	A = triangle.getPoint(0);
	B = triangle.getPoint(1);
	C = triangle.getPoint(2);

	//account for vertical centering
	A.y += yShift;
	B.y += yShift;
	C.y += yShift;

	//calculate weights of the point relative to triangle
	//refer to: https://www.youtube.com/watch?v=HYAgJN3x4GA

	float w1, w2;
	w1 = (A.x * (C.y - A.y) + (point.y - A.y) * (C.x - A.x) - point.x * (C.y - A.y));
	w1 /= (B.y - A.y) * (C.x - A.x) - (B.x - A.x) * (C.y - A.y);

	w2 = (point.y - A.y - w1 * (B.y - A.y)) / (C.y - A.y);

	if (w1 >= 0 and w2 >= 0 and w1 + w2 <= 1)
	{
		return true;
	}

	return false;
}

sf::Vector2f randomDotPos(sf::CircleShape &triangle)
{
	float triangleX = 0; //THIS MIGHT BREAK!
	float triangleYShift = triangle.getPosition().y;

	sf::Vector2f randomPoint(0, 0);

	while (isInTriangle(randomPoint, triangle) == false)
	{
		//continuously generate new random points until they are in the triangle
		randomPoint.x = rand() % 800 + 0;
		randomPoint.y = rand() % 800 + 0;
	}
	return randomPoint;
}

sf::Vector2f calculateVertexMidpoint(sf::Vector2f &vertex, sf::Vector2f &point)
{
	float vx, vy, px, py;
	vx = vertex.x;
	vy = vertex.y;
	px = point.x;
	py = point.y;

	float xval, yval;
	xval = (vx + px) / 2;
	yval = (vy + py) / 2;

	sf::Vector2f midPoint(xval, yval);
	return midPoint;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "SFML works!");
	sf::CircleShape triangle(400, 3); //We create a triangle to use it as a reference object

	sf::CircleShape dot(0.5);

	//CONSTANTS
	float yShift = 75;

	//ONE-TIME TRANSFORMATIONS
	triangle.setOutlineThickness(5);
	triangle.setOutlineColor(sf::Color::White);
	triangle.setFillColor(sf::Color::Black);
	triangle.move(0, yShift);

	sf::Clock clock;
	sf::Vector2f point(randomDotPos(triangle));

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (clock.getElapsedTime().asMicroseconds() >= 1000)
		{
			
			//Choose random vertex
			int vertexNum = rand() % 3 + 1; //1, 2, or 3
			sf::Vector2f vertexPos(triangle.getPoint(vertexNum));
			vertexPos.y += yShift; //must account for vertical centering

			sf::Vector2f midPoint(calculateVertexMidpoint(vertexPos, point));

			dot.setPosition(midPoint.x, midPoint.y);
			point.x = midPoint.x;
			point.y = midPoint.y;

			clock.restart();
		}

		window.draw(dot);
		window.display();
	}
}