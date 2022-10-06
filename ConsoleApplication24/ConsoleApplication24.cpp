#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <ctime>
#include <vector>

using namespace std;

//Black         Чёрный			    // 0
//Blue          Синий			    // 1
//Green         Зеленый			    // 2
//Cyan          Голубой			    // 3
//Red           Красный			    // 4
//Magenta       Пурпурный		    // 5
//Brown         Коричневый			// 6
//LightGray     Светло-серый		// 7
//DarkGray      Темно-серый		    // 8
//LightBlue     Голубой		        // 9
//LightGreen    Светло-зеленый	    // 10
//LightCyan     Светло-голубой		// 11
//LightRed      Светло-красный		// 12
//LightMagenta  Светло-пурпурный	// 13
//Yellow        Желтый			    // 14

enum Directional
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};


void SetColor(int text, int background)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

enum faceColors {
    WallC = 13,
    PlayerC = 14,
    AmfibiaC = 1,
    BatC = 8,
    OrcC = 10,
    WaterC = 9,
    BridgeC = 6,
    WoodC = 2,
    BigMountainC = 15,
    MountainC = 7
};

void setColor(char face = 15)
{
    if (face == '#')
        SetColor(WallC, 0);

    else if (face == 'P')
        SetColor(PlayerC, 0);

    else if (face == 'A')
        SetColor(AmfibiaC, 0);
    else if (face == 'B')
        SetColor(BatC, 0);
    else if (face == 'O')
        SetColor(OrcC, 0);

    else if (face == '~')
        SetColor(WaterC, 0);
    else if (face == '=')
        SetColor(BridgeC, 0);
    else if (face == 'W' || face == 'w')
        SetColor(WoodC, 0);
    else if (face == 'M')
        SetColor(BigMountainC, 0);
    else if (face == 'm')
        SetColor(MountainC, 0);
    else if (face == 'H')
        SetColor(BridgeC, 0);
    else SetColor(15, 0);
}

class Map;

class ListEnemies;

class Time
{
public:
    long int TIME;  // время с запуска программы
    long int realTime;
    long int pressButtonTime;
    long int pressButtonFire;
    long int flyTime;
    SYSTEMTIME _t0, _t;
    double speed;
    int getTime(bool);
    Time()
    {
        TIME = 0;
        GetLocalTime(&_t0);
        realTime = (_t0.wHour * 3600 + _t0.wMinute * 60 + _t0.wSecond) * 1000 + _t0.wMilliseconds;
        pressButtonTime = realTime;
        pressButtonFire = realTime;
        flyTime = realTime;
    }
};

int Time::getTime(bool forFlyAmmo = false)
{
    if (!forFlyAmmo)
    {
        GetLocalTime(&_t);
        TIME = (_t.wHour * 3600 + _t.wMinute * 60 + _t.wSecond) * 1000 + _t.wMilliseconds - realTime;
        return TIME;

    }
    else if (forFlyAmmo)
    {
        SYSTEMTIME _t;
        GetLocalTime(&_t);
        return (_t.wHour * 3600 + _t.wMinute * 60 + _t.wSecond) * 1000 + _t.wMilliseconds;
    }
}

class weaponAmmo : public Time
{
public:
    int damage;
    int x, y;
    int range;
    string whatRun;
    double velocity;
    char faceAmmo = '+';
    char prevFaceAmmo = ' ';
    Directional direction;
    bool isEnd;

    weaponAmmo(double, int, int, int, char);

    //const weaponAmmo operator = (const weaponAmmo& x)
    //{
    //    if (&x == this) return *this;

    //}
};

weaponAmmo::weaponAmmo(double velocity = 1, int damage = 1, int x = 1, int y = 1, char faceAmmo = '+') : Time()
{
    this->velocity = velocity; this->damage = damage; this->faceAmmo = faceAmmo; this->x = x; this->y = y;
    this->isEnd = false;
}

class weapon : public weaponAmmo
{
private:
    friend weaponAmmo;
    int countAmmo;
    int maxCountAmmo;
    int countFlyAmmo = 0;
    int id;
    bool isSpawnAmmo = false;
    string name;
    HANDLE hStdOutWeapon = GetStdHandle(STD_OUTPUT_HANDLE);

public:

    weaponAmmo* flyAmmolist = nullptr;

    int xG = 0, yG = 0;

    weapon(int);
    ~weapon();

    int RetCountAmmo() { return countAmmo; }
    void SetCountAmmo(int countAmmo) { this->countAmmo = countAmmo; }

    int RetCountFlyAmmo() { return countFlyAmmo; }
    void SetCountFlyAmmo(int countFlyAmmo) { this->countFlyAmmo = countFlyAmmo; }

    int RetMaxCountAmmo() { return maxCountAmmo; }
    void SetMaxCountAmmo(int maxCountAmmo) { this->maxCountAmmo = maxCountAmmo; }

    int RetID() { return id; }
    void SetID(int id) { this->id = id; }

    string RetName() { return name; }
    void SetName(string name) { this->name = name; }

    bool RetIsSpawnAmmo() { return isSpawnAmmo; }
    void SetIsSpawnAmmo(bool isSpawnAmmo) { this->isSpawnAmmo = isSpawnAmmo; }

    HANDLE RetHW() { return hStdOutWeapon; }

    void InsertCountAmmo(int n = 1)
    {
        if (countAmmo - n < 0)
            countAmmo = 0;
        else countAmmo -= n;
    }
    void AddCountAmmo(int n = 1)
    {
        if (countAmmo + n > maxCountAmmo)
            countAmmo = maxCountAmmo;
        else countAmmo += n;
    }
    void InsertCountFlyAmmo()
    {
        countFlyAmmo = 0;
    }
    void AddCountFlyAmmo(int n = 1)
    {
        countFlyAmmo += n;
    }

