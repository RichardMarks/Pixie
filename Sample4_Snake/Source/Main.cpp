// Main.cpp

#include "Pixie.h"
#include <windows.h>

static const unsigned WINDOW_WIDTH = 800;
static const unsigned WINDOW_HEIGHT = 600;

static const unsigned CELL_SIZE = 20;
static const unsigned CELL_WIDTH = WINDOW_WIDTH / CELL_SIZE;
static const unsigned CELL_HEIGHT = WINDOW_HEIGHT / CELL_SIZE;

static const unsigned GO_UP = 0;
static const unsigned GO_DOWN = 1;
static const unsigned GO_LEFT = 2;
static const unsigned GO_RIGHT = 3;

namespace snake
{
    class Entity
    {
    public:
        unsigned x;
        unsigned y;
        Sprite sprite;

        inline void Sync()
        {
            sprite.SetPosition((float)x * CELL_SIZE, (float)y * CELL_SIZE);
        }
    };
};

class SnakeSegment : public snake::Entity
{
public:
    SnakeSegment(unsigned x = 65535, unsigned y = 65535)
    {
        sprite.SetBitmap("snake.pix");
        if (x != 65535 && y != 65535)
        {
            this->x = x;
            this->y = y;
            sprite.SetVisible(true);
        }
        else
        {
            sprite.SetVisible(false);
        }
        Sync();
    }
};

class SnakeCoin : public snake::Entity
{
public:
    SnakeCoin()
    {
        sprite.SetBitmap("coin.png");
        Respawn();
    }

    void Respawn()
    {
        x = 1 + Rand() % (CELL_WIDTH - 2);
        y = 1 + Rand() % (CELL_HEIGHT - 2);
        Sync();
    }
};

class Snake
{
public:
    Snake();
    ~Snake();

    void Run(GameHelper& gameHelper);

private:
    Snake& operator=(const Snake& o) {}

    InputManager& input;
    Array<SnakeSegment> segments;
    unsigned direction;
    unsigned time;
    unsigned timeout;
    unsigned coins;
    unsigned speed;
    bool dead;
    bool waiting;
    SnakeCoin coin;
    StaticText infoText;
    StaticText gameOverMessage;

