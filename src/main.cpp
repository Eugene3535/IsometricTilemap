#include <vector>
#include <SFML/Graphics.hpp>

#define TILE_SIZE 32
#define MAP_WIDTH 16
#define MAP_HEIGHT 16

const int level[] =
{
    0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
    0, 1, 0, 0, 0, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
    0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 0, 0, 0,
    0, 0, 1, 0, 3, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 0, 3, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
    0, 0, 1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
    0, 1, 0, 0, 0, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
    0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 0, 0, 0,
    0, 0, 1, 0, 3, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 0, 3, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
    0, 0, 1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1
};

sf::Texture* pTileset;

std::vector<sf::Vertex> GenerateIsometricVertices(int size, unsigned width, unsigned height)
{
    std::vector<sf::Vertex> vertices;

    for (size_t i = 0; i < width; ++i)
        for (size_t j = 0; j < height; ++j)
        {
            int tileNumber = level[i + j * width];
            
            int tu = tileNumber % (pTileset->getSize().x / size);
            int tv = tileNumber / (pTileset->getSize().x / size);


            sf::Vector2f topLeft(i * size, j * size);
            sf::Vector2f topRight((i + 1) * size, j * size);
            sf::Vector2f bottomRight((i + 1) * size, (j + 1) * size);
            sf::Vector2f bottomLeft(i * size, (j + 1) * size);

            vertices.emplace_back(topLeft, sf::Color::White, sf::Vector2f(tu * size, tv * size));
            vertices.emplace_back(topRight, sf::Color::White, sf::Vector2f((tu + 1) * size, tv * size));
            vertices.emplace_back(bottomRight, sf::Color::White, sf::Vector2f((tu + 1) * size, (tv + 1) * size));
            vertices.emplace_back(bottomLeft, sf::Color::White, sf::Vector2f(tu * size, (tv + 1) * size));
        }

// Translate to isometric
    for(auto& v : vertices)
    {
        float isoX = v.position.x - v.position.y;
        float isoY = (v.position.x + v.position.y) * 0.5f;

        v.position = { isoX, isoY };
    }

    return vertices;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1100, 800), "The Game!");
    window.setFramerateLimit(60u);

    sf::Texture tileset;
    if (!tileset.loadFromFile("tileset.png"))
        return -1;

    pTileset = &tileset;

    auto vertices = GenerateIsometricVertices(TILE_SIZE, MAP_WIDTH, MAP_HEIGHT); 

    sf::Transformable transform;
    transform.setOrigin(MAP_WIDTH << 4, MAP_HEIGHT << 4); 
    transform.setPosition(800, 400);
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
        }

        sf::RenderStates states;
        states.texture = pTileset;
        states.transform = transform.getTransform();
        

        window.clear();
        window.draw(&vertices[0], vertices.size(), sf::Quads, states);
        window.display();
    }

    return 0;
}