    void AddflyAmmo(Directional, int, int, Map&);
    void IsertflyAmmo();
    void DrawFlyAmmo(Map&, ListEnemies&);
    void CheckInsert();
    bool CheckMove(int, int, Directional, int, ListEnemies&, Map&, bool);

    void SpawnAmmo(Map&);



};

weapon::weapon(int countAmmo = 10)
{
    this->countAmmo = countAmmo;
}

weapon::~weapon()
{
    if (flyAmmolist == nullptr)
    {
        delete[] flyAmmolist;
        flyAmmolist = nullptr;
    }
}

void weapon::IsertflyAmmo()
{
    InsertCountFlyAmmo();
    if (flyAmmolist != nullptr)
    {
        delete[] flyAmmolist;
        flyAmmolist = nullptr;
    }
}

class Player : public Time {
private:
    char face;
    char prevFace = ' ';
    int x, y;
    int color = 14;         // Желтый
    double speed;
    double FireSpeed;
    Directional direction;
    bool isDead = false;
public:

    int selectedWeapon;
    int countWeapon;
    weapon* ListWeapon = nullptr;

    Player(char, int, int, double, double);
    ~Player();
    int RetX() { return x; }
    int RetY() { return y; }
    char RetF() { return face; }
    bool RetIsDead() { return isDead; }
    void SetIsDead(bool isDead) { this->isDead = isDead; }
    void Move(Map&, ListEnemies&);
    void UI(Map&);
    void LoadWeapon();
    void LoadAmmo();

    void SpawninGroundAmmo(Map&);

    //void SetP(Map &map, int x, int y, char face = 'P')
    //{
    //    map.RetMapxy(x, y, face);
    //}

};

Player::Player(char face = 'S', int x = 2, int y = 2, double speed = 1, double FireSpeed = 0.2) : Time()
{
    this->face = face;
    this->x = x;
    this->y = y;
    this->speed = speed;
    this->FireSpeed = FireSpeed;
    direction = UP;
    isDead = false;
}

Player::~Player()
{
    if (ListWeapon != nullptr)
    {
        delete[] ListWeapon;
        ListWeapon = nullptr;
    }
}

void Player::SpawninGroundAmmo(Map& map)
{
    for (int i = 0; i < countWeapon; i++)
    {
        if (ListWeapon[i].RetCountAmmo() <= 0 && ListWeapon[i].RetIsSpawnAmmo() == false)
        {
            ListWeapon[i].SetIsSpawnAmmo(true);
            ListWeapon[i].SpawnAmmo(map);
        }
        if (ListWeapon[i].RetIsSpawnAmmo() == true)
        {
            if (x == ListWeapon[i].xG && y == ListWeapon[i].yG)
            {
                ListWeapon[i].AddCountAmmo(100);
                ListWeapon[i].SetIsSpawnAmmo(false);
            }
        }
    }
}

class Map
{
private:
    HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
    int x = 0, y = 0;
    string name;
    char** mapxy;
    char symbols[10];
public:
    int GetX() { return x; }
    int GetY() { return y; }
    char RetMapxy(int x, int y) { return mapxy[x][y]; }
    void SetMapxy(int x, int y, char face) { mapxy[x][y] = face; }
    HANDLE RetH() { return hStd; }

    Map(string name = "map1.txt")
    {
        this->name = name;
        ifstream read(name);

        if (!read)
        {
            cout << "Ошибка";
        }
        else {
            string stroka;
            getline(read, stroka);
            x = stroka.length();

            while (read)
            {
                getline(read, stroka);
                y++;
            }

            mapxy = new char* [x];
            for (int i = 0; i < x; i++)
            {
                mapxy[i] = new char[y];
            }
        }
    }

    void SetPl(Player& player)
    {
        SetConsoleCursorPosition(hStd, { (SHORT)player.RetX(), (SHORT)player.RetY() });
        setColor(player.RetF());
        cout << player.RetF();
        mapxy[player.RetX()][player.RetY()] = player.RetF();
    }

    void DelPl(int x, int y, char prevFace)
    {
        SetConsoleCursorPosition(hStd, { (SHORT)x, (SHORT)y });
        setColor(prevFace);
        cout << prevFace;
        mapxy[x][y] = prevFace;
    }

    void MovePl(int x, int y, char& prevFace, char face)
    {
        SetConsoleCursorPosition(hStd, { (SHORT)x, (SHORT)y });
        prevFace = mapxy[x][y];

        /*if (mapxy[x][y] != ' ') {
            char temp;
            temp = mapxy[x][y];

        }*/
        setColor(face);
        cout << face;
        mapxy[x][y] = face;
    }

    bool CheckWall(int x, int y)
    {
        for (int i = 0; i < 10; i++)
        {
            if (symbols[i] == '\0') break;
            if (mapxy[x][y] == symbols[i]) return false;
        }
        return true;
    }

    bool CheckEnemies(int x, int y)
    {
        if (mapxy[x][y] == 'A' || mapxy[x][y] == 'B' || mapxy[x][y] == 'O') return true;
        return false;
    }

    void Read()
    {
        ifstream read(name);

        if (!read)
        {
            cout << "Ошибка";
        }
        else {
            string stroka;
            for (int i = 0; i < y; i++)
            {
                getline(read, stroka);
                for (int j = 0; j < x; j++)
                {
                    if (stroka[j] != ' ')
                    {
                        bool check = true;
                        for (int k = 0; k < 10; k++)
                        {
                            if (symbols[k] == stroka[j])
                            {
                                check = false;
                                break;
                            }
                        }
                        if (check)
                        {
                            for (int k = 0; k < 10; k++)
                            {
                                if (symbols[k] == '\0')
                                {
                                    symbols[k] = stroka[j];
                                    break;
                                }
                            }
                        }
                    }
                    //cout << stroka[j];
                    mapxy[j][i] = stroka[j];
                }
                //cout << endl;
            }
        }
        //for (int k = 0; k < 10; k++)
        //{
        //    cout << symbols[k] << endl;
        //}
        read.close();
    }