    void Restart();
    void Update();
    void UpdateInputControls();
    void UpdateHUD();
    void AddSegment(unsigned x, unsigned y);
    void MoveSnake();
    bool CheckForWallCollision();
    bool CheckForSelfCollision();
    bool CheckForCoinCollision();
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	DefaultWinMain(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
    GameHelper gameHelper("Snake");
    Snake snake;
    snake.Run(gameHelper);
    return 0;
}

Snake::Snake() : input(*InputManager::GetInstance()) {}

Snake::~Snake() {}

void Snake::Run(GameHelper& gameHelper)
{
	Screen screen(WINDOW_WIDTH, WINDOW_HEIGHT);
	FrameTime time;

    infoText.SetPriority(FLOAT_MAX);
    gameOverMessage.SetText("G A M E  O V E R");
    gameOverMessage.SetPosition((WINDOW_WIDTH - 128) * 0.5f, (WINDOW_HEIGHT - 8) * 0.5f);
    gameOverMessage.SetPriority(FLOAT_MAX);
    gameOverMessage.SetColor(RGB16(255, 0, 0));

    Restart();

    bool running = true;
	while (running)
	{
	    float deltaTime = time.Update();
	    running = gameHelper.UpdateGame(deltaTime, screen);
        if (input.WasKeyPressed(KEY_ESCAPE))
		{
		    running = false;
		}
        Update();
        screen.GetBackBuffer().Clear();
	}
}

void Snake::Restart()
{
    unsigned startx = CELL_WIDTH / 2;
    unsigned starty = CELL_HEIGHT / 2;

    if (segments.GetItemCount() > 0)
    {
        for (int i = 0; i < segments.GetItemCount(); i++)
        {
            SnakeSegment& segment = segments.Get(i);
            segment.sprite.SetVisible(false);
        }
        segments.Clear();
    }

    AddSegment(startx, starty);
    AddSegment(startx - 1, starty);
    AddSegment(startx - 2, starty);
    segments.Get(0).sprite.SetCel(0);

    direction = GO_RIGHT;
    timeout = 10;
    time = 0;
    dead = false;
    waiting = true;
    coins = 0;
    speed = 1;
    coin.Respawn();
    gameOverMessage.SetVisible(false);
    UpdateHUD();
}

void Snake::Update()
{
    if (dead)
    {
        gameOverMessage.SetVisible(true);
        if (input.WasAnyKeyPressed())
        {
            Restart();
        }
        return;
    }

    if (waiting)
    {
        if (input.WasAnyKeyPressed())
        {
            waiting = false;
        }
        return;
    }

    UpdateInputControls();

    time += speed;
    if (time < timeout)
    {
        return;
    }

    time = 0;

    if (CheckForWallCollision() || CheckForSelfCollision())
    {
        dead = true;
        return;
    }

    if (CheckForCoinCollision())
    {
        coin.Respawn();
        coins++;
        if (coins % 5 == 0)
        {
            speed++;
        }
    }
    else
    {
        SnakeSegment& tail = segments.Get(segments.GetItemCount() - 1);
        tail.sprite.SetVisible(false);
        segments.RemoveLast(); 
    }

    MoveSnake();
    
    UpdateHUD();
}

void Snake::UpdateInputControls()
{
    if (input.IsKeyDown(KEY_UP) && direction != GO_DOWN)
    {
        direction = GO_UP;
    }
    else if (input.IsKeyDown(KEY_DOWN) && direction != GO_UP)
    {
        direction = GO_DOWN;
    }
    else if (input.IsKeyDown(KEY_LEFT) && direction != GO_RIGHT)
    {
        direction = GO_LEFT;
    }
    else if (input.IsKeyDown(KEY_RIGHT) && direction != GO_LEFT)
    {
        direction = GO_RIGHT;
    }
}

void Snake::UpdateHUD()
{
    char text[255];
    SNPrintF(text, 255, 
        "Length: %4d    Coins: %4d    Speed: %4d", segments.GetItemCount(), coins, speed);
    infoText.SetText(text);
}

void Snake::AddSegment(unsigned x, unsigned y)
{
    SnakeSegment segment(x, y);
    segment.sprite.SetCel(1);
    segments.Add(segment);
}

void Snake::MoveSnake()
{
    SnakeSegment& head = segments.Get(0);
    unsigned headx = head.x;
    unsigned heady = head.y;

    int movex[] = { 0, 0, -1, 1 };
    int movey[] = { -1, 1, 0, 0 };

    SnakeSegment nextsegment(headx + movex[direction], heady + movey[direction]);
    nextsegment.sprite.SetCel(0);
    segments.InsertBefore(0, nextsegment);

    for (int i = 1; i < segments.GetItemCount(); i++)
    {
        segments.Get(i).sprite.SetCel(1);
    }
}

bool Snake::CheckForWallCollision()
{
    unsigned headx = segments.Get(0).x;
    unsigned heady = segments.Get(0).y;

    return (
            (headx == 0) ||
            (heady == 0) ||
            (headx == CELL_WIDTH-1) ||
            (heady == CELL_HEIGHT-1));
}

bool Snake::CheckForSelfCollision()
{
    unsigned headx = segments.Get(0).x;
    unsigned heady = segments.Get(0).y;

    for (int i = 1; i < segments.GetItemCount(); i++)
    {
        if (segments.Get(i).x == headx && segments.Get(i).y == heady)
        {
            return true;
        }
    }
    return false;
}

bool Snake::CheckForCoinCollision()
{
    return (segments.Get(0).x == coin.x && segments.Get(0).y == coin.y);
}