    void Out()
    {
        void* handle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO structCursorInfo;
        GetConsoleCursorInfo(handle, &structCursorInfo);
        structCursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(handle, &structCursorInfo);

        for (int i = 0; i < y; i++)
        {
            for (int j = 0; j < x; j++)
            {
                setColor(mapxy[j][i]);
                cout << mapxy[j][i];
                SetColor(15, 0);
            }
            cout << endl;
        }
    }
};

class Enemy : public Time
{
protected:
    int x, y;
    struct Characteristics {
        float speed;
        int rangeOfDetection;
    };

    struct Skills
    {
        bool toSwim = false;
        bool toFly = false;
    };

    Characteristics ch;
    Skills sk;

    char face;
    char prevFace = ' ';

    //long int TIME;  // время с запуска программы
    //long int realTime;
    //long int pressButtonTime;
    //SYSTEMTIME _t0, _t;
public:
    Enemy() : Time() {};
    int RetX() { return x; }
    int RetY() { return y; }
    char RetFace() { return face; }
    void Move(Map& map, Player& player);
};

class Amfibia : public Enemy
{
public:
    Amfibia(Map& map, int x = 1, int y = 1, float speed = 0.2, int rangeOfDetection = 5)
    {
        this->x = x;
        this->y = y;
        ch.speed = speed;
        ch.rangeOfDetection = rangeOfDetection;
        face = 'A';
        sk.toSwim = true;
        map.SetMapxy(x, y, face);
    }
};

class Bat : public Enemy
{
public:
    Bat(Map& map, int x = 1, int y = 1, float speed = 0.4, int rangeOfDetection = 5)
    {
        this->x = x;
        this->y = y;
        ch.speed = speed;
        ch.rangeOfDetection = rangeOfDetection;
        face = face = 'B';
        sk.toFly = true;
        map.SetMapxy(x, y, face);
    }
};

class Orc : public Enemy
{
public:
    Orc(Map& map, int x = 1, int y = 1, float speed = 0.3, int rangeOfDetection = 5)
    {
        this->x = x;
        this->y = y;
        ch.speed = speed;
        ch.rangeOfDetection = rangeOfDetection;
        face = 'O';
        map.SetMapxy(x, y, face);
    }
};

class ListEnemies : Enemy
{
private:
    int qOfEnemies = 0;
    vector <Enemy> List;
public:
    void AddAmfibia(Map& map, int x = 1, int y = 1, float speed = 0.2, int rangeOfDetection = 5)
    {
        qOfEnemies++;
        Amfibia A(map, x, y, speed, rangeOfDetection);
        List.push_back(A);
    }

    void AddBat(Map& map, int x = 1, int y = 1, float speed = 0.4, int rangeOfDetection = 8)
    {
        qOfEnemies++;
        Bat A(map, x, y, speed, rangeOfDetection);
        List.push_back(A);
    }

    void AddOrc(Map& map, int x = 1, int y = 1, float speed = 0.3, int rangeOfDetection = 4)
    {
        qOfEnemies++;
        Orc A(map, x, y, speed, rangeOfDetection);
        List.push_back(A);
    }

    void MoveAll(Map& map, Player& player)
    {
        for (int i = 0; i < qOfEnemies; i++)
            List.at(i).Move(map, player);

    }

    void UI(Map& map)
    {
        setColor();
        SetConsoleCursorPosition(map.RetH(), { (SHORT)82,(SHORT)3 });
        cout << "Оставшиеся враги: " << qOfEnemies;
    }

    void Delete(int i, Map& map)
    {
        qOfEnemies--;
        vector<Enemy>::iterator pos = List.begin() + i;
        map.SetMapxy(List.at(i).RetX(), List.at(i).RetY(), ' ');
        swap(List[i], List.back());
        List.pop_back();
    }

    vector <Enemy> RetList() { return List; }
    int RetqOfEnemies() { return qOfEnemies; }
};

void Enemy::Move(Map& map, Player& player)
{
    GetLocalTime(&_t);
    TIME = (_t.wHour * 3600 + _t.wMinute * 60 + _t.wSecond) * 1000 + _t.wMilliseconds - realTime;

    bool isMoved = false;
    bool isFound = false;
    int random;
    int Px, Py;
    if (TIME + realTime - pressButtonTime >= (int)(100 / ch.speed))
    {

        for (int i = -ch.rangeOfDetection; i <= ch.rangeOfDetection; i++)
        {
            for (int j = -ch.rangeOfDetection; j <= ch.rangeOfDetection; j++)
            {
                if (i + x > 0 && i + x < map.GetX() && j + y > 0 && j + y < map.GetY())
                {
                    if (map.RetMapxy(i + x, j + y) == 'P')
                    {
                        Px = i + x;
                        Py = j + y;
                        isFound = true;
                        break;
                    }
                }
            }
            if (isFound) break;
        }


        if (isFound)
        {
            while (!isMoved) {
                random = 1 + rand() % 2;
                if (random == 1)
                {
                    if (Px == x);
                    else if (Px < x)
                    {
                        if ((sk.toFly || (sk.toSwim && map.RetMapxy(x - 1, y) == '~')) && map.RetMapxy(x - 1, y) != '#' && !map.CheckEnemies(x - 1, y))
                        {
                            map.DelPl(x, y, prevFace);
                            map.MovePl(--(x), y, prevFace, face);
                            isMoved = true;
                        }
                        else
                            if (map.CheckWall(x - 1, y) && !map.CheckEnemies(x - 1, y))
                            {
                                map.DelPl(x, y, prevFace);
                                map.MovePl(--(x), y, prevFace, face);
                                isMoved = true;
                            }
                            else
                            {
                                random = 1 + rand() % 2;
                                if (random == 1)
                                {
                                    if (!map.CheckEnemies(x, y - 1) && map.CheckWall(x, y - 1))
                                    {
                                        map.DelPl(x, y, prevFace);
                                        map.MovePl(x, --(y), prevFace, face);
                                        isMoved = true;
                                    }
                                }
                                else
                                {
                                    if (!map.CheckEnemies(x, y + 1) && map.CheckWall(x, y + 1))
                                    {
                                        map.DelPl(x, y, prevFace);
                                        map.MovePl(x, ++(y), prevFace, face);
                                        isMoved = true;
                                    }
                                }
                            }
                    }
                    else
                    {
                        if ((sk.toFly || (sk.toSwim && map.RetMapxy(x + 1, y) == '~')) && map.RetMapxy(x + 1, y) != '#' && !map.CheckEnemies(x + 1, y))
                        {
                            map.DelPl(x, y, prevFace);
                            map.MovePl(++(x), y, prevFace, face);
                            isMoved = true;
                        }
                        else
                            if (map.CheckWall(x + 1, y) && !map.CheckEnemies(x + 1, y)) {
                                map.DelPl(x, y, prevFace);
                                map.MovePl(++(x), y, prevFace, face);
                                isMoved = true;
                            }
                            else
                            {
                                random = 1 + rand() % 2;
                                if (random == 1)
                                {
                                    if (!map.CheckEnemies(x, y - 1) && map.CheckWall(x, y - 1))
                                    {
                                        map.DelPl(x, y, prevFace);
                                        map.MovePl(x, --(y), prevFace, face);
                                        isMoved = true;
                                    }
                                }
                                else
                                {
                                    if (map.CheckWall(x, y + 1) && !map.CheckEnemies(x, y + 1)) {
                                        map.DelPl(x, y, prevFace);
                                        map.MovePl(x, ++(y), prevFace, face);
                                        isMoved = true;
                                    }
                                }
                            }
                    }
                }
                else
                {
                    if (Py == y);
                    else if (Py < y)
                    {
                        if ((sk.toFly || (sk.toSwim && map.RetMapxy(x, y - 1) == '~')) && map.RetMapxy(x, y - 1) != '#' && !map.CheckEnemies(x, y - 1))
                        {
                            map.DelPl(x, y, prevFace);
                            map.MovePl(x, --y, prevFace, face);
                            isMoved = true;
                        }
                        else
                            if (map.CheckWall(x, y - 1) && !map.CheckEnemies(x, y - 1)) {
                                map.DelPl(x, y, prevFace);
                                map.MovePl(x, --(y), prevFace, face);
                                isMoved = true;
                            }
                            else
                            {
                                random = 1 + rand() % 2;
                                if (map.CheckWall(x - 1, y) && !map.CheckEnemies(x - 1, y))
                                {
                                    map.DelPl(x, y, prevFace);
                                    map.MovePl(--(x), y, prevFace, face);
                                    isMoved = true;
                                }
                                else
                                    if (map.CheckWall(x + 1, y) && !map.CheckEnemies(x + 1, y))
                                    {
                                        map.DelPl(x, y, prevFace);
                                        map.MovePl(++(x), y, prevFace, face);
                                        isMoved = true;
                                    }
                            }

                    }
                    else
                    {
                        if ((sk.toFly || (sk.toSwim && map.RetMapxy(x, y + 1) == '~')) && map.RetMapxy(x, y + 1) != '#' && !map.CheckEnemies(x, y + 1))
                        {
                            map.DelPl(x, y, prevFace);
                            map.MovePl(x, ++y, prevFace, face);
                            isMoved = true;
                        }
                        else
                            if (map.CheckWall(x, y + 1) && !map.CheckEnemies(x, y + 1)) {
                                map.DelPl(x, y, prevFace);
                                map.MovePl(x, ++(y), prevFace, face);
                                isMoved = true;
                            }
                            else
                            {
                                {
                                    random = 1 + rand() % 2;
                                    if (map.CheckWall(x - 1, y) && !map.CheckEnemies(x - 1, y))
                                    {
                                        map.DelPl(x, y, prevFace);
                                        map.MovePl(--(x), y, prevFace, face);
                                        isMoved = true;
                                    }
                                    else
                                        if (map.CheckWall(x + 1, y) && !map.CheckEnemies(x + 1, y))
                                        {
                                            map.DelPl(x, y, prevFace);
                                            map.MovePl(++(x), y, prevFace, face);
                                            isMoved = true;
                                        }
                                }
                            }
                    }
                }
            }
        }
        else
        {
            while (!isMoved) {
                random = 1 + rand() % 4;
                if (random == 1)
                {
                    if ((sk.toFly || (sk.toSwim && map.RetMapxy(x - 1, y) == '~')) && map.RetMapxy(x - 1, y) != '#' && !map.CheckEnemies(x - 1, y))
                    {
                        map.DelPl(x, y, prevFace);
                        map.MovePl(--(x), y, prevFace, face);
                        isMoved = true;
                    }
                    else
                        if (map.CheckWall(x - 1, y) && !map.CheckEnemies(x - 1, y))
                        {
                            map.DelPl(x, y, prevFace);
                            map.MovePl(--(x), y, prevFace, face);
                            isMoved = true;
                        }
                }
                else if (random == 2)
                {
                    if ((sk.toFly || (sk.toSwim && map.RetMapxy(x + 1, y) == '~')) && map.RetMapxy(x + 1, y) != '#' && !map.CheckEnemies(x + 1, y))
                    {
                        map.DelPl(x, y, prevFace);
                        map.MovePl(++(x), y, prevFace, face);
                        isMoved = true;
                    }
                    else
                        if (map.CheckWall(x + 1, y) && !map.CheckEnemies(x + 1, y)) {
                            map.DelPl(x, y, prevFace);
                            map.MovePl(++(x), y, prevFace, face);
                            isMoved = true;
                        }
                }
                else if (random == 3)
                {
                    if ((sk.toFly || (sk.toSwim && map.RetMapxy(x, y - 1) == '~')) && map.RetMapxy(x, y - 1) != '#' && !map.CheckEnemies(x, y - 1))
                    {
                        map.DelPl(x, y, prevFace);
                        map.MovePl(x, --y, prevFace, face);
                        isMoved = true;
                    }
                    else
                        if (map.CheckWall(x, y - 1) && !map.CheckEnemies(x, y - 1)) {
                            map.DelPl(x, y, prevFace);
                            map.MovePl(x, --(y), prevFace, face);
                            isMoved = true;
                        }
                }
                else
                {
                    if ((sk.toFly || (sk.toSwim && map.RetMapxy(x, y + 1) == '~')) && map.RetMapxy(x, y + 1) != '#' && !map.CheckEnemies(x, y + 1))
                    {
                        map.DelPl(x, y, prevFace);
                        map.MovePl(x, ++y, prevFace, face);
                        isMoved = true;
                    }
                    else
                        if (map.CheckWall(x, y + 1) && !map.CheckEnemies(x, y + 1))
                        {
                            map.DelPl(x, y, prevFace);
                            map.MovePl(x, ++(y), prevFace, face);
                            isMoved = true;
                        }
                }
            }
        }
        GetLocalTime(&_t);
        pressButtonTime = (_t.wHour * 3600 + _t.wMinute * 60 + _t.wSecond) * 1000 + _t.wMilliseconds;
    }

    if (prevFace == 'P' && player.RetIsDead() == false)
    {
        map.SetMapxy(Px, Py, ' ');
        SetConsoleCursorPosition(map.RetH(), { (SHORT)Px, (SHORT)Py });
        player.SetIsDead(true);
    }
}

void Player::Move(Map& map, ListEnemies& enemies)
{
    if (isDead) return;
    getTime();
    if (_kbhit())
    {
        bool isMove;
        char key;
        key = _getch();
        if (TIME + realTime - pressButtonTime >= (int)(100 / speed))
        {
            if (key == 'A' || key == 'a' || key == 'Ф' || key == 'ф')      // Влево
            {
                if (x - 1 > 0) {
                    switch (map.RetMapxy(x - 1, y))
                    {
                    case '#':
                    {
                        isMove = false;
                        break;
                    }
                    case 'H':
                    {
                        isMove = true;
                        break;
                    }
                    case 'W':
                    {
                        isMove = false;
                        break;
                    }
                    case 'w':
                    {
                        isMove = false;
                        break;
                    }
                    case 'M':
                    {
                        isMove = false;
                        break;
                    }
                    case 'm':
                    {
                        isMove = false;
                        break;
                    }
                    case '=':
                    {
                        isMove = true;
                        break;
                    }
                    case '~':
                    {
                        isMove = false;
                        break;
                    }
                    case ' ':
                    {
                        isMove = true;
                        break;
                    }
                    case '+':
                    {
                        isMove = true;
                        break;
                    }
                    case 'o':
                    {
                        isMove = true;
                        break;
                    }
                    }
                    if (isMove)
                    {
                        map.DelPl(x, y, prevFace);
                        map.MovePl(--x, y, prevFace, face);
                        direction = LEFT;
                        GetLocalTime(&_t);
                        pressButtonTime = (_t.wHour * 3600 + _t.wMinute * 60 + _t.wSecond) * 1000 + _t.wMilliseconds;
                    }
                }
            }
            else if (key == char('D') || key == 'd' || key == 'В' || key == 'в') // Вправо
            {
                if (x + 1 < map.GetX() - 1) {
                    switch (map.RetMapxy(x + 1, y))
                    {
                    case '#':
                    {
                        isMove = false;
                        break;
                    }
                    case 'H':
                    {
                        isMove = true;
                        break;
                    }
                    case 'W':
                    {
                        isMove = false;
                        break;
                    }
                    case 'w':
                    {
                        isMove = false;
                        break;
                    }
                    case 'M':
                    {
                        isMove = false;
                        break;
                    }
                    case 'm':
                    {
                        isMove = false;
                        break;
                    }
                    case '=':
                    {
                        isMove = true;
                        break;
                    }
                    case '~':
                    {
                        isMove = false;
                        break;
                    }
                    case ' ':
                    {
                        isMove = true;
                        break;
                    }
                    case '+':
                    {
                        isMove = true;
                        break;
                    }
                    case 'o':
                    {
                        isMove = true;
                        break;
                    }
                    }
                    if (isMove)
                    {
                        map.DelPl(x, y, prevFace);
                        map.MovePl(++x, y, prevFace, face);
                        direction = RIGHT;
                        GetLocalTime(&_t);
                        pressButtonTime = (_t.wHour * 3600 + _t.wMinute * 60 + _t.wSecond) * 1000 + _t.wMilliseconds;
                    }
                }
            }
            else if (key == 'W' || key == 'w' || key == 'Ц' || key == 'ц') // Вверх
            {
                if (y - 1 > 0) {
                    switch (map.RetMapxy(x, y - 1))
                    {
                    case '#':
                    {
                        isMove = false;
                        break;
                    }
                    case 'H':
                    {
                        isMove = true;
                        break;
                    }
                    case 'W':
                    {
                        isMove = false;
                        break;
                    }
                    case 'w':
                    {
                        isMove = false;
                        break;
                    }
                    case 'M':
                    {
                        isMove = false;
                        break;
                    }
                    case 'm':
                    {
                        isMove = false;
                        break;
                    }
                    case '=':
                    {
                        isMove = true;
                        break;
                    }
                    case '~':
                    {
                        isMove = false;
                        break;
                    }
                    case ' ':
                    {
                        isMove = true;
                        break;
                    }
                    case '+':
                    {
                        isMove = true;
                        break;
                    }
                    case 'o':
                    {
                        isMove = true;
                        break;
                    }
                    }
                    if (isMove)
                    {
                        map.DelPl(x, y, prevFace);
                        map.MovePl(x, --y, prevFace, face);
                        direction = UP;
                        GetLocalTime(&_t);
                        pressButtonTime = (_t.wHour * 3600 + _t.wMinute * 60 + _t.wSecond) * 1000 + _t.wMilliseconds;
                    }
                }
            }
            else if (key == 'S' || key == 's' || key == 'Ы' || key == 'ы') // Вниз
            {
                if (y + 1 < map.GetY() - 1) {
                    switch (map.RetMapxy(x, y + 1))
                    {
                    case '#':
                    {
                        isMove = false;
                        break;
                    }
                    case 'H':
                    {
                        isMove = true;
                        break;
                    }
                    case 'W':
                    {
                        isMove = false;
                        break;
                    }
                    case 'w':
                    {
                        isMove = false;
                        break;
                    }
                    case 'M':
                    {
                        isMove = false;
                        break;
                    }
                    case 'm':
                    {
                        isMove = false;
                        break;
                    }
                    case '=':
                    {
                        isMove = true;
                        break;
                    }
                    case '~':
                    {
                        isMove = false;
                        break;
                    }
                    case ' ':
                    {
                        isMove = true;
                        break;
                    }
                    case '+':
                    {
                        isMove = true;
                        break;
                    }
                    case 'o':
                    {
                        isMove = true;
                        break;
                    }
                    }
                    if (isMove)
                    {
                        map.DelPl(x, y, prevFace);
                        map.MovePl(x, ++y, prevFace, face);
                        direction = DOWN;
                        GetLocalTime(&_t);
                        pressButtonTime = (_t.wHour * 3600 + _t.wMinute * 60 + _t.wSecond) * 1000 + _t.wMilliseconds;
                    }
                }
            }
        }
        if (TIME + realTime - pressButtonFire >= (int)(100 / FireSpeed) && ListWeapon[selectedWeapon - 1].RetCountAmmo() > 0)
        {
            if (key == ' ')
            {
                ListWeapon[selectedWeapon - 1].AddflyAmmo(direction, x, y, map);
                GetLocalTime(&_t);
                pressButtonFire = (_t.wHour * 3600 + _t.wMinute * 60 + _t.wSecond) * 1000 + _t.wMilliseconds;
            }
        }
        if (key == 'E' || key == 'e' || key == 'Й' || key == 'й')
        {
            if (selectedWeapon == countWeapon)
            {
                selectedWeapon = countWeapon;
            }
            else
            {
                selectedWeapon++;
                SetConsoleCursorPosition(map.RetH(), { (SHORT)82,(SHORT)4 });
                cout << "                     ";
            }
        }
        else if (key == 'Q' || key == 'q' || key == 'У' || key == 'у')
        {
            if (selectedWeapon == 1)
            {
                selectedWeapon = 1;
            }
            else
            {
                selectedWeapon--;
                SetConsoleCursorPosition(map.RetH(), { (SHORT)82,(SHORT)4 });
                cout << "                     ";
            }
        }
    }
    for (int i = 0; i < countWeapon; i++)
    {
        ListWeapon[i].DrawFlyAmmo(map, enemies);
    }
    UI(map);
    for (int i = 0; i < countWeapon; i++)
    {
        ListWeapon[i].CheckInsert();
    }
    SpawninGroundAmmo(map);
}

void Player::UI(Map& map)
{
    /*SetConsoleCursorPosition(hStd, { (SHORT)82,(SHORT)0 });
    cout << TIME;
    SetConsoleCursorPosition(hStd, { (SHORT)82,(SHORT)1 });
    cout << realTime;
    SetConsoleCursorPosition(hStd, { (SHORT)82,(SHORT)2 });
    cout << pressButtonTime;*/
    setColor();
    SetConsoleCursorPosition(map.RetH(), { (SHORT)82,(SHORT)1 });
    cout << "Оружие: ";
    SetConsoleCursorPosition(map.RetH(), { (SHORT)90,(SHORT)1 });
    cout << "         ";
    SetConsoleCursorPosition(map.RetH(), { (SHORT)90,(SHORT)1 });
    cout << ListWeapon[selectedWeapon - 1].RetName();
    SetConsoleCursorPosition(map.RetH(), { (SHORT)104,(SHORT)1 });
    cout << "     ";
    SetConsoleCursorPosition(map.RetH(), { (SHORT)104,(SHORT)1 });
    cout << ListWeapon[selectedWeapon - 1].RetCountFlyAmmo();
    SetConsoleCursorPosition(map.RetH(), { (SHORT)82,(SHORT)2 });
    cout << "Боеприпасы:   ";
    SetConsoleCursorPosition(map.RetH(), { (SHORT)94,(SHORT)2 });
    cout << ListWeapon[selectedWeapon - 1].RetCountAmmo();
}

void Player::LoadWeapon()
{
    ifstream read("weapon1.txt");

    if (!read)
    {
        cout << "Ошибка";
    }
    else {
        string stroka;
        int n;
        int j = 0;

        getline(read, stroka);

        n = stoi(stroka);
        countWeapon = n;

        ListWeapon = new weapon[countWeapon];
        string* temp = new string[(countWeapon * 4)];
        while (!read.eof())
        {
            getline(read, stroka);
            temp[j] = stroka;
            j++;
        }
        j = 0;
        for (int i = 0; i < countWeapon; i++)
        {
            ListWeapon[i].SetCountAmmo(stoi(temp[j]));
            j++;
            ListWeapon[i].SetMaxCountAmmo(stoi(temp[j]));
            j++;
            ListWeapon[i].SetID(stoi(temp[j]));
            j++;
            ListWeapon[i].SetName(temp[j]);
            j++;
        }
        delete[] temp;
        selectedWeapon = ListWeapon[0].RetID();
        /*for (int i = 0; i < countWeapon; i++)
        {
            cout << ListWeapon[i].RetCountAmmo() << endl;
            cout << ListWeapon[i].RetCountFlyAmmo() << endl;
            cout << ListWeapon[i].RetID() << endl;
            cout << ListWeapon[i].RetName() << endl;
        }*/
    }
    read.close();

}

void weapon::DrawFlyAmmo(Map& map, ListEnemies& enemies)
{
    for (int i = 0; i < countFlyAmmo; i++)
    {
        long int tt;
        tt = flyAmmolist[i].getTime(true);
        if (tt - flyAmmolist[i].TIME >= (int)(100 / flyAmmolist[i].velocity) && flyAmmolist[i].isEnd == false)
        {
            if (!CheckMove(flyAmmolist[i].x, flyAmmolist[i].y, flyAmmolist[i].direction, i, enemies, map, false))
            {
                map.DelPl(flyAmmolist[i].x, flyAmmolist[i].y, flyAmmolist[i].prevFaceAmmo);
                flyAmmolist[i].isEnd = true;
            }
            else
            {
                flyAmmolist[i].TIME = tt;
                //SetConsoleCursorPosition(RetHW(), { (SHORT)flyAmmolist[i].x, (SHORT)flyAmmolist[i].y });
                //cout << " ";
                map.DelPl(flyAmmolist[i].x, flyAmmolist[i].y, flyAmmolist[i].prevFaceAmmo);
                switch (flyAmmolist[i].direction)
                {
                case UP: { flyAmmolist[i].y--; break; }
                case DOWN: { flyAmmolist[i].y++; break; }
                case LEFT: { flyAmmolist[i].x--; break; }
                case RIGHT: { flyAmmolist[i].x++; break; }
                }
                //SetConsoleCursorPosition(RetHW(), { (SHORT)flyAmmolist[i].x, (SHORT)flyAmmolist[i].y });
                //cout << flyAmmolist[i].faceAmmo;
                map.MovePl(flyAmmolist[i].x, flyAmmolist[i].y, flyAmmolist[i].prevFaceAmmo, flyAmmolist[i].faceAmmo);
            }
        }
    }
}

void weapon::CheckInsert()
{
    for (int i = 0; i < countFlyAmmo;)
    {
        if (flyAmmolist[i].isEnd == true)
        {
            i++;
        }
        else if (flyAmmolist[i].isEnd == false)
            i = countFlyAmmo + 1;
        if (i == countFlyAmmo)
        {
            IsertflyAmmo();
        }
    }
}

void Player::LoadAmmo()
{
    if (ListWeapon != nullptr)
    {
        ifstream read("ammo.txt");

        if (!read)
        {
            cout << "Ошибка";
        }
        else
        {
            string stroka;
            int n;
            int j = 0;

            string* temp = new string[(countWeapon * 5)];
            while (!read.eof())
            {
                getline(read, stroka);
                temp[j] = stroka;
                //cout<<temp[j]<< endl;
                j++;
            }
            j = 0;
            for (int i = 0; i < countWeapon; i++)
            {
                ListWeapon[i].damage = stoi(temp[j]);
                j++;
                ListWeapon[i].range = stoi(temp[j]);
                j++;
                ListWeapon[i].whatRun = temp[j];
                j++;
                ListWeapon[i].velocity = stod(temp[j]);
                j++;
                char cstr[2];
                strcpy_s(cstr, temp[j].c_str());
                //cout << *cstr << endl;
                ListWeapon[i].faceAmmo = *cstr;
                j++;
            }
            delete[] temp;
            //for (int i = 0; i < countWeapon; i++)
            //{
            //    cout << ListWeapon[i].damage << endl;
            //    cout << ListWeapon[i].range << endl;
            //    cout << ListWeapon[i].whatRun << endl;
            //    cout << ListWeapon[i].velocity << endl;
            //    cout << ListWeapon[i].faceAmmo<< endl;
            //}
        }
    }
}

void weapon::AddflyAmmo(Directional dir, int x, int y, Map& map)
{
    if (flyAmmolist == nullptr)
    {

        InsertCountAmmo();
        AddCountFlyAmmo();
        flyAmmolist = new weaponAmmo[countFlyAmmo];

        flyAmmolist[countFlyAmmo - 1].direction = dir;

        switch (dir)
        {
        case UP: { y--; break; }
        case DOWN: { y++; break; }
        case LEFT: { x--; break; }
        case RIGHT: { x++; break; }
        }

        map.DelPl(x, y, flyAmmolist[countFlyAmmo - 1].prevFaceAmmo);

        flyAmmolist[countFlyAmmo - 1].x = x;
        flyAmmolist[countFlyAmmo - 1].y = y;

        flyAmmolist[countFlyAmmo - 1].damage = damage;
        flyAmmolist[countFlyAmmo - 1].range = range;
        flyAmmolist[countFlyAmmo - 1].whatRun = whatRun;
        flyAmmolist[countFlyAmmo - 1].velocity = velocity;
        flyAmmolist[countFlyAmmo - 1].faceAmmo = faceAmmo;

        map.MovePl(x, y, flyAmmolist[countFlyAmmo - 1].prevFaceAmmo, flyAmmolist[countFlyAmmo - 1].faceAmmo);


    }
    else if (flyAmmolist != nullptr)
    {
        switch (dir)
        {
        case UP: { y--; break; }
        case DOWN: { y++; break; }
        case LEFT: { x--; break; }
        case RIGHT: { x++; break; }
        }
        int tempcountFlyAmmo = countFlyAmmo;
        weaponAmmo* tempflyAmmo = new weaponAmmo[countFlyAmmo];
        for (int i = 0; i < countFlyAmmo; i++)
        {
            tempflyAmmo[i] = flyAmmolist[i];
        }
        delete[] flyAmmolist;
        InsertCountAmmo();
        AddCountFlyAmmo();
        flyAmmolist = new weaponAmmo[countFlyAmmo];
        for (int i = 0; i < tempcountFlyAmmo; i++)
        {
            flyAmmolist[i] = tempflyAmmo[i];
        }
        flyAmmolist[countFlyAmmo - 1].direction = dir;

        flyAmmolist[countFlyAmmo - 1].x = x;
        flyAmmolist[countFlyAmmo - 1].y = y;

        flyAmmolist[countFlyAmmo - 1].damage = damage;
        flyAmmolist[countFlyAmmo - 1].range = range;
        flyAmmolist[countFlyAmmo - 1].whatRun = whatRun;
        flyAmmolist[countFlyAmmo - 1].velocity = velocity;
        flyAmmolist[countFlyAmmo - 1].faceAmmo = faceAmmo;

        SetConsoleCursorPosition(RetHW(), { (SHORT)flyAmmolist[countFlyAmmo - 1].x,(SHORT)flyAmmolist[countFlyAmmo - 1].y });
        cout << flyAmmolist[countFlyAmmo - 1].faceAmmo;
    }
}

void weapon::SpawnAmmo(Map& map)
{
    while (map.RetMapxy(xG, yG) != ' ')
    {
        xG = 1 + rand() % map.GetX() - 1;
        yG = 1 + rand() % map.GetY() - 1;
    }

    SetConsoleCursorPosition(map.RetH(), { (SHORT)xG, (SHORT)yG });
    SetColor(9, 0);
    cout << faceAmmo;
}

bool weapon::CheckMove/*чек мув*/(int x, int y, Directional direction, int i, ListEnemies& enemies, Map& map, bool checkformap = false)
{
    char c = '\0';
    CONSOLE_SCREEN_BUFFER_INFO con;

    switch (direction)
    {
    case UP: { y--; break; }
    case DOWN: { y++; break; }
    case LEFT: { x--; break; }
    case RIGHT: { x++; break; }
    }

    SetConsoleCursorPosition(hStdOutWeapon, { (SHORT)x, (SHORT)y });

    if (hStdOutWeapon != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(hStdOutWeapon, &con))
    {
        DWORD read = 0;
        if (!ReadConsoleOutputCharacterA(hStdOutWeapon, &c, 1, con.dwCursorPosition, &read) || read != 1)
            c = '\0';
    }
    char whatRun[5];
    if (!checkformap)
    {
        strcpy_s(whatRun, flyAmmolist[i].whatRun.c_str());
    }
    else
    {
        strcpy_s(whatRun, this->whatRun.c_str());
    }

    switch (c)
    {
    case '#':
    {
        if (*whatRun == c) return true;
        else
            return false;
    }
    case 'H':
    {
        if (*whatRun == c) return true;
        else
            return false;
    }
    case 'W':
    {
        if (*whatRun == c) return true;
        else
            return false;
    }
    case 'w':
    {
        if (*whatRun == c) return true;
        else
            return false;
    }
    case 'M':
    {
        if (*whatRun == c) return true;
        else
            return false;
    }
    case 'm':
    {
        if (*whatRun == c) return true;
        else
            return false;
    }
    case ' ':
    {
        return true;
    }
    case '+':
    {
        return false;
    }
    case 'o':
    {
        return false;
    }
    case 'A':
    {
        for (int i = 0; i < enemies.RetqOfEnemies(); i++)
        {
            if (x == enemies.RetList().at(i).RetX() && y == enemies.RetList().at(i).RetY())
            {
                SetConsoleCursorPosition(hStdOutWeapon, { (SHORT)x, (SHORT)y });
                cout << ' ';
                enemies.Delete(i, map);
                break;
            }
        }
        return false;
    }
    case 'B':
    {
        for (int i = 0; i < enemies.RetqOfEnemies(); i++)
        {
            if (x == enemies.RetList().at(i).RetX() && y == enemies.RetList().at(i).RetY())
            {
                SetConsoleCursorPosition(hStdOutWeapon, { (SHORT)x, (SHORT)y });
                cout << ' ';
                enemies.Delete(i, map);
                break;
            }
        }
        return false;
    }
    case 'O':
    {
        for (int i = 0; i < enemies.RetqOfEnemies(); i++)
        {
            if (x == enemies.RetList().at(i).RetX() && y == enemies.RetList().at(i).RetY())
            {
                SetConsoleCursorPosition(hStdOutWeapon, { (SHORT)x, (SHORT)y });
                cout << ' ';
                enemies.Delete(i, map);
                break;
            }
        }
        return false;
    }
    }
}

int main()
{
    srand(time(NULL));
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Map map("map1.txt");
    Player player('P', 18, 10, 1);
    player.LoadWeapon();
    player.LoadAmmo();
    ListEnemies enemies;
    enemies.AddAmfibia(map, 1, 3);
    enemies.AddAmfibia(map, 3, 1);
    enemies.AddBat(map, 5, 5);
    enemies.AddOrc(map, 5, 7);
    enemies.AddOrc(map, 7, 5);
    map.Read();
    map.Out();
    map.SetPl(player);
    while (!player.RetIsDead())
    {
        player.Move(map, enemies);
        enemies.MoveAll(map, player);
        enemies.UI(map);
        if (enemies.RetqOfEnemies() == 0) break;
    }
    setColor();
    SetConsoleCursorPosition(map.RetH(), { (SHORT)0, (SHORT)(map.GetY() + 1) });
    if (player.RetIsDead()) cout << "Вы проиграли!" << endl;
    else cout << "Вы победили всех монстров!" << endl;
